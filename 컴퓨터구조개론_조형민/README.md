# 컴퓨터구조개론 — RISC-V 해독기와 실행 시뮬레이터

ISA·프로세서·메모리 계층을 배우며, C++로 **명령어를 해독하는 도구**와 **명령어가 상태를 바꾸는 시뮬레이터**를 순서대로 구현한 개인 과제를 보관한다.

## 대표 구현

| 과제 | 수행 내용 | 관련 파일 |
|---|---|---|
| Project 1 | little-endian 바이너리, 37개 RV32I 명령 해독, 비트 필드·immediate·부호 확장 | [구현 설명](./PA/PA_1/study.md), [main.cpp](./PA/PA_1/main.cpp), [Makefile](./PA/PA_1/Makefile) |
| Project 2 | 27개 RV32I 실행, 레지스터·PC·64KiB 메모리, 분기·jump·콘솔 I/O | [구현·검증 기록](./PA/PA_2/study.md), [main.cpp](./PA/PA_2/main.cpp), [Makefile](./PA/PA_2/Makefile) |

Proj2는 공개 예제 11개에서 reference와 출력이 일치했고, 경계 주소·음수·x0·분기·jump 등에 대한 추가 비교를 진행했다. 11/16 서버 제출을 마쳤다. 구현 구조와 ISA 검토 과정은 상세 기록에서 볼 수 있다.

자소서에서는 [C1/C2 경험 카드](../경험-근거지도.md#c1c2--risc-v-명령어-해독에서-실행-상태-관리로), 전체 과제는 [pa-study.md](./pa-study.md)를 읽는다. 명령어 비트에서 실행 상태 변화까지 연결하는 과정을 정리했다.

## 강의·시험 자료

성능 분석 → RISC-V 명령어 → 산술·데이터패스 → pipeline·hazards → cache·memory hierarchy로 연결한다.

- [중간 대비](./midterm-prep/목차.md) · [기말 대비 PDF 5개](./final-prep/)
- 2020~2022 문제의 RISC-V 변환본과 2023 기출·2024 샘플을 모아 기말 대비 자료로 구성했다.

## 수업 자료 찾기

[1주차](./1주차/study.md) · [2주차](./2주차/study.md) · [3주차](./3주차/study.md) · [4주차](./4주차/study.md) · [5주차](./5주차/study.md) · [6주차](./6주차/study.md) · [7주차](./7주차/study.md) · [9주차](./9주차/study.md) · [10주차](./10주차/study.md) · [11주차](./11주차/study.md) · [12주차](./12주차/study.md)

- [공지 원문](./notices.md) · [과제·퀴즈·제출 기록](./assignments.md) · [전체 일정](../tasks.md)
- [전체 경험·근거 지도](../경험-근거지도.md) · [저장소 홈](../README.md)

정리 기준: 2026-11-17. 각 프로젝트의 코드·보고서·학습 자료를 위 링크에서 볼 수 있다.
