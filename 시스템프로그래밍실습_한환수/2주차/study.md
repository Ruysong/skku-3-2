# 시스템프로그래밍실습 - 한환수 2주차 학습지도

작성 코드 복습: [2주차 제출 압축본 구현 설명](../lab-study.md#2주차-다중-소스와-디버깅), [소스·GDB 로그](./submitted/).

## 한줄 결론

2주차는 Linux에서 C 프로그램을 "만들고 끝"내는 단계가 아니라, 명령줄에서 파일을 다루고, `gcc`가 source code를 executable까지 어떻게 바꾸는지 이해하고, `gdb`로 프로그램이 실제로 어디서 어떻게 멈추는지 확인하는 주차입니다. 앞으로 file I/O, process, IPC, socket, pthread 실습을 할 때도 결국 이 루틴을 반복합니다.

```text
source files (.c)
      |
      | gcc: preprocess -> compile -> assemble
      v
object files (.o)
      |
      | gcc: link
      v
executable (w2.out)
      |
      | gdb: breakpoint / run / inspect
      v
reproducible debugging record (gdb.txt, stdout.txt)
```

이번 주 Lab Exercise의 본질도 여기에 있습니다. `main.c`, `plus.c`, `minus.c`를 각각 object file로 만들고, linker로 `w2.out`을 만든 뒤, GDB에서 함수가 반환한 값을 register 수준에서 확인합니다. 마지막에는 불필요한 build 결과물은 빼고 source와 기록만 남긴 `week2.tar.zst`를 iCampus에 제출합니다.

## 빠른 복습 요약 (사용자 정리)

SYSTEM PROGRAMMING LAB — WEEK 2 최종 요약노트
0. 전체 목차
WEEK 2
│
├─ 1. Terminal / Shell / Command Line
│
├─ 2. Linux Filesystem / Path
│   ├─ Absolute Path
│   ├─ Relative Path
│   └─ .  ..  ~  /
│
├─ 3. Linux Commands
│   ├─ man / echo / date / ps
│   ├─ pwd / cd / ls
│   ├─ mkdir / rmdir / mv
│   ├─ cp / rm
│   ├─ cat / head / tail / grep
│   └─ tar
│
├─ 4. Compiler / GCC
│   ├─ Optimization Level
│   ├─ GCC Procedure
│   ├─ -o
│   ├─ -save-temps
│   ├─ -E
│   ├─ -S
│   ├─ -c
│   └─ Linking
│
├─ 5. Error / Bug / Debugging
│
├─ 6. GDB
│   ├─ Breakpoint
│   ├─ run
│   ├─ next / step
│   ├─ continue
│   ├─ variable
│   ├─ register
│   └─ stack / backtrace
│
└─ 7. Week 2 Lab
    ├─ .c → .o
    ├─ link → w2.out
    ├─ GDB
    ├─ stdout.txt
    ├─ gdb.txt
    └─ week2.tar.zst

교안은 Linux command → GCC → GDB → Lab 순서로 진행한다.

1. Linux 기본 구조
Path 기호
기호	의미	암기
/	Root Directory	파일시스템 최상위
~	Home Directory	내 집
.	Current Directory	여기
..	Parent Directory	한 단계 위

네 환경:

~ = /home/spl

예:

cd ~
cd ..
./w2.out

교안에서 absolute/relative path와 ., .., ~를 설명한다.

2. Linux 명령어 암기표
명령	원래 의미	기능
man	manual	설명서
echo	echo	문자열 출력
date	date	날짜/시간
ps	Process Status	프로세스 보기
pwd	Print Working Directory	현재 경로
cd	Change Directory	디렉터리 이동
ls	list	파일 목록
mkdir	Make Directory	디렉터리 생성
rmdir	Remove Directory	빈 디렉터리 삭제
cp	copy	복사
mv	move	이동 / 이름 변경
rm	remove	삭제
cat	concatenate	파일 내용 출력/연결
head	head	앞부분
tail	tail	뒷부분
grep	pattern 검색 명령	문자열/패턴 검색
tar	Tape Archive	파일 묶기

교안에서 이 명령들을 순서대로 다룬다.

기능별 암기
위치
pwd cd ls

파일 조작
mkdir cp mv rm rmdir

내용 보기
cat head tail grep

관리
man ps tar
3. Program vs Process
Program
= 저장되어 있는 실행파일

Process
= 실행 중인 프로그램

예:

w2.out
= program

./w2.out 실행 중
= process

ps = Process Status

4. GCC 전체 구조 🔥

GCC = GNU Compiler Collection

C source
  .c
   │
   │ Preprocessor
   ▼
  .i
   │
   │ Compiler
   ▼
  .s
   │
   │ Assembler
   ▼
  .o
   │
   │ Linker
   ▼
Executable

교안의 GCC procedure가 바로 이 흐름이다.

5. 각 파일 상태
파일	상태
.c	원본 C source
.i	전처리가 끝난 C source
.s	Assembly source
.o	Machine code를 포함한 Object file
executable	링크까지 끝난 실행파일
.i 전처리에서 하는 것

대표적으로:

#include 처리
macro 치환
comment 제거

교안은 -E를 통해 preprocessed source를 확인하도록 설명한다.

6. GCC 암기법 🔥
파일 순서
I S O

.i → .s → .o
옵션 순서
E S C

-E → -S → -c
암기
ISO ↔ ESC

그리고:

-E
→ preprocessing까지만

-S
→ assembly까지

-c
→ object file까지만
→ linking 하지 않음

교안의 옵션 설명과 대응한다.

7. GCC 주요 옵션
옵션	의미
-E	전처리 후 멈춤
-S	Assembly 생성 후 멈춤
-c	Object 생성 후 멈춤
-o name	Output 파일 이름 지정
-save-temps	.i .s .o 중간파일 보존
-ggdb	GDB용 debugging 정보 포함
-O0	Optimization Level 0

교안에서 -o, -save-temps, -E, -S, -c를 각각 설명한다.

절대 혼동 금지
-o
소문자 o
= output

-O0
대문자 O + 숫자 0
= optimization level zero
8. Optimization Level
최적화 약함                         최적화 강함
   ↓                                    ↓

-O0 → -O1 → -O2 → -O3
옵션	핵심
-O0	최적화 끔 / debugging 쉬움
-O1	기본 최적화
-O2	더 많은 최적화
-O3	강한 실행속도 최적화
-Og	debugging 친화적 최적화
-Os	code size 최적화

교안은 각 optimization level의 의미와 debugging 시 -O0을 설명한다.

핵심 이유
Optimization ↑
→ 원래 C 코드와 실제 machine code의 대응이 달라질 수 있음
→ debugging 어려워짐

따라서 실습:
-ggdb + -O0
9. Linking 🔥

각 파일을 따로 compile:

main.c      plus.c      minus.c
  ↓           ↓            ↓
main.o      plus.o      minus.o
   \          |          /
    \         |         /
          Linker
             ↓
           w2.out

Linker:

여러 Object file 사이의 함수/심볼 참조를 연결해서 실행파일을 만든다.

교안도 여러 object file이 linker를 통해 executable이 되는 구조를 보여준다.

이번 실습
gcc -ggdb -c -O0 main.c
gcc -ggdb -c -O0 plus.c
gcc -ggdb -c -O0 minus.c

gcc -O0 main.o plus.o minus.o -o w2.out

10. ISA vs Executable

이 부분은 교안 바깥 연결 개념.

ISA = Instruction Set Architecture

예:

x86-64
ARM64
RISC-V

ISA는:

CPU가 이해할 수 있는 instruction의 규칙/체계

Executable:

해당 architecture에서 실행할 machine code 등을 담은 실제 파일

ISA
= 언어의 규칙

Executable
= 그 규칙으로 작성된 실제 프로그램

둘은 같은 게 아님.

11. Error / Bug
Error
├─ Syntax Error
│   └─ 문법 오류
│
└─ Semantic / Logic Error
    └─ 실행은 되지만 의미/논리 잘못됨

Bug 예:

잘못된 결과
Infinite Loop
Segmentation Fault

교안은 syntax error와 semantic/logic error를 구분하고 debugging 방법을 소개한다.

12. Debugging / GDB

GDB = GNU Debugger

일반 실행

START ─────────────→ END


GDB

START
 ↓
Breakpoint 🛑
 ↓
상태 관찰
 ↓
한 줄 실행
 ↓
계속 실행
 ↓
END

GDB에서는 source line, assembly, variable/register, stack frame 등을 볼 수 있다.

13. Breakpoint
Break + Point
= 멈추는 지점
break plus

→ plus 함수에 도착하면 실행을 멈춤.

14. GDB 필수 명령어 🔥
명령	축약	의미
run	r	실행
list	l	source 보기
break	b	breakpoint
next	n	다음 줄, 함수 내부 X
step	s	다음 줄, 함수 내부 O
continue	c	다음 breakpoint까지 계속
print	p	변수 값
quit	q	종료
backtrace	bt	함수 호출 경로
info registers	-	register 확인

교안 기본 GDB 명령표에 해당 내용이 나온다.

15. next vs step 🔥🔥

현재:

r = plus(a, b);
next
plus 내부 실행
   ↓
내부에는 안 들어감
   ↓
다음 줄
step
plus 호출
   ↓
plus 함수 내부로 들어감

암기:

NEXT
= 넘어간다

STEP
= 직접 한 발 들어간다
16. Register / rax

Register:

CPU 내부의 매우 빠른 작은 저장공간.

이번 x86-64 실습에서는 함수 반환값을 확인하기 위해:

info registers rax

를 사용한다. 교안에서도 x64에서는 rax, 다른 architecture에서는 r0 또는 x0를 확인하도록 한다.

plus(100,80)
→ 180

minus(100,80)
→ 20
17. Stack / Backtrace
main()
  ↓
plus()

개념적으로:

Stack

┌─────────┐
│ plus    │ ← 현재
├─────────┤
│ main    │
└─────────┘
backtrace

또는:

bt

→ 어떤 함수 호출을 거쳐 현재 위치까지 왔는지 확인.

18. Argument와 run

이번 main.c:

int main(int argc, char* argv[])

실행:

run 100 80

그러면:

argc = 3

argv[0] = 프로그램 이름
argv[1] = "100"
argv[2] = "80"

run만 하면 이번 프로그램은 입력을 기다리지 않고 Usage를 출력하고 종료한다. 교안 코드가 argc != 3이면 바로 종료하도록 작성되어 있기 때문이다.

19. stdin / stdout / stderr

이 부분은 앞으로 시스템 프로그래밍에서 계속 나올 개념.

stdin
= Standard Input
= 표준 입력

stdout
= Standard Output
= 표준 출력

stderr
= Standard Error
= 표준 오류

암기:

IN / OUT / ERR
20. > Redirection
run 100 80 > stdout.txt

>:

Output Redirection

원래

program
  ↓ stdout
terminal


> 사용

program
  ↓ stdout
stdout.txt

이번 실습 결과:

180
20

이 stdout.txt에 들어간다.

21. gdb.txt vs stdout.txt 🔥
GDB가 보여준 내용
        ↓
     gdb.txt


프로그램의 printf 출력
        ↓
    stdout.txt

GDB logging:

set logging enabled on

교안 실습에서 logging을 켜도록 요구한다.

22. Week 2 Lab 최종 구조
main.c
plus.c
minus.c
   │
   │ gcc -ggdb -c -O0
   ▼
main.o
plus.o
minus.o
   │
   │ Link
   ▼
w2.out
   │
   │ GDB
   ▼
break plus
break minus
run 100 80 > stdout.txt
rax 확인
logging
   │
   ├── gdb.txt
   └── stdout.txt

그리고 .o, w2.out 제거 후:

week2/
├── main.c
├── plus.c
├── minus.c
├── gdb.txt
└── stdout.txt

교안 제출 과정도 object와 executable을 삭제한 뒤 archive를 생성하도록 한다.

23. 최종 압축
tar -cav -f week2.tar.zst week2/

결과:

week2.tar.zst

iCampus 제출 파일.

🔥 최종 암기장
[PATH]

/   = root
~   = home
.   = current
..  = parent

./w2.out = 현재 폴더의 w2.out
/w2.out  = root에 있는 w2.out
[LINUX]

pwd = Print Working Directory
cd  = Change Directory
ls  = List

mkdir = Make Directory
cp    = Copy
mv    = Move
rm    = Remove

cat   = Concatenate
head  = 앞
tail  = 뒤
grep  = 검색

ps    = Process Status
man   = Manual
tar   = Tape Archive
[GCC]

GNU Compiler Collection

.c
 ↓ Preprocessor
.i
 ↓ Compiler
.s
 ↓ Assembler
.o
 ↓ Linker
Executable
파일 = ISO

.i
.s
.o

옵션 = ESC

-E
-S
-c
-o  = output
-O0 = Optimization Level 0

-ggdb
= GDB용 debugging information
[GDB]

GNU Debugger

b = break
r = run
l = list
n = next
s = step
c = continue
p = print
q = quit

bt = backtrace
NEXT
= 함수 안 안 들어감

STEP
= 함수 안 들어감
Breakpoint
= 실행을 멈추는 지점

Register
= CPU 내부의 빠른 저장공간

rax
= 이번 x86-64 실습에서
  함수 반환값 확인에 사용
stdin  = Standard Input
stdout = Standard Output
stderr = Standard Error

> = Output Redirection
gdb.txt
= GDB 로그

stdout.txt
= 프로그램 printf 결과
최종 한 장 구조도
                 SYSTEM PROGRAMMING WEEK 2
                           │
         ┌─────────────────┼─────────────────┐
         │                 │                 │
       LINUX              GCC               GDB
         │                 │                 │
   Files / Paths        Build             Debug
         │                 │                 │
 / ~ . ..            .c source         executable
         │                 │                 │
 pwd cd ls          Preprocessor         run
 mkdir cp mv rm          │                 │
 cat grep                .i             breakpoint
 ps man tar               │                 │
                      Compiler          STOP 🛑
                          │                 │
                         .s          inspect state
                          │          ├─ variable
                      Assembler      ├─ register
                          │          └─ stack
                         .o              │
                          │          next / step
                       Linker             │
                          │           continue
                     executable           │
                          └──────→─────── END

---

## 원본 자료와 범위

- 원본: `./week2-gcc&gdb.pdf`
- 분량: 36쪽
- Part 1: Linux command line와 file system
- Part 2: GCC compiler, compile pipeline, linking
- Part 3: GDB debugger와 2주차 Lab Exercise
- 제출 마감: 2026-09-08 화요일 23:59
- 제출물: `week2.tar.zst`
- 지각 제출: 불가

이 자료는 명령어를 많이 나열하지만, 시험이나 실습에서 중요한 연결은 `shell -> path/file -> gcc -> linker -> gdb -> archive/submit`이다. 각각을 따로 외우기보다 이 순서가 한 번에 떠올라야 한다.

## 오늘 밤 실습 전에 먼저 할 일

- [ ] Linux terminal을 열고 `pwd`, `ls`, `cd`로 현재 위치와 작업 위치를 확인한다.
- [ ] 작업할 곳에 `week2/` 디렉터리를 만든다.
- [ ] 제공된 `main.c`가 요구하는 함수 interface를 먼저 읽는다.
- [ ] `plus.c`, `minus.c`는 직접 작성한다. 제출 코드 자체는 본인이 이해하고 작성해야 한다.
- [ ] `-ggdb -c -O0`로 source마다 object file을 만든다.
- [ ] object file을 link하여 `w2.out`을 만든다.
- [ ] 일반 실행으로 출력부터 확인한다.
- [ ] GDB에서 logging을 켜고 `plus`, `minus` breakpoint를 설정한다.
- [ ] `100`, `80` 입력으로 실행하면서 반환값과 출력이 맞는지 확인한다.
- [ ] object file과 executable을 지운 뒤 archive를 만들고, archive가 비어 있지 않은지 확인한다.
- [ ] iCampus 제출 뒤에만 과제 상태를 완료로 바꾼다.

## Part 1. Linux command line: 왜 이것부터 하는가

### Terminal, shell, command line

슬라이드는 세 가지를 구분한다.

- Terminal: 사용자가 명령을 입력하는 interactive window다.
- Shell: terminal 안에서 실행되며 사용자의 명령을 해석하는 system program이다.
- Command line: `$` prompt 뒤에서 사용자가 실제로 입력하는 한 줄이다.

Shell은 크게 세 종류의 명령을 처리한다. `cd`처럼 shell 내부에 구현된 built-in command, `/usr/bin/gcc`처럼 실행 파일(binary)을 찾아 실행하는 command, 그리고 shell script처럼 script language로 작성한 program이다. 이 구분이 뒤 3주차 shell script와 Makefile로도 이어진다.

슬라이드에서 `$`는 prompt 표시다. 실제로 `gcc`나 `cd`를 칠 때 `$`까지 입력하는 문자가 아니다. 첫 주 `sudo apt`에서와 같은 포인트지만, terminal을 처음 쓰면 이 기호까지 복사하는 실수가 자주 나온다.

### File system과 path

Linux file system은 root directory `/`를 꼭대기로 하는 계층 구조다. 현재 shell이 작업하고 있는 디렉터리를 current working directory, 줄여서 cwd라고 한다. `pwd`는 지금 cwd의 absolute path를 보여주고, `cd`는 cwd를 바꾼다.

Path는 파일이나 디렉터리를 가리키는 계층적 이름이다.

| 표현 | 의미 | 예시 |
| --- | --- | --- |
| absolute path | root `/`부터 전체 위치를 쓴다 | `/home/user/week2/main.c` |
| relative path | 현재 cwd를 기준으로 쓴다 | `week2/main.c` |
| `.` | 현재 directory | `./w2.out` |
| `..` | 부모 directory | `cd ..` |
| `~` | 현재 사용자의 home directory | `cd ~` |

여기서 `./w2.out`의 `./`는 "현재 폴더에 있는 실행 파일"이라는 뜻이다. shell은 보안상 현재 폴더를 무조건 command search path에 넣지 않는 경우가 많으므로, 직접 만든 실행 파일을 실행할 때 `./w2.out`이 필요해질 수 있다. 반대로 `/home/...`처럼 absolute path를 쓰면 현재 위치와 무관하게 같은 대상을 지정할 수 있다.

### 이번 주에 쓸 기본 명령어

| 명령 | 역할 | 이번 실습에서의 쓰임 |
| --- | --- | --- |
| `man command` | 명령/함수 manual 보기 | 옵션이 헷갈릴 때 1차 기준 |
| `echo string` | standard output에 문자열 출력 | shell과 stdout 감각 확인 |
| `date` | 날짜/시간 출력 또는 설정 | 제출 직전 시스템 시각 확인용 |
| `ps`, `ps -ef` | process status 보기 | 뒤 process 단원의 기반 |
| `pwd` | 현재 absolute path 출력 | archive를 어느 위치에서 만드는지 확인 |
| `cd directory` | cwd 이동 | parent directory와 `week2/` 사이 이동 |
| `ls`, `ls -al` | directory contents 보기 | 제출 전 남은 파일 확인 |
| `mkdir directory` | 빈 directory 생성 | `mkdir week2` |
| `rmdir directory` | 빈 directory 삭제 | 비어 있을 때만 사용 가능 |
| `mv source dest` | 이동 또는 이름 변경 | 파일명 수정/상위 폴더 이동 |
| `cp source dest` | 복사 | 원본 백업이나 파일 복사 |
| `rm file` | 파일 삭제 | object/executable 제거 |
| `cat file` | text file 내용 출력 | 작은 결과 파일 빠르게 보기 |
| `head`, `tail` | 앞/뒤 일부 출력 | 긴 log 빠르게 확인 |
| `grep pattern file` | pattern 검색 | log나 source에서 특정 문자열 찾기 |
| `tar` | archive 생성/해제 | 최종 `week2.tar.zst` 만들기 |

`rm -rf`는 슬라이드에 소개되지만, 재귀적으로 강제 삭제하는 명령이다. `week2` 제출 준비에서는 object file과 `w2.out`만 명시적으로 지우면 된다. 경로를 확인하지 않은 `rm -rf`는 실습 폴더 밖의 파일까지 지울 수 있으므로 습관처럼 쓰지 않는 편이 맞다.

### tar와 최종 archive

`tar`는 여러 파일을 하나의 archive로 묶고, 확장자에 맞춰 압축할 수 있다. 슬라이드 예시는 `tar -cavf week2.tar.zst week2/`다.

- `c`: 지정한 파일/디렉터리로 archive를 만든다.
- `x`: archive에서 파일을 푼다.
- `a`: archive 확장자에 맞춰 compression algorithm을 고른다.
- `v`: 묶거나 푸는 파일 목록을 보여준다.
- `f`: archive file name을 받는다.

중요한 순서는 `week2/` 안을 정리한 뒤 parent directory로 올라가서 archive를 만드는 것이다. archive 안에는 source와 과제에서 요구한 log/output 기록이 남아야 하고, `*.o`와 `w2.out` 같은 재생성 가능한 build artifact는 남기지 않는다는 것이 슬라이드의 요구다. archive 작성 뒤에는 slide가 안내한 대로 `vim week2.tar.zst` 등으로 내용이 비어 있지 않은지 확인한다. 단, binary archive를 text editor로 편집하는 것이 아니라 존재와 대략적인 내용 확인 용도로만 연다는 점을 구분한다.

## Part 2. GCC: C file은 어떤 단계를 거쳐 실행되는가

### Compiler의 역할

Compiler는 source code를 다른 표현, 보통 executable binary로 번역한다. 이때 한 번에 마법처럼 binary가 생기는 것이 아니라 intermediate file이 생기는 여러 단계가 있다.

```text
hello.c
  |
  | Preprocessor: #include, #define 처리
  v
hello.i
  |
  | Compiler: C를 assembly로 번역
  v
hello.s
  |
  | Assembler: assembly를 machine-code object로 번역
  v
hello.o
  |
  | Linker: 여러 object와 library를 연결
  v
hello 또는 a.out
```

GCC는 GNU Compiler Collection이다. 처음에는 GNU C Compiler로 시작했지만 C만이 아니라 여러 언어의 compiler 집합이라는 의미로 Collection이라는 이름을 쓴다. 이번 주에는 C source를 기준으로 이 흐름을 직접 관찰한다.

### 전처리, compile, assemble, link의 차이

전처리(preprocessing)는 `#include`, macro 같은 preprocessor directive를 처리한다. 그래서 `.i` 파일에는 header file의 내용이 포함되고 macro가 바뀌며 comment가 제거된 source 형태가 남는다.

compile proper는 C language를 assembly language로 번역하는 단계다. 결과가 `.s` 파일이다. assembler는 `.s`를 object code로 바꾸며 결과는 `.o` 파일이다. `.o`에는 machine code 조각이 들어 있지만, 다른 source file의 함수나 library symbol과의 연결이 끝나지 않았을 수 있으므로 아직 보통 바로 실행하지 않는다.

linker는 여러 `.o` file과 필요한 library를 연결해 하나의 executable을 만든다. `main.c`가 `plus.c` 또는 `minus.c`의 함수를 호출하면, compile은 각 파일을 독립적으로 `.o`까지 만들고 linking이 function definition과 call site를 연결한다. 그래서 multi-file program에서 linker error가 나면 "함수를 선언했나"뿐 아니라 "해당 source/object를 link command에 포함했나"까지 봐야 한다.

### `gcc`의 기본 output과 `-o`

`gcc source.c`처럼 `-o`를 지정하지 않고 compile/link까지 진행하면 기본 executable 이름은 `a.out`이다. `-o filename`을 쓰면 output 이름을 지정할 수 있다.

```bash
# 기본 executable 이름: a.out
gcc hello.c

# 원하는 executable 이름 지정
gcc hello.c -o hello
```

이번 실습에서 final executable 이름은 `w2.out`으로 요구된다. object file을 만들 때는 compiler가 source file 이름에 맞춰 `main.o`, `plus.o`, `minus.o`를 만드는 흐름이고, link 결과만 `-o w2.out`으로 명확히 지정하면 된다.

### 중간 결과를 보는 옵션

| 옵션 | 어디까지 진행하는가 | 결과/의미 |
| --- | --- | --- |
| `-save-temps` | 전체 compile 과정 | 평소 temporary인 `.i`, `.s`, `.o`를 보관 |
| `-E` | preprocessing에서 멈춤 | preprocessed C source 출력 |
| `-S` | compile에서 멈춤 | assembly file `.s` 출력, assemble 안 함 |
| `-c` | assemble까지 진행 | object file `.o` 출력, link 안 함 |
| `-o file` | output 이름 지정 | default `a.out` 대신 원하는 이름 |

`-E`를 보면 source에 써 둔 comment가 없어지고 header code와 macro expansion이 들어간다는 슬라이드 설명을 실제로 볼 수 있다. `-S`는 C와 assembly 사이를 보고 싶을 때, `-c`는 여러 source를 따로 object file로 만든 뒤 link하고 싶을 때 쓴다. 이번 과제는 바로 이 `-c` 방식을 요구한다.

### Optimization level과 debugging의 관계

슬라이드는 GCC optimization level도 정리한다.

| 옵션 | 의미 |
| --- | --- |
| `-O0` | 기본값. compile time이 짧고 debugging이 쉬움 |
| `-O` 또는 `-O1` | execution time과 code size를 어느 정도 줄임 |
| `-O2` | code size trade-off를 크게 하지 않는 다수 optimization 사용 |
| `-O3` | execution time 중심의 강한 optimization, code size가 커질 수 있음 |
| `-Og` | debugging 경험을 고려한 optimization |
| `-Os` | size 최적화, 일부 size 증가 optimization 제외 |

디버깅에서 `-O0`이 중요한 이유는 compiler가 코드를 많이 바꾸지 않아 source line과 실제 실행 흐름의 대응이 비교적 직관적이기 때문이다. optimization이 강하면 variable이 register로 사라지거나, line order가 달라 보이거나, 함수가 inline되어 breakpoint 동작이 달라질 수 있다. 그래서 이번 과제는 object file에 `-ggdb -c -O0`, executable linking에도 `-O0`을 요구한다.

### 여러 source file을 link하는 예시 구조

슬라이드 21~22쪽은 `main.c`와 `sum.c` 예시로 linking을 보여준다. `main.c`에는 function prototype이 있고, 실제 `sum` definition은 `sum.c`에 있다. 각각을 object file로 만든 뒤 linker가 연결해야 final executable이 된다.

2주차 Lab Exercise도 같은 형태다. `main.c`가 `plus`, `minus`를 선언하고 호출하며, 실제 function definition은 각자 `plus.c`, `minus.c`에 둔다. 이 구조가 중요한 이유는 source마다 compile error를 분리해서 볼 수 있고, large program에서도 module별로 build한 뒤 link하는 방식으로 확장되기 때문이다.

실습에서 요구한 build 흐름은 다음과 같이 이해하면 된다. 이것은 제출 코드가 아니라 slide가 요구하는 build 단계다.

```bash
gcc -ggdb -c -O0 main.c
gcc -ggdb -c -O0 plus.c
gcc -ggdb -c -O0 minus.c
gcc -O0 main.o plus.o minus.o -o w2.out
```

첫 세 command는 source를 object file로 분리한다. 마지막 command는 세 object file을 link해 `w2.out`을 만든다. 처음부터 모든 `.c`를 한 command로 넣어 executable을 만들 수는 있지만, PDF는 object file을 명시적으로 만든 뒤 link하라고 요구하며, 그렇지 않으면 GDB output이 달라질 수 있다고 경고한다.

## Part 3. GDB: "왜 틀렸는지"를 실행 중에 확인하기

### Bug와 compiler error는 다르다

슬라이드의 warm-up code는 local variable `a`의 address를 반환하는 함수다. function call이 끝나면 local variable의 lifetime도 끝나므로, 그 address를 반환해 나중에 사용하는 것은 안전하지 않다. 이 경우 문법이 맞아 compiler가 executable을 만들어줄 수 있어도, 의미상 잘못된 program이 된다.

이 예시는 syntax error와 semantic/logic error를 나누기 위한 것이다.

- Syntax error: code formatting이나 language rule을 어긴 실수다. compiler/linker가 알려준다.
- Semantic/logic error: programmer가 생각한 동작과 실제 program 동작이 다른 문제다. compiler가 자동으로 잡지 못할 수 있고, 이런 문제가 bug다.

실제로 bug는 segmentation fault/core dump처럼 바로 죽는 형태, infinite loop, 실행은 되지만 output이 틀린 형태로 나타날 수 있다. line by line으로 읽기, 값을 중간중간 print하기, debugger 쓰기가 대표 전략이며, 실무에서는 이 셋을 섞어 쓴다.

### GDB를 쓰기 위한 compile 조건

GDB는 GNU Debugger다. text-based debugger라 terminal에서 실행한다.

```bash
gdb w2.out
```

GDB는 executable file뿐 아니라 core dump file이나 이미 실행 중인 process의 PID도 대상으로 받을 수 있다. 하지만 이번 실습은 executable `w2.out`을 load하는 형태다.

Source-level debugging 정보를 충분히 보려면 compile할 때 `-ggdb`를 넣어야 한다. `-ggdb`는 GDB가 source line, variable, function을 더 잘 연결해서 보여줄 수 있도록 debug information을 넣는다. `-O0`은 code transformation을 줄여 debugger에서 source 흐름을 따라가기 쉽게 한다. GDB가 설치되지 않았다면 slide가 제시한 `sudo apt install gdb`로 설치할 수 있다.

### GDB에서 볼 수 있는 것

GDB는 source code line, assembly, variable/register value, stack frame을 볼 수 있고 variable 값 변경, breakpoint 설정, step-by-step execution을 할 수 있다. 즉 "출력이 180과 20으로 나왔네"에서 끝나는 것이 아니라, 어느 function call에서 어떤 값이 계산되었는지 실행 중간에 멈춰서 검증할 수 있다.

이 과제에서는 `plus`와 `minus` function에 breakpoint를 두고, 각각의 return value가 register `rax`에 들어오는 것을 확인하는 것이 목표다. x86-64 ABI에서 integer/pointer 계열 return value가 일반적으로 `rax`에 전달되는 convention을 직접 보는 셈이다. ARM 등 non-x64 환경은 `r0` 또는 `x0`을 사용하라고 PDF가 별도로 안내한다.

### GDB 기본 명령어

| 명령 | 축약 | 의미 |
| --- | --- | --- |
| `gdb executable` |  | executable을 load하고 GDB 시작 |
| `run args` | `r args` | command-line argument와 함께 실행 |
| `list point` | `l` | line/function 주변 source 보기 |
| `break point` | `b` | 특정 line 또는 function에 breakpoint 설정 |
| `continue` | `c` | 다음 breakpoint 또는 종료까지 실행 계속 |
| `print value` | `p` | 현재 지점의 variable/function return value 보기 |
| `display value` | `disp` | 멈출 때마다 value 자동 표시 |
| `watch variable` | `wa` | variable이 바뀌면 execution pause |
| `next` | `n` | 다음 line 실행, function 내부는 건너뜀 |
| `step` | `s` | function call 내부 첫 line으로 들어감 |
| `backtrace` | `bt` | 현재 위치까지의 function call sequence 확인 |
| `up`, `down` |  | stack frame을 위/아래로 이동 |
| `info registers` | `ir` | register 내용 확인 |
| `quit` | `q` | GDB 종료 |
| `help topic` | `h topic` | 특정 command 도움말 |

`next`와 `step`의 차이는 꼭 기억할 만하다. `next`는 function call을 한 line처럼 넘기고 싶을 때, `step`은 그 function 내부에 들어가고 싶을 때 쓴다. `backtrace`는 crash나 이상한 return value가 나왔을 때 "어떤 함수 호출을 거쳐 여기까지 왔나"를 보는 명령이다.

### register와 output을 왜 둘 다 보는가

일반 실행은 user-visible output이 맞는지 확인한다. 반면 GDB의 `rax` 확인은 function이 caller에게 값을 돌려주는 순간의 low-level convention을 확인한다. output만 맞다고 해도 우연히 다른 코드가 값을 덮어썼을 가능성을 공부할 수 있고, register만 맞다고 해도 formatting이나 I/O가 틀렸을 수 있다. 이번 과제는 두 증거가 같은 값을 가리키게 하는 실습이다.

`plus(100, 80)`의 return value와 `minus(100, 80)`의 return value는 program output과 일치해야 한다. 단순히 expected number만 적는 것이 아니라, actual source -> object -> linked executable -> GDB stop -> register -> stdout가 한 흐름으로 맞는지를 확인하는 것이 채점 포인트라고 보면 된다.

## 2주차 Lab Exercise: 요구사항을 작업 순서로 풀어쓰기

### 1. 디렉터리와 source 구성

원하는 위치에 `week2/` directory를 만든다. 그 안에 PDF가 제시한 `main.c`를 두고, `plus.c`, `minus.c`는 본인이 작성한다. `main.c`는 command-line argument 두 개를 받아 `strtoll`로 숫자로 바꾼 후 `plus`와 `minus`를 호출하고 두 결과를 출력하는 구조다.

여기서 source file을 한 folder에 둬야 하는 이유는 compile command와 final archive의 기준이 명확해지기 때문이다. 파일을 여기저기 흩어 놓고 object만 한 곳에 만들면 link error나 제출 누락이 생기기 쉽다.

### 2. object file 세 개 만들기

각 `.c` file에 대해 `-ggdb -c -O0` 옵션을 사용한다. 결과는 `main.o`, `plus.o`, `minus.o`다.

- `-ggdb`: GDB용 debug information
- `-c`: link하지 않고 object file까지만 생성
- `-O0`: debugging 친화적인 no optimization level

이 단계에서 compile error가 나면 아직 linker 문제가 아니다. 해당 `.c` file의 syntax, include, type, function declaration을 먼저 확인해야 한다.

### 3. link해서 `w2.out` 만들기

세 object file을 input으로 하여 `w2.out`을 만든다. `main.o`가 `plus`, `minus` symbol을 참조하고, 다른 두 `.o`가 definition을 제공하는 구조다. 만약 "undefined reference"가 나오면 보통 object file 하나를 마지막 link command에 넣지 않았거나 function signature가 맞지 않는 상황을 의심하면 된다.

PDF는 object file을 거치지 않고 곧바로 `w2.out`을 만들면 GDB output이 달라질 수 있다고 명시한다. 따라서 이번에는 편한 one-line compile보다 요구한 object-file 단계가 우선이다.

### 4. 일반 실행으로 output 확인

GDB에 들어가기 전 executable이 정상 실행되는지 먼저 확인한다. argument 두 개가 필요하므로 argument가 빠졌을 때 usage message가 나오는지, `100 80`처럼 정상 입력을 줄 때 두 operation 결과가 출력되는지 각각 보면 좋다. 이 단계가 맞지 않으면 debugger 안에서 register를 보기 전에 source/build부터 고치는 편이 빠르다.

### 5. GDB logging과 breakpoint

GDB를 시작한 뒤 먼저 logging을 켠다.

```gdb
set logging enabled on
```

slide는 log file 예시를 `gdb.txt`로 제시한다. 이어서 `plus`, `minus`에 breakpoint를 두고 input `100`, `80`으로 실행한다. PDF의 실행 예시 화면은 standard output을 `stdout.txt`로 redirection하는 흐름을 보여준다. 그러면 terminal 출력과 GDB command/response가 섞이지 않아 결과물을 분리해 남길 수 있다.

breakpoint에서 멈췄다는 것만으로 반환값이 이미 register에 있다는 뜻은 아니다. function entry인지 return 이후인지에 따라 `rax` 값의 시점이 다르다. 따라서 slide가 보여준 GDB 흐름대로 function 실행을 진행한 뒤 return value를 확인해야 하며, log에는 breakpoint와 register 확인 과정이 남아야 한다.

### 6. 반환 register 확인

x64 machine에서는 `rax`를 확인한다. GDB의 `print`나 `info registers` 계열 명령으로 register value를 보고, `plus`와 `minus`의 각 결과가 program output과 맞는지 대조한다. ARM 계열 또는 non-x64면 `rax`가 아니라 `r0` 또는 `x0`을 시도하라는 PDF 안내가 있다.

이 항목은 "내 컴퓨터가 어떤 architecture인가"도 한 번 생각하게 한다. 수업 예시는 x86-64 기준이지만, Apple Silicon/ARM VM 같은 환경에서는 register naming convention이 달라질 수 있다. 이런 표준 환경이 아닌 환경을 썼다면 제출 comment field에 그 환경을 명시하라고 되어 있다.

### 7. 제출용 folder를 정리한다

실습 중에는 `.o`와 `w2.out`이 있어야 하지만, 최종 제출 archive에는 그 build artifact를 남기지 않는다. source code, GDB log, stdout file처럼 재현/검증에 필요한 결과를 남기고 object file과 executable은 삭제한다. 삭제 전 `ls -al`로 현재 folder를 보면서 어떤 파일이 남았는지 확인하는 습관이 좋다.

슬라이드의 삭제 표현이 짧게 보이더라도, 실제로는 생성한 executable 이름 `w2.out`을 정확히 지정해 지우는 방식이 안전하다. 모호한 wildcard나 경로 없이 recursive delete를 쓰지 않는다.

### 8. archive 생성, 검증, iCampus 제출

`week2/`의 parent directory에서 `week2.tar.zst`를 만든다. archive를 만든 뒤 실제 archive file이 존재하고 비어 있지 않은지 확인한다. PDF는 `vim week2.tar.zst`로 확인하는 예시를 제공한다. 최종적으로 iCampus에 이 archive 하나를 제출한다.

마감은 오늘 23:59이고 no late submission이다. WSL, Docker, ARM 등 standard environment를 쓰지 않았다면 iCampus comment field에 환경을 적는다. 이 comment는 변명용 문장이 아니라 TA가 architecture/register/output 차이를 해석할 수 있도록 남기는 기술 메모다.

## 헷갈리기 쉬운 지점

- `.c`, `.i`, `.s`, `.o`, executable은 서로 다른 build 단계의 산출물이다.
- `-c`는 compile을 멈추는 옵션이지 executable을 만드는 옵션이 아니다.
- `-ggdb`는 GDB가 source와 symbol을 보기 위한 debug information이고, `-O0`은 debug 흐름을 덜 뒤틀리게 한다.
- function prototype은 declaration이고, 다른 `.c` file의 실제 body는 definition이다. linker는 definition을 연결한다.
- `break plus`는 plus function에 들어가기 전에 멈추는 것이다. return value 확인 시점은 그 뒤 실행 흐름에 따라 달라진다.
- `rax`는 x86-64 기준이다. ARM 계열에는 `r0`/`x0`이라는 PDF의 별도 지시가 있다.
- `gdb.txt`와 `stdout.txt`는 서로 성격이 다르다. 전자는 debugger session 기록, 후자는 program standard output 기록이다.
- archive에는 source/기록은 남기고 `.o`, `w2.out`은 남기지 않는다.
- `tar.zst`가 만들어졌다는 사실만으로 충분하지 않다. 실제로 archive가 비어 있지 않은지 확인해야 한다.

## 이 주차가 다음 주차와 연결되는 방식

2주차의 gcc/gdb는 단지 C compiler 사용법이 아니다. 3주차 Git과 Makefile을 배우면 여러 source/object dependency를 더 체계적으로 관리하게 된다. 4주차 이후에는 `open`, `read`, `write`로 file I/O system call을 쓰고, 5주차에는 process, 6주차에는 signal, 7~10주차에는 IPC/socket으로 program이 system resource와 다른 process를 다루게 된다.

그 뒤 pthread와 synchronization으로 가면 "source가 여러 개" 수준에서 "실행 흐름도 여러 개"인 문제로 커진다. 그때도 compiler option, linker error, GDB breakpoint, backtrace, register/stack frame 관찰은 계속 쓰인다. 그래서 이번 주에 command-line build와 debugger를 한 번 제대로 손에 익혀두면 후반 PA에서 시간을 크게 아낀다.

운영체제 수업과의 연결도 분명하다. 운영체제는 process, memory, system call, concurrency의 원리를 설명하고, 시프실은 Linux user program에서 compile/debugging을 거쳐 실제 API와 실행 상태를 만지는 실습이다. 이번 주의 `ps`는 process 관찰의 입구이고, `gdb`의 stack/register는 user program의 실행을 들여다보는 입구다.

## 복습 체크 질문

- Terminal과 shell은 무엇이 다르고, command line의 `$`는 무엇을 뜻하는가?
- absolute path와 relative path는 언제 각각 유리한가?
- `.` , `..`, `~`는 각각 무엇을 가리키는가?
- `gcc`의 preprocessing, compilation, assembly, linking 결과 extension은 각각 무엇인가?
- `-E`, `-S`, `-c`, `-save-temps`, `-o`는 어떤 단계에서 멈추거나 무엇을 바꾸는가?
- `-O0`을 GDB 실습에 쓰는 이유는 무엇인가?
- object file은 왜 만들고, linker는 무엇을 해결하는가?
- compiler가 잡아주는 syntax error와 GDB가 필요한 logic error는 어떻게 다른가?
- `next`와 `step`은 function call을 만났을 때 어떻게 다른가?
- `backtrace`는 어떤 상황에서 유용한가?
- x64에서 이번 과제가 `rax`를 확인하라고 하는 이유는 무엇인가?
- 제출 archive에 `.o`와 `w2.out`을 빼라는 이유는 무엇인가?
- 표준 환경이 아닌 WSL/Docker/ARM을 쓰면 무엇을 추가로 해야 하는가?

## 페이지별 흐름

| 페이지 | 내용 | 이번 주에 가져갈 것 |
| --- | --- | --- |
| 1 | Linux Command / GCC / GDB 표지 | command line, build, debugging 세 묶음 |
| 2 | Linux Commands 표지 | terminal part 시작 |
| 3 | Terminal, shell, command line | prompt와 shell 역할 |
| 4 | `man`, `echo` | manual을 1차 자료로 쓰기 |
| 5 | `date`, `ps` | 시간과 process status |
| 6 | Linux file system, cwd, path | absolute/relative path와 `.`, `..`, `~` |
| 7 | `pwd`, `cd`, `ls` | 작업 위치와 파일 목록 확인 |
| 8 | `mkdir`, `rmdir`, `mv` | folder 생성과 파일 이동/이름 변경 |
| 9 | `cp`, `rm` | 복사와 안전한 삭제 |
| 10 | `cat`, `head`, `tail`, `grep` | text/log 빠른 확인 |
| 11 | `tar` | `tar.zst` archive 생성/해제 |
| 12 | GCC 표지 | build part 시작 |
| 13 | compiler와 optimization level | `-O0`과 debugging 연결 |
| 14 | GCC procedure | `.i -> .s -> .o -> executable` |
| 15 | compile pipeline 그림 | source부터 binary까지의 변환 |
| 16 | `-o` | default `a.out`, output 이름 지정 |
| 17 | `-save-temps` | 중간 파일 보존 |
| 18 | `-E` | preprocessing 결과 확인 |
| 19 | `-S` | assembly 출력 |
| 20 | `-c` | object file 생성, link는 안 함 |
| 21 | linking 개념 그림 | object file 여러 개를 executable로 연결 |
| 22 | `main.c`/`sum.c` example | prototype와 definition이 다른 file에 있을 수 있음 |
| 23 | GDB 표지 | debugging part 시작 |
| 24 | local variable address bug | compile 성공과 correct program은 다름 |
| 25 | syntax vs semantic/logic error | bug의 의미 |
| 26 | bug 찾기 전략 | debugger는 전문 개발자의 기본 도구 |
| 27 | GDB 실행과 `-ggdb` | source-level debugging 준비 |
| 28 | GDB capabilities | line, assembly, variable, register, stack frame |
| 29 | GDB command table | `b`, `r`, `n`, `s`, `bt`, `p`, `q` |
| 30 | string literal 수정 예제 | segfault/debugging example |
| 31 | Lab Exercise source/build 요구 | `main.c`, `plus.c`, `minus.c`, object file, `w2.out` |
| 32 | Lab Exercise GDB 요구 | logging, breakpoint, `100 80`, return register |
| 33 | GDB run 예시 | stdout redirection을 포함한 session 흐름 |
| 34 | `gdb.txt`, `stdout.txt` 예시 | 두 기록 파일의 역할 확인 |
| 35 | submission folder와 archive | build artifact 삭제 후 tar 생성 |
| 36 | 제출 주의 | archive nonempty 확인, iCampus 제출, no late, 환경 comment |

## 오늘의 종료 조건

오늘은 "강의를 봤다"가 아니라 아래 상태가 되면 끝이다.

- `week2.tar.zst`가 iCampus에 올라가 있다.
- archive 안에는 source와 요구된 debugging/output 기록이 있고, `.o`와 `w2.out`은 없다.
- 일반 실행 output과 GDB register에서 본 반환값이 서로 맞는다.
- 내 환경이 표준 Ubuntu x64가 아니라면 제출 comment에 그 사실을 적었다.
- 제출 뒤 iCampus 상태를 직접 확인했다.
