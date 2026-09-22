# 시스템프로그래밍실습 - 3주차 스터디: Shell, Makefile, Git

## 한 줄 결론

3주차는 C program을 한 번 build하는 단계에서 나아가 Shell로 반복 작업을 자동화하고, Makefile로 dependency를 관리하고, Git으로 변경 이력을 남기는 주차다. 2주차의 gcc -> linker -> gdb 흐름을 재현 가능하게 만든다.

    source files + input
           |
           | shell script: command 순서 자동화
           v
         make
           |
           | dependency / modification time
           v
       w3 executable
           |
           | output / diff / clean
           v
    repeatable result -> git add / commit -> history

## 목차

1. 원본 자료와 전체 구조
2. Shell, standard descriptor, redirection, pipe
3. Shell script: shebang, variable, argument, condition, loop
4. Makefile: rule, dependency, variable, automatic variable
5. Git: working tree, staging, commit, branch
6. Week 3 Lab Exercise
7. 혼동 방지와 복습 질문
8. PDF 페이지 지도

---

## 1. 원본 자료와 전체 구조

- 원본: week3-shell&makefile&git.pdf (39쪽)
- 배포 코드: main.c, plus.c, minus.c
- 범위: Shell script, Makefile, Git, Makefile Lab, score.sh Lab
- 핵심: 같은 C code를 일관되게 build, test, clean, archive하는 흐름 만들기

    WEEK 3
        |
        +-- Shell: command flow, stdin/stdout/stderr, redirection, pipe
        +-- Makefile: build graph, target, prerequisite, recipe
        +-- Git: working tree -> index -> commit -> branch / merge
        +-- Lab: make -> expected output -> actual output -> diff -> clean

2주차에서는 source file을 각각 object file로 만들고 link했다. 이번 주에는 command를 Makefile 규칙으로 기록하고, shell script가 build와 결과 비교를 담당한다. Git은 source, Makefile, script가 어떤 단계로 바뀌었는지 저장한다.

## 2. Shell: user와 Unix 사이의 interface

Shell은 user의 command를 해석해 Unix에 전달하는 interface program이다. Terminal은 화면 창이고, Shell은 command를 해석하는 program이며, command line은 prompt 뒤에 입력하는 한 줄이다.

    Terminal -> Shell (bash) -> built-in command / external program / shell script

Prompt의 달러 기호는 입력 대기 표시다. 반면 variable 앞의 달러 기호는 expansion 문법이다. 이 둘을 구분해야 한다.

### Standard file descriptor

| FD | 이름 | 기본 연결 | 역할 |
| --- | --- | --- | --- |
| 0 | stdin | keyboard | standard input |
| 1 | stdout | terminal | normal output |
| 2 | stderr | terminal | error output |

이 정수 descriptor는 4주차 Unix I/O로 이어진다. 3주차에는 shell이 이 data flow를 어디로 보낼지 바꾸는 방법을 배운다.

### Redirection과 pipe

- greater-than: stdout을 새 file에 기록하고 기존 내용을 overwrite한다.
- double greater-than: stdout을 file 끝에 append한다.
- less-than: stdin을 file에서 받는다.
- vertical bar: 왼쪽 process stdout을 오른쪽 process stdin으로 연결한다.

    ./w3 A B -> stdout -> output.txt
    cat test.txt -> stdout -> pipe -> stdin -> grep "a"

이번 Lab의 output redirection은 w3가 output file을 직접 여는 것이 아니라 shell이 stdout 연결을 바꾸는 것이다.

## 3. Shell script

### Shebang과 실행

Shell script는 command를 text file에 저장해 interpreter가 처리하도록 만든 file이다. 첫 줄의 shebang은 interpreter를 지정한다. 나머지 hash 시작 줄은 comment다.

실행 방식은 Bash를 명시해 script를 해석하거나, owner execute permission을 준 뒤 current directory의 script를 실행하는 방식이 있다. current directory에서 실행할 때 dot slash가 필요하고, permission이 없으면 chmod u+x가 먼저 필요하다.

### Variable, command substitution, arithmetic

Shell assignment에는 공백을 넣지 않는다. VAR="Hi"는 맞고 VAR = "Hi"는 아니다.

| 표현 | 의미 |
| --- | --- |
| dollar VAR | variable value |
| dollar(parentheses command) | command stdout을 문자열로 사용 |
| dollar(double-parentheses expression) | integer arithmetic |
| dollar 1, dollar 2 | 첫째, 둘째 argument |
| dollar hash | argument 개수 |
| dollar 0 | script name |
| dollar dollar | script process ID |
| HOME, PATH | environment variable |

score.sh A B에서 첫 argument는 A, 둘째 argument는 B다. 그래서 shell arithmetic으로 A+B와 A-B를 expected result로 만든다.

### Condition과 loop

Shell if block은 then으로 시작해 fi로 끝난다. File test는 file 존재나 directory 존재를 보고, string test는 string equality를 보고, arithmetic test는 C와 유사한 integer comparison과 logical operator를 사용한다.

For loop는 value set을 순회하고, C-style for 문도 사용할 수 있다. While loop는 condition이 true인 동안 반복한다. Shell은 braces 대신 do/done, then/fi를 쓰므로 delimiter를 빠뜨리는 syntax error를 조심한다.

## 4. Makefile: build dependency를 규칙으로 적기

여러 source를 매번 전부 compile하면 비효율적이고 변경을 놓치기 쉽다. Makefile은 target을 만들 prerequisite와 recipe를 기록하는 build description이다.

    main.c  -> main.o  --+
    plus.c  -> plus.o  --+-> w3
    minus.c -> minus.o --+

Rule은 target, prerequisites, recipe로 구성된다. Recipe indentation은 반드시 tab이다. 첫 rule target은 make만 입력했을 때 default goal이 된다.

Make는 target이 없거나 prerequisite가 target보다 새로울 때 recipe를 실행한다. 따라서 수정하지 않은 source에 해당하는 object file은 다시 build하지 않는다.

### Makefile variable과 automatic variable

| automatic variable | 뜻 |
| --- | --- |
| target marker | current target |
| all prerequisite marker | all prerequisites |
| newer prerequisite marker | target보다 새 prerequisite |
| first prerequisite marker | first prerequisite |

일반 variable은 target name, compiler, compiler flags, object list 같은 반복 값을 한 곳에 둔다. Lab은 variable 하나 이상과 automatic variable 하나 이상을 요구한다.

Clean은 actual file을 만드는 target이 아니라 action이므로 phony target으로 선언한다. Clean rule은 자신이 만든 object file과 executable만 지워야 한다. Pattern rule의 percent wildcard는 source-to-object처럼 반복되는 rule을 일반화한다.

## 5. Git: 변경을 commit으로 남기기

Git의 기본 흐름은 아래다.

    working tree -> git add -> index/staging area -> git commit -> history

| command | 역할 |
| --- | --- |
| git init | repository 생성 |
| git status | modified, staged, untracked 상태 확인 |
| git add | 다음 commit 대상 stage |
| git rm | working tree와 index에서 제거 |
| git commit | staged change를 history에 기록 |
| git log | history 확인 |
| git branch | branch 확인/생성/삭제 |
| git switch | branch 전환 |
| git merge | history 결합 |

git add는 commit이 아니다. Add는 다음 commit에 넣을 content를 선택하는 단계다. 따라서 broad add 전에 status로 generated file, test output, archive, source를 구분한다.

gitignore는 build output, log, generated data처럼 track하지 않을 pattern을 선언한다. 이미 tracked인 file이 자동으로 untrack되는 것은 아니다.

## 6. Week 3 Lab Exercise

### Exercise 1 - Makefile

week2의 main.c, plus.c, minus.c를 사용해 week3 directory를 만들고 Makefile을 직접 작성한다.

필수:
- Makefile variable 1개 이상
- automatic variable 1개 이상
- w3 executable을 만드는 rule
- 생성한 object file과 executable만 지우는 clean rule

확인은 make가 한 번 성공하는 데 그치지 않는다. source 하나를 수정한 뒤 make가 필요한 object만 rebuild하는지 이해해야 한다.

### Exercise 2 - score.sh

    ./score.sh A B
        |
        +-- make
        +-- A+B, A-B 계산 -> answer.txt
        +-- ./w3 A B -> output.txt
        +-- diff -> result.txt
        +-- make clean

PDF가 지정한 순서는 Makefile build, shell arithmetic expected output 생성, w3 actual output 생성, diff result 기록, clean이다. 정상이라면 result.txt는 empty다. w3는 stdin이 아니라 ARGV로 argument를 받으므로 command line argument 방식으로 실행한다. 별도 error checking은 요구하지 않는다.

제출 전 week3 folder에는 main.c, plus.c, minus.c, Makefile, score.sh만 남긴다. Object file, executable, answer/output/result text file은 generated artifact이므로 넣지 않는다. STUDENTID_w3.tar.zst 형식으로 iCampus에 제출한다. AMD64가 아닌 ARM64 VM 또는 ARM 위 Docker 환경이면 PDF 지시대로 comment에 환경을 적는다. PDF가 end of day로만 적은 실제 마감은 iCampus 제출 페이지를 최종 기준으로 확인한다.

## 7. 혼동 방지

- prompt의 dollar와 variable expansion의 dollar는 다르다.
- output overwrite와 append는 다르다.
- pipe는 left stdout을 right stdin으로 연결한다.
- shell assignment는 공백 없이 쓴다.
- command substitution과 arithmetic expansion은 다르다.
- Makefile recipe indentation은 tab이다.
- Make automatic variable과 shell variable은 다르다.
- clean은 phony action이며 만든 artifact만 지운다.
- git add와 git commit은 staging과 history 기록이라는 차이가 있다.
- diff output이 empty면 두 file 내용이 같다.
- score script 결과 file은 검증용이며 제출 source 목록에 포함되지 않는다.

## 8. 복습 질문

1. Terminal, Shell, command line의 차이는?
2. stdin, stdout, stderr의 descriptor number는?
3. Redirection과 pipe는 data flow를 어떻게 바꾸는가?
4. Shebang과 execute permission은 왜 필요한가?
5. Script argument와 environment variable은 어떻게 다른가?
6. Make rule의 target, prerequisite, recipe는 무엇인가?
7. Make는 rebuild 여부를 무엇으로 판단하는가?
8. Clean rule이 안전해야 하는 이유는?
9. Git add와 commit의 차이는?
10. score.sh의 answer, output, result file은 각각 무엇을 검증하는가?

## 9. PDF 페이지 지도

| 페이지 | 내용 | 핵심 |
| --- | --- | --- |
| 2-16 | Shell | interface, FD, redirection, pipe, script, variable, condition, loop |
| 17-24 | Makefile | rule, timestamp dependency, variable, automatic variable, phony/pattern |
| 25-35 | Git | add/commit/status, branch/switch/merge/log, ignore |
| 36 | Exercise 1 | variable + automatic variable + w3 + clean |
| 37-38 | Exercise 2 | score script와 diff verification |
| 39 | 제출 | source 5개만 archive, environment comment |

## 이번 주 종료 조건

- make만으로 w3가 build된다.
- source 수정 뒤 필요한 object만 다시 build되는 이유를 설명한다.
- score.sh가 expected와 actual output을 diff로 비교한다.
- clean 뒤에는 내가 만든 object와 executable만 사라진다.
- archive 안에는 source, Makefile, script만 남아 있다.
- git status에서 tracked, staged, untracked를 설명할 수 있다.
## 10. Make가 실제로 갱신을 판단하는 흐름

Make는 단순히 위에서 아래로 command를 실행하는 script가 아니다. Default target에서 시작해 prerequisite를 재귀적으로 확인하고, 각 prerequisite의 modification time과 target의 modification time을 비교한다.

    make w3
      |
      +-- main.o 필요 -> main.c가 main.o보다 새롭나?
      |     +-- yes: main.o recipe 실행
      |     +-- no : main.o 유지
      +-- plus.o 필요 -> plus.c 비교
      +-- minus.o 필요 -> minus.c 비교
      +-- object 중 하나라도 새로 생성/갱신됐나?
            +-- yes: w3 link recipe 실행
            +-- no : w3 유지

그래서 header file dependency를 Makefile에 빠뜨리면 header를 바꿔도 관련 object가 rebuild되지 않을 수 있다. 이번 Lab source는 단순하지만, target의 모든 실제 input을 prerequisite에 적는 습관은 뒤의 큰 C project에서도 중요하다.

Make variable은 반복되는 compiler와 option, target name, object list의 불일치를 줄인다. Automatic variable은 rule을 generic하게 만들어 source name을 recipe마다 복사하지 않게 한다. Pattern rule은 file name이 달라도 동일한 source-to-object 변환을 적용한다. 이 세 요소를 같이 쓰면 file이 늘어나도 Makefile을 덜 중복해 유지할 수 있다.

## 11. Git 상태를 읽는 법

Git status는 세 영역의 차이를 보여 준다.

    repository HEAD: 마지막 commit
          |
          +-- index: 다음 commit에 포함될 staged version
          |
          +-- working tree: 현재 disk 위 file

Modified but not staged는 working tree만 바뀐 상태다. Changes to be committed는 index까지 올린 상태다. Untracked는 Git이 아직 관리하기 시작하지 않은 file이다. 실습에서는 source/Makefile/script만 stage하고, executable, object file, output log, archive가 섞이지 않았는지 status로 확인한다.

Commit message는 나중에 내가 보아도 무엇이 바뀌었는지 알 수 있게 적는다. Branch는 실험적인 수정이나 다른 기능 작업을 main history와 분리할 때 쓰며, merge는 두 history를 결합한다. 이번 개인 실습에서 복잡한 branch workflow가 필수는 아니지만, Git이 backup command가 아니라 변경 단위를 관리하는 도구라는 점은 분명히 잡아둔다.

## 12. 실습 직전 5분 체크

- 현재 directory와 week3 directory를 pwd, ls로 확인한다.
- Makefile recipe 앞에 tab이 들어갔는지 editor에서 확인한다.
- make 후 w3가 생성되는지, make clean 후 source는 남는지 확인한다.
- score.sh에 execute permission이 있는지 확인한다.
- answer.txt와 output.txt가 같은 줄 순서와 newline을 가지는지 확인한다.
- diff result가 empty인지 확인한다.
- archive를 만들기 전 generated file을 week3 folder에서 제거한다.
- git status처럼 folder contents를 한 번 더 확인한다.