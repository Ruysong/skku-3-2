# RoverAid — Live Perception & AI

A laptop demo that turns YOLO observations into continuous, local AI mission proposals.
The interface, mission prompt, observation summaries, and AI explanations are in English.

## Quick start

Double-click **Start Demo.cmd**. The GUI starts local Qwen and the pedestrian sample automatically.

- **Start / Stop** are the main controls.
- **Input** selects a video or webcam.
- **View > Details** shows current state, decisions, startup errors, and the output directory.
- No API key, shell command, manual AI request, or manual save step is required.

For console diagnostics:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File .\start_demo.ps1
```

## Continuous flow

```text
Live perception                 Accumulated observations
Person boxes, IDs, pose          Latest 5 seconds: people, sides, cues
           |                               |
           +-------------------------------+
                           |
                   Local Qwen reasoning
                           |
             Large action proposal + explanation
             Response time + decision history
```

Evidence accumulates before the GUI display queue. The first review starts after five seconds.
Only one AI request runs at a time. Slow calls are followed by the newest window, not a backlog.
Cancelled sessions are ignored; expired evidence is marked historical and is not applied.
Records and snapshots are saved automatically to `output/<timestamp>/`.

## Local model

The GUI starts and supervises its own loopback-only server. On this laptop it selects
**RTX 3050 / Vulkan**. GPU startup failure falls back to the bundled CPU runtime.
An already healthy server is reused. Closing the GUI stops only a server it started.

- Model: Qwen3-VL-2B-Instruct Q4_K_M.
- Input: structured text by default; optional images with `--attach-image`.
- Endpoint: `http://127.0.0.1:8091`.
- Logs: `runtime/local-model.log` and `runtime/application.log`.
- YOLO currently uses CPU PyTorch; Qwen GPU acceleration is independent.

## Scope

Body cues are uncertain pose evidence, not independent leg detection or identity/injury recognition.
A missed person detection cannot produce pose cues. Real partial-body performance needs further testing.
The screen displays proposals such as **Recheck person right** or **Continue exploration**.
There is no physical motor output. Image-side information is not a calibrated turn angle.
Map and navigation inputs are synthetic until a ROS 2 adapter is integrated.

## Modules

| Module | Responsibility |
|---|---|
| app.py | Video worker and entrypoint |
| console.py | English Start/Stop interface |
| continuous.py | Rolling evidence and non-queuing scheduler |
| perception.py / observations.py | Pose, tracking, body cues and short memory |
| reasoner.py | Compact input and constrained JSON decisions |
| local_model.py | Automatic GPU/CPU server startup and supervision |
| mission.py / navigation.py | Mission validation and mock navigation boundary |
| recording.py | Events, snapshots, summaries |

## Validation

```powershell
.\.venv\Scripts\python.exe -X utf8 -m unittest discover -s tests -v
```

The November 13 setup passed **58 tests** and syntax checks. The real pedestrian-video
and local-Qwen loop was exercised. GPU text reviews took roughly two seconds in this
sample; this is an observed result, not a latency guarantee. Server interruption and
recovery are recorded in the session events. `output/demo-readiness.json` summarizes the run.

The source, tests and development notes are archived in this coursework repository.
Models, runtimes, environments, samples and outputs are excluded from Git.
The November 13 validation above is a recorded result, not a fresh run in this documentation update.

## Setup and sources

Run `setup_environment.ps1` on a new Windows machine.
Sources: [Ultralytics](https://docs.ultralytics.com/tasks/pose/),
[Qwen GGUF](https://huggingface.co/Qwen/Qwen3-VL-2B-Instruct-GGUF),
[llama.cpp](https://github.com/ggml-org/llama.cpp),
[OpenCV sample](https://github.com/opencv/opencv/blob/4.x/samples/data/vtest.avi).
Observe their licenses when distributing code, models, or samples.
