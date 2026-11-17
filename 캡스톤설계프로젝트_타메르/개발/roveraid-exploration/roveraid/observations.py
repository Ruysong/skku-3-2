"""Turn pose outputs into bounded text evidence, without inventing geometry."""
from dataclasses import dataclass
import math

KEYPOINTS = ('nose', 'left_eye', 'right_eye', 'left_ear', 'right_ear',
             'left_shoulder', 'right_shoulder', 'left_elbow', 'right_elbow',
             'left_wrist', 'right_wrist', 'left_hip', 'right_hip',
             'left_knee', 'right_knee', 'left_ankle', 'right_ankle')


def body_evidence(box, points, confidences, frame_shape, threshold=.5):
    height, width = frame_shape[:2]
    if height <= 0 or width <= 0:
        raise ValueError('Invalid frame dimensions')
    supported, locations = [], []
    if points is not None and confidences is not None:
        for i, (point, confidence) in enumerate(zip(points, confidences)):
            if i >= len(KEYPOINTS): break
            x, y = float(point[0]), float(point[1])
            values = (x, y, float(confidence))
            if (all(math.isfinite(v) for v in values) and confidence >= threshold
                    and 0 < x < width and 0 < y < height
                    and box[0]-5 <= x <= box[2]+5 and box[1]-5 <= y <= box[3]+5):
                supported.append(i)
                locations.append((x, y))
    lower = [i for i in supported if i in (13, 14, 15, 16)]
    upper = [i for i in supported if i in range(11)]
    if len(lower) >= 2 and not upper:
        cue = 'lower_body_hint'
        description = 'Lower-body keypoint evidence with limited upper-body support. Reobservation is needed.'
    elif len(supported) >= 10:
        cue = 'person_keypoints'
        description = 'Person candidate supported by multiple pose keypoints.'
    elif supported:
        cue = 'partial_pose_hint'
        description = 'Partial keypoint support. Occlusion, framing, and model uncertainty need further observation.'
    else:
        cue = 'person_box_only'
        description = 'Person box detected with insufficient reliable pose keypoints.'
    # For a lower-body cue use supported pixels, not a possibly extrapolated full-body box.
    center_x = sum(x for x, _ in locations)/len(locations) if cue == 'lower_body_hint' else (box[0]+box[2])/2
    nx = max(0., min(1., center_x/width))
    side = 'left' if nx < 1/3 else 'right' if nx > 2/3 else 'center'
    return {'body_cue':cue, 'supported_keypoints':[KEYPOINTS[i] for i in supported],
            'keypoint_confidence_threshold':threshold, 'description':description,
            'image_direction':{'side':side, 'center_x_normalized':round(nx,4)},
            'touches_image_edge':bool(box[0] <= width*.02 or box[2] >= width*.98
                                      or box[1] <= height*.02 or box[3] >= height*.98),
            'bearing_camera_deg':None, 'range_m':None,
            'geometry_status':'uncalibrated: image side is not a robot turn angle',
            'evidence_source':'YOLO person detection and pose estimates; not independent leg detection'}


class CueEvents:
    """Debounce short observations and keep brief last-seen evidence for reobservation."""
    def __init__(self, min_hits=3, min_span=.4, max_gap=1.0, memory_seconds=15.0):
        self.min_hits, self.min_span = min_hits, min_span
        self.max_gap, self.memory_seconds = max_gap, memory_seconds
        self.epoch = None
        self.records = {}
        self.counter = 0
        self.brief_regions = {}

    def reset(self, epoch):
        self.epoch = epoch
        self.records.clear()
        self.counter = 0
        self.brief_regions.clear()

    def event(self, kind, person, now, level):
        self.counter += 1
        direction=person.get('image_direction') or {}
        region=f"{direction.get('side','unknown')}:{int(direction.get('center_x_normalized',.5)*4)}"
        return {'id':f'{self.epoch}-E{self.counter}','type':kind,'track_id':person['track_id'],
                'evidence_level':level,'cue_region':region,'image_direction':direction,
                'person_observation':person.copy(),'observed_at':now}

    def update(self, people, epoch, now):
        if epoch != self.epoch: self.reset(epoch)
        for tid in list(self.records):
            if now-self.records[tid]['last']>self.memory_seconds: del self.records[tid]
        events = []
        seen = set()
        for p in people:
            tid = str(p['track_id'])
            if tid in seen: continue
            seen.add(tid)
            old = self.records.get(tid)
            if old is None:
                old = self.records[tid] = {'start':now,'last':now,'hits':0,'cue_start':now,
                    'cue_hits':0,'cue':p.get('body_cue'),'emitted':set(),'person':p.copy()}
            elif now-old['last'] > self.max_gap:
                old.update(start=now,hits=0,cue_start=now,cue_hits=0)
            if old['cue'] != p.get('body_cue'):
                old.update(cue=p.get('body_cue'),cue_start=now,cue_hits=0)
            old.update(last=now,person=p.copy())
            old['hits'] += 1
            old['cue_hits'] += 1
            if old['cue']=='lower_body_hint' and p.get('confidence',0)>=.6 and 'brief_lower_body_hint' not in old['emitted']:
                e=self.event('brief_lower_body_hint',p,now,'provisional_single_observation')
                old['emitted'].add('brief_lower_body_hint')
                if now-self.brief_regions.get(e['cue_region'],-math.inf)>=2:
                    self.brief_regions[e['cue_region']]=now
                    events.append(e)
            kind = ('lower_body_hint_confirmed' if old['cue']=='lower_body_hint'
                    and old['cue_hits']>=self.min_hits and now-old['cue_start']>=self.min_span
                    else 'person_candidate_confirmed')
            stable = old['hits']>=self.min_hits and now-old['start']>=self.min_span
            if stable and kind not in old['emitted']:
                # A lower-body event supersedes the generic event for the same track.
                old['emitted'].update((kind,'person_candidate_confirmed'))
                events.append(self.event(kind,p,now,'temporally_repeated'))
        for tid in list(self.records):
            if now-self.records[tid]['last']>self.memory_seconds: del self.records[tid]
        return events

    def recent(self, now):
        return [{**r['person'], 'last_seen_age_seconds':round(now-r['last'],3)}
                for r in self.records.values() if 0 <= now-r['last'] <= self.memory_seconds]


def observation_request(person, event_id=None):
    """A request for a future adapter, never an executable motor command."""
    return {'type':'reobserve_person', 'target_id':str(person['track_id']),
            'event_id':event_id, 'image_direction':person.get('image_direction'),
            'image_roi_pixels':person.get('box'), 'body_cue':person.get('body_cue'),
            'bearing_camera_deg':person.get('bearing_camera_deg'), 'range_m':person.get('range_m'),
            'last_seen_age_seconds':person.get('last_seen_age_seconds',0),
            'source_epoch':person.get('source_epoch'),
            'observed_robot_cell':person.get('observed_robot_cell'),
            'coordinate_frame':'camera_image', 'translation_allowed':False,
            'execution_status':'pending_observation_adapter',
            'requires':'camera calibration, camera-to-robot transform, current sensor safety checks'}


@dataclass(frozen=True)
class DecisionContext:
    source_epoch: str
    mission: str
    requested_at: float
    control_revision: int

    def valid(self, state, now, control_revision, max_age=30):
        return (self.source_epoch==state['source_epoch'] and self.mission==state['mission']
                and self.control_revision==control_revision
                and 0 <= now-self.requested_at <= max_age)
