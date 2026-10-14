# RoverAid: 제안서 기반 시스템 설계 교안

**한 줄 결론:** 저비용 6륜 로버에서 Raspberry Pi는 장치 제어·통신·안전을, 노트북은 인지·상황 해석·항법을 담당하도록 설계한 반자율 재난 탐색 프로젝트다.

근거: [최종 제안서 PDF](./Team%20RoverAid_Capstone%20Design-Proposal_2026.pdf), [발표 자료 파일](../5주차/RoverAid_Proposal_Presentation.pptx). 아래 세부 설계는 제안서의 내용이며, 아직 로컬 소스로 확인되지 않은 기능을 구현 완료라고 쓰지 않는다.

## 목차

1. [산출물과 구현 상태](#산출물과-구현-상태)
2. [전체 계층](#전체-계층)
3. [부품·통신·World State](#부품통신world-state)
4. [항법과 안전](#항법과-안전)
5. [역할·평가·확장 목표](#역할평가확장-목표)

## 산출물과 구현 상태

| 내용 | 확인 수준 |
| --- | --- |
| 문제 정의·제안 구조·역할·개발 계획 | 제출 제안서에서 확인 |
| 발표 산출물 | PPTX 파일 존재, 사용자 완성 확인 |
| 기구·3D 프린팅 관련 자료 | 로컬 자료 존재. 이번 구현 아카이브에 중복 복사하지 않음 |
| 모터 제어·통신·센서 코드 | 이 과목 폴더에서 완성 소스 미확인 |
| 안전 정지·waypoint 주행·인지 실험 | 실행 로그나 측정값 미확인 |

제안서에는 “will”, “planned”, “may”로 구분된 목표가 있다. 설계 목록을 완료 체크리스트처럼 읽지 않는다. 보고서가 추가되면 실제 기능별 소스 경로·revision·실험 결과를 이 표에 연결한다.

## 전체 계층

```text
Human operator / high-level mission
                 |
Laptop: perception -> World State -> navigation
                 |
        command channel (TCP)
                 |
Raspberry Pi: message validation / local safety
                 |
        kinematics / hardware control
                 |
       motor and steering drivers
                 |
       6-wheel drive / 4-wheel steering

Sensors -> Pi -> telemetry(TCP) -> Laptop
Camera  -> Pi -> video(UDP)     -> Laptop
```

핵심은 AI inference 결과를 모터 PWM에 직접 연결하지 않는 것이다. 인지는 상황 정보를 만들고, navigation이 이동 명령을 만들며, 로봇 쪽 validation과 safety가 마지막으로 허용 여부를 결정한다. 노트북이나 연결이 불안정해도 최소한의 정지 판단은 로봇에 남아야 한다는 설계다.

## 부품·통신·World State

제안서 p.5~6의 플랫폼은 Sawppy 개념의 6륜 구동·4륜 조향이며, JGB37-520 모터 6개, MG996R 조향 servo 4개, PCA9685, RPLIDAR C1, BNO055, 초음파 센서, USB camera, Raspberry Pi 4를 계획한다. Pico 2는 timing-sensitive sensing이 필요할 때의 선택 요소다. 부품 목록이 구매·조립 완료 증거는 아니다.

Pi의 Linux scheduling 때문에 microsecond 단위 초음파 pulse 측정의 변동이 생길 수 있어, 선택적으로 Pico에서 측정하고 처리된 값을 Pi로 전달하는 구상이다. 이 설계를 실제 sensor driver 코드가 있는 것처럼 표현하지 않는다.

통신은 command와 telemetry에 TCP, video에 UDP를 계획한다. 신뢰성 있는 메시지 전달과 지연에 민감한 영상의 목적 차이를 반영한 선택이다. 제안서에 아직 구체적인 framing, serialization, sequence, timeout 숫자까지 확정된 것은 아니므로 임의의 JSON schema나 packet format을 최종 구현처럼 추가하지 않았다.

World State는 robot pose, detected people, obstacles, map, hazards, communication state, mission 등의 구조적 정보다. 한 장면을 단일 재난 label 하나로만 나누지 않고 사람·연기·막힌 통로 등 여러 관찰을 동시에 담는 목표다. 어떤 모델·threshold·데이터셋으로 구현했는지는 이번 소스 확인 범위에 없다.

## 항법과 안전

제안서는 Manual Mode를 개발·하드웨어 시험·fallback으로 사용하고 Semi-Autonomous Mode에서는 사람이 waypoint 또는 mission을 지정하도록 한다. encoder odometry와 IMU를 우선 연결하고, LiDAR mapping 또는 SLAM은 안정성·진행 상황에 따라 확장한다. **full SLAM은 최초 시연의 필수 전제라고 명시되어 있지 않다.**

```text
target waypoint
      |
navigation -> steering/velocity command
      |
robot-side safety
      +-> connection lost? -> stop
      +-> command timeout? -> stop
      +-> malformed/unsupported? -> reject
      +-> range exceeded? -> constrain/reject
      |
hardware control
```

제안서 p.9의 safety에는 연결 손실, 유효 명령 timeout, 잘못된 명령 거부, 속도·조향 유효 범위, 안전 종료, 센서 기반 비상 상태가 있다. TCP 연결이 있다는 사실만으로 command timeout이 자동 구현되는 것은 아니다. 실제 코드와 시험에서는 각 조건이 별도 상태 전이로 확인되어야 한다.

## 역할·평가·확장 목표

송준혁의 초기 역할은 robot-side software, Raspberry Pi와 system integration이다. 제안서 p.12에서는 통신·로봇 소프트웨어 기반을 Song, motor/sensor/telemetry를 Kang+Song, World State·perception 및 localization/navigation을 Lee+Song, 전체 통합·평가를 All로 배정했다. 일정표는 당시 계획이며 완료 로그가 아니다.

평가 계획은 다음을 다룬다.

- 인지: 사람·선택 hazard의 detection, obstacle reliability.
- 항법: waypoint 성공률, 소요 시간, 충돌·비상 정지·사람 개입 횟수.
- 위치: ground truth를 확보할 수 있는 환경에서 odometry/localization error.
- 통신·안전: command/telemetry reliability, 지연, 연결 손실 후 정지 시간, timeout 동작.
- 전체: 사전 정의한 탐색 시나리오와 상황 보고, 수동/반자율 비교.

agent-based mission reasoning과 자동 상황 보고는 통신·인지·항법 기반이 안정된 뒤의 확장 목표다. agent는 World State에서 mission을 제안하되 모터 PWM을 직접 생성하거나 Pi safety를 우회하지 않는다.

이번 문서는 프로젝트가 **무엇을 만들려고 했고, 자료상 어디까지 확인되는지**를 정리한 것이다. 구현 완료 목록은 실제 코드·테스트가 들어온 뒤 별도 근거로 확장한다.

[캡스톤 프로젝트 목차](../pa-study.md)
