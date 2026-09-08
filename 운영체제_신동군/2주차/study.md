# 운영체제 - 신동군 2주차 학습지도

## 한줄 결론

2주차는 OS가 CPU를 여러 process에게 나눠 주면서도 통제권을 잃지 않는 방법을 다룹니다. 핵심은 **user mode와 kernel mode**, **system call/trap**, **timer interrupt**, **scheduler/context switch**이고, 그 통제를 받는 실행 단위가 **process**입니다. 즉 이 주차의 전체 이야기는 아래 한 줄로 연결됩니다.

```text
프로그램을 빠르게 직접 실행시키고 싶다
        +
프로그램이 CPU, memory, I/O를 마음대로 쓰면 안 된다
        |
        v
user mode / kernel mode + trap / interrupt
        |
        v
OS가 CPU를 되찾는다
        |
        v
scheduler가 다음 process를 고른다
        |
        v
context switch로 CPU virtualization을 만든다
```

## 빠른 구조도

```text
                              [ 2주차 전체 ]
                     CPU Virtualization의 실제 구현
                "여러 프로그램이 CPU를 나눠 쓰게 하려면?"
                                  |
                                  v
                             [ Process ]
                 = "실행 중인 프로그램 하나"
             Kernel이 등록하고 관리하는 실행 단위
                                  |
       +--------------------------+--------------------------+
       |                          |                          |
       v                          v                          v
[ Process의 상태 ]              [ PCB ]                 [ Process 생성 ]
"지금 무엇을 하는가"       "OS의 관리 노트"          "새 실행 단위 만들기"
       |                          |                          |
 Running / Ready / Blocked  PID / register / state        fork()
       |                    memory / I/O / context          |
 I/O 완료 -> Ready                   |                    child 생성
       |                              |                       |
       +------------------------------+                    exec()
                                                        program image 교체
                                                               |
                                                             wait()
                                                        parent가 child 종료 대기

                                  |
                                  v
                 [ CPU를 실제로 어떻게 나눠 쓰는가? ]
                                  |
                                  v
                  [ Limited Direct Execution ]
       "직접 실행으로 빠르게, OS는 통제권을 절대 놓지 않게"
                                  |
             +--------------------+--------------------+
             |                                         |
             v                                         v
 [ 문제 1: 위험한 작업 제한 ]               [ 문제 2: CPU를 다시 뺏기 ]
 "program이 I/O를 마음대로?"              "program이 멈추지 않으면?"
             |                                         |
             v                                         v
   [ User / Kernel Mode ]                    [ Timer Interrupt ]
             |                           일정 시간마다 OS가 control 회복
 privileged 작업 필요                                  |
             |                                         v
             v                                  [ Scheduler ]
      [ System Call ]                       "다음 누구를 실행할지"
     "OS야, 대신 해줘"                               |
             |                                         v
           [ Trap ]                           [ Context Switch ]
 user mode -> kernel mode                 현재 process context 저장
             |                            다음 process context 복원
             v                                         |
     kernel이 작업 수행                               v
             |                                  Process B 실행
             v
    return-from-trap
             |
             v
      user mode 복귀
```

이 구조는 두 PDF의 순서를 한 번에 묶는다. `OS03`은 process가 무엇이며 어떤 상태와 PCB를 가지는지를 설명하고, `OS02`는 그런 process들이 CPU를 번갈아 쓰도록 OS가 hardware privilege, timer, scheduler, context switch를 어떻게 사용하는지를 설명한다.
## 원본 자료와 범위

| 자료 | 역할 |
| --- | --- |
| `OS02-LimitedDirectExecution.pdf` | OS가 CPU를 통제하는 hardware mechanism: privilege, trap, timer interrupt, scheduling, context switch |
| `OS03-Process.pdf` | process abstraction과 PCB, state, creation, `fork`, `wait`, `exec`, process tree, zombie/orphan, signal |
| OSTEP | 이 주차와 연결되는 읽기 기준서. 슬라이드 기준 Chapter 6, Chapter 4~5 흐름 |

### 퀴즈 메모

- 수업에서 **퀴즈를 매주 본다**고 전달받은 것으로 관리한다.
- 현재 PDF와 저장된 공지에는 퀴즈의 날짜, 시간, 응시 방식, 정확한 범위가 아직 없다.
- 따라서 이번 주 공부는 `OS02`와 `OS03`의 굵은 개념어를 설명 가능한 수준으로 만드는 것이 맞고, 실제 범위는 iCampus 공지가 올라오면 확정한다.

## 이번 주 우선순위

### 반드시 이해

- CPU virtualization이 필요한 이유
- user mode와 kernel mode의 차이
- privileged instruction이 왜 필요한가
- system call, trap, interrupt, exception의 구분
- timer interrupt가 infinite loop process에서도 OS 통제권을 되찾게 하는 이유
- scheduler와 context switch의 역할 차이
- process = running program이라는 말의 정확한 의미
- PCB, process state, address space, register context
- `fork()`, `wait()`, `exec()`의 역할 분담

### 흐름으로 이해

- application API -> system call -> trap -> kernel service -> return-from-trap
- disk I/O -> blocked -> 다른 process running -> I/O 완료 -> ready
- timer interrupt -> OS regains control -> scheduler -> context switch -> 다른 process resume
- executable on disk -> memory load -> stack/heap/I/O setup -> `main()` -> process

### 암기/용어

- PID, PCB, ready/running/blocked
- program counter, stack pointer, frame pointer
- kernel stack, user register, kernel register
- trap table, mode bit, privileged instruction
- zombie, orphan, reaper/subreaper
- `SIGINT`, `SIGTSTP`, `SIGKILL`, `SIGSTOP`

## Part 1. 왜 Limited Direct Execution인가

### CPU virtualization의 딜레마

OS는 physical CPU가 몇 개 없는데도 많은 program이 동시에 실행되는 것처럼 보여야 한다. 이를 CPU virtualization이라고 한다. 가장 빠른 방법은 application program을 CPU에서 직접 실행시키는 것이다. 하지만 제한 없이 직접 실행시키면 program이 disk I/O를 마음대로 요청하거나, CPU를 계속 점유하거나, memory나 system resource에 무단 접근할 수 있다.

그래서 문제는 두 조건을 동시에 만족하는 것이다.

1. application은 native speed에 가깝게 실행되어야 한다.
2. OS는 system 전체의 최종 통제권을 유지해야 한다.

Limited Direct Execution은 application에게 CPU를 직접 실행하게 하되, hardware가 정한 경계를 통해 privileged operation과 CPU scheduling은 OS만 다루게 하는 방식이다. OS가 application instruction을 하나하나 interpreter처럼 실행하지 않아도 되므로 빠르고, user/kernel mode와 interrupt 덕분에 통제도 가능하다.

### user mode와 kernel mode

Hardware는 mode bit 등을 통해 code가 어떤 privilege level에서 실행되는지 구분한다.

| 구분 | user mode | kernel mode |
| --- | --- | --- |
| 실행 주체 | 일반 application/process code | OS kernel code |
| 권한 | 제한됨 | privileged operation 가능 |
| I/O request | 직접 실행 불가 | 실행 가능 |
| privileged instruction | 실행 시 exception | 실행 가능 |
| 목적 | application 격리와 보호 | resource 관리와 system control |

user mode의 process가 disk controller, interrupt setting, timer, control register 같은 것을 직접 만질 수 있다면 OS는 더 이상 OS 역할을 못 하고 단순 library가 된다. 따라서 I/O control, timer management, interrupt management, user mode 전환 같은 machine instruction은 kernel mode에서만 실행하도록 한다.

슬라이드에 나온 Intel privileged instruction 목록은 외울 시험 항목이라기보다 "CPU 자체가 OS만 실행할 수 있는 instruction을 hardware level에서 구분한다"는 증거다. `HLT`, control register 접근, interrupt descriptor table 접근 같은 instruction이 그 예다.

## Part 2. system call, trap, interrupt는 어떻게 다른가

### system call의 전체 경로

user program은 privileged task가 필요하면 OS에 요청해야 한다. 보통 C program은 `write()` 같은 high-level API/library function을 사용하고, 내부적으로 system call interface를 거쳐 generic trap instruction을 실행한다.

```text
User application
  |
  | printf(), read(), write(), fork() 같은 API 호출
  v
Library / system-call wrapper
  |
  | system-call number를 지정
  v
trap instruction
  |
  | CPU: user mode -> kernel mode
  |      caller register 저장
  v
trap table의 handler 주소로 jump
  |
  v
Kernel system-call implementation
  |
  | 필요한 privileged operation 수행
  v
return-from-trap
  |
  | register 복원, kernel mode -> user mode
  v
원래 user program 다음 instruction 재개
```

system call number는 "어떤 OS service를 원하는가"를 나타낸다. trap table은 그 number에 대응하는 kernel handler의 jump address를 관리한다. 그래서 user program이 arbitrary kernel address로 뛰어들 수 있는 것이 아니라, hardware와 OS가 정한 entry point로만 들어간다.

MIPS의 `syscall`, RISC-V의 `ecall`, ARM의 `swi`, x86의 `INT`/`SYSENTER` 같은 것은 architecture마다 표현은 달라도 같은 계열의 역할을 한다. user code가 kernel service를 요청하는 통제된 입구다.

### trap과 interrupt의 구분

둘 다 CPU를 kernel mode로 전환시키고 OS handler를 실행하게 만들 수 있지만, 원인이 다르다.

| 구분 | 원인 | 예시 | 성격 |
| --- | --- | --- | --- |
| system call | user program의 의도적 요청 | `read`, `write`, `fork` | software-generated trap |
| exception | 현재 instruction의 오류/예외 | divide by zero | running program에서 발생 |
| interrupt | 외부/비동기 event | timer, disk I/O completion, console | CPU 밖 또는 비동기 signal |

슬라이드는 trap을 software-generated interrupt로 설명하며, 그 안에 exception과 system call을 둔다. 반면 interrupt는 I/O interrupt, clock interrupt, console interrupt, machine check, IPC interrupt처럼 예상 시점을 정확히 알기 어려운 external event다.

이 차이를 한 문장으로 잡으면 이렇다.

> System call은 process가 OS를 부르는 것, exception은 process가 문제를 일으킨 것, interrupt는 바깥 event가 CPU를 부르는 것이다.

### mode change와 register 보존

trap이나 interrupt가 발생하면 hardware는 user mode에서 kernel mode로 바꾸고, 나중에 원래 code로 정확히 돌아가기 위해 register state를 저장해야 한다. x86 example에서는 각 process가 kernel stack을 가지며 transition 때 register를 저장/복원한다.

여기서 중요한 건 "kernel로 들어갔다가 return한다"가 function call보다 훨씬 강한 경계라는 점이다. privilege level이 바뀌고, OS는 process의 register context와 kernel data structure를 안전하게 관리해야 한다. 잘못 복원하면 다른 process의 register나 address space가 섞이는 심각한 문제가 된다.

## Part 3. OS는 CPU를 어떻게 다시 되찾는가

### cooperative approach의 한계

가장 단순한 방식은 process가 system call을 하거나 `yield`를 호출할 때 CPU를 넘겨준다고 믿는 것이다. exception으로 trap이 발생해도 OS는 control을 되찾을 수 있다. 이를 cooperative approach라고 한다.

하지만 malicious process나 bug가 있는 process가 infinite loop를 돌면서 system call을 전혀 하지 않으면 어떻게 될까? 이 방식에서는 OS가 다시 CPU를 뺏어올 수 없다. time sharing이 깨지고 다른 program은 영원히 실행되지 못한다.

### non-cooperative approach: timer interrupt

그래서 OS는 timer device를 program해 일정 milliseconds마다 timer interrupt를 발생시킨다.

```text
process A 실행
      |
      | timer interrupt 발생
      v
CPU가 A를 멈추고 OS interrupt handler 실행
      |
      | A의 state/register 저장
      v
OS가 CPU control 회복
      |
      +--> A를 계속 실행
      |
      +--> scheduler로 B를 선택
                    |
                    v
              context switch 후 B 실행
```

timer interrupt는 application의 협조가 없어도 발생한다. 그래서 infinite loop process도 일정 시간 뒤에는 OS에게 CPU를 빼앗긴다. 이게 preemptive time sharing의 hardware 기반이다.

슬라이드 15쪽의 timeline은 `app1 -> OS -> app2 -> OS -> app3 ...`가 timer interrupt마다 반복되는 모습을 보여준다. 사용자는 여러 application이 동시에 실행된다고 느끼지만, single CPU라면 OS가 짧은 시간 단위로 번갈아 실행시키는 illusion일 수 있다.

### scheduler와 context switch는 다르다

OS가 CPU control을 되찾은 뒤에는 두 가지 일이 분리된다.

1. **Scheduler**: 현재 process를 계속 돌릴지, 어떤 ready process를 다음으로 실행할지 결정하는 policy다.
2. **Context switch**: 결정된 다음 process가 실행되도록 실제 machine state를 바꾸는 low-level mechanism이다.

scheduler는 "누구를 고를까"의 문제이고, context switch는 "선택한 process의 register와 실행 위치를 실제 CPU에 올리는 방법"의 문제다. 이 구분은 앞으로 scheduling algorithm과 process mechanism을 따로 배울 때 계속 중요하다.

## Part 4. Context switch를 정확히 이해하기

### context란 무엇인가

process가 잠시 멈췄다가 나중에 같은 위치에서 이어서 실행되려면 CPU state를 기억해야 한다. 대표적으로 program counter, general-purpose register, stack pointer, frame pointer 같은 register 값이 필요하다. 이 상태 묶음을 context라고 한다.

```text
Process A context                    Process B context
PC = A의 다음 instruction            PC = B의 다음 instruction
SP = A의 stack 위치                  SP = B의 stack 위치
registers = A의 계산 상태             registers = B의 계산 상태
```

Context saving은 현재 process의 register context를 저장하는 것이고, context restoring은 다음 process의 register context를 다시 load하는 것이다. 둘을 이어서 context switching이라고 한다.

### kernel stack과 PCB

슬라이드는 현재 process의 일부 register value를 kernel stack에 저장하고, 다음 process의 값은 그 process의 kernel stack에서 복원하는 예를 든다. PCB(Process Control Block)에도 process별 context save area가 있다. 구현 detail은 OS마다 다를 수 있지만 핵심은 process마다 "나중에 다시 실행할 수 있는 machine state"가 kernel space에 유지된다는 점이다.

Hardware가 transition에서 implicitly save하는 user register와, OS가 explicitly save하는 kernel-side register가 구분될 수 있다. Context switch는 pure overhead다. user program의 실제 일을 진전시키지 않고 switching 비용만 낸다. PCB가 복잡할수록, 저장해야 할 state가 많을수록, hardware support가 약할수록 시간이 더 든다.

일부 processor는 multiple register set을 제공해서 current register set을 가리키는 pointer만 바꾸는 식으로 switch cost를 줄이기도 한다. 하지만 일반적으로 cache/TLB 효과까지 생각하면 process switch는 단순 register copy보다 더 넓은 성능 영향을 가진다.

### return-from-trap이 다른 process로 돌아갈 수 있는 이유

처음 trap이 발생한 A의 state를 저장한 다음 scheduler가 B를 고르면, OS는 B의 context를 복원한다. 이후 return-from-trap을 실행하면 CPU는 처음 trap을 건 A가 아니라 B의 user code로 돌아갈 수 있다. 이것이 time sharing을 구현하는 핵심 trick이다.

겉으로는 "A가 interrupt를 당했다가 B가 실행된다"지만, 안에서는 A save -> choose B -> B restore -> return이라는 sequence다.

## Part 5. Kernel 내부 concurrency

interrupt가 OS를 통제하게 해 주지만, 동시에 kernel 안에서도 concurrency 문제가 생긴다.

- system call handler를 처리하는 중 timer interrupt가 올 수 있다.
- 하나의 interrupt handler를 처리하는 중 다른 interrupt가 올 수 있다.
- multiprocessor에서는 여러 CPU가 kernel internal data structure에 동시에 접근할 수 있다.

가장 단순한 방법은 interrupt processing 동안 interrupt를 disable하는 것이다. 하지만 너무 오래 disable하면 중요한 interrupt를 놓칠 수 있고 system responsiveness가 나빠진다. 더 정교한 방법은 lock을 사용해 internal data structure에 대한 concurrent access를 보호하는 것이다.

이 마지막 slide는 뒤 concurrency 단원과 직접 이어지는 복선이다. Limited Direct Execution은 CPU virtualization을 시작하는 mechanism이고, lock은 그 mechanism을 multi-core/multi-activity 환경에서도 안전하게 만드는 도구가 된다.

## Part 6. Process는 무엇인가

### program과 process의 차이

program은 disk에 저장된 executable file, code와 static data의 묶음이다. process는 그 program이 memory에 load되어 CPU에서 실행되고 kernel에 등록되어 관리되는 active entity다.

```text
Disk에 있는 executable
      |
      | load + address space 생성 + context 초기화
      v
Process
      |
      | CPU time을 받아 실행, resource 요청/사용/해제
      v
Kernel이 PCB로 추적하는 실행 단위
```

process를 "running program"이라고 부르지만, 정확히는 실행 중이거나 ready/blocked 상태로 kernel의 관리 대상인 entity다. 지금 CPU를 쓰지 않아도 PCB가 있고 kernel이 관리한다면 process다.

CPU virtualization은 physical CPU가 몇 개뿐이어도 time sharing을 통해 many virtual CPUs가 있는 것처럼 보이게 한다. OS는 process 하나를 실행하고, 멈추고, 다른 process를 실행하는 일을 반복한다.

### process가 포함하는 state

| 구성 | 설명 |
| --- | --- |
| address space | process가 사용하는 memory view |
| text/code | program instruction |
| data | global/static data |
| stack | local variable, function parameter, return address |
| heap | runtime dynamic allocation area |
| registers | PC, stack pointer, frame pointer, general registers |
| I/O information | open file list 등 |

주소 공간만 있어도 process가 되지는 않는다. 어디까지 실행했는지를 나타내는 PC와 register, 어떤 file을 열고 있는지 같은 I/O state가 함께 있어야 pause/resume이 가능하다.

### PCB(Process Control Block)

PCB는 kernel space에 유지되는 process별 관리 record다. slide는 task control block이라고도 표현한다.

| PCB 항목 | 의미 |
| --- | --- |
| PID | process identification number |
| process state | running, ready, waiting 등 |
| program counter | 다음에 execute할 instruction 위치 |
| CPU registers | process-centric register contents |
| scheduling information | priority, scheduling parameter |
| memory management information | page table, segment table, base/limit register 등 |
| I/O status information | allocated device, open file list |
| accounting information | CPU 사용량, elapsed time, time limit |
| context save area | switch를 위한 register context |

PCB는 "OS가 process를 잠깐 멈췄다가 정확히 재개하려면 무엇을 기억해야 하나"의 답이다. PCB 없이 kernel은 PID만 알고 실제 실행 state를 잃어버릴 것이다.

## Part 7. Process lifecycle

### process API

운영체제는 process를 다루기 위해 보통 다음 API를 제공한다.

- Create: 새 process 생성
- Destroy: process 강제 종료
- Wait: 특정 process가 끝날 때까지 기다림
- Miscellaneous control: suspend/resume 등 실행 제어
- Status: process가 얼마나 실행됐는지, 어떤 state인지 조회

이 API들은 OS가 application에게 process abstraction을 제공하는 interface다. user program이 hardware registers를 직접 만져서 process를 만들지 않고, kernel에게 request한다.

### process creation의 6단계

새 process를 만들 때 OS는 대략 다음 일을 한다.

1. disk에 executable format으로 있는 program을 찾는다.
2. code와 static data를 process address space로 load한다.
3. runtime stack을 allocate하고 `argc`, `argv` 같은 argument로 initialize한다.
4. heap을 allocate한다.
5. I/O setup 등 초기화 task를 한다.
6. `main()` routine으로 jump해서 CPU control을 새 process에 넘긴다.

load는 eager loading일 수도 있고, paging/swapping과 관련된 lazy loading일 수도 있다. 이 문장은 process creation과 virtual memory가 완전히 분리된 주제가 아니라는 것을 보여준다.

### process states

| 상태 | 의미 | 대표 전이 |
| --- | --- | --- |
| Running | processor에서 instruction을 실행 중 | timer interrupt로 ready가 될 수 있음 |
| Ready | 실행할 준비는 됐지만 OS가 아직 CPU를 선택하지 않음 | scheduler가 선택하면 running |
| Blocked | 어떤 event를 기다리느라 실행 불가 | I/O completion 뒤 ready |

disk I/O를 요청한 process는 response가 올 때까지 blocked가 된다. 이때 OS는 CPU를 놀리지 않고 ready state의 다른 process를 실행할 수 있다. I/O 완료 interrupt가 오면 blocked process가 ready queue로 돌아와 scheduler 선택을 기다린다.

이 state model이 process management의 기본이다. Ready와 blocked를 혼동하면 안 된다. Ready는 CPU만 받으면 실행할 수 있는 상태이고, blocked는 CPU를 줘도 필요한 event가 아직 안 와서 실행할 수 없는 상태다.

## Part 8. `fork()`, `wait()`, `exec()`

### `fork()`

UNIX에서 `fork()` system call은 child process를 만든다. child는 parent address space의 duplicate로 시작한다. 같은 `fork()` call에서 return value가 다르기 때문에 parent와 child가 서로 다른 path를 실행할 수 있다.

| `fork()` 반환값 | 의미 |
| --- | --- |
| parent에서 child PID | parent는 어떤 child를 만들었는지 안다 |
| child에서 `0` | child 자신임을 구분한다 |
| 음수 | creation failure |

`fork()` 뒤 parent와 child가 모두 실행을 계속할 수 있으므로 기본 실행 순서는 scheduler에 따라 달라질 수 있다. `wait()`를 넣으면 parent가 child termination을 기다리도록 ordering을 만들 수 있다.

### `wait()`

`wait()`는 parent가 child가 끝날 때까지 기다리게 한다. return value로 terminated child의 PID와 status information을 받을 수 있다. slide의 `wait()` example에서 child가 먼저 print하는 것은 parent가 wait하도록 작성된 그 예제의 ordering이다.

`wait()`는 단순히 "기다린다"가 아니라 child 종료 정보를 회수(reap)하는 interface이기도 하다. 이 역할을 하지 않으면 종료한 child가 zombie가 되는 문제로 이어진다.

### `exec()`

`exec()`는 현재 process의 memory space를 새 program으로 교체한다. 보통 shell이 command를 실행할 때 `fork()`로 child를 만든 뒤 child에서 `exec()`로 다른 program image를 load하는 구조가 대표적이다.

성공한 `exec()`는 호출 지점으로 return하지 않는다. 기존 code/data/address space가 새 executable의 것으로 바뀌기 때문이다. Linux에는 `execl`, `execlp`, `execle`, `execv`, `execvp`, `execvpe` 등 여러 variant가 있다.

```text
shell
  |
  | fork()
  +--> parent: 필요하면 wait()
  |
  +--> child: exec() -> 새 program image 실행
```

`fork`는 process를 복제/생성하는 mechanism이고, `exec`는 그 process가 실행할 program image를 바꾸는 mechanism이다. 둘을 한 function처럼 생각하면 안 된다.

## Part 9. Process tree, zombie, orphan, signal

### process tree와 PID

parent process는 child를 만들고, child도 다시 child를 만들 수 있어 tree 구조가 된다. Linux의 `systemd`/과거 `init`은 PID 1로 root에 가까운 역할을 하며, shell, sshd, editor, `ps` 같은 process가 그 아래로 이어진다.

`ps` command는 현재 system의 process를 관찰하는 입구다. 시프실의 `ps`, `ps -ef`와 운영체제의 process tree가 같은 지점에서 만난다. 시프실에서는 command output을 보고, 운체에서는 kernel이 그 process들을 PID/PCB/state로 관리한다.

### zombie와 orphan

| 상태 | 발생 조건 | 처리 |
| --- | --- | --- |
| zombie | child는 종료했지만 parent가 `wait()`으로 status를 회수하지 않음 | exit 정보가 남아 있다가 parent/reaper가 회수 |
| orphan | parent가 child보다 먼저 종료 | init 또는 subreaper가 새 parent가 되어 관리/회수 |

zombie는 "죽었지만 완전히 정리되지 않은 process"다. execution은 끝났어도 PID와 exit status를 parent가 알아야 하므로 최소 정보가 남는다. orphan은 "부모를 잃은 child"다. modern Linux에서는 반드시 PID 1만이 아니라 subreaper가 reparenting을 받을 수도 있다.

### signal로 process 제어

signal은 process control mechanism이다.

| 입력/API | signal | 효과 |
| --- | --- | --- |
| `Ctrl-C` | `SIGINT` | foreground process interrupt/종료 요청 |
| `Ctrl-Z` | `SIGTSTP` | foreground process pause 요청 |
| `kill(pid, sig)` | 지정 signal | 특정 process에 signal 전달 |
| `signal(signum, handler)` | handler 등록 | signal 처리 방식 지정 |

`SIGKILL`과 `SIGSTOP`은 catch하거나 ignore할 수 없다. OS가 multiple user를 지원하므로 일반 user는 자기 process만 제어할 수 있고, superuser는 모든 process를 제어할 수 있다.

## 시프실과 연결해서 보기

운체 2주차와 시프실 2주차는 정확히 이어진다.

```text
시프실
  gcc / gdb / ps / shell command
        |
        v
운체
  program -> process
  process -> PCB / state / register
  trap -> system call
  timer -> scheduler / context switch
```

시프실에서 `./w2.out`을 실행하면 shell이 executable을 실행하고 OS는 그 실행을 process로 관리한다. GDB에서 register를 보는 것은 process context 일부를 보는 입구다. 시프실의 `ps`는 process 목록을 보여주고, 운체의 PCB는 kernel이 그 목록의 각 entity를 어떻게 관리하는지 설명한다.

## 퀴즈 대비 체크 질문

- CPU virtualization은 어떤 illusion을 만드는가?
- OS가 direct execution만 사용하면 왜 문제가 되는가?
- user mode와 kernel mode의 권한 차이는 무엇인가?
- privileged instruction이 필요한 이유는 무엇인가?
- system call, exception, interrupt를 각각 한 예시로 설명할 수 있는가?
- trap table과 system-call number는 어떤 관계인가?
- return-from-trap은 무엇을 복원하고 어떤 mode change를 하는가?
- cooperative scheduling이 infinite loop에 약한 이유는 무엇인가?
- timer interrupt가 OS control을 보장하는 이유는 무엇인가?
- scheduler와 context switch의 차이는 무엇인가?
- context에 PC, SP, register가 필요한 이유는 무엇인가?
- ready와 blocked의 차이는 무엇인가?
- process와 program은 어떻게 다른가?
- PCB에는 어떤 정보가 들어가고 왜 kernel space에 있는가?
- process creation의 6단계를 순서대로 말할 수 있는가?
- `fork()`에서 parent와 child가 받는 return value는 무엇인가?
- `fork()`와 `exec()`를 분리하는 이유는 무엇인가?
- zombie와 orphan의 차이는 무엇인가?
- `Ctrl-C`, `Ctrl-Z`가 보내는 signal은 무엇인가?
- kernel 안에서 interrupt concurrency가 왜 문제가 되는가?

## 페이지별 흐름

### OS02 - Limited Direct Execution

| 페이지 | 내용 | 핵심 |
| --- | --- | --- |
| 1 | Chapter 6 표지 | Limited Direct Execution 시작 |
| 2 | Direct execution의 문제 | 성능과 통제의 동시 달성 |
| 3 | Restricted operation | user/kernel mode와 제한 |
| 4 | System call | trap, register save, trap table, return-from-trap |
| 5 | System mode | mode bit, privileged instruction |
| 6 | Interrupt and trap | external interrupt, exception, system call 구분 |
| 7~8 | Interrupt handling | handler로 control transfer |
| 9 | Privileged instruction | I/O, timer, interrupt management은 kernel only |
| 10 | System call interface | application/process와 OS의 경계, API |
| 11~12 | kernel stack, API relationship | transition에서 state 보존, `printf -> write` example |
| 13 | Cooperative scheduling | system call을 기다리는 방식의 한계 |
| 14~15 | Timer interrupt | non-cooperative control 회복, time-sharing timeline |
| 16~19 | Scheduler/context switch | 선택 policy와 register context save/restore |
| 20 | Kernel concurrency | interrupt disable과 locking으로 연결 |

### OS03 - Process

| 페이지 | 내용 | 핵심 |
| --- | --- | --- |
| 1 | Chapter 4, 5 표지 | process abstraction 시작 |
| 2~3 | Process concept | running program, active kernel-managed entity, CPU virtualization |
| 4 | Machine state | address space, register, I/O information |
| 5~6 | PCB | PID, state, PC, register, scheduling/memory/I/O/accounting info |
| 7 | Process API | create/destroy/wait/control/status |
| 8 | Process creation | load, stack, heap, I/O setup, `main()` |
| 9~10 | Process states | running/ready/blocked와 transition |
| 11 | `fork()` | child duplicate, return-value 분기 |
| 12 | `wait()` | child 종료 순서와 PID return |
| 13 | `exec()` | address space를 새 program으로 교체 |
| 14~16 | Process tree / `ps` | PID와 Linux process hierarchy |
| 17 | Zombie & orphan | wait/reaping, reparenting |
| 18 | Linux state | Linux 관점의 process state |
| 19 | Signal | `kill`, `Ctrl-C`, `Ctrl-Z`, user 권한 |

## 공부 순서

1. 위 구조도를 보며 `user -> trap -> kernel -> scheduler -> context switch`를 말로 설명한다.
2. `OS02` 3~6쪽으로 mode/trap/interrupt 용어를 구분한다.
3. `OS02` 13~18쪽을 timer interrupt와 context switch 한 흐름으로 읽는다.
4. `OS03` 2~6쪽을 process와 PCB 중심으로 읽는다.
5. `OS03` 8~13쪽을 creation -> state -> fork/wait/exec 순서로 잡는다.
6. `OS03` 17~19쪽 zombie/orphan/signal을 마무리한다.
7. 퀴즈 전에는 체크 질문에 막힘 없이 답할 수 있는지 확인한다.

## 이번 주 종료 조건

- user mode와 kernel mode를 보호 관점에서 설명할 수 있다.
- system call, trap, interrupt, exception을 구분할 수 있다.
- timer interrupt부터 다른 process 실행까지의 흐름을 그릴 수 있다.
- scheduler와 context switch를 혼동하지 않는다.
- process, PCB, ready/running/blocked를 설명할 수 있다.
- `fork`, `wait`, `exec`을 각각 한 문장으로 구분할 수 있다.
- 이번 주 퀴즈의 실제 일정/범위가 공지되면 `assignments.md`에 즉시 확정 반영한다.
