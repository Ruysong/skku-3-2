import copy
import time
import unittest
import numpy as np

from roveraid.mission import MockMap, MissionManager, baseline, world_state, validate_decision
from roveraid.navigation import MockNavigation
from roveraid.perception import posture_candidate


class MissionTests(unittest.TestCase):
    def setUp(self):
        self.map = MockMap()
        self.navigation = MockNavigation(self.map)
        self.observation = {'people':[], 'observed_at':time.monotonic(), 'frame':1, 'media_time':0}
        self.state = world_state(self.observation,self.map,'Search','test.mp4','1:0')

    def test_frontiers_are_reachable_known_free(self):
        self.assertTrue(self.state['frontiers'])
        for f in self.state['frontiers']:
            self.assertIn(tuple(f['cell']),self.map.reachable())
            self.assertTrue(any(self.map.grid[y][x]==-1 for x,y in self.map.neighbors(f['cell'])))

    def test_baseline_exploration_executes_and_reveals(self):
        before=sum(v==-1 for r in self.map.grid for v in r)
        manager=MissionManager()
        self.assertEqual(manager.apply(baseline(self.state),self.state,self.navigation),'MOCK NAVIGATING')
        for _ in range(80):self.map.step()
        after=sum(v==-1 for r in self.map.grid for v in r)
        self.assertLess(after,before)
        self.assertTrue(self.map.visited_goals)

    def test_stale_video_holds(self):
        self.state['video_age_seconds']=4
        self.assertEqual(baseline(self.state)['action'],'hold')
        self.assertIn('stale',MissionManager().apply({'action':'hold','target_id':None,'reason':'wait'},self.state,self.navigation))

    def test_visible_person_preempts_exploration(self):
        d=baseline(self.state)
        self.state['people']=[{'track_id':4,'confidence':.9}]
        self.assertEqual(baseline(self.state)['action'],'inspect_person')
        self.assertIn('VERIFY',MissionManager().apply(d,self.state,self.navigation))
        self.assertFalse(self.map.route)

    def test_unknown_target_rejected(self):
        with self.assertRaises(ValueError):
            validate_decision({'action':'explore','target_id':'invented','reason':'go'},self.state)

    def test_arbitrary_motor_command_rejected(self):
        with self.assertRaises(ValueError):
            validate_decision({'action':'drive','target_id':None,'reason':'go'},self.state)

    def test_extra_model_fields_rejected(self):
        with self.assertRaises(ValueError):
            validate_decision({'action':'hold','target_id':None,'reason':'go','speed':.5},self.state)

    def test_lost_person_target_rejected(self):
        with self.assertRaises(ValueError):
            validate_decision({'action':'inspect_person','target_id':'9','reason':'look'},self.state)

    def test_walls_not_navigable(self):
        self.assertFalse(self.map.navigate((0,0)))

    def test_cancel_discards_route(self):
        self.map.navigate(self.state['frontiers'][0]['cell'])
        pose=self.map.pose
        self.map.cancel();self.map.step()
        self.assertEqual(pose,self.map.pose)

    def test_no_metric_person_position_invented(self):
        self.assertIn('unknown',self.state['person_coordinates'])
        self.assertIn('synthetic',self.state['map_source'])


class PostureTests(unittest.TestCase):
    def test_occluded_body_is_unknown(self):
        self.assertEqual(posture_candidate([0,0,50,150],np.zeros((17,2)),np.zeros(17)),'unknown')

    def test_horizontal_body_candidate(self):
        points=np.zeros((17,2));points[5]=[20,30];points[6]=[20,50]
        points[11]=[90,30];points[12]=[90,50]
        self.assertEqual(posture_candidate([0,0,150,70],points,np.ones(17)),'lying candidate')

    def test_upright_body_candidate(self):
        points=np.zeros((17,2));points[5]=[20,20];points[6]=[40,20]
        points[11]=[20,80];points[12]=[40,80];points[13]=[20,120];points[14]=[40,120]
        self.assertEqual(posture_candidate([0,0,60,170],points,np.ones(17)),'upright candidate')


if __name__=='__main__':unittest.main()
