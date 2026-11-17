"""Rolling perception summaries and a non-queuing periodic AI scheduler."""
from collections import Counter, deque
from copy import deepcopy
import threading

from .observations import CueEvents


class ObservationWindow:
    def __init__(self, seconds=5, max_frames=600):
        self.seconds = seconds
        self.frames = deque(maxlen=max_frames)
        self.events = deque(maxlen=12)
        self.cues = CueEvents()
        self.lock = threading.RLock()
        self.epoch = None
        self.first_observed_at = None
        self.total_frames = 0
        self.latest = None

    def clear(self):
        with self.lock:
            self.frames.clear();self.events.clear();self.latest=None
            self.epoch=None;self.first_observed_at=None;self.total_frames=0
            self.cues.reset(None)

    def add(self, observation):
        # Called from the inference worker before the GUI's latest-frame queue.
        # Thus GUI frame dropping does not drop evidence from the rolling summary.
        with self.lock:
            epoch=observation['source_epoch']
            if epoch != self.epoch:
                self.clear();self.epoch=epoch
            stamp=observation['observed_at']
            if self.first_observed_at is None:self.first_observed_at=stamp
            if self.latest and stamp < self.latest['observed_at']:
                return
            people=deepcopy(observation['people'])
            for p in people:p['source_epoch']=epoch
            packet={k:deepcopy(v) for k,v in observation.items() if k not in ('image','history','people')}
            packet['people']=people
            self.latest=packet
            self.frames.append(packet)
            self.total_frames+=1
            for event in self.cues.update(people,epoch,stamp):self.events.append(event)
            self._prune(stamp)

    def _prune(self, now):
        while self.frames and self.frames[0]['observed_at'] < now-self.seconds:
            self.frames.popleft()

    def snapshot(self, now):
        with self.lock:
            self._prune(now)
            tracks={}
            for frame in self.frames:
                for p in frame['people']:
                    tid=str(p['track_id'])
                    t=tracks.setdefault(tid,{'track_id':p['track_id'],'frames_seen':0,
                        'first':frame['observed_at'],'last':frame['observed_at'],
                        'sides':Counter(),'body_cues':Counter(),'postures':Counter(),'latest':p})
                    t['frames_seen']+=1;t['last']=frame['observed_at'];t['latest']=p
                    t['sides'][(p.get('image_direction') or {}).get('side','unknown')]+=1
                    t['body_cues'][p.get('body_cue','unknown')]+=1
                    t['postures'][p.get('posture','unknown')]+=1
            ordered=sorted(tracks.values(),key=lambda t:(-t['body_cues']['lower_body_hint'],-t['frames_seen']))
            summaries=[{'track_id':t['track_id'],'frames_seen':t['frames_seen'],
                'first_seen_age_seconds':round(now-t['first'],2),
                'last_seen_age_seconds':round(now-t['last'],2),
                'image_sides':dict(t['sides']),'body_cue_counts':dict(t['body_cues']),
                'posture_counts':dict(t['postures'])} for t in ordered[:6]]
            fresh_events=[{k:deepcopy(v) for k,v in e.items() if k not in ('observed_at','person_observation')}
                          | {'age_seconds':round(now-e['observed_at'],2)}
                          for e in self.events if 0<=now-e['observed_at']<=15]
            return {'latest':deepcopy(self.latest),'source_epoch':self.epoch,
                'first_observed_at':self.first_observed_at,'total_frames':self.total_frames,
                'window':{'seconds':self.seconds,'processed_frames':len(self.frames),
                    'peak_visible_count':max((len(f['people']) for f in self.frames),default=0),
                    'current_visible_count':len(self.latest['people']) if self.latest else 0,
                    'observed_track_count':len(tracks),'tracks':summaries},
                'recent_people':deepcopy(self.cues.recent(now)),'events':fresh_events}


class ReviewScheduler:
    """One in-flight call. Busy intervals are coalesced, never queued."""
    def __init__(self, interval=5):
        if interval<=0:raise ValueError('Review interval must be positive')
        self.interval=interval
        self.busy=False
        self.last_started=None
        self.request_count=0
        self.response_count=0

    def remaining(self, now, first_observed_at):
        if first_observed_at is None:return self.interval
        return max(0, max(first_observed_at+self.interval,
                          self.last_started+self.interval if self.last_started is not None else -float('inf'))-now)

    def due(self, now, snapshot):
        latest=snapshot['latest']
        return (not self.busy and latest is not None
                and 0<=now-latest['observed_at']<=3
                and self.remaining(now,snapshot['first_observed_at'])<=0)

    def begin(self, now):
        if self.busy:raise RuntimeError('A model call is already running')
        self.busy=True;self.last_started=now;self.request_count+=1

    def finish(self):
        self.busy=False;self.response_count+=1

    def restart(self):
        # Keep busy while an old HTTP call completes; its context is invalidated separately.
        if not self.busy:self.last_started=None


def observation_lines(snapshot):
    w=snapshot['window']
    side_names={'left':'Left','center':'Center','right':'Right','unknown':'Unknown direction'}
    lines=[f"Last {w['seconds']:g}s / {w['processed_frames']} processed frames / {w['current_visible_count']} visible now"]
    for t in w['tracks'][:4]:
        side=max(t['image_sides'],key=t['image_sides'].get)
        cue=('lower-body cue' if t['body_cue_counts'].get('lower_body_hint') else
             'partial pose' if t['body_cue_counts'].get('partial_pose_hint') else 'person candidate')
        lines.append(f"{side_names.get(side,side)} / ID {t['track_id']} / {cue} / seen {t['frames_seen']} times")
    if not w['tracks']:lines.append('No person cues in the current observation window.')
    return '\n'.join(lines)


def decision_title(decision, state):
    if decision['action']=='inspect_person':
        candidates=state['people']+state.get('recent_person_cues',[])
        p=next((p for p in candidates if str(p['track_id'])==decision['target_id']),{})
        side=(p.get('image_direction') or {}).get('side','unknown')
        location={'left':'left','center':'ahead','right':'right'}.get(side,'the observed direction')
        return f'Recheck person {location}'
    return {'explore':'Continue exploration','report':'Report observations','hold':'Hold for more evidence'}[decision['action']]
