# 2~7주차 실습: 실제 구현 복습 교안

각 절은 **해당 주차 제출용 압축본**을 설명한다. 교수님 강의자료 전문을 대체하는 문서가 아니라, 직접 작성한 프로그램의 책임과 흐름을 복습하는 교안이다. 전체 목록과 버전 선택 근거는 [pa-study](./pa-study.md)에 있다.

## 목차

1. [2주차 다중 소스와 디버깅](#2주차-다중-소스와-디버깅)
2. [3주차 Makefile과 shell script](#3주차-makefile과-shell-script)
3. [4주차 File I/O와 줄 번호](#4주차-file-io와-줄-번호)
4. [5주차 Process와 간단한 shell](#5주차-process와-간단한-shell)
5. [6주차 Signal로 부모자식 통신](#6주차-signal로-부모자식-통신)
6. [7주차 Pipe와 redirection](#7주차-pipe와-redirection)
7. [PA로 이어지는 관계](#pa로-이어지는-관계)

## 2주차 다중 소스와 디버깅

근거: [main.c](./2주차/submitted/main.c), [plus.c](./2주차/submitted/plus.c), [minus.c](./2주차/submitted/minus.c), [GDB 기록](./2주차/submitted/gdb.txt), [stdout](./2주차/submitted/stdout.txt), [원본 해시](./2주차/submitted/source-manifest.json). 수업 내용은 [2주차 study](./2주차/study.md).

```text
argv[1], argv[2]
       |
strtoll(..., base 10)
       |
long long a, b
       |
   +---+---+
   |       |
 plus()  minus()
   |       |
   +---+---+
       |
printf two results
```

main은 인자 개수가 프로그램 이름까지 3인지 검사한다. 두 수를 `strtoll`로 읽고, 다른 소스 파일에 있는 `plus`·`minus`를 호출해 결과를 출력한다. 각 함수는 `long long` 두 개를 받아 덧셈 또는 뺄셈 결과를 반환한다.

핵심은 복잡한 산술이 아니라 **선언과 정의의 분리, 컴파일과 링크, 디버거에서 함수 호출을 따라가는 일**이다. 함수 prototype을 main에 선언했다고 함수 body가 생기는 것은 아니다. 최종 실행파일을 만들 때 세 translation unit의 정의를 연결해야 한다.

이 버전의 printf에는 두 번째 결과 앞의 공백이 들어 있다. 뒤 주차의 자동 `diff`용 코드와 무조건 같은 출력이라고 가정하지 않는다. 숫자 변환 오류·범위 초과를 상세 검사하는 프로그램도 아니며, `strtoll`의 end pointer와 errno 검사는 구현하지 않았다.

복습: 소스를 따로 컴파일한 object의 역할, 함수 선언 누락과 정의 누락의 차이, breakpoint·step·next에서 함수 호출이 어떻게 보이는지를 설명한다. 보관된 GDB 로그는 당시 기록이지 이번에 다시 실행한 로그가 아니다.

## 3주차 Makefile과 shell script

근거: [소스](./3주차/submitted/), [Makefile](./3주차/submitted/Makefile), [score.sh](./3주차/submitted/score.sh), [manifest](./3주차/submitted/source-manifest.json). 수업 내용은 [3주차 study](./3주차/study.md).

```text
main.c    plus.c    minus.c
   |         |         |
 main.o   plus.o    minus.o
   +---------+---------+
             |
           link
             |
             w3

score.sh:
make -> expected answer -> ./w3 -> diff -> make clean
```

Makefile은 `CC=gcc`, `CFLAGS=-Wall`, `OBJECTS=main.o plus.o minus.o`, `TARGET=w3`를 둔다. 최종 타깃은 object들에 의존하고, 패턴 규칙 `%.o: %.c`로 각 소스를 컴파일한다. `$@`는 현재 타깃, `$^`는 prerequisite 전체, `$<`는 첫 prerequisite이다.

`score.sh`는 두 인자에 대해 Bash 산술 확장으로 덧셈·뺄셈 expected 결과를 `answer.txt`에 쓰고, 프로그램 stdout을 `output.txt`에 저장한 뒤 `diff` 출력을 `result.txt`에 남긴다. 마지막으로 clean한다.

여기서 stdout redirection은 프로그램 코드가 output.txt를 열어서 쓰는 것과 다르다. shell이 fd 1을 파일에 연결해 주고 프로그램은 평소처럼 출력한다. `diff` 결과 파일이 비면 동일한 출력이라는 의미지만, script 자체에 모든 단계 실패를 엄격히 중단하는 로직이 있는 것은 아니다. 파일만 생성되었다고 통과를 보장하지 않는다.

복습: 소스 하나만 수정했을 때 필요한 object만 재생성되는 이유, 컴파일과 링크가 분리된 이유, `>`와 `>>`, stdout과 exit status의 차이를 설명한다.

## 4주차 File I/O와 줄 번호

근거: [완성 소스](./4주차/submitted/w4_skeleton_code.c), [Makefile](./4주차/submitted/Makefile), [manifest](./4주차/submitted/source-manifest.json). 파일명에는 skeleton이 남아 있지만 압축본의 내용에는 완성된 처리 루프가 있다. 수업 내용은 [4주차 study](./4주차/study.md).

```text
input path -> open(O_RDONLY)
output path = input + '.no'
           -> open(O_WRONLY|O_CREAT|O_TRUNC, 0644)
read 4028-byte chunks
       |
each byte:
  line_start? -> write number + ' | '
  write original byte
  newline?   -> number++, line_start=true
       |
close both descriptors
```

`line_start`는 read 호출을 넘어 유지된다. chunk 중간에서 줄이 이어져도 번호를 새로 붙이지 않고, 실제 newline 뒤 첫 문자에서만 번호를 붙인다. `line_number`는 1부터, 숫자는 `to_string()`이 21바이트 배열의 뒤쪽부터 계산해 만든다. `get_output_filename()`은 `.no`를 붙이고 `w4_strlen()`은 문자열 길이를 직접 센다.

`write_all()`은 short write가 생기면 남은 바이트를 이어 쓴다. 실제 코드에서는 원문 문자마다 1바이트 write를 수행하므로 read는 batch 방식이어도 write syscall 횟수가 많다. PA1의 출력 버퍼 방식과 비교해볼 수 있는 지점이다.

빈 파일은 줄 번호도 쓰지 않는다. 마지막에 newline이 없는 줄도 첫 문자에서 번호가 붙고 원문 내용은 보존된다. `O_TRUNC`는 기존 출력파일을 초기화한다. 이 함수의 write_all은 PA1 버전과 달리 `EINTR`을 따로 재시도하지 않으므로 같은 이름만 보고 완전히 같은 함수라고 생각하지 않는다.

복습: fd와 FILE stream 차이, read가 반환한 크기만 처리하는 이유, chunk 경계와 line 경계의 차이, close 오류 처리와 출력 파일 flags를 설명한다.

## 5주차 Process와 간단한 shell

근거: [완성 week5.c](./5주차/submitted/week5.c), [Makefile](./5주차/submitted/Makefile), [manifest](./5주차/submitted/source-manifest.json). 원래 폴더의 skeleton이 아니라 제출 압축본을 사용한다. 수업 내용은 [5주차 study](./5주차/study.md).

```text
readline('$ ') -> command -> parse_args
                      |
                   evaluate
                 /          \
          builtin exit     external command
                              |
                            fork
                         /         \
                    child           parent
                    execvp          waitpid
                    _exit on error  next prompt
```

`get_input()`은 readline과 history를 사용한다. Ctrl-D/EOF일 때 종료하고, `parse_args()`는 `strtok_r`의 space 구분으로 argv를 만들어 NULL로 끝낸다. quote·escape를 다루는 범용 shell parser는 아니다.

`exit`는 shell 자체가 끝나야 하므로 builtin으로 처리한다. 인자가 없으면 0, 한 정수이면 `strtol`로 끝까지 유효한지 확인하고 종료 코드를 정한다. 범위 0~255 밖의 값은 현재 코드에서 255로 처리한다. 인자가 너무 많거나 정수 형식이 아니면 오류를 출력한다.

external command는 child에서 `execvp`로 PATH 검색을 시도한다. 실패했고 명령에 slash가 없으면 `./명령`도 시도한다. 실패 종류에 따라 127 또는 126으로 `_exit`한다. parent는 자식 종료를 기다린다. exec 성공은 새 process를 하나 더 생성하는 것이 아니라 child의 프로그램 이미지를 교체한다.

이 구현은 foreground 단일 명령 연습이다. background, job control, pipe, redirection까지 구현한 shell로 표시하지 않는다. parent의 waitpid는 EINTR 재시도 루프가 없는 버전이므로 이후 주차의 helper와 구분한다.

복습: fork 이후 두 실행 흐름, exec 성공 후 원래 코드로 돌아오지 않는 이유, parent가 wait하는 이유, builtin exit를 child에서만 실행하면 shell이 안 끝나는 이유를 설명한다.

## 6주차 Signal로 부모자식 통신

근거: [완성 w6.c](./6주차/submitted/w6.c), [Makefile](./6주차/submitted/Makefile), [manifest](./6주차/submitted/source-manifest.json). 수업 내용은 [6주차 study](./6주차/study.md).

```text
parent                          child
alarm -> SIGALRM flag
send SIGUSR1 -----------------> pending count
                                main loop handles it
                                num++
ack counter <------------------ SIGUSR1 acknowledgement
when target count reached:
send SIGINT ------------------> termination flag
                                print count / exit
```

handler는 `volatile sig_atomic_t` counter 또는 flag만 바꾸고, 출력과 실제 통신 흐름은 main loop에서 처리한다. 비동기 handler 안에서 일반적인 printf 작업을 바로 수행하지 않으려는 설계다. parent는 주기 alarm으로 요청을 보내고 ACK를 세며, child는 요청을 처리할 때 응답한다. parent는 목표에 도달하면 child에게 SIGINT를 보낸다.

signal은 함수 호출처럼 즉시·정확히 한 번 전달되는 메시지 큐가 아니다. 표준 signal은 같은 종류가 연달아 pending되면 합쳐질 수 있다. 이 코드의 timer 재시도는 그런 전달 특성을 다루려는 시도다. pending 처리와 다음 `pause()` 사이에는 lost wake-up 가능성이 있고, counter를 읽고 비우는 동안 새 signal이 들어오는 경계도 주의해야 한다. 신뢰성 있는 통신 프로토콜을 완성했다고 표현하지 않는다.

`sig_atomic_t`는 handler와 일반 실행이 공유하는 단순 변수 접근을 위한 타입이다. `volatile`과 함께 쓴다고 복합 read-modify-write나 신호 마스킹이 자동 해결되는 것은 아니다.

복습: SIGALRM/SIGUSR1/SIGINT의 역할, handler와 main loop의 책임 분리, kill이 이름과 달리 signal 전송 함수인 점, fork 후 getppid와 상대 PID를 사용하는 이유, queued signal과 표준 signal의 차이를 설명한다.

## 7주차 Pipe와 redirection

근거: [week7.c](./7주차/submitted/week7.c), [helper.c](./7주차/submitted/helper.c), [helper.h](./7주차/submitted/helper.h), [Makefile](./7주차/submitted/Makefile), [manifest](./7주차/submitted/source-manifest.json). 아래는 이 제출 압축본의 구현 설명이며 PA2 전체 강의·과제 명세와 구분한다.

```text
readline -> evaluate (helper)
               |
              lex (helper)
               |
             parse (week7.c)
               |
           Pipeline / Command
               |
           interpret (week7.c)
          /                      \
one command                      two commands
redirect stdin/stdout            pipe -> fork left/right
do_exec                          dup2 write/read ends
restore shell descriptors        execvp both / parent wait
```

helper가 tokenizing, token 해제, 기본 exec, evaluate를 제공하고 week7.c가 parse·interpret와 fd 연결을 구현한다. `Token`은 command/argument/operator, `Command`는 argv와 stdin/stdout 파일명, `Pipeline`은 최대 두 command, `Job`은 실행 process 기록이다. 구조체가 있다는 이유만으로 background job control까지 구현한 것은 아니다.

parse는 **단일 pipe 하나 또는 단일 redirection 하나**를 허용한다. `<`, `>`, `>>`를 구분하고 출력 append 여부를 저장한다. multiple pipe, pipe와 redirection 조합, background와 command separator는 현재 연습 범위에서 거부한다.

단일 명령 redirection에서는 shell의 원래 stdin/stdout을 dup으로 보관하고, 파일을 열어 `dup2`로 0 또는 1에 연결한 뒤 helper의 실행 함수를 호출한다. 실행이 끝나면 원래 fd를 복원한다. 복원하지 않으면 다음 prompt나 입력도 파일로 가는 문제가 생긴다. 출력 버퍼와 fd 교체 순서를 맞추기 위해 fflush를 사용한다.

pipe에서는 왼쪽 child의 stdout을 pipe write end, 오른쪽 child의 stdin을 read end로 연결한다. 양쪽 child와 parent 모두 필요 없는 pipe end를 닫는다. **parent가 write end를 계속 열어 두면 오른쪽 프로세스가 EOF를 못 받을 수 있다.** 두 child의 실행 이미지는 execvp로 바뀌고 parent는 둘을 wait한다.

자체 `wait_for_child()`는 EINTR이면 waitpid를 다시 시도한다. 이는 helper의 단일 command 실행 경로와 별도다. 현재 Makefile은 GNU11, warnings, O2와 libedit 링크로 w7을 만든다.

복습: pipe의 두 fd 방향, dup와 dup2의 차이, exec 후 표준 fd 연결이 유지되는 이유, 모든 불필요한 end를 닫아야 하는 이유, shell의 fd를 복원해야 하는 이유를 직접 그려본다.

## PA로 이어지는 관계

```text
W2 separate sources / debugger
       -> W3 automated build / output comparison
       -> W4 low-level file I/O -> PA1 scanner
       -> W5 process execution
       -> W6 asynchronous events
       -> W7 fd connections -> PA2 shell foundation
```

모든 snapshot은 압축본 내용과 해시를 대조했지만, 이번 정리 작업에서 Linux 환경의 전체 실습을 다시 빌드·실행하지 않았다. source 존재, 당시 제출용 archive 존재, 현재 실행 검증, LMS 제출 완료는 각각 다른 확인 단계다.
