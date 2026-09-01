# 운영체제 - 신동군 1주차 학습지도

## 한줄 결론

1주차 `OS01-OS intro.pdf`는 운영체제 과목 전체의 지도입니다. 앞부분은 수업 운영과 평가 방식, 교재, 선수지식을 설명하고, 본문은 운영체제가 왜 필요한지와 운영체제가 제공하는 핵심 기능을 소개합니다. 핵심 축은 CPU virtualization, memory virtualization, concurrency, persistence입니다. 운영체제는 프로그램을 쉽게 실행하게 만들고, 여러 프로그램이 CPU와 메모리와 디스크를 함께 쓰게 하며, system call과 protection을 통해 user program과 hardware 사이의 안전한 경계를 만듭니다.

## 원본 자료

- `OS01-OS intro.pdf`: 운영체제 1주차 도입 강의 자료

## 수업 운영 핵심

- 교수: Dongkun Shin
- 연구실/Office: Semiconductor Building Room 400310
- 이메일: `dongkun@skku.edu`
- Office hours: 월요일 14:00-16:00 또는 appointment
- 강의자료: iCampus
- 강의자료와 수업 설명은 영어로 진행
- 수업 방식: flipped learning
- 온라인 사전 제작 영상 1.5h + 오프라인 수업 1.5h
- 오프라인 강의실: 330110
- 매주 오프라인 수업 전에 pre-production video study를 완료해야 함
- 강의실에서 hands-on exercise를 할 수 있으므로 개인 노트북 지참 권장

## 평가 방식

| 항목 | 비율 | 메모 |
| --- | --- | --- |
| Midterm exam | 35% | 시험 하나라도 빠지면 F 위험 |
| Final exam | 35% | 시험 하나라도 빠지면 F 위험 |
| Assignment | 30% | Report, Project 포함 |

중간과 기말이 각각 35%라서 시험 비중이 매우 큽니다. Assignment도 30%로 작지 않습니다. 특히 PDF에 “If you miss one or both of exams, you will fail this course”가 명시되어 있으므로 시험 일정이 나오면 바로 `assignments.md`와 전체 `tasks.md`에 박아두어야 합니다. Cheating은 시험과 과제 모두에서 허용되지 않고, 해당 시험/과제에서 0점 또는 negative point를 받을 수 있다고 되어 있습니다.

## 교재와 선수지식

주교재는 `Operating Systems: Three Easy Pieces`, 흔히 OSTEP입니다. 무료 온라인 책이며 PDF에는 “very easy and funny”라고 소개되어 있습니다. 중요한 운영 방식은 scheduled chapter를 수업 시간 전에 읽어와야 한다는 점입니다. 즉 이 과목은 온라인 강의만 듣고 끝내기보다, OSTEP 해당 챕터를 같이 읽는 구조로 갈 가능성이 높습니다.

선수지식은 세 가지입니다.

- C programming
- System Programming
- Computer Architecture

C programming이 익숙하지 않으면 수강을 강하게 비추천한다고 되어 있습니다. System Programming에서는 Linux shell과 Linux APIs에 익숙해야 한다고 하고, Computer Architecture에서는 cache, TLB, virtual memory, exception, interrupt 등을 언급합니다. 이 말은 운영체제 수업이 추상 개념만 다루는 것이 아니라, C 코드, Linux API, hardware mechanism을 계속 연결해서 설명할 가능성이 높다는 뜻입니다.

## 과목 전체 목차

| 파트 | 주제 | 세부 내용 |
| --- | --- | --- |
| Part 1 | CPU Virtualization | Process, CPU Scheduling |
| Part 2 | Memory Virtualization | Address Space, Allocation, Address Translation, Paging, TLB, Swapping |
| Part 3 | Concurrency | Thread, Lock, Semaphore |
| Part 4 | Persistence | I/O Systems, Storage, File System |

이 네 축이 학기 전체의 뼈대입니다. 운영체제는 “컴퓨터 자원을 어떻게 추상화하고, 나누고, 보호하고, 지속적으로 저장하게 하는가”를 보는 과목입니다. CPU virtualization은 하나 또는 적은 수의 CPU를 여러 프로그램이 동시에 쓰는 것처럼 보이게 만드는 주제입니다. Memory virtualization은 각 process가 자기만의 독립된 memory를 가진 것처럼 보이게 만드는 주제입니다. Concurrency는 여러 실행 흐름이 동시에 진행될 때 correctness를 어떻게 보장하는지 보는 주제입니다. Persistence는 전원이 꺼져도 data가 남도록 file system과 storage를 어떻게 관리하는지 보는 주제입니다.

## 페이지별 흐름

| 페이지 | 내용 | 공부 포인트 |
| --- | --- | --- |
| 1 | Operating Systems 표지 | 1주차 도입 |
| 2 | Syllabus | 교수/오피스아워/영어 강의/flipped learning/노트북 지참 |
| 3 | About Me | 교수님 연구 분야와 과목 연결점 |
| 4 | Syllabus cont'd | OSTEP 교재, grading policy, cheating 경고 |
| 5 | Course Outline | CPU virtualization, memory virtualization, concurrency, persistence |
| 6 | Assignments/Prerequisites | textbook homework, xv6 projects, C/Linux/컴구 선수지식 |
| 7 | Introduction to OS | 본문 시작 |
| 8 | What happens when a program runs? | Von Neumann model: fetch, decode, execute |
| 9 | OS | OS가 프로그램 실행, memory sharing, device interaction을 쉽게 만듦 |
| 10 | OS role | virtualization, system call, resource manager |
| 11 | Computer System Organization | application, process, system lib, syscall interface, OS, hardware 계층 |
| 12 | Virtualizing the CPU | 여러 program이 동시에 실행되는 것처럼 보이는 예시 |
| 13 | Virtualizing the CPU | virtual CPU illusion, scheduling policy 질문 |
| 14 | Virtualizing Memory | 두 process가 같은 virtual address를 써도 독립적으로 보이는 예시 |
| 15 | Virtualizing Memory | OS가 virtual address를 physical memory에 mapping |
| 16 | Concurrency | multi-thread counter 예시, race condition, atomicity 문제 |
| 17 | Persistence | DRAM volatile, file system, open/write/close system calls |
| 18 | Persistence | I/O management 그림 |
| 19 | I/O Mechanism | DMA, interrupt handler, I/O controller |
| 20 | Design Goals | abstraction, performance, protection, reliability, security 등 |
| 21 | History | libraries, protection, system call, trap, user/kernel mode, multiprogramming, UNIX |
| 22 | History | PC 시대, DOS/Mac OS 문제, Mac OS X, Windows NT, Linux |
| 23 | History of UNIX | UNIX 계보, BSD/System V/Linux/MacOS 흐름 |
| 24 | Ask to ChatGPT | 복습 질문: privilege, scheduling, VM, thread, files |

## 1. 운영체제를 왜 배우는가

운영체제는 프로그램과 하드웨어 사이에 있는 software layer입니다. 사용자는 프로그램을 실행하고, 파일을 저장하고, 여러 프로그램을 동시에 켜고, 네트워크와 장치를 씁니다. 하지만 CPU, memory, disk, device는 제한된 물리 자원이고, 여러 program이 이를 동시에 사용하려고 하면 충돌이 생길 수 있습니다. 운영체제는 이 복잡한 hardware resource를 program이 쓰기 쉬운 형태로 바꾸고, 동시에 correctness와 efficiency를 유지합니다.

9쪽은 OS가 하는 일을 간단히 정리합니다. OS는 program을 쉽게 실행하게 만들고, 많은 program을 동시에 실행할 수 있게 하며, program들이 memory를 share하게 하고, device와 interaction하게 합니다. 그래서 OS는 system이 correct하고 efficient하며 easy-to-use하게 동작하도록 책임지는 software입니다.

이 정의에서 중요한 단어는 correct, efficient, easy-to-use입니다. Correct는 program들이 서로 망가뜨리지 않고 올바르게 동작하는 것입니다. Efficient는 CPU, memory, disk 같은 resource를 낭비하지 않는 것입니다. Easy-to-use는 programmer나 user가 hardware 세부를 직접 다루지 않아도 되는 interface를 제공하는 것입니다.

## 2. Von Neumann model과 program execution

8쪽은 program이 실행될 때 무슨 일이 일어나는지 Von Neumann model로 설명합니다. Processor는 memory에서 instruction을 fetch하고, 어떤 instruction인지 decode하고, 그 instruction이 해야 할 일을 execute합니다. 예를 들어 두 수를 더하거나, memory에 접근하거나, 조건을 확인하거나, function으로 jump할 수 있습니다. 이 instruction이 끝나면 processor는 다음 instruction으로 이동하고 이 과정을 반복합니다.

이 관점은 운영체제의 시작점입니다. Program execution은 결국 instruction stream의 실행입니다. 하지만 현대 시스템에서는 여러 program이 동시에 실행되는 것처럼 보이고, 각 program은 자기 memory를 가진 것처럼 보이고, device도 안전하게 공유됩니다. 이 “그럴듯한 illusion”을 만드는 software가 OS입니다.

## 3. OS의 첫 번째 역할: virtualization

10쪽은 OS의 역할을 크게 virtualization과 resource manager로 나눕니다. Virtualization은 physical resource를 더 general하고 powerful하고 easy-to-use한 virtual form으로 바꾸는 것입니다. Processor, memory, disk 같은 물리 자원이 OS를 거치면 virtual CPU, virtual address space, file 같은 abstraction으로 보입니다.

System call은 사용자가 OS에게 무엇을 해달라고 요청하는 interface입니다. User program은 hardware를 직접 제어하지 않고, system call을 통해 OS 기능을 사용합니다. 예를 들어 file을 열 때 `open`, 읽을 때 `read`, 쓸 때 `write`, 닫을 때 `close` 같은 system call을 사용할 수 있습니다. 이 구조 때문에 user program은 hardware 세부를 몰라도 OS가 제공하는 기능을 사용할 수 있습니다.

Virtualization은 concurrent execution도 가능하게 합니다. 실제 CPU가 하나뿐이어도 OS가 time sharing과 scheduling을 통해 여러 process가 동시에 실행되는 것처럼 보이게 할 수 있습니다. 실제 physical memory는 하나의 shared resource지만, OS가 address translation을 통해 각 process에게 private virtual address space를 제공할 수 있습니다.

## 4. OS의 두 번째 역할: resource manager

OS는 CPU, memory, disk를 포함한 resource manager입니다. 여러 program이 동시에 CPU를 쓰고 싶어 하면 어떤 program을 먼저 실행할지 결정해야 합니다. 이것이 scheduling policy의 문제입니다. 여러 process가 memory를 쓰면 누구에게 얼마를 배정하고, 어떤 page를 memory에 남기고, 어떤 것을 disk로 내보낼지 결정해야 합니다. Disk와 file system도 마찬가지입니다.

Resource management에는 efficiency와 fairness가 모두 들어갑니다. Efficiency는 시스템 전체 처리량이나 응답 시간을 좋게 만드는 것이고, fairness는 특정 process가 resource를 독점하지 않도록 균형을 맞추는 것입니다. 현실의 OS는 workload, device, user requirement에 따라 이 목표들 사이에서 trade-off를 선택합니다.

## 5. Computer system organization

11쪽 그림은 application부터 hardware까지의 계층을 보여줍니다. 맨 위에는 application이 있고, application은 process로 실행됩니다. Application은 system library를 사용할 수 있고, system library나 program은 system call interface를 통해 OS로 들어갑니다. OS 내부에는 memory manager, process manager, file system, I/O manager, device driver가 있습니다. 아래에는 hardware와 ROM/bootstrap이 있습니다.

이 구조에서 system call interface가 매우 중요합니다. User program과 kernel 사이의 문입니다. User program은 마음대로 OS 내부나 hardware를 건드리지 못하고, 정해진 system call을 통해 요청합니다. Device driver는 OS가 구체적인 hardware device와 상호작용하는 부분입니다. Bootstrap은 컴퓨터가 켜질 때 OS를 올리는 초기 흐름과 연결됩니다.

## 6. CPU virtualization

12쪽과 13쪽은 CPU virtualization입니다. 예제 프로그램 `cpu.c`는 문자열을 인자로 받아 계속 출력합니다. `./cpu A`를 실행하면 A를 반복 출력합니다. 그런데 `./cpu A & ; ./cpu B & ; ./cpu C & ; ./cpu D &`처럼 여러 program을 background로 실행하면, CPU가 하나뿐이어도 A, B, C, D가 번갈아 출력되면서 모두 동시에 실행되는 것처럼 보입니다.

이 예시는 OS가 single CPU 또는 small set of CPUs를 seemingly infinite number of CPUs처럼 보이게 만든다는 점을 보여줍니다. 각각의 process는 자기 CPU를 가진 것처럼 실행되지만, 실제로는 OS가 process들을 번갈아 scheduling합니다.

여기서 바로 policy 질문이 나옵니다. 둘 이상의 program이 같은 시간에 실행되고 싶어 하면, 어떤 program이 실행되어야 하는가? 이 질문이 CPU scheduling의 핵심입니다. 나중에 FIFO, SJF, round-robin, MLFQ 같은 scheduling policy를 배우게 될 가능성이 높습니다. 첫 주에는 “CPU virtualization은 process에게 virtual CPU illusion을 제공하고, scheduling policy가 필요하다”를 잡으면 됩니다.

## 7. Memory virtualization

14쪽과 15쪽은 memory virtualization입니다. 예제 `mem` 프로그램은 `malloc`으로 integer 하나를 할당하고, 그 address와 값을 반복 출력합니다. 흥미로운 점은 두 program을 동시에 실행했을 때 둘 다 `0x200000` 같은 같은 address를 출력할 수 있다는 것입니다. 그런데 두 process의 값은 서로 독립적으로 증가합니다.

이것이 virtual address space의 핵심입니다. 각 process는 자기만의 private virtual address space를 접근합니다. Program 입장에서는 `0x200000`이라는 address가 진짜 자기 memory처럼 보입니다. 하지만 실제 physical memory는 OS가 공유 resource로 관리합니다. OS는 virtual address를 physical memory address로 mapping합니다.

15쪽은 이를 명확히 말합니다. 한 program 안의 memory reference는 다른 process의 address space나 OS 자체에 영향을 주지 않습니다. Running program 입장에서는 physical memory를 자기 혼자 쓰는 것처럼 보입니다. 하지만 reality는 physical memory가 shared resource이고 OS가 이를 관리한다는 것입니다.

이 부분은 컴퓨터구조의 cache/TLB/virtual memory 지식과 연결됩니다. Virtual address를 physical address로 translation하려면 hardware support가 필요하고, TLB가 성능에 영향을 줍니다. 그래서 6쪽 prerequisites에서 Computer Architecture의 cache, TLB, virtual memory, exception, interrupt를 언급한 것입니다.

## 8. Concurrency

16쪽은 concurrency 문제를 multi-threaded counter 예제로 보여줍니다. 두 thread가 같은 global variable `counter`를 증가시키고, loop 횟수가 작을 때는 final value가 기대한 값으로 나올 수 있습니다. 예를 들어 각 thread가 1000번씩 증가시키면 final value가 2000으로 나옵니다.

하지만 loop를 100000으로 키우면 final value가 200000이 아니라 143012, 137298처럼 매번 다르게 나올 수 있습니다. 이유는 `counter = counter + 1`이 atomic operation이 아니기 때문입니다. 이 한 줄은 실제로는 대략 세 단계입니다.

1. memory에서 counter 값을 register로 load
2. register 값을 increment
3. register 값을 memory에 store

두 thread가 이 세 단계를 서로 끼어들며 실행하면 lost update가 발생합니다. 예를 들어 둘 다 같은 기존 값을 읽고, 각각 1을 더한 뒤 같은 결과를 다시 저장하면 증가가 한 번 사라집니다. 이것이 race condition의 기본 예시입니다.

운영체제의 concurrency 파트는 이런 문제를 해결하기 위한 primitive를 다룹니다. Lock, semaphore가 course outline에 나온 이유가 여기에 있습니다. 핵심 질문은 “How can we build a correctly working multi-threaded program?”과 “What primitives are needed from the OS?”입니다.

## 9. Persistence

17쪽은 persistence입니다. DRAM은 volatile입니다. 전원이 꺼지면 내용이 사라집니다. 그래서 data를 지속적으로 저장하려면 hardware와 software가 필요합니다. Hardware에는 HDD, SSD가 있고, software에는 file system이 있습니다. File system은 user가 만든 file을 disk에 reliable하고 efficient하게 저장하는 역할을 합니다.

예제 코드는 `/tmp/file`을 열고, `hello world\n`을 쓰고, 닫습니다. 여기서 나오는 system call 또는 API 흐름은 `open`, `write`, `close`입니다. PDF에는 system calls로 `open`, `read`, `write`, `close`가 나옵니다. Persistence 파트에서는 file과 directory를 어떻게 다루는지, I/O가 어떻게 처리되는지, file system이 어떻게 reliability와 performance를 맞추는지 배우게 될 가능성이 큽니다.

중요한 점은 OS가 각 application에게 private virtualized disk를 만들어주지는 않는다는 것입니다. File은 사용자들이 정보를 공유하기 위해 존재합니다. Memory virtualization과 달리 disk/file은 공유와 영속성이 중요합니다. 따라서 protection, naming, consistency, crash recovery 같은 문제가 함께 따라옵니다.

## 10. I/O management와 DMA/interrupt

18쪽은 user program과 kernel, I/O buffer 사이의 관계를 그림으로 보여줍니다. User program이 I/O를 요청하면 kernel이 이를 처리하고, data는 I/O buffer를 거쳐 이동합니다. 이 과정에서 user mode와 kernel mode 전환, system call, device driver, buffer management가 연결됩니다.

19쪽은 I/O mechanism을 보여줍니다. I/O request는 block 단위 data transfer를 일으키고, DMA를 통해 I/O device가 CPU 개입 없이 main memory로 직접 data를 전송할 수 있습니다. I/O controller는 command를 받아 device를 제어하고, OS는 interrupt handler를 포함합니다. Device가 작업을 끝내면 interrupt로 CPU/OS에 알릴 수 있습니다.

이 그림은 운영체제가 왜 hardware privilege와 interrupt를 알아야 하는지 보여줍니다. CPU가 모든 I/O byte를 직접 옮기면 비효율적입니다. DMA는 CPU 부담을 줄이고, interrupt는 I/O 완료 같은 event를 OS가 처리하게 합니다. 나중에 storage, file system, device driver, performance 파트와 연결될 수 있습니다.

## 11. Design goals

20쪽은 운영체제의 설계 목표를 정리합니다. 첫째, abstraction을 만들어 system을 convenient하고 easy to use하게 해야 합니다. 둘째, high performance를 제공해야 합니다. 셋째, OS overhead, 즉 extra time and space를 최소화해야 합니다. 넷째, application 사이와 OS/application 사이의 protection을 제공해야 합니다. Process isolation은 protection의 핵심입니다.

Reliability도 중요합니다. OS는 non-stop으로 실행되어야 합니다. OS가 실패하면 그 위에서 실행 중인 모든 application이 함께 실패합니다. 이 때문에 OS code는 일반 application보다 훨씬 보수적으로 설계되어야 하고, kernel bug는 시스템 전체 장애로 이어질 수 있습니다.

그 밖에도 energy-efficiency, security, mobility가 목표로 나옵니다. OS가 어떤 환경에서 쓰이는지에 따라 목표가 달라질 수 있습니다. 서버 OS는 throughput과 reliability를 중시할 수 있고, mobile OS는 energy와 mobility, security를 더 강하게 중시할 수 있습니다. Embedded system이나 storage system도 또 다른 설계 목표를 가질 수 있습니다.

## 12. History: library에서 protection으로

21쪽은 운영체제 역사를 간단히 정리합니다. Early operating systems는 just libraries에 가까웠습니다. 즉 program이 library를 호출하듯 OS 기능을 쓰는 형태였습니다. 하지만 system이 복잡해지고 여러 program을 함께 실행하려면 protection이 필요해졌습니다.

System call 개념은 user program에서 OS로 control을 넘기면서 동시에 hardware privilege level을 올리는 아이디어입니다. User application은 user mode에서 실행되고, hardware는 user mode program이 할 수 있는 일을 제한합니다. Trap이 발생하면 privilege level이 kernel mode로 올라가고, OS는 hardware에 대한 full access를 갖고 필요한 작업을 수행합니다.

이 user mode/kernel mode 구분은 OS protection의 기본입니다. User program이 마음대로 disk를 지우거나 다른 process memory를 읽거나 interrupt handler를 바꾸면 시스템은 안전할 수 없습니다. 따라서 privileged instruction은 kernel mode에서만 실행되도록 하고, user program은 system call을 통해 통제된 방식으로 OS 기능을 사용합니다.

Multiprogramming 시대에는 OS가 여러 job을 memory에 올리고 빠르게 switch하면서 CPU utilization을 높였습니다. 하지만 여러 job이 함께 있으면 system resource, including files에 대한 access를 통제해야 하므로 protection mechanism이 필요합니다. Concurrency issue도 이 시기에 중요해집니다. UNIX는 Ken Thompson과 Dennis Ritchie가 Bell Labs에서 만든 operating system으로 소개됩니다.

## 13. Modern era와 UNIX 계보

22쪽은 personal computer 시대를 말합니다. Apple II, IBM PC 같은 개인용 컴퓨터는 대중화 측면에서는 중요했지만, OS 관점에서는 초기에 minicomputer 시대의 교훈을 잊은 부분이 있었습니다. DOS에는 memory protection이 없었고, Mac OS v9 이전은 cooperative job scheduling이었습니다. 이는 한 program이 잘못 행동하면 전체 시스템이 쉽게 영향을 받을 수 있음을 의미합니다.

현대에는 Mac OS X, Windows NT, Linux가 언급됩니다. Mac OS X는 Steve Jobs가 Apple로 돌아오며 UNIX-based NeXTStep 운영 환경을 가져온 흐름과 연결됩니다. Linux는 Linus Torvalds가 UNIX 원칙과 아이디어를 많이 빌리되, original UNIX code base를 사용하지 않아 legality issue를 피한 것으로 설명됩니다.

23쪽 UNIX history 그림은 MIT ITS, Bell Labs의 Multics/UNIX/C, UC Berkeley의 BSD, AT&T System V, FreeBSD, Linux, MacOS, Solaris, HPUX, AIX 등 UNIX 계열 흐름을 보여줍니다. 이 페이지는 세부 연도 암기보다, 현대 OS의 많은 아이디어가 UNIX 계열에서 발전했고 Linux/MacOS가 그 계보와 깊게 연결되어 있다는 감을 잡으면 됩니다.

## 14. 마지막 복습 질문 해석

24쪽은 “Ask to ChatGPT”라는 제목으로 복습 질문을 제시합니다. 이 질문들은 이번 주차의 핵심을 잘 요약합니다.

| 질문 | 이번 주 기준 답 방향 |
| --- | --- |
| What is the hardware privilege level? | user mode와 kernel mode처럼 hardware가 실행 권한을 구분하는 수준 |
| How to Schedule Multiple Programs in an OS? | CPU virtualization에서 여러 process 중 무엇을 언제 실행할지 정하는 policy 문제 |
| Why is Virtual Memory Useful in an OS? | process isolation, private address space illusion, protection, efficient memory management |
| How to Implement a Multithreaded Program? | pthread 같은 thread API를 쓰되 shared data의 race condition을 lock/semaphore 등으로 제어해야 함 |
| How to Handle Files and Directories in an OS? | file system과 `open/read/write/close` system call을 통해 persistent storage를 관리 |

이 질문은 그대로 퀴즈나 오프라인 discussion 준비용으로 쓸 수 있습니다. 특히 hardware privilege level, scheduling, virtual memory, multithreading, file handling은 학기 전체 파트와 바로 연결됩니다.

## 이번 주 핵심 개념 정리

### Operating System

운영체제는 application과 hardware 사이에서 abstraction, resource management, protection을 제공하는 software입니다. Program을 쉽게 실행하게 하고, 많은 program이 동시에 실행되는 것처럼 보이게 하며, memory와 device를 공유하게 해줍니다.

### Virtualization

Virtualization은 physical resource를 virtual form으로 바꾸는 것입니다. CPU virtualization은 process가 자기 CPU를 가진 것처럼 보이게 하고, memory virtualization은 process가 자기 memory를 가진 것처럼 보이게 합니다. Disk/file 영역에서는 완전히 private disk를 만드는 것이 아니라, file system을 통해 persistent data를 관리하고 공유합니다.

### System Call

System call은 user program이 OS 기능을 요청하는 interface입니다. User program은 직접 hardware를 제어하지 않고, trap을 통해 kernel mode로 들어가 OS에게 작업을 맡깁니다. 이 구조는 convenience와 protection을 동시에 제공합니다.

### User Mode / Kernel Mode

User mode는 application이 실행되는 제한된 mode이고, kernel mode는 OS가 hardware에 full access를 갖는 privileged mode입니다. Hardware privilege level은 application이 위험한 작업을 마음대로 하지 못하게 하는 기반입니다.

### Process

Process는 실행 중인 program의 abstraction입니다. 각 process는 자기 CPU와 memory를 가진 것처럼 보이지만, 실제로는 OS가 CPU time과 physical memory를 나눠줍니다.

### Scheduling

Scheduling은 여러 process/thread 중 어떤 것을 언제 실행할지 결정하는 문제입니다. CPU virtualization에서 반드시 필요한 정책입니다.

### Virtual Address Space

Virtual address space는 각 process가 보는 자기만의 address 공간입니다. 같은 virtual address라도 process가 다르면 다른 physical memory에 mapping될 수 있습니다.

### Concurrency

Concurrency는 여러 execution flow가 동시에 진행될 때 생기는 문제입니다. Multi-threaded counter 예시처럼 shared data를 atomic하지 않게 수정하면 race condition이 생길 수 있습니다.

### Persistence

Persistence는 전원이 꺼져도 data가 남도록 저장하는 성질입니다. DRAM은 volatile이므로 HDD/SSD와 file system이 필요합니다. File system은 persistent data를 reliable하고 efficient하게 저장합니다.

### DMA / Interrupt

DMA는 I/O device가 CPU 개입 없이 main memory로 data를 직접 전송하는 방식입니다. Interrupt는 device나 event가 OS에게 처리가 필요함을 알리는 mechanism입니다. I/O performance와 OS control에 중요합니다.

## 퀴즈/구술 대비 질문

- 운영체제는 왜 필요한가?
- OS가 correct, efficient, easy-to-use하게 만든다는 말은 무슨 뜻인가?
- Von Neumann model에서 instruction execution은 어떤 순서로 일어나는가?
- Virtualization이란 무엇인가?
- CPU virtualization은 어떤 illusion을 제공하는가?
- 여러 program이 동시에 실행되고 싶을 때 OS는 어떤 policy 문제가 생기는가?
- Memory virtualization에서 같은 virtual address가 process마다 다르게 동작할 수 있는 이유는 무엇인가?
- Physical memory와 virtual address space의 관계는 무엇인가?
- `counter = counter + 1`이 atomic하지 않다는 말은 무슨 뜻인가?
- Race condition은 왜 발생하는가?
- DRAM이 volatile이라는 말은 무슨 뜻인가?
- File system은 persistence에서 어떤 역할을 하는가?
- `open`, `read`, `write`, `close`는 어떤 종류의 interface인가?
- User mode와 kernel mode의 차이는 무엇인가?
- Trap은 system call에서 어떤 역할을 하는가?
- Protection mechanism이 없으면 어떤 문제가 생기는가?
- Multiprogramming은 CPU utilization을 어떻게 높이는가?
- DOS에 memory protection이 없었다는 것은 어떤 의미인가?
- Linux와 UNIX의 관계는 무엇인가?
- OSTEP의 네 파트는 각각 무엇인가?

## 헷갈리기 쉬운 점

- CPU virtualization은 CPU를 실제로 무한히 만드는 것이 아닙니다. OS가 process들을 빠르게 전환하고 scheduling해서 여러 CPU가 있는 것처럼 보이게 하는 것입니다.
- Memory virtualization은 각 process가 같은 address를 써도 서로 독립적으로 보이게 합니다. 하지만 실제 physical memory는 공유 resource입니다.
- Concurrency 문제는 CPU가 여러 개일 때만 생기는 것이 아닙니다. 하나의 CPU에서도 context switch가 끼어들면 shared state 문제가 생길 수 있습니다.
- `counter = counter + 1`은 C 코드 한 줄이지만 machine level에서는 여러 instruction으로 나뉩니다. 그래서 atomic하지 않습니다.
- Persistence는 단순히 file을 저장한다는 말보다 넓습니다. File system이 reliability, efficiency, sharing, crash 상황까지 고려해야 합니다.
- System call은 일반 function call과 다릅니다. System call은 user mode에서 kernel mode로 privilege level 변화가 일어나는 통제된 진입점입니다.
- OS design goal은 하나가 아닙니다. Convenience, performance, protection, reliability, energy, security 등이 서로 trade-off를 만들 수 있습니다.

## 이번 주 공부 순서

1. `OS01-OS intro.pdf`의 syllabus 부분을 먼저 보고 평가 방식과 선수지식을 확인합니다.
2. OSTEP이 주교재이고, scheduled chapter를 수업 전에 읽어야 한다는 점을 기억합니다.
3. Course Outline의 네 파트: CPU virtualization, memory virtualization, concurrency, persistence를 외웁니다.
4. 페이지 8~10에서 OS가 왜 필요한지와 virtualization/resource manager 역할을 잡습니다.
5. 페이지 12~15에서 CPU virtualization과 memory virtualization 예제를 비교합니다.
6. 페이지 16의 counter 예제로 concurrency와 atomicity 문제를 이해합니다.
7. 페이지 17~19에서 persistence, file system, I/O mechanism, DMA, interrupt를 연결합니다.
8. 페이지 20~23의 design goals/history는 배경으로 읽되, user/kernel mode와 system call/trap은 꼭 표시합니다.
9. 페이지 24 질문을 스스로 답해봅니다.

## 목요일 전 최소컷

- 온라인 사전 강의 수강 완료 여부 확인
- OSTEP 교재 링크와 읽어야 할 chapter 확인
- 평가 방식 암기: 중간 35, 기말 35, 과제 30
- 시험 결시 시 F 가능성 기억
- 네 파트 암기: CPU virtualization, memory virtualization, concurrency, persistence
- OS 역할 두 가지 암기: virtualization, resource manager
- system call, user mode, kernel mode, trap의 관계 설명 가능
- CPU virtualization 예제와 memory virtualization 예제 설명 가능
- concurrency counter 예제에서 왜 결과가 틀어지는지 설명 가능

## 시프와 같이 보는 연결점

이번 주 시스템프로그램 `SP01-FC.pdf`도 `hello.c` 실행 경로에서 OS abstraction을 다룹니다. 시프에서는 `hello.c`가 source file에서 executable이 되고, shell과 OS와 hardware를 거쳐 실행되는 흐름을 봅니다. 운체에서는 그 흐름을 가능하게 하는 OS 원리, 즉 process, virtual memory, file, system call, scheduling, protection을 더 깊게 봅니다.

따라서 화/목 공부는 같이 묶는 것이 맞습니다. 시프가 “프로그램이 시스템 위에서 어떻게 움직이는가”를 코드와 hardware 가까이에서 보여주고, 운체가 “그 시스템을 OS가 어떤 abstraction과 policy로 관리하는가”를 보여줍니다. 시프실은 여기에 Linux API와 C 실습을 붙이는 역할입니다.

## 다음 업데이트

- 온라인 강의를 실제로 들은 뒤 교수님이 강조한 예시와 표현 추가
- OSTEP reading 범위가 공지되면 이 파일 상단에 추가
- 첫 assignment/report/project가 열리면 `assignments.md`에 반영
- xv6 project 관련 공지가 나오면 별도 섹션 생성
