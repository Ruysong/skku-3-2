import unittest

from person_demo import DetectionGate, parse_source


class DetectionGateTests(unittest.TestCase):
    def test_requires_consecutive_frames(self):
        gate = DetectionGate(required_frames=3)
        self.assertFalse(gate.update(True))
        self.assertFalse(gate.update(False))
        self.assertFalse(gate.update(True))
        self.assertFalse(gate.update(True))
        self.assertTrue(gate.update(True))
        self.assertFalse(gate.update(True))

    def test_webcam_index(self):
        self.assertEqual(parse_source("0"), 0)


if __name__ == "__main__":
    unittest.main()
