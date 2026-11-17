import unittest

from roveraid.continuous import ObservationWindow, ReviewScheduler, observation_lines, decision_title


def person(tid=5,side='right',cue='lower_body_hint'):
    return {'track_id':tid,'confidence':.9,'image_direction':{'side':side,'center_x_normalized':.8},
            'body_cue':cue,'posture':'unknown','supported_keypoints':['right_knee','right_ankle']}


def frame(stamp,people=None,epoch='1:0'):
    return {'observed_at':stamp,'people':people or [],'source_epoch':epoch,'frame':int(stamp*10),
            'media_time':stamp,'source':'SYNTHETIC_TEST','image':'not copied into memory'}


class WindowTests(unittest.TestCase):
    def test_aggregates_all_frames_and_does_not_store_images(self):
        w=ObservationWindow()
        for t in (0,1,2):w.add(frame(t,[person()]))
        s=w.snapshot(2)
        self.assertEqual(s['window']['processed_frames'],3)
        self.assertEqual(s['window']['tracks'][0]['frames_seen'],3)
        self.assertEqual(s['window']['tracks'][0]['image_sides'],{'right':3})
        self.assertNotIn('image',s['latest'])

    def test_window_is_latest_five_seconds(self):
        w=ObservationWindow()
        for t in range(13):w.add(frame(t,[person(tid=t)]))
        s=w.snapshot(12)
        self.assertEqual(s['window']['processed_frames'],6)
        self.assertEqual(s['total_frames'],13)
        self.assertEqual({p['track_id'] for p in s['window']['tracks']},{7,8,9,10,11,12})

    def test_brief_cue_remains_when_current_frame_has_no_person(self):
        w=ObservationWindow();w.add(frame(0,[person()]));w.add(frame(1,[]))
        s=w.snapshot(1)
        self.assertEqual(s['window']['current_visible_count'],0)
        self.assertEqual(s['window']['tracks'][0]['body_cue_counts'],{'lower_body_hint':1})
        self.assertEqual(s['recent_people'][0]['track_id'],5)

    def test_replay_epoch_resets_ids_history_and_window_start(self):
        w=ObservationWindow();w.add(frame(0,[person()]));w.add(frame(8,[],epoch='1:1'))
        s=w.snapshot(8)
        self.assertEqual(s['window']['observed_track_count'],0)
        self.assertEqual(s['first_observed_at'],8)
        self.assertEqual(s['recent_people'],[])

    def test_ingest_copies_evidence_before_caller_mutates_it(self):
        p=person();w=ObservationWindow();w.add(frame(0,[p]));p['image_direction']['side']='left'
        self.assertEqual(w.snapshot(0)['window']['tracks'][0]['image_sides'],{'right':1})

    def test_human_summary_distinguishes_window_from_current_count(self):
        w=ObservationWindow();w.add(frame(0,[person()]));w.add(frame(1,[]))
        text=observation_lines(w.snapshot(1))
        self.assertIn('0 visible now',text);self.assertIn('Right',text);self.assertIn('lower-body cue',text)


class SchedulerTests(unittest.TestCase):
    def test_first_request_waits_for_five_seconds_of_observation(self):
        w=ObservationWindow();g=ReviewScheduler()
        w.add(frame(10,[person()]))
        self.assertFalse(g.due(14,w.snapshot(14)))
        w.add(frame(15,[person()]))
        self.assertTrue(g.due(15,w.snapshot(15)))

    def test_busy_model_does_not_accumulate_requests(self):
        w=ObservationWindow();g=ReviewScheduler();w.add(frame(0,[person()]));g.begin(5)
        for t in (10,15,16):
            w.add(frame(t,[person()]));self.assertFalse(g.due(t,w.snapshot(t)))
        self.assertEqual(g.request_count,1)
        with self.assertRaises(RuntimeError):g.begin(16)
        g.finish()
        self.assertTrue(g.due(16,w.snapshot(16)))
        g.begin(16);self.assertEqual(g.request_count,2)
        self.assertEqual(w.snapshot(16)['window']['processed_frames'],2)

    def test_fast_model_still_waits_for_next_period(self):
        w=ObservationWindow();g=ReviewScheduler();w.add(frame(0));g.begin(5);g.finish()
        w.add(frame(7))
        self.assertFalse(g.due(7,w.snapshot(7)))
        w.add(frame(10));self.assertTrue(g.due(10,w.snapshot(10)))

    def test_stale_video_blocks_new_requests(self):
        w=ObservationWindow();g=ReviewScheduler();w.add(frame(0,[person()]))
        self.assertFalse(g.due(8,w.snapshot(8)))

    def test_restart_does_not_allow_second_call_while_old_one_runs(self):
        g=ReviewScheduler();g.begin(5);g.restart()
        self.assertTrue(g.busy);self.assertEqual(g.last_started,5)

    def test_direction_is_rendered_as_reobservation_not_a_turn_angle(self):
        d={'action':'inspect_person','target_id':'5','reason':'test'}
        self.assertEqual(decision_title(d,{'people':[person()]}),'Recheck person right')


if __name__=='__main__':unittest.main()
