import argparse
from pathlib import Path
import queue
import threading
import time

import cv2

from .mission import MockMap, baseline, world_state
from .reasoner import LocalReasoner
from .recording import Recorder

ROOT = Path(__file__).resolve().parents[1]


class VideoWorker(threading.Thread):
    def __init__(self, source, model, confidence, size, device, output, on_observation=None, session_id='video'):
        super().__init__(daemon=True)
        self.source, self.model, self.confidence = source, model, confidence
        self.size, self.device, self.output = size, device, output
        self.stop_event, self.paused = threading.Event(), threading.Event()
        self.epoch = 0
        self.on_observation=on_observation
        self.session_id=session_id

    def put(self, data):
        try:
            self.output.put_nowait(data)
        except queue.Full:
            try: self.output.get_nowait()
            except queue.Empty: pass
            self.output.put_nowait(data)

    def run(self):
        cap = None
        try:
            from .perception import Perception
            detector = Perception(self.model, self.confidence, self.size, self.device)
            camera = self.source.isdigit()
            cap = cv2.VideoCapture(int(self.source) if camera else self.source)
            if not cap.isOpened(): raise RuntimeError('Cannot open video source: ' + self.source)
            fps = cap.get(cv2.CAP_PROP_FPS)
            fps = fps if 1 <= fps <= 240 else 30
            index = 0
            read_failures = 0
            while not self.stop_event.is_set():
                if self.paused.is_set():
                    self.stop_event.wait(.05)
                    continue
                began = time.monotonic()
                ok, frame = cap.read()
                if not ok:
                    if camera: raise RuntimeError('Camera frame unavailable')
                    read_failures += 1
                    if read_failures > 1: raise RuntimeError('Video has no decodable frames')
                    cap.set(cv2.CAP_PROP_POS_FRAMES, 0)
                    detector.reset()
                    self.epoch += 1
                    index = 0
                    continue
                read_failures = 0
                observation = detector.process(frame, index/fps)
                epoch_id=f'{self.session_id}:{self.epoch}'
                observation.update(epoch=self.epoch,source=self.source,source_fps=fps,
                                   source_epoch=epoch_id,session=epoch_id.replace(':','-'))
                if self.on_observation:self.on_observation(observation)
                self.put(observation)
                index += 1
                # Frame-by-frame replay at <= source FPS. CPU inference may slow playback.
                self.stop_event.wait(max(0, 1/fps - (time.monotonic()-began)))
        except Exception as e:
            self.put({'error':str(e)})
        finally:
            if cap is not None: cap.release()




def headless(args):
    from .perception import Perception
    detector=Perception(ROOT/'models/yolo26n-pose.pt',args.conf,args.imgsz,args.device)
    recorder=Recorder(ROOT/'output');m=MockMap()
    cap=cv2.VideoCapture(int(args.source) if args.source.isdigit() else args.source)
    if not cap.isOpened():raise RuntimeError('Cannot open video')
    fps=cap.get(cv2.CAP_PROP_FPS) or 30
    observation=None
    writer=None
    try:
        for index in range(args.frames):
            ok,frame=cap.read()
            if not ok:break
            observation=detector.process(frame,index/fps)
            recorder.observation(observation)
            recorder.log('frame',frame=index,media_time=observation['media_time'],people=observation['people'],
                         inference_ms=observation['inference_ms'])
            if writer is None:
                h,w=frame.shape[:2]
                writer=cv2.VideoWriter(str(recorder.directory/'annotated.mp4'),cv2.VideoWriter_fourcc(*'mp4v'),fps,(w,h))
            writer.write(observation['image'])
            if observation['people']:recorder.image('evidence.jpg',observation['image'])
        if observation is None:raise RuntimeError('No decoded frames')
        state=world_state(observation,m,'Search for people',args.source,'headless')
        # Use the observation's timestamp; source is processed frame-by-frame.
        decision=baseline(state)
        if args.vlm:
            began=time.monotonic()
            decision,meta=LocalReasoner(args.endpoint).decide(state,observation['image'] if args.attach_image else None)
            recorder.log('vlm_decision',decision=decision,meta=meta,elapsed_seconds=time.monotonic()-began)
        recorder.summary(observation,state,decision)
        print(json.dumps({'output':str(recorder.directory),'frames':observation['frame'],
                          'tracks':len(observation['history']),'visible':len(observation['people']),
                          'decision':decision},ensure_ascii=False,indent=2))
    finally:
        cap.release()
        if writer:writer.release()
        recorder.close()


def main():
    parser=argparse.ArgumentParser()
    parser.add_argument('--source',default='0',help='Video filename, MJPEG URL or camera index')
    parser.add_argument('--conf',type=float,default=.35)
    parser.add_argument('--imgsz',type=int,default=960)
    parser.add_argument('--device',default='cpu')
    parser.add_argument('--endpoint',default='http://127.0.0.1:8091')
    parser.add_argument('--autostart',action='store_true')
    parser.add_argument('--headless',action='store_true')
    parser.add_argument('--frames',type=int,default=90)
    parser.add_argument('--vlm',action='store_true')
    parser.add_argument('--attach-image',action='store_true',help='Optional visual input; AI requests default to text only')
    parser.add_argument('--no-auto-model',action='store_true',help='Connect to an existing server without starting a local model')
    args=parser.parse_args()
    if args.headless:headless(args)
    else:
        from .console import Console
        Console(args).root.mainloop()


if __name__=='__main__':main()
