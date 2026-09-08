# 시스템프로그래밍실습 - 한환수 공지

## 2026-09-01 1주차 Course Overview

출처: `1주차/week1-course_overview.pdf`

### 핵심 체크

- 과목명: SWE2024 System Programming Lab
- 수업 시간: 화요일 18:00-21:45
- PDF 표기 강의실: `#85702`
- TA: Gwangbin Rhee, Jinseok Park
- iCampus 사용처: 공지, 강의콘텐츠, 주간 실습 제출, Programming Assignment 제출, Q&A board
- 참고 교재: `Computer Systems: A Programmer's Perspective`, Randal E. Bryant and David R. O'Hallaron, 3rd edition, Pearson, 2015
- 실습 환경: Linux/Ubuntu 기반
- 전자출결 사용

### TA 연락처

| 이름 | 이메일 |
| --- | --- |
| Gwangbin Rhee | gwangbin61@arcs.skku.edu |
| Jinseok Park | jinseokpark160713@gmail.com |

### 평가/출석 공지

| 항목 | 비율/조건 |
| --- | --- |
| Class attendance | 10% |
| Lab exercise | 15% |
| 3 Programming assignments | 45% = 10% + 15% + 20% |
| Final Exam | 30% |
| 결석 허용 | 최대 2회까지 허용 |
| 결석 패널티 | 2회 초과부터 최종 성적에 영향 |
| 자동 F | 4회 결석 |

### Academic Misconduct

Cheating/plagiarism으로 보는 것:

- 다른 사람의 작업을 제출하는 것
- 인터넷에서 가져온 작업을 제출하는 것
- 다른 사람이 자기 작업을 과제로 사용하도록 허용하는 것

Cheating이 아닌 것으로 안내된 것:

- 시스템이나 도구 사용법을 도와주는 것
- high-level design issue를 토론하는 것
- 다른 사람의 코드 디버깅을 도와주는 것

패널티:

- 원본과 복사본 모두 성적 패널티 가능
- 심각한 부정행위는 학교에 보고될 수 있음

### 출결/Q&A 안내

- 전자출결을 사용합니다.
- 성균관대학교 전자출결(SAM) 앱을 사용하거나, 리더기에 Kingo-M QR code 또는 학생증을 사용할 수 있습니다.
- 출결을 깜빡하면 출결변경신청을 할 수 있습니다.
- 실습 마감은 일반적으로 같은 날 자정, 즉 화요일 23:59입니다.
- 질문은 iCampus Q&A board(문의게시판)를 사용합니다.
- Programming Assignment 관련 질문은 가능한 한 빨리 하는 것이 좋습니다.
- 수업은 21:45에 끝나며, 마감 전 추가 질문은 iCampus 또는 이메일로 할 수 있습니다.

### 주의할 점

- PDF의 강의실 `#85702`와 기존 수강신청표의 강의실 정보가 다를 수 있으므로 실제 수업 장소는 iCampus/첫 수업 공지 기준으로 재확인합니다.
- In-Ui-Ye-Ji cluster(인의예지 클러스터)는 이 과목에서 strongly discouraged입니다.
- Ubuntu 22.04 이상 사용 가능, 그보다 오래된 버전은 권장되지 않습니다.
- VMware, UTM, native Linux server/computer는 허용됩니다.
- 다른 distro나 WSL2는 자기 책임으로 사용해야 하며, 환경 차이 때문에 감점 가능성이 있습니다.

## 2026-09-08 ARCS System Programming Lab 기준 페이지

출처: <https://arcs.skku.edu/Courses/SWPractice2>

ARCS는 이 과목의 장기 운영 정보를 확인할 수 있는 연구실/수업 페이지다. 강의 슬라이드는 iCampus에 올라온다는 안내가 명시되어 있으므로, 매주 실제 수업 자료와 주간 제출 지시는 iCampus를 우선하고, 아래의 학기 전체 일정과 PA 마감은 ARCS 기준으로 관리한다.

### 과목 기본 정보

| 항목 | 내용 |
| --- | --- |
| 과목 | SWE2024 System Programming Lab, C language 실습 |
| 수업 | 화요일 18:00-21:45 |
| 장소 | 85702 |
| 담당 | 한환수 교수님 |
| TA | Gwangbin Rhee, Jinseok Park |
| 참고서 | Computer Systems: A Programmer's Perspective, 3rd ed. |

### 학기 주제 흐름과 고정 일정

| 주차 | 주제 | 비고 |
| --- | --- | --- |
| 1 | Course Introduction, Linux Install, Vim |  |
| 2 | Compiling, debugging codes in Linux |  |
| 3 | Git usage, Makefile, Shell scripts |  |
| 4 | Basic file I/O system calls | PA#1 연결 |
| 5 | Creating, managing process |  |
| 6 | Sending and handling signals | PA#2 연결 |
| 7 | IPC: pipe, redirection |  |
| 8 | Midterm week | 수업 없음 |
| 9 | System V IPC: shared memory, named pipe, message queue |  |
| 10 | Basic client/server program using socket | PA#3 연결 |
| 11 | Pthread API |  |
| 12 | Multi-process, multi-thread programming |  |
| 13 | Synchronization: mutex, conditional variables |  |
| 14 | Linux daemon programming |  |
| 15 | Study Week | 수업 없음 |
| 16 | Final Exam | 20:00-21:30 |

### ARCS에 명시된 Programming Assignment 마감

| 과제 | 주제 | 마감 |
| --- | --- | --- |
| PA#1 | File I/O | 2026-10-04 일요일 23:59 |
| PA#2 | Command-line shell | 2026-11-01 일요일 23:59 |
| PA#3 | Server-client programming | 2026-12-06 일요일 23:59 |

### 평가 비율 재확인

- Attendance 10%
- Weekly Lab Assignments 15%
- Programming Assignments 45%: PA#1 10%, PA#2 15%, PA#3 20%
- Final Exam 30%

### 바로 쓸 운영 판단

- ARCS는 주제 흐름과 큰 마감을 미리 보는 곳이다.
- iCampus의 주차별 PDF, 과제 페이지, 공지가 실제 제출 형식과 세부 마감의 최종 기준이다.
- 2주차 이후에는 화요일 수업 전 iCampus에서 해당 주 Lab Exercise 제출처를 확인하고, 같은 날 23:59 마감 여부를 먼저 체크한다.