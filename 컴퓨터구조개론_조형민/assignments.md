# 컴퓨터구조개론 과제/퀴즈

## 평가 비율

출처: `1주차/CA_Lecture_01.pdf`

| 항목 | 비율 |
| --- | ---: |
| Midterm exam | 30% |
| Final exam | 40% |
| Programming assignments | 30% |

## 출석 기준

| 항목 | 기준 |
| --- | --- |
| penalty 없는 결석 | 최대 4회 |
| 감점 | 4회 초과 결석 시 10% deduction |
| F 조건 | 전체 수업의 1/4 초과 결석 |
| 녹화강의 | iCampus 업로드 예정, 시청은 출석 대체 불가 |

## Programming Assignments 계획

- 총 3개 programming assignments 예정
- 큰 방향: RISC-V simulator 만들기
- simulator는 RISC-V CPU의 동작을 소프트웨어로 흉내 내는 프로그램
- 과제는 multiple-stage assignment로 진행 예정
- 앞 과제의 기능을 이어서 다음 과제에 추가하는 누적형 구조로 보임
- 한 과제를 실패하면 다음 과제를 완료하기 어려울 수 있음
- 세부 사항은 TBA

## 1만라인 프로젝트 관련

- 컴퓨터구조개론의 마지막 assignment가 1만라인 프로젝트 과제라고 안내됨
- 마지막 assignment를 제출하지 않으면 B 이상을 받을 수 없음
- 1만라인 프로젝트 수행을 위해 C++ 이해/프로그래밍 능력이 필요
- 본인이 소프트웨어학과 2022학번 이후 졸업요건 대상인지 확인 필요

## 시험 일정 메모

확정 중간고사: **2026-10-21 (수)**. iCampus 공지 `Class schedule` 기준이며, 기말고사 날짜와 시험 범위는 별도 공지 대기.

## 확인 필요

- PA 1 스펙, 마감, 제출처
- 지정 discussion thread 링크
- Midterm/Final 정확한 날짜와 범위
- 녹화강의 업로드 위치

## Project 0 - Submission Practice

출처: PA/PA_1/CA_Proj1.pdf, pp.25-27

| 항목 | 내용 |
| --- | --- |
| 성격 | 선택 과제, 성적 반영 없음 |
| 목적 | 제출 시스템과 디렉터리 구조를 실제로 연습 |
| 제출 ID | proj0 |
| 제출물 | 지정된 두 줄을 stdout에 출력하는 source + C/C++ Makefile 또는 Python launcher |
| 마감 | 2026-10-09 금요일 23:59:59 |
| 권장 | Proj1부터 packaging 오류는 무점 처리될 수 있으므로 실제 제출 연습 권장 |

## Project 1 - RISC-V Disassembly

출처: PA/PA_1/CA_Proj1.pdf

| 항목 | 내용 |
| --- | --- |
| 목표 | RISC-V binary file을 읽어 assembly instruction으로 disassemble |
| 실행 이름 | C/C++ executable은 riscv-sim, Python main은 riscv-sim.py와 executable launcher riscv-sim |
| 입력 | 첫 command-line argument로 binary file pathname |
| 출력 | inst N: 8-digit hex code disassembled instruction 형식, instruction당 한 줄 |
| 지원 명령 | RV32I의 U/J/B/load/store/immediate/register instruction 목록. decode 불가 시 unknown instruction |
| format | mnemonic 소문자, xN register 번호, immediate/offset은 signed decimal, exact stdout match 필수 |
| 검증 | department server reference output과 diff 비교 |
| 환경 | department Linux server에서 반드시 test |
| C/C++ | server에서 riscv-sim을 build하는 Makefile 필수 |
| Python | executable riscv-sim launcher 필수, Makefile 불필요 |
| 제출 | 부모 directory에서 ~swe3005/bin/submit proj1 project1 형태로 directory 이름만 지정 |
| 제출 확인 | ~swe3005/bin/check-submission proj1 |
| package | build artifact와 object file 제거, project file을 제출 directory 바로 아래에 둠 |
| 협업/AI | 협업 금지. AI 보조는 허용되지만 본인 code의 originality와 설명 책임은 본인에게 있음 |
| 마감 | 2026-10-16 금요일 23:59:59, 지각 제출 불가 |