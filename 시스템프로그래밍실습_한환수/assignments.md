# 시스템프로그래밍실습 - 한환수 과제/실습

## 평가 방식

| 항목 | 비율 | 메모 |
| --- | --- | --- |
| Class attendance | 10% | 전자출결 사용 |
| Lab exercise | 15% | 주간 실습 과제 |
| Programming Assignment 1 | 10% | 세부 공지 대기 |
| Programming Assignment 2 | 15% | 세부 공지 대기 |
| Programming Assignment 3 | 20% | 세부 공지 대기 |
| Final Exam | 30% | 기말고사 |

## 출석 기준

- 결석은 최대 2회까지 허용됩니다.
- 2회 초과 결석부터 최종 성적에 영향이 있습니다.
- 4회 결석하면 자동 F입니다.
- 전자출결(SAM 앱, Kingo-M QR code, 학생증)을 사용합니다.
- 출결을 놓친 경우 출결변경신청 가능 여부를 확인합니다.

## 1주차 Lab Exercise

출처: `1주차/week1-course_overview.pdf`, p.33

| 항목 | 내용 |
| --- | --- |
| 과제 | Vim으로 `hello.c` 작성 |
| 프로그램 동작 | `Hello, Linux!` 출력 |
| 제출물 | iCampus에 스크린샷만 제출 |
| 파일명 | `STUDENT ID_w1.png` |
| 마감 | 2026-09-08 23:59 |

### 제출 전 체크

- [ ] Ubuntu/Linux 환경에서 터미널 실행 가능
- [ ] `vim` 설치 또는 사용 가능
- [ ] `gcc`/`build-essential` 설치 확인
- [ ] Vim으로 `hello.c` 직접 작성
- [ ] 컴파일과 실행 확인
- [ ] 실행 결과가 `Hello, Linux!`로 보이는 화면 캡처
- [ ] 파일명을 `STUDENT ID_w1.png` 형식으로 저장
- [ ] iCampus 지정 제출처에 스크린샷 제출

## 일반 Lab Exercise 운영

- 일반 실습 과제 마감은 같은 날 자정, 즉 화요일 23:59로 안내되었습니다.
- 단, 1주차 Lab Exercise는 별도로 `Next week (9/8 23:59)`가 명시되어 있습니다.
- 이후 주차도 PDF/공지/과제 페이지에 적힌 실제 마감을 우선합니다.

## Programming Assignment 운영 메모

- 총 3회 Programming Assignment가 예정되어 있습니다.
- 배점은 각각 10%, 15%, 20%입니다.
- PA 관련 질문은 가능한 한 빨리 iCampus Q&A board 또는 안내된 채널로 해야 합니다.
- 제출물은 다른 사람 작업/인터넷 작업을 제출하면 cheating으로 간주됩니다.
- 도구 사용법 도움, high-level design discussion, 디버깅 도움은 cheating이 아닌 것으로 안내되었습니다.

## 주차별 체크표

| 주차 | Lab Exercise | 제출 상태 | 마감/메모 |
| --- | --- | --- | --- |
| 1주차 | `hello.c` 작성 후 실행 스크린샷 제출 | 제출 완료 | 2026-09-08 23:59 |
| 2주차 | GCC object file/link + GDB logging 실습 | 미제출 | 2026-09-08 23:59, `week2.tar.zst`, 지각 제출 불가 |
| 3주차 | Makefile + score.sh 자동 채점 실습 | 제출 상태 미확인 | STUDENTID_w3.tar.zst, iCampus 실제 마감 확인 |
| 4주차 | Unix I/O로 Aladdin.txt.no 생성 | 제출 상태 미확인 | source + Makefile tar, iCampus 실제 마감 확인 |
| 5주차 | 미확인 | 미제출 |  |
| 6주차 | 미확인 | 미제출 |  |
| 7주차 | 미확인 | 미제출 |  |
| 8주차 | No class | 해당 없음 |  |
| 9주차 | 미확인 | 미제출 |  |
| 10주차 | 미확인 | 미제출 |  |
| 11주차 | 미확인 | 미제출 |  |
| 12주차 | 미확인 | 미제출 |  |
| 13주차 | 미확인 | 미제출 |  |
| 14주차 | 미확인 | 미제출 |  |
| 15주차 | No class | 해당 없음 |  |
| 16주차 | Final Exam | 해당 없음 |  |

## 2주차 Lab Exercise

출처: `2주차/week2-gcc&gdb.pdf`, pp.31-36

| 항목 | 내용 |
| --- | --- |
| 작업 폴더 | 원하는 위치에 `week2/` 생성 |
| 소스 | 제공된 구조의 `main.c`와 본인이 작성한 `plus.c`, `minus.c` |
| object file | `-ggdb -c -O0` 옵션으로 `plus.o`, `minus.o`, `main.o` 생성 |
| executable | object file을 link해 `w2.out` 생성 (`-O0`) |
| GDB | `plus`, `minus`에 breakpoint 설정 후 입력 `100 80`으로 실행 |
| 확인 | 각 함수의 반환값을 `rax`에서 확인. non-x64는 `r0` 또는 `x0` 사용 |
| 로그 | GDB 시작 뒤 `set logging enabled on`으로 기록. 예시에는 `gdb.txt`, `stdout.txt`가 제시됨 |
| 제출물 | object file과 executable을 지운 뒤 `week2/`를 `week2.tar.zst`로 archive |
| 제출 | iCampus에 `week2.tar.zst` 제출 |
| 마감 | 2026-09-08 화요일 23:59, no late submission |

### 제출 전 체크

- [ ] `week2/`에 `main.c`, `plus.c`, `minus.c`가 있다.
- [ ] `plus.o`, `minus.o`, `main.o`를 거쳐 `w2.out`을 만들었다.
- [ ] GDB에서 `plus`, `minus` breakpoint와 `100 80` 실행을 확인했다.
- [ ] 실행 출력과 register 반환값이 일치한다.
- [ ] GDB logging 결과와 stdout redirection 결과를 확인했다.
- [ ] archive 전 `*.o`, `w2.out`을 삭제했다.
- [ ] `week2.tar.zst`를 열어 비어 있지 않은지 확인했다.
- [ ] iCampus에 archive 파일을 제출했다.
- [ ] WSL/Docker/ARM 등 표준 환경이 아니라면 iCampus comment에 환경을 썼다.

## Programming Assignment 확정 일정

출처: ARCS System Programming Lab course page. 세부 스펙과 제출 형식은 iCampus 공지를 우선한다.

| 과제 | 주제 | 배점 | 마감 |
| --- | --- | --- | --- |
| PA#1 | File I/O | 10% | 2026-10-04 일요일 23:59 |
| PA#2 | Command-line shell | 15% | 2026-11-01 일요일 23:59 |
| PA#3 | Server-client programming | 20% | 2026-12-06 일요일 23:59 |
## 3주차 Lab Exercise

출처: 3주차/week3-shell&makefile&git.pdf, pp.36-39

| 항목 | 내용 |
| --- | --- |
| Exercise 1 | Makefile로 main.c, plus.c, minus.c를 build |
| 필수 | variable 1개 이상, automatic variable 1개 이상, w3 rule, safe clean rule |
| Exercise 2 | score.sh A B: make -> expected answer -> w3 output -> diff -> clean |
| 정상 결과 | result.txt가 empty |
| 제출 | main.c, plus.c, minus.c, Makefile, score.sh만 STUDENTID_w3.tar.zst로 제출 |

## 4주차 Lab Exercise

출처: 4주차/week4-file_io.pdf, pp.50-54

| 항목 | 내용 |
| --- | --- |
| 목표 | 각 input text line 앞에 number space vertical-bar space를 붙여 input.no 생성 |
| 제한 | Unix File I/O만 사용. Standard I/O/편의 변환 함수 사용은 0점 위험 |
| 입력 | command-line txt filename |
| 제출 | source code + Makefile을 tar로 묶어 iCampus 제출 |
| 검증 | cat -e output으로 line ending과 line count 확인 |

## PA1 - File Search

출처: PA/PA_1/PA1.pdf

| 항목 | 내용 |
| --- | --- |
| 목표 | arbitrary-size text file에서 interactive search를 수행하는 pa1 |
| query | single word, multiple words, quoted phrase, word1*word2 |
| memory | whole-file load 금지; line/word streaming 처리 |
| output | query마다 한 줄, 결과 없어도 blank line, 순서대로 출력 |
| build | Linux, Makefile이 pa1 binary 생성, hand-implemented header 최소 1개 |
| 제출 | pa1 directory의 c/h/Makefile을 STUDENTID_pa1.tar.zst로 제출 |
| deadline | 본문/ARCS는 2026-10-04 23:59. PDF의 2026-04-05 표기는 오류로 보여 iCampus timestamp 최종 확인 |

### 제출 및 재검증 상태

- 1차 제출 완료.
- 마감 전 이번 주에 2~3시간을 따로 확보해, PA1 PDF와 본인 구현을 처음부터 대조하고 재시험한다.
- 확인 순서: 네 query 형식별 결과 -> no-match/빈 줄/문장부호/대소문자 경계 -> `make clean` 후 재빌드 -> archive 내용 확인.
- 사양과 다른 동작을 발견했을 때만 수정하고, 수정했다면 archive를 다시 만들어 재제출한다.