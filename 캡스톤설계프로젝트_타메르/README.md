# 캡스톤 RoverAid — 통신·인식·상위 판단의 연결

RoverAid는 재난탐사 로버 팀 프로젝트다. **보고서에 명시된 송준혁의 통신 담당**, 노트북 인식·로컬 AI 데모, 팀 전체의 기구·구동·주행 계획을 나누어 읽는다.

## 구현과 산출물

| 묶음 | 확인된 내용 | 근거와 범위 |
|---|---|---|
| PC–Pi 통신 런타임 | Python 서버·C++ 클라이언트, 명령·telemetry, 길이 접두부 JSON, 재접속·3초 timeout MockMotor 정지 | [역할·보고서 정리](./9주차/study.md), [작성 보고서](./중간발표/RoverAid_Bi-Weekly_Report-1_.docx). 본인 담당이 보고서에 명시됨 |
| 노트북 인식·로컬 AI 데모 | YOLO 관측·추적, 시간 창, Qwen 제안, 대상·최신성 검증, GUI·기록 | [실행·구조 README](./개발/roveraid-exploration/README.md), [소스](./개발/roveraid-exploration/roveraid/), [tests](./개발/roveraid-exploration/tests/) |
| 제안·발표·진행 보고 | 문제 정의, 시스템 역할, 일정·통합 진행 | [산출물 목차](./pa-study.md), [제안서](./4주차/Team%20RoverAid_Capstone%20Design-Proposal_2026.pdf), [중간발표 폴더](./중간발표/) |

## 읽을 때 구분할 경계

- 실물 Pi의 MockMotor 통신 검증 기록과 실제 모터 구동·로버 전체 자율주행 완료를 구분한다.
- 인식 데모는 사전학습 YOLO·Qwen을 사용한다. 모델을 새로 학습한 결과로 적지 않는다.
- 노트북 데모의 navigation·map은 mock 경계가 있으며 실제 모터 출력은 없다. [11/13 개발 기록](./개발/roveraid-exploration/docs/11.13-worknote.md)을 함께 읽는다.
- [외부 통신 코드 저장소](https://github.com/Ruysong/autonomous-rescue-rover)의 최신 구현·테스트는 별도로 확인한다. 이 저장소가 모든 Pi/Pico/ROS 코드를 포함하는 것은 아니다.
- 템플릿과 실제 작성 발표자료는 파일 이름과 내용으로 구분한다. 팀 산출물 전체를 개인 단독 기여로 처리하지 않는다.

자소서·면접은 [R1/R2 경험 카드](../경험-근거지도.md#r1--roveraid-pcpi-통신과-상태-피드백)에서 역할·근거·확인할 질문을 먼저 읽는다.

## 수업 자료 찾기

[1주차](./1주차/study.md) · [2주차](./2주차/study.md) · [3주차](./3주차/study.md) · [4주차](./4주차/study.md) · [5주차](./5주차/study.md) · [6주차](./6주차/study.md) · [7주차](./7주차/study.md) · [9주차](./9주차/study.md) · [10주차](./10주차/study.md) · [11주차](./11주차/study.md) · [12주차](./12주차/study.md)

- [공지 원문](./notices.md) · [과제·퀴즈·제출 기록](./assignments.md) · [전체 일정](../tasks.md)
- [전체 경험·근거 지도](../경험-근거지도.md) · [저장소 홈](../README.md)

최신 정리: 2026-11-17. 소스·보고서의 보관 상태와 수강·개인 학습 완료는 별도로 관리한다.
