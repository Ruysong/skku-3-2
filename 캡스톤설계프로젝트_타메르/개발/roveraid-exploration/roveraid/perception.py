from collections import Counter, deque
from dataclasses import asdict, dataclass, field
from pathlib import Path
import math
import time

import cv2
import numpy as np
import torch
from ultralytics import YOLO
from .observations import body_evidence


def cpu_numpy(value):
    return value.detach().cpu().numpy() if isinstance(value,torch.Tensor) else np.asarray(value)


@dataclass
class Person:
    track_id: int | str
    confidence: float
    box: list
    posture: str
    first_seen: float
    last_seen: float
    observations: int = 1
    history: deque = field(default_factory=lambda: deque(maxlen=7), repr=False)
    evidence: dict = field(default_factory=dict)

    def public(self):
        return {**{k: v for k, v in asdict(self).items() if k not in ('history','evidence')}, **self.evidence}


def posture_candidate(box, points, confidence):
    """A visible-geometry heuristic, not a medical or trained fall classifier."""
    if points is None or confidence is None:
        return 'unknown'
    visible = [i for i, c in enumerate(confidence) if c >= .40]
    if len(visible) < 6 or not all(i in visible for i in (5, 6, 11, 12)):
        return 'unknown'
    shoulders = (points[5] + points[6]) / 2
    hips = (points[11] + points[12]) / 2
    dx, dy = abs(float(hips[0] - shoulders[0])), float(hips[1] - shoulders[1])
    width, height = max(1, box[2] - box[0]), max(1, box[3] - box[1])
    if math.hypot(dx, dy) < .08 * height:
        return 'unknown'
    if dx > abs(dy) * 1.4 and width / height > .85:
        return 'lying candidate'
    if dy > dx * 1.3 and dy > height * .12:
        if all(i in visible for i in (13, 14)):
            knees = (points[13] + points[14]) / 2
            if abs(float(knees[1] - hips[1])) < .17 * height:
                return 'seated candidate'
        return 'upright candidate'
    return 'unknown'


class Perception:
    def __init__(self, model: Path, confidence=.35, image_size=960, device='cpu'):
        if device == 'cpu':
            torch.set_num_threads(4)
        self.model = YOLO(str(model))
        self.confidence, self.image_size, self.device = confidence, image_size, device
        self.people = {}
        self.frame_number = 0

    def reset(self):
        # Avoid carrying tracker identities across files or replay boundaries.
        for tracker in getattr(getattr(self.model, 'predictor', None), 'trackers', []):
            tracker.reset()
        self.people.clear()
        self.frame_number = 0

    def process(self, frame, media_time):
        started = time.monotonic()
        result = self.model.track(frame, persist=True, tracker='bytetrack.yaml',
                                  conf=self.confidence, imgsz=self.image_size,
                                  device=self.device, verbose=False)[0]
        self.frame_number += 1
        annotated = result.plot(labels=False, conf=False)
        visible = []
        boxes = result.boxes
        if boxes is not None and len(boxes) > 0:
            coords = cpu_numpy(boxes.xyxy)
            ids = cpu_numpy(boxes.id).astype(int).tolist() if boxes.id is not None else [None]*len(boxes)
            scores = cpu_numpy(boxes.conf).tolist()
            xy = cpu_numpy(result.keypoints.xy) if result.keypoints is not None else None
            cf = cpu_numpy(result.keypoints.conf) if result.keypoints is not None and result.keypoints.conf is not None else None
            for i, (associated_id, box, score) in enumerate(zip(ids, coords, scores)):
                tid = associated_id if associated_id is not None else f'untracked-{self.frame_number}-{i}'
                pose = posture_candidate(box, xy[i] if xy is not None else None,
                                         cf[i] if cf is not None else None)
                p = self.people.get(tid)
                if p is None:
                    p = Person(tid, score, box.tolist(), 'unknown', media_time, media_time, 0)
                    if associated_id is not None:self.people[tid] = p
                p.observations += 1
                p.confidence, p.box, p.last_seen = score, box.tolist(), media_time
                p.evidence = body_evidence(box, xy[i] if xy is not None else None,
                                          cf[i] if cf is not None else None, frame.shape)
                p.evidence['track_status'] = 'associated' if associated_id is not None else 'untracked_single_frame'
                p.history.append(pose)
                # Require at least three matching recent observations.
                ranking = Counter(p.history).most_common(2)
                p.posture = ranking[0][0]
                if ranking[0][1] < 3 or (len(ranking)>1 and ranking[0][1]==ranking[1][1]):
                    p.posture = 'unknown'
                visible.append(p.public())
                label_id=str(tid) if associated_id is not None else '?'
                text = f"#{label_id} {p.evidence['image_direction']['side'][0].upper()}"
                if p.evidence['body_cue']=='lower_body_hint':text+=' / lower-body cue'
                size,_=cv2.getTextSize(text,cv2.FONT_HERSHEY_SIMPLEX,.55,2)
                x=min(max(0,int(box[0])),max(0,frame.shape[1]-size[0]-8))
                y=max(24,int(box[1])-10)
                cv2.rectangle(annotated,(x,y-size[1]-5),(x+size[0]+5,y+4),(15,25,35),-1)
                cv2.putText(annotated, text, (x, y), cv2.FONT_HERSHEY_SIMPLEX, .55, (30, 245, 240), 2)
        elapsed = time.monotonic() - started
        cv2.putText(annotated, f'Visible: {len(visible)} | Session tracks: {len(self.people)}',
                    (14, 32), cv2.FONT_HERSHEY_SIMPLEX, .65, (255, 255, 255), 2)
        return {'image': annotated, 'people': visible,
                'history': [p.public() for p in self.people.values()],
                'media_time': round(media_time, 3), 'frame': self.frame_number,
                'inference_ms': round(elapsed * 1000, 1), 'observed_at': time.monotonic()}
