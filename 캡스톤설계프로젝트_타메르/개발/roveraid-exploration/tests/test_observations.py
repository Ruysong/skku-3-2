import json
import unittest
from unittest.mock import Mock

from roveraid.observations import body_evidence, CueEvents, DecisionContext, observation_request
from roveraid.reasoner import LocalReasoner
from roveraid.mission import MockMap, MissionManager, validate_decision, world_state
from roveraid.navigation import MockNavigation


def lower_body():
    points=[[0.,0.] for _ in range(17)]
    confidence=[0.]*17
    for i,p in {13:[500,300],14:[530,300],15:[510,430],16:[540,430]}.items():
        points[i]=p;confidence[i]=.9
    return body_evidence([480,150,620,479],points,confidence,(480,640,3))


class EvidenceTests(unittest.TestCase):
    def test_lower_body_is_a_hint_not_a_pose_diagnosis(self):
        evidence=lower_body()
        self.assertEqual(evidence['body_cue'],'lower_body_hint')
        self.assertEqual(evidence['image_direction']['side'],'right')
        self.assertEqual(len(evidence['supported_keypoints']),4)
        self.assertIsNone(evidence['bearing_camera_deg'])
        self.assertIsNone(evidence['range_m'])

    def test_low_confidence_does_not_support_keypoints(self):
        e=body_evidence([10,20,100,200],[[40,50]]*17,[.1]*17,(480,640))
        self.assertEqual(e['body_cue'],'person_box_only')
        self.assertEqual(e['supported_keypoints'],[])

    def test_origin_placeholder_is_not_a_visible_joint(self):
        e=body_evidence([0,0,100,200],[[0,0]]*17,[.99]*17,(480,640))
        self.assertEqual(e['supported_keypoints'],[])

    def test_outside_box_or_frame_joint_is_rejected(self):
        e=body_evidence([10,10,100,100],[[500,500]]*17,[.99]*17,(480,640))
        self.assertEqual(e['supported_keypoints'],[])

    def test_single_joint_does_not_claim_lower_body_hint(self):
        p=[[0,0] for _ in range(17)];c=[0.]*17;p[15]=[500,430];c[15]=.9
        e=body_evidence([480,150,620,479],p,c,(480,640))
        self.assertEqual(e['body_cue'],'partial_pose_hint')

    def test_reobservation_is_not_an_executable_turn_or_approach(self):
        r=observation_request({'track_id':5,'box':[480,150,620,479],**lower_body()})
        self.assertFalse(r['translation_allowed'])
        self.assertEqual(r['execution_status'],'pending_observation_adapter')
        self.assertIsNone(r['bearing_camera_deg'])
        self.assertNotIn('motor_speed',r)


class EventTests(unittest.TestCase):
    def setUp(self):
        self.gate=CueEvents()
        self.p={'track_id':5,'confidence':.85,**lower_body()}

    def confirm(self):
        events=[]
        for t in (0,.25,.5): events+=self.gate.update([self.p],'s1',t)
        return events

    def test_one_frame_can_be_provisional_but_is_not_confirmed(self):
        events=self.gate.update([self.p],'s1',0)
        self.assertEqual([e['type'] for e in events],['brief_lower_body_hint'])
        self.assertEqual(events[0]['evidence_level'],'provisional_single_observation')

    def test_stable_lower_cue_emits_once(self):
        events=self.confirm()
        self.assertEqual([e['type'] for e in events],['brief_lower_body_hint','lower_body_hint_confirmed'])
        self.assertEqual(self.gate.update([self.p],'s1',.75),[])

    def test_recent_hint_survives_brief_disappearance_then_expires(self):
        self.confirm();self.gate.update([],'s1',1)
        self.assertEqual(self.gate.recent(1.2)[0]['track_id'],5)
        self.assertEqual(self.gate.recent(16),[])

    def test_source_epoch_clears_memory(self):
        self.confirm();self.gate.update([],'s2',.6)
        self.assertEqual(self.gate.recent(.6),[])

    def test_return_after_memory_expiry_requires_new_confirmation(self):
        self.confirm()
        self.assertEqual(len(self.gate.update([self.p],'s1',20)),1)
        self.assertEqual(self.gate.update([self.p],'s1',20.25),[])
        self.assertEqual(len(self.gate.update([self.p],'s1',20.5)),1)

    def test_long_gap_does_not_count_as_continuous_evidence(self):
        self.gate.update([self.p],'s1',0)
        self.gate.update([self.p],'s1',.2)
        self.assertEqual(self.gate.update([self.p],'s1',2),[])

    def test_untracked_ids_in_same_region_do_not_flood_brief_events(self):
        self.assertEqual(len(self.gate.update([{**self.p,'track_id':'untracked-1'}],'s1',0)),1)
        self.assertEqual(self.gate.update([{**self.p,'track_id':'untracked-2'}],'s1',.1),[])


class PerceptionBoundaryTests(unittest.TestCase):
    def test_untracked_pose_is_retained_as_a_temporary_cue(self):
        from pathlib import Path
        from unittest.mock import patch
        import numpy as np
        from ultralytics.engine.results import Results
        from roveraid.perception import Perception
        frame=np.zeros((480,640,3),dtype=np.uint8)
        keypoints=np.zeros((1,17,3),dtype=float)
        for i,xy in {13:[500,300],14:[530,300],15:[510,430],16:[540,430]}.items():
            keypoints[0,i]=[*xy,.9]
        result=Results(frame,path='SYNTHETIC_FIXTURE',names={0:'person'},
                       boxes=np.array([[480,150,620,479,.85,0]]),keypoints=keypoints)
        model=Mock();model.track.return_value=[result]
        with patch('roveraid.perception.YOLO',return_value=model):
            observation=Perception(Path('unused.pt')).process(frame,0)
        self.assertEqual(len(observation['people']),1)
        p=observation['people'][0]
        self.assertTrue(str(p['track_id']).startswith('untracked-'))
        self.assertEqual(p['body_cue'],'lower_body_hint')
        self.assertEqual(p['image_direction']['side'],'right')
        self.assertEqual(p['track_status'],'untracked_single_frame')
        self.assertEqual(observation['history'],[])


class TextReasonerTests(unittest.TestCase):
    def setUp(self):
        self.map=MockMap()
        self.state=world_state(None,self.map,'Search','fixture','s1')
        self.state['video_age_seconds']=.1
        self.person={'track_id':5,'confidence':.85,**lower_body()}
        self.state['people']=[self.person]
        self.reasoner=LocalReasoner()

    def test_text_is_default_and_contains_pose_and_direction(self):
        payload=self.reasoner.build_payload(self.state)
        content=payload['messages'][1]['content']
        self.assertEqual([c['type'] for c in content],['text'])
        self.assertIn('lower_body_hint',content[0]['text'])
        self.assertIn('right',content[0]['text'])
        self.assertNotIn('image_url',json.dumps(payload))

    def test_image_is_explicit_opt_in(self):
        import numpy as np
        p=self.reasoner.build_payload(self.state,np.zeros((32,32,3),dtype=np.uint8))
        self.assertEqual(p['messages'][1]['content'][1]['type'],'image_url')

    def test_http_path_uses_text_only_and_validates_response(self):
        body={'choices':[{'finish_reason':'stop','message':{'content':json.dumps({
            'action':'inspect_person','target_id':'5','reason':'오른쪽 하체 관절 후보를 재관측한다.'})}}]}
        response=Mock();response.read.return_value=json.dumps(body).encode()
        response.__enter__=Mock(return_value=response);response.__exit__=Mock(return_value=False)
        self.reasoner.http.open=Mock(return_value=response)
        decision,meta=self.reasoner.decide(self.state)
        sent=json.loads(self.reasoner.http.open.call_args.args[0].data)
        self.assertNotIn('image_url',json.dumps(sent))
        self.assertEqual(decision['target_id'],'5')
        self.assertEqual(meta['input_mode'],'text_only')

    def test_fresh_last_seen_cue_can_request_reobservation(self):
        self.state['people']=[]
        self.state['recent_person_cues']=[{**self.person,'last_seen_age_seconds':1.0}]
        d={'action':'inspect_person','target_id':'5','reason':'재관측'}
        validate_decision(d,self.state)
        manager=MissionManager();manager.apply(d,self.state,MockNavigation(self.map))
        self.assertEqual(manager.last_observation_request['image_direction']['side'],'right')
        self.assertFalse(manager.last_observation_request['translation_allowed'])

    def test_old_cue_cannot_be_applied(self):
        self.state['people']=[]
        self.state['recent_person_cues']=[{**self.person,'last_seen_age_seconds':15.1}]
        with self.assertRaises(ValueError):
            validate_decision({'action':'inspect_person','target_id':'5','reason':'look'},self.state)

    def test_last_seen_direction_not_reused_after_mock_robot_moved(self):
        self.state['people']=[]
        self.state['recent_person_cues']=[{**self.person,'last_seen_age_seconds':1,
                                         'observed_robot_cell':[0,0],'source_epoch':'s1'}]
        with self.assertRaises(ValueError):
            validate_decision({'action':'inspect_person','target_id':'5','reason':'look'},self.state)

    def test_response_schema_limits_target_ids_per_action(self):
        schema=self.reasoner.build_payload(self.state)['response_format']['json_schema']['schema']
        inspect=next(s for s in schema['oneOf'] if s['properties']['action']['enum']==['inspect_person'])
        self.assertEqual(inspect['properties']['target_id']['enum'],['5'])

    def test_request_age_is_measured_from_start(self):
        context=DecisionContext('s1','Search',100,0)
        self.assertTrue(context.valid(self.state,110,0))
        self.assertFalse(context.valid(self.state,131,0))

    def test_hold_invalidates_inflight_decision(self):
        context=DecisionContext('s1','Search',100,0)
        self.assertFalse(context.valid(self.state,101,1))


if __name__=='__main__':unittest.main()
