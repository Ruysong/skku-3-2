# 캡스톤 RoverAid — 통신·인식·상위 판단의 연결

RoverAid는 재난탐사 로버 팀 프로젝트다. 송준혁은 PC–Pi 통신 런타임을 담당하고, 노트북에서 영상 인식·로컬 AI 판단·실행 상태를 연결하는 데모를 개발했다.

## 구현과 산출물

| 묶음 | 구현·수행 내용 | 관련 파일 |
|---|---|---|
| PC–Pi 통신 런타임 | Python 서버·C++ 클라이언트, 명령·telemetry, 길이 접두부 JSON, 재접속·3초 timeout MockMotor 정지 | [역할·보고서 정리](./9주차/study.md), [작성 보고서](./중간발표/RoverAid_Bi-Weekly_Report-1_.docx) |
| 노트북 인식·로컬 AI 데모 | YOLO 관측·추적, 시간 창, Qwen 제안, 대상·최신성 검증, GUI·기록 | [실행·구조 README](./개발/roveraid-exploration/README.md), [소스](./개발/roveraid-exploration/roveraid/), [tests](./개발/roveraid-exploration/tests/) |
| 제안·발표·진행 보고 | 문제 정의, 시스템 역할, 일정·통합 진행 | [산출물 목차](./pa-study.md), [제안서](./4주차/Team%20RoverAid_Capstone%20Design-Proposal_2026.pdf), [중간발표 폴더](./중간발표/) |

## 기술적 구성

- Python 노트북 서버와 C++ Pi 클라이언트의 명령·telemetry 채널을 구성했다. 실물 Pi Wi-Fi와 MockMotor로 통신·timeout 정지·재접속을 시험했다.
- 사전학습 YOLO·Qwen을 활용해 관측·상위 행동 제안·판단 검증을 연결하고, MockNavigation을 갖춘 노트북 실행 구조를 개발했다.
- 비동기 요청의 수명과 최신 관측을 관리하고 GUI·이벤트·스냅샷을 기록했다. [개발 기록](./개발/roveraid-exploration/docs/11.13-worknote.md).
- [통신 코드 저장소](https://github.com/Ruysong/autonomous-rescue-rover), 이 저장소의 [인식·AI 데모](./개발/roveraid-exploration/README.md), 팀 보고서·발표자료로 개발 과정을 볼 수 있다.

자소서·면접 준비는 [R1/R2 경험 카드](../경험-근거지도.md#r1--roveraid-pcpi-통신과-상태-피드백)에서 담당 역할·구현·결과를 읽고 시작한다.

## 수업 자료 찾기

[1주차](./1주차/study.md) · [2주차](./2주차/study.md) · [3주차](./3주차/study.md) · [4주차](./4주차/study.md) · [5주차](./5주차/study.md) · [6주차](./6주차/study.md) · [7주차](./7주차/study.md) · [9주차](./9주차/study.md) · [10주차](./10주차/study.md) · [11주차](./11주차/study.md) · [12주차](./12주차/study.md)

- [공지 원문](./notices.md) · [과제·퀴즈·제출 기록](./assignments.md) · [전체 일정](../tasks.md)
- [전체 경험·근거 지도](../경험-근거지도.md) · [저장소 홈](../README.md)

정리 기준: 2026-11-17. 각 프로젝트의 코드·보고서·학습 자료를 위 링크에서 볼 수 있다.
