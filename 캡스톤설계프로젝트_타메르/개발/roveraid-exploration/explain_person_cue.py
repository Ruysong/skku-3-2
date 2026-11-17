"""Inspect the text contract without video, a camera, GUI, or motor output.

The keypoints here are SYNTHETIC FIXTURES, not a model detection performance test.
Use --ask-ai only when the local model server is running.
"""
import argparse
import json
from pathlib import Path
import time
import sys
from datetime import datetime

from roveraid.observations import body_evidence, CueEvents, observation_request
from roveraid.mission import MockMap, MissionManager, world_state
from roveraid.navigation import MockNavigation
from roveraid.reasoner import LocalReasoner


def main():
    sys.stdout.reconfigure(encoding='utf-8')
    parser=argparse.ArgumentParser()
    parser.add_argument('--ask-ai',action='store_true')
    parser.add_argument('--endpoint',default='http://127.0.0.1:8091')
    args=parser.parse_args()
    points=[[0.,0.] for _ in range(17)];confidence=[0.]*17
    for i,p in {13:[500,300],14:[530,300],15:[510,430],16:[540,430]}.items():
        points[i]=p;confidence[i]=.9
    person={'track_id':5,'confidence':.85,'box':[480,150,620,479],
            'posture':'unknown','observations':3,
            **body_evidence([480,150,620,479],points,confidence,(480,640))}
    gate=CueEvents();now=time.monotonic();events=[]
    for offset in (-.6,-.3,0):events+=gate.update([person],'fixture-1',now+offset)
    m=MockMap()
    observation={'people':[person],'observed_at':now,'frame':3,'media_time':.6}
    state=world_state(observation,m,'새로운 사람 후보를 확인하고 관측 결과를 보고한다',
                      'SYNTHETIC KEYPOINT FIXTURE / not real detection','fixture-1',gate.recent(now),events)
    state['evidence_provenance']='Synthetic keypoints. No video or physical robot is involved.'
    reasoner=LocalReasoner(args.endpoint)
    text=reasoner.build_payload(state)['messages'][1]['content'][0]['text']
    folder=Path(__file__).resolve().parent/'output'/'text-contract'/datetime.now().strftime('%Y%m%d-%H%M%S-%f')
    folder.mkdir(parents=True,exist_ok=True)
    (folder/'input.json').write_text(json.dumps(json.loads(text),ensure_ascii=False,indent=2),encoding='utf-8')
    result={'input_mode':'text_only','evidence_provenance':state['evidence_provenance'],
            'observation':person,'events':events}
    if args.ask_ai:
        start=time.monotonic();decision,meta=reasoner.decide(state)
        result.update(decision=decision,metadata=meta,elapsed_seconds=round(time.monotonic()-start,3))
        if decision['action']=='inspect_person':
            result['proposed_observation_request']=observation_request({**person,
                'last_seen_age_seconds':round(time.monotonic()-observation['observed_at'],3),
                'source_epoch':'fixture-1','observed_robot_cell':list(m.pose)})
        manager=MissionManager()
        # Recheck evidence freshness after inference, exactly as runtime application must.
        current=world_state(observation,m,state['mission'],state['source'],'fixture-1',gate.recent(time.monotonic()),events)
        manager.apply(decision,current,MockNavigation(m))
        result.update(application_status=manager.status,observation_request=manager.last_observation_request)
    (folder/'result.json').write_text(json.dumps(result,ensure_ascii=False,indent=2),encoding='utf-8')
    print(json.dumps(result,ensure_ascii=False,indent=2))


if __name__=='__main__':main()
