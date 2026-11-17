# 컴퓨터구조개론 — RISC-V 해독기와 실행 시뮬레이터

ISA·프로세서·메모리 계층을 배우며, C++로 **명령어를 해독하는 도구**와 **명령어가 상태를 바꾸는 시뮬레이터**를 순서대로 구현한 개인 과제를 보관한다.

## 대표 구현

| 과제 | 수행 내용 | 먼저 볼 근거 |
|---|---|---|
| Project 1 | little-endian 바이너리, 37개 RV32I 명령 해독, 비트 필드·immediate·부호 확장 | [구현 설명](./PA/PA_1/study.md), [main.cpp](./PA/PA_1/main.cpp), [Makefile](./PA/PA_1/Makefile) |
| Project 2 | 27개 RV32I 실행, 레지스터·PC·64KiB 메모리, 분기·jump·콘솔 I/O | [구현·검증 기록](./PA/PA_2/study.md), [main.cpp](./PA/PA_2/main.cpp), [Makefile](./PA/PA_2/Makefile) |

Proj2는 11/16 사용자 서버 제출 완료 확인이다. 검증 기록은 공개 11개 일치, 자체/reference 171개 중170개 일치와 추가 JALR 사례 1개 차이를 함께 기록한다. 이 수치를 공식 채점으로 바꾸지 않는다. 명세의 AI 보조 허용과 개인의 설명 책임은 [구현 기록](./PA/PA_2/study.md)에 연결했다.

자소서에서는 [C1/C2 경험 카드](../경험-근거지도.md#c1c2--risc-v-명령어-해독에서-실행-상태-관리로), 전체 과제는 [pa-study.md](./pa-study.md)를 읽는다. 이 구현은 소프트웨어 시뮬레이터이며 RTL 설계·실물 RISC-V 보드 개발은 별도 경험이다.

## 강의·시험 자료

성능 분석 → RISC-V 명령어 → 산술·데이터패스 → pipeline·hazards → cache·memory hierarchy로 연결한다.

- [중간 대비](./midterm-prep/목차.md) · [기말 대비 PDF 5개](./final-prep/)
- 2020~2022 RISC-V 변환 문제와 2023 기출·2024 샘플은 학습 자료다. 이를 개인 구현 프로젝트나 원래 RISC-V 출제본으로 오인하지 않는다.

## 수업 자료 찾기

[1주차](./1주차/study.md) · [2주차](./2주차/study.md) · [3주차](./3주차/study.md) · [4주차](./4주차/study.md) · [5주차](./5주차/study.md) · [6주차](./6주차/study.md) · [7주차](./7주차/study.md) · [9주차](./9주차/study.md) · [10주차](./10주차/study.md) · [11주차](./11주차/study.md) · [12주차](./12주차/study.md)

- [공지 원문](./notices.md) · [과제·퀴즈·제출 기록](./assignments.md) · [전체 일정](../tasks.md)
- [전체 경험·근거 지도](../경험-근거지도.md) · [저장소 홈](../README.md)

최신 정리: 2026-11-17. 소스·보고서의 보관 상태와 수강·개인 학습 완료는 별도로 관리한다.
