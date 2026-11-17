# 캡스톤 프로젝트 학습목차

이 과목은 PA 번호로 구분되는 개별 코드 과제가 아니라 팀 프로젝트 산출물로 관리한다.

| 산출물 | 수행 내용 | 보관 |
| --- | --- | --- |
| RoverAid Proposal | 12페이지 제안서, 문제·구조·역할·평가 계획 | [제안서 PDF](./4주차/Team%20RoverAid_Capstone%20Design-Proposal_2026.pdf) |
| Proposal Presentation | 발표 자료 파일 | [발표 PPTX](./5주차/RoverAid_Proposal_Presentation.pptx) |
| 프로젝트 구조 학습 | 데이터·명령 흐름과 모듈별 역할 | [설계 교안](./4주차/project-study.md) |
| 중간발표 템플릿 | 발표 구성용 13슬라이드 양식 | [PPTX](./중간발표/CapstoneDesign-MidPresentation-Template2026.pptx) |
| Bi-Weekly Report | 양식과 작성본을 분리. 작성본에 본인 통신 런타임 구현·MockMotor 검증 기록 | [9주차 보고서 자료·진행 목차](./9주차/study.md) |

제안서·발표자료·Bi-Weekly Report에 팀 역할과 구현 진행을 정리했다. 송준혁은 Python 노트북 서버와 C++ Pi 클라이언트를 연결하는 통신 런타임을 담당했고, 실물 Pi Wi-Fi·MockMotor로 명령·상태·정지·재접속을 시험했다. [통신 코드 저장소](https://github.com/Ruysong/autonomous-rescue-rover).

## 11/17 최신 구현 연결

- [노트북 인식·로컬 AI 데모](./개발/roveraid-exploration/README.md)는 영상 관측·시간 창·행동 제안·판단 검증·GUI·기록을 연결한다. 소스·tests·개발 기록을 함께 보관했다.
- PC–Pi 통신은 외부 저장소, 노트북 인식·AI 데모는 이 저장소의 개발 폴더에서 읽는다.
- 자소서·면접용 역할·구현·결과는 [R1/R2 경험 카드](../경험-근거지도.md#r1--roveraid-pcpi-통신과-상태-피드백)로 연결한다.
