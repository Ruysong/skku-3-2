# 시스템프로그래밍실습 - 한환수 공지

## 2026-10-27 보관: [PA2] Q&A (10/13 최신화)

- 사용자 제공 공지 원문이며, 10/27은 보관일이다. 제목의 최신화 표기는 10/13이다.
- 작성자: 박진석 TA.
- [질답 9개 원문과 빠른 확인표](./PA/PA_2/qa.md)를 PA2 명세 옆에 보관했다.
- [iCampus 원문](https://canvas.skku.edu/courses/75630/discussion_topics/651940).

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

<!-- canvas-notice-651940-20261109:start -->
## [PA2] Q&A (11/01 최신화) (11/9 API 원문 확인)

- 게시: 2026-10-13 19:15:29
- 출처: https://canvas.skku.edu/courses/75630/discussion_topics/651940
- 본문 전체를 API HTML에서 텍스트로 옮겼다. 문단·줄바꿈을 정돈했으며 원본 HTML은 로컬 API 스냅샷에 보관한다.

### 원문

질문 1. pa2_cat 등 coreutil 구현체에 stdin으로부터 한 번에 들어오는 최대 길이가 궁금합니다.



-> 최대 길이는 정해져 있지 않습니다. 입력이 한 번에 모두 들어온다고 가정하지 말고, 고정 크기 버퍼를 사용해 read()가 0(EOF)을 반환할 때까지 반복해서 읽어 주세요.



질문 2. 보고서는 국문으로 쓰는지 영문으로 쓰는지 궁금합니다.



-> 보고서는 국문, 영문 중 편한 언어로 작성하시면 됩니다. 


질문 3. Makefile 따로 수정할 필요 없는지 궁금합니다. 예시로 -std=gnu2x이 지정되어 있던데, C23 표준안(with GNU extensions)도 사용이 가능한지 여쭤봅니다.


-> Makefile은 수정하지 않고 그대로 사용해도 됩니다. 필요하면 수정해도 되지만, 채점은 제출한 Makefile로 하므로 채점 환경에서 컴파일되는지 꼭 확인해 주세요.


질문 4. PPT 및 소스코드 내 설명에서 따옴표가 (‘) (') (’) 등으로 여러 개의 문자가 섞여 있습니다. 키보드로 입력 가능한 ASCII 문자 (') 를 사용하면 되는지 (ex, cannot remove 'foo') 여쭤봅니다.


-> 키보드의 ASCII 작은따옴표(', U+0027)를 사용하면 됩니다. 문서 변환 과정에서 따옴표가 섞인 것이라 따옴표 종류로 감점하지는 않습니다.


질문 5. 슬라이드 18은 "Error 항목에 없는 에러는 command: %s(strerror)로 출력"하라고 합니다. 그런데 rm의 Error 항목에는 "인자 없음 / 파일 없음 / 디렉터리"만 있고, 다음 힌트 슬라이드에는 EACCES, ENOTDIR, EPERM 같은 다른 errno도 나옵니다. 그럼 "파일 없음, 디렉터리"는 pa2_rm: cannot remove 'FILE': 이유로, 나머지 errno는 pa2_rm: 이유로 출력하면 되나요?


-> rm은 파일 삭제(unlink)가 실패하는 모든 경우에 pa2_rm: cannot remove 'FILE': strerror(errno) 형식으로 통일해 주시면 됩니다. 슬라이드에 나온 두 경우(파일이 없을 때, 디렉터리일 때)는 이 형식의 예시이고, EACCES 같은 다른 errno도 같은 형식에서 뒤의 이유만 달라집니다. (실제 rm도 rm: cannot remove 'foo': Permission denied처럼 출력합니다.)



슬라이드 18의 command: %s 형식(예: pa2_rm: Cannot allocate memory)은 파일 삭제와 관계없는, 형식이 정해지지 않은 기타 에러에 사용하면 됩니다.



명세에 나오지 않은 errno는 채점할 때 메시지 문구를 엄격하게 보지 않을 예정입니다.


질문 6. rm은 여러 인자로 들어온 파일 중 일부가 없는 경우 해당 파일은 오류를 내고, 나머지는 처리하는 것으로 명시되어 있습니다. cp와 cat 등 다른 명령어의 경우에도 여러 개의 인자 중 하나/일부 파일만 없어도 나머지 파일들에 대해 복사/concat 작업을 수행해야 할까요?



-> 실제 cat, cp와 같이 문제가 있는 파일은 에러 메시지를 출력하고, 나머지 파일은 계속 처리해 주시면 됩니다. 이때 하나라도 에러가 있었다면 0이 아닌 값으로 종료하면 됩니다. (mv도 여러 파일을 옮길 때 같습니다.)


이 부분은 rm 외에는 명세에 명시되지 않았던 내용이라, 채점할 때 엄격하게 보지는 않을 예정입니다.


질문 7. head 등 옵션을 받는 명령어의 경우 잘못된 옵션이 주어지거나(ex, -a, -l 등), 정확한 옵션이더라도 파라미터가 동반되지 않는 경우(ex, head -n) 이러한 오류는 errno에 담기지 않는데, 어떤 식으로 에러 메시지를 작성해야 하는지 궁금합니다.



-> 두 경우 모두 getopt가 '?'를 반환합니다. Part 1의 getopt 슬라이드처럼 이 경우를 에러로 처리하고, 0이 아닌 값(예: EXIT_FAILURE)으로 종료해 주세요.


메시지는 getopt가 자동으로 출력하는 형식이면 충분합니다.



 - 잘못된 옵션: pa2_head: invalid option -- 'a'


 - 값이 없는 경우: pa2_head: option requires an argument -- 'n'



이 경우는 명세에 정해진 형식이 없어, 채점할 때 메시지 문구까지 엄격하게 보지는 않을 예정입니다.



질문 8. 기본으로 제공해주신 pa2_basename에서 기본 Makefile을 적용하는 경우 const가 잘려 컴파일이 되지 않는 문제가 있습니다. (-Werror=discarded-qualifiers) 이 경우 기본 제공된 코드를 임의로 수정해도 되는지 질문드립니다.



-> 네, 제공된 코드는 자유롭게 수정해도 됩니다. 



질문 9. Skeleton 주석과 pdf의 에러 출력 예시(Output)가 설명 문장과 다른 부분이 있는 것 같습니다. 어느 쪽을 따라야 하나요?



-> Skeleton 주석과 pdf의 에러 출력 예시에 일부 오타가 있습니다. 각 항목의 설명 문장 부분이 맞는 형식입니다.



 - pa2_cat 파일이 없을 때: pa2_cat: foo: No such file or directory


 - pa2_cp 인자가 하나일 때: pa2_cp: missing destination file operand after 'foo'


 - pa2_mv 인자가 하나일 때: pa2_mv: missing destination file operand after 'foo'



주석이나 pdf의 예시 출력이 설명 문장과 다른 부분이 있으면, pdf의 설명 문장(print "…" 부분)의 형식을 따라 주시면 됩니다.



질문 10. exit의 두 에러 메시지 (슬라이드 71)에서 There are still jobs active.(job table이 비어 있지 않을 때)와 exit: There are still running jobs(백그라운드 job이 실행 중일 때)가 있는데, 백그라운드 job이 실행 중이면 두 조건이 모두 참이라 어느 쪽을 출력해야 되나요? 둘 중에 어떤 에러를 먼저 확인해야 되는지도 궁금합니다.


-> 둘 중 하나만 출력하면 됩니다. job이 남아 있는지와 인자 오류(too many arguments, invalid integer) 중 무엇을 먼저 검사할지는 정해져 있지 않아, 어느 순서로 검사해도 됩니다.
채점에서는 job이 남아 있을 때 셸이 종료되지 않고 에러를 출력하는지만 확인하며, 두 메시지 중 어느 것을 사용해도 감점하지 않습니다.



질문 11. 채점은 터미널에서 직접 명령을 입력하는 방식으로만 하시나요, 아니면 스크립트 파일(bin/pa2 FILE)이나 ./bin/pa2 < tests.txt처럼 표준 입력으로 명령을 넣는 방식도 사용하시나요?


-> 채점은 셸을 실행한 뒤 터미널에서 명령을 입력하는 방식(대화형)으로만 진행합니다. 스크립트 파일 인자(./bin/pa2 FILE)나 표준 입력 리다이렉션(./bin/pa2 < tests.txt) 방식은 사용하지 않습니다.




질문 12. 명세에는 Ctrl-D(EOF)에 대한 언급이 없지만, 스켈레톤의 main은 readline이 NULL을 반환하면 job table 정리나 메모리 해제 없이 그대로 반복문을 빠져나와 return합니다. 스켈레톤 동작 그대로 두면 될까요, 아니면 Ctrl-D에서도 남은 job과 메모리를 정리해야 하나요?


-> Ctrl-D(EOF)는 명세에 포함되지 않은 동작이라 skeleton 동작 그대로 두셔도 됩니다. 채점에서 Ctrl-D로 종료하는 경우의 job 정리나 메모리 해제는 확인하지 않습니다.
원하시면 Ctrl-D에서도 exit처럼 남은 job을 확인하거나 메모리를 정리하도록 구현해도 되지만, 필수는 아닙니다.



질문 13. 슬라이드 9에는 명령을 찾지 못했을 때(EXECUTABLE: command not found, 127)만 나와 있습니다. 파일은 있지만 실행 권한이 없으면 bash처럼 NAME: Permission denied를, 디렉터리이면 NAME: Is a directory를 출력하고 126을 반환하고, 127은 명령을 찾지 못한 경우에만 사용하는 것이 맞을까요?


-> 실행할 수 없는 파일(권한 없음, 디렉터리 등)은 명세에 정해져 있지 않은 부분이라, 명령을 찾지 못한 경우와 똑같이 NAME: command not found(127)로 처리해도 정답으로 처리합니다. 말씀하신 bash 방식(NAME: strerror(errno), 126)으로 구현해도 괜찮습니다.



질문 14. cd nowhere나 exit abc처럼 built-in이 실패했을 때의 $? 값에 대한 규정이 없습니다. 실패한 built-in은 1을 반환하고, fg/bg는 스켈레톤대로 잘못된 인자일 때 2를 반환하는 것이 맞을까요?


-> 실패한 내장 명령의 종료 상태는 명세에 정해져 있지 않은 부분이라, 0이 아닌 값이면 정답으로 처리합니다. 실패한 내장 명령은 1을 반환하고, fg/bg는 skeleton에 구현된 값을 그대로 사용하시면 됩니다.



질문 15. 슬라이드 68의 cd에는 디렉터리가 없을 때의 에러와 인자가 없을 때/~일 때만 나와 있습니다.
- 인자가 둘 이상이면 cd: too many arguments를 출력하고, cd -는 지원하지 않아도 될까요?
- pwd의 기본값이 논리 경로(PWD)이므로, cd가 성공하면 PWD를 getcwd() 결과로 갱신하는 것이 맞을까요?
- 그 밖의 에러(EACCES, ENOTDIR 등)도 cd: DIR: strerror 형식으로 출력하면 될까요?


-> 
- 인자가 둘 이상이면 cd: too many arguments를 출력하시면 되고, cd -는 지원하지 않아도 됩니다. 두 경우 모두 명세에 없는 부분이라 채점하지 않습니다.
- cd가 성공하면 PWD를 갱신해 주세요. pwd의 기본값이 PWD 환경 변수를 출력하기 때문에, 갱신하지 않으면 cd 후 pwd가 이전 디렉터리를 출력합니다. getcwd() 결과로 갱신하시면 됩니다.
- 그 밖의 에러도 cd: DIR: strerror(errno) 형식으로 출력하시면 됩니다.




질문 16. pwd -L -P처럼 두 옵션을 함께 주면 마지막 옵션을 따르는 것이 맞을까요? 그리고 PWD 환경 변수가 설정되어 있지 않으면 getcwd()로 대신하면 될까요?


-> 두 경우 모두 명세에 없는 부분이라 고려하지 않아도 됩니다.



질문 17. week 5 실습 코드(handle_exit)에서는 인자 개수를 먼저 확인해 둘 이상이면 exit: too many arguments를 출력하고, 그다음 strtol로 문자열 전체가 정수인지 확인해 아니면(3abc 등) exit: CODE: invalid integer를 출력하고, 0~255를 벗어나거나 overflow가 나는 값은 255로 종료했습니다. PA2의 exit도 week 5와 같은 방식으로 처리하면 맞을까요?


-> 네, 5주차와 같은 방식으로 처리하시면 됩니다. 다만 PA2에서는 두 가지가 다릅니다.


- CODE 없이 exit만 입력하면, 0이 아니라 마지막으로 실행한 명령의 종료 상태로 종료합니다.
- job table에 job이 남아 있으면 종료하지 않고 에러 메시지를 출력합니다.


overflow가 나는 값은 255로 종료해도 되고, invalid integer로 처리해도 됩니다.




질문 18. 예시 외에 에러 사항들은 고려하지 않고 작성해도 괜찮을까요?


-> 슬라이드에 예시로 나온 에러 외에는 고려하지 않아도 됩니다. 채점은 명세에 나온 에러만 확인합니다.




PA2에 대한 추가적인 질문이 있으시면 jinseokpark160713@gmail.com으로 메일을 보내주시기 바랍니다.
<!-- canvas-notice-651940-20261109:end -->
