"""Run a local person-detection demo on a video file or webcam.

Example: python person_demo.py --source corridor.mp4 --show
This demo never sends commands to the rover.
"""

from __future__ import annotations

import argparse
import json
import math
from dataclasses import dataclass
from pathlib import Path


@dataclass
class DetectionGate:
    required_frames: int = 3
    consecutive_frames: int = 0
    confirmed: bool = False

    def update(self, detected: bool) -> bool:
        if self.confirmed:
            return False
        self.consecutive_frames = self.consecutive_frames + 1 if detected else 0
        if self.consecutive_frames < self.required_frames:
            return False
        self.confirmed = True
        return True


def parse_source(value: str) -> str | int:
    if value.isdecimal():
        return int(value)
    source = Path(value).expanduser().resolve()
    if not source.is_file():
        raise argparse.ArgumentTypeError(f"Video file not found: {source}")
    return str(source)


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--source", type=parse_source, required=True, help="MP4 path or webcam index, e.g. 0")
    parser.add_argument("--output", type=Path, default=Path(__file__).parent / "output")
    parser.add_argument("--model", default="yolo26n.pt", help="COCO person-detection model")
    parser.add_argument("--confidence", type=float, default=0.4)
    parser.add_argument("--confirm-frames", type=int, default=3)
    parser.add_argument("--show", action="store_true", help="Show the annotated video while processing")
    args = parser.parse_args()
    if not 0 < args.confidence <= 1:
        parser.error("--confidence must be in (0, 1]")
    if args.confirm_frames < 1:
        parser.error("--confirm-frames must be at least 1")
    return args


def main() -> None:
    args = parse_args()
    try:
        import cv2
        from ultralytics import YOLO
    except ImportError as exc:
        raise SystemExit("Install dependencies with: python -m pip install -r requirements.txt") from exc

    args.output.mkdir(parents=True, exist_ok=True)
    model = YOLO(args.model)
    if model.names[0] != "person":
        raise SystemExit("The selected model must use COCO class 0 for 'person'.")

    capture = cv2.VideoCapture(args.source)
    if not capture.isOpened():
        raise SystemExit(f"Could not open video source: {args.source}")

    fps = capture.get(cv2.CAP_PROP_FPS)
    if not math.isfinite(fps) or fps <= 0:
        fps = 20.0
    gate = DetectionGate(required_frames=args.confirm_frames)
    event = None
    frame_index = 0
    video_path = args.output / "annotated.mp4"
    writer = None

    try:
        while True:
            ok, frame = capture.read()
            if not ok:
                break
            frame_index += 1
            if writer is None:
                height, width = frame.shape[:2]
                fourcc = cv2.VideoWriter_fourcc(*"mp4v")
                writer = cv2.VideoWriter(str(video_path), fourcc, fps, (width, height))
                if not writer.isOpened():
                    raise RuntimeError(f"Could not create output video: {video_path}")

            result = model.predict(frame, classes=[0], conf=args.confidence, verbose=False)[0]
            best_confidence = None
            if result.boxes is not None and len(result.boxes):
                best_confidence = max(float(value) for value in result.boxes.conf)
            if gate.update(best_confidence is not None):
                event = {
                    "frame": frame_index,
                    "video_seconds": round((frame_index - 1) / fps, 2),
                    "person_confidence": round(best_confidence, 3),
                    "action": "stop_requested_simulated",
                }
                print(f"[{event['video_seconds']:.2f}s] Person candidate confirmed")
                print("STOP REQUESTED (SIMULATED; nothing was sent to the rover)")

            annotated = result.plot()
            if gate.confirmed:
                status = "PERSON CANDIDATE - STOP (SIMULATED)"
            elif gate.consecutive_frames:
                status = f"VERIFYING PERSON ({gate.consecutive_frames}/{gate.required_frames})"
            else:
                status = "SEARCHING"
            height, width = annotated.shape[:2]
            cv2.rectangle(annotated, (0, 0), (width, 82), (20, 20, 20), -1)
            cv2.putText(annotated, "MISSION: FIND PERSON", (14, 30), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255, 255, 255), 2)
            cv2.putText(annotated, status, (14, 65), cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 215, 255), 2)
            writer.write(annotated)
            if event is not None and event["frame"] == frame_index:
                cv2.imwrite(str(args.output / "discovery.jpg"), annotated)

            if args.show:
                cv2.imshow("Rover person-detection demo", annotated)
                if cv2.waitKey(1) & 0xFF == ord("q"):
                    break
    finally:
        capture.release()
        if writer is not None:
            writer.release()
        if args.show:
            cv2.destroyAllWindows()

    if writer is None:
        raise SystemExit("No frames were read from the video source.")
    report = {
        "source": str(args.source),
        "model": args.model,
        "processed_frames": frame_index,
        "person_candidate_found": event is not None,
        "event": event,
        "rover_command_sent": False,
    }
    report_path = args.output / "report.json"
    report_path.write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(f"Annotated video: {video_path}")
    print(f"Detection report: {report_path}")


if __name__ == "__main__":
    main()
