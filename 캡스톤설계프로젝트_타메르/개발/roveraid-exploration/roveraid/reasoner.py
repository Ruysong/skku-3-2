"""Local OpenAI-compatible llama.cpp multimodal endpoint."""
import json
import urllib.error
import urllib.request

from .mission import validate_decision, person_targets

SCHEMA = {'type':'object', 'properties':{
    'action':{'type':'string','enum':['explore','inspect_person','report','hold']},
    'target_id':{'type':['string','null']},
    'reason':{'type':'string','maxLength':120}},
    'required':['action','target_id','reason'], 'additionalProperties':False}

SYSTEM = '''You supervise a rover search mission using structured perception evidence.
MISSION PRIORITY: locate and review person cues. When people or valid recent person cues are provided,
choose inspect_person for a relevant target, or report when appropriate, before continuing exploration.
Prioritize uncertain partial-body/pose cues that need more observation. Use only the offered target IDs.
Choose explore mainly when there are no unresolved person cues. Do not let frontier scores override person review.
observation_window summarizes the latest 5 seconds: consider repeated cues, side, and recency.
Inputs are YOLO pose and tracking observations; an image is optional. Without an image, do not
claim to see doors, rooms, identity, injury, or anything absent from the supplied evidence.
lower_body_hint is uncertain person/pose evidence, not a separate leg detector or confirmed person.
image_direction is image-side information, never a robot turn angle. Do not invent null distances.
The map and navigation are synthetic. Do not describe them as real LiDAR or SLAM measurements.
explore: select an ID from frontiers using distance, exploration gain, and provided history.
inspect_person: select a track_id string from people or recent_person_cues (same mock view, <=15s).
A recent partial-body cue justifies reobservation, even when pose, identity, distance, or angle is unknown.
This is an observation proposal, not a physical turn or approach command.
report: prepare an observation report; target_id null.
hold: wait if evidence is unavailable or video age exceeds 3 seconds; target_id null.
Do not ignore valid recent cues merely because no image is attached or posture is unknown.
Return exactly action, target_id, reason. Reason MUST be a short English sentence, at most 18 words.
Never output PWM, speed, steering, arbitrary coordinates, or medical conclusions. Track IDs are temporary.'''


class LocalReasoner:
    def __init__(self, endpoint='http://127.0.0.1:8091', timeout=120):
        self.endpoint = endpoint.rstrip('/')
        self.timeout = timeout
        # A system HTTP proxy must never route local camera frames off-device.
        self.http = urllib.request.build_opener(urllib.request.ProxyHandler({}))

    def health(self):
        try:
            with self.http.open(self.endpoint + '/health', timeout=1) as r:
                return r.status == 200
        except (OSError, urllib.error.URLError):
            return False

    def build_payload(self, state, image=None):
        compact={k:state[k] for k in ('mission','video_age_seconds','visited_goal_count','active_goal_cell') if k in state}
        compact['map_source']='synthetic / mock navigation'
        compact['frontiers']=[{k:f[k] for k in ('id','distance_cells','unknown_gain') if k in f}
                              for f in state['frontiers'][:5]]
        current_ids={str(p['track_id']) for p in state.get('people',[])}
        for name in ('people','recent_person_cues'):
            persons=[p for p in state.get(name,[]) if name=='people' or str(p['track_id']) not in current_ids]
            persons = sorted(persons,key=lambda p:(p.get('body_cue')!='lower_body_hint',-p.get('confidence',0)))
            compact[name]=[]
            for p in persons[:4]:
                item={k:p[k] for k in ('track_id','track_status','posture','body_cue') if k in p}
                item['confidence']=round(p.get('confidence',0),2)
                item['image_direction']={'side':(p.get('image_direction') or {}).get('side','unknown')}
                item['supported_keypoints']=p.get('supported_keypoints',[])[:8]
                if 'last_seen_age_seconds' in p:item['last_seen_age_seconds']=round(p['last_seen_age_seconds'],1)
                compact[name].append(item)
        window=state.get('observation_window')
        if window:
            compact['observation_window']={k:window[k] for k in ('seconds','processed_frames','peak_visible_count','current_visible_count')}
            compact['observation_window']['tracks']=[{'track_id':t['track_id'],'frames':t['frames_seen'],
                'sides':list(t['image_sides']),'cues':list(t['body_cue_counts']),
                'postures':list(t['posture_counts']),'last_seen_s':t['last_seen_age_seconds']}
                for t in window.get('tracks',[])[:4]]
        compact['recent_events'] = [{k:e[k] for k in ('id','type','track_id','age_seconds','image_direction') if k in e}
                                    for e in state.get('recent_events',[])[-5:]]
        valid_cues=person_targets(state)
        compact['person_review_targets']=[str(p['track_id']) for p in compact['people']+compact['recent_person_cues']
                                          if str(p['track_id']) in valid_cues]
        compact['review_priority']='review these person cues before exploration' if compact['person_review_targets'] else 'no person cue: evaluate exploration frontiers'
        compact['input_mode'] = 'text_only' if image is None else 'text_and_image'
        content = [{'type':'text','text':json.dumps(compact, ensure_ascii=False, allow_nan=False,separators=(',',':'))}]
        choices = {'hold':[None],'report':[None]}
        if compact['frontiers']:choices['explore']=[f['id'] for f in compact['frontiers']]
        known = person_targets(state)
        offered=[str(p['track_id']) for p in compact['people']+compact['recent_person_cues']
                 if str(p['track_id']) in known]
        if offered:choices['inspect_person']=list(dict.fromkeys(offered))
        schema = {'oneOf':[{'type':'object','properties':{
            'action':{'type':'string','enum':[action]},
            'target_id':{'enum':targets},'reason':SCHEMA['properties']['reason']},
            'required':['action','target_id','reason'],'additionalProperties':False}
            for action,targets in choices.items()]}
        if image is not None:
            import base64
            import cv2
            height, width = image.shape[:2]
            scale = min(1,512/max(width,height))
            resized = cv2.resize(image,(max(1,int(width*scale)),max(1,int(height*scale))))
            ok, jpg = cv2.imencode('.jpg',resized,[cv2.IMWRITE_JPEG_QUALITY,80])
            if not ok: raise ValueError('Cannot encode decision image')
            data_url = 'data:image/jpeg;base64,'+base64.b64encode(jpg).decode('ascii')
            content.append({'type':'image_url','image_url':{'url':data_url}})
        payload = {'model':'local-qwen3-vl-2b', 'temperature':0, 'max_tokens':160,
                   'messages':[{'role':'system','content':SYSTEM},
                               {'role':'user','content':content}],
                   'response_format':{'type':'json_schema','json_schema':{
                       'name':'mission_decision','strict':True,'schema':schema}}}
        return payload

    def decide(self, state, image=None):
        payload = self.build_payload(state,image)
        req = urllib.request.Request(self.endpoint + '/v1/chat/completions',
              data=json.dumps(payload).encode('utf-8'), headers={'Content-Type':'application/json'})
        with self.http.open(req, timeout=self.timeout) as r:
            response = json.load(r)
        choice = response['choices'][0]
        if choice.get('finish_reason') == 'length':
            raise ValueError('Local model response truncated; no decision was applied. Retry.')
        text = choice['message']['content'].strip()
        if text.startswith('```'):
            text = text.split('\n',1)[1].rsplit('```',1)[0].strip()
        try:
            decision = validate_decision(json.loads(text), state)
        except (ValueError,TypeError) as e:
            raise ValueError(f'Invalid model response ({e}): {text[:800]}') from e
        return decision, {'usage':response.get('usage',{}), 'raw':text,
                          'input_mode':'text_only' if image is None else 'text_and_image'}
