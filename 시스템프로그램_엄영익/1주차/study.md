# 시스템프로그램 - 엄영익 1주차 학습지도

## 한줄 결론

1주차 `SP01-FC.pdf`는 단순히 C 첫 예제를 보여주는 자료가 아니라, `hello.c`라는 아주 작은 프로그램 하나가 컴퓨터 시스템 전체를 어떻게 지나가는지 추적하는 강의입니다. 소스 파일은 ASCII 문자들의 나열이고, 컴파일 시스템은 이를 전처리, 컴파일, 어셈블, 링크 단계를 거쳐 실행 파일로 바꾸며, 실행 시에는 shell, CPU, 메모리, I/O 장치, 캐시, 운영체제, 네트워크, 병렬성 개념까지 한 번에 연결됩니다. 이번 주 목표는 `hello, world`를 외우는 것이 아니라, 프로그램이 작성되고 번역되고 실행되는 동안 시스템 내부에서 어떤 일이 일어나는지 큰 지도를 만드는 것입니다.

## 강의콘텐츠

| 순서 | 콘텐츠 | 길이 | 상태 | 마감 |
| --- | --- | --- | --- | --- |
| 1 | `SP01-S01-15` | 33:25 | 완료 | 2026-09-13 23:59 |
| 2 | `SP01-S16-33#` | 01:02:23 | 수강 예정 | 2026-09-13 23:59 |

## 원본 자료

- `SP00(CourseOutline)-(2026-2)-(SWE2001-41).pdf`: 과목 운영, flipped-class 방식, 출결, 퀴즈, 과제, 시험 안내
- `SP01-FC.pdf`: 1주차 본강의 자료. CS:APP Chapter 1 성격의 `A Tour on Computer Systems`

## 이번 주 공부 목표

- `hello.c`가 단순한 텍스트 파일에서 실행 가능한 binary executable로 바뀌는 과정을 설명할 수 있어야 합니다.
- source file, text file, binary file, ASCII code의 차이를 구분할 수 있어야 합니다.
- compilation system의 4단계인 preprocessing, compilation, assembly, linking을 순서대로 말할 수 있어야 합니다.
- shell이 `./hello` 명령을 받았을 때 built-in command 여부를 보고 executable을 실행하는 흐름을 이해해야 합니다.
- CPU, main memory, disk, I/O bus, controller, adapter가 실행 과정에서 어떻게 연결되는지 큰 그림을 잡아야 합니다.
- cache와 storage hierarchy가 왜 필요한지, register/L1/L2/L3/main memory/disk가 어떤 속도와 역할 차이를 갖는지 이해해야 합니다.
- 운영체제의 핵심 추상화인 process, virtual memory, file을 이 강의 수준에서 설명할 수 있어야 합니다.
- concurrency와 parallelism의 차이를 말할 수 있어야 합니다.
- instruction-level parallelism, SIMD, multicore, hyperthreading이 병렬성의 서로 다른 층위라는 점을 이해해야 합니다.
- 정보 표현 문제에서 같은 bit pattern이 문자, 정수, 실수, 명령어 등 문맥에 따라 다르게 해석될 수 있음을 이해해야 합니다.

## 페이지별 흐름

| 페이지 | 내용 | 공부 포인트 |
| --- | --- | --- |
| 1 | A Tour on Computer Systems 표지 | 이번 주는 컴퓨터 시스템 전체 조망 |
| 2 | Goal | `hello` program 실행 시 무슨 일이 생기는지 이해 |
| 3 | Source File | `hello.c`가 문자와 ASCII 숫자로 저장됨 |
| 4 | Source File notes | text file, binary file, ASCII standard |
| 5 | ASCII Code Table | 문자가 8-bit integer value로 표현됨 |
| 6 | Translation | `gcc -o hello hello.c`, 4 phases |
| 7 | Preprocessing | `#include`, `#define`, `#ifdef` 등 처리 |
| 8 | Compilation | C program을 assembly-language program으로 변환 |
| 9 | Assembly and Linking | binary executable code 생성 |
| 10 | Running | shell이 명령을 해석하고 executable을 로드/실행 |
| 11 | HW organization | CPU, bus, memory, disk, display 등 전체 구조 |
| 12 | HW notes | bus/word, controller/adapter, instruction cycle |
| 13 | Running | 사용자가 입력한 `./hello` 문자열이 메모리에 저장됨 |
| 14 | Running | executable code/data가 메모리에 로드됨 |
| 15 | Running | 프로그램 실행과 `hello, world\n` 출력 |
| 16 | Caches | processor-memory gap, L1/L2/L3 cache, locality |
| 17 | Multi-core | multicore processor와 shared L3 cache |
| 18 | Storage Hierarchy | 위 계층이 아래 계층의 cache 역할을 함 |
| 19 | Storage table | register부터 web cache까지 latency 차이 |
| 20 | Operating Systems | OS 기능: UI, resource/process management, networking/security |
| 21 | Operating Systems | HW 조작을 위한 uniform interface와 protection |
| 22 | OS abstractions | processes, virtual memory, files |
| 23 | Process | running program abstraction, context switching |
| 24 | Virtual memory | process별 virtual address space |
| 25 | File | byte sequence, I/O device에 대한 uniform view |
| 26 | Virtual machine | entire system abstraction |
| 27 | Networks | network도 individual system 관점에서는 I/O device |
| 28 | Remote execution | telnet remote execution 5단계 |
| 29 | Concurrency/Parallelism | alternating execution vs multiple processors |
| 30 | Multicore processors | core별 register/cache, shared L3 |
| 31 | Hyperthreading | 한 CPU core에서 multiple control flows 지원 |
| 32 | ILP/SIMD | instruction-level parallelism과 data parallelism |
| 33 | Summary | 본문 요약 지점 |
| 34 | Supplementary | instruction cycle, VAS/executable, information representation |
| 35 | Instruction cycle | IF, ID, OF, EX, IC&H |
| 36 | Instruction cycle diagram | CPU, memory, I/O가 cycle과 연결됨 |
| 37 | VAS and executable | virtual address space와 ELF executable 구조 대응 |
| 38 | Character data | ASCII code 복습 |
| 39 | Character exercise | `'v'`, `'K'`, `'6'`, `'/'`, `"System"` 표현 |
| 40 | Integer exercise | short int의 binary representation |
| 41 | Negative integer | 음수 표현 문제 |
| 42 | Float data | float representation 문제 |
| 43 | Integer 1 vs float 1.0 | 같은 값처럼 보여도 bit pattern은 다름 |
| 44 | Instruction | `movl $0,%eax`가 bit로 표현됨 |
| 45 | Meaning of bits | memory의 bit pattern은 해석 문맥이 필요함 |
| 46 | Summary | 1주차 전체 마무리 |

## 1. `hello.c`는 무엇을 보여주는가

자료의 시작점은 다음과 같은 아주 작은 C 프로그램입니다.

```c
#include <stdio.h>
int main()
{
    printf("hello, world\n");
    return 0;
}
```

겉으로 보면 `printf` 한 줄짜리 프로그램이지만, 시스템프로그램 관점에서는 이 파일 하나에 소스 파일, 문자 인코딩, 컴파일러, 어셈블러, 링커, shell, process, memory, I/O, OS abstraction이 모두 연결됩니다. 교수님이 이 예제를 첫 주에 쓰는 이유는 “시스템프로그램은 코드 문법만 보는 과목이 아니라, 코드가 실제 컴퓨터 위에서 실행되는 경로를 보는 과목”이라는 점을 보여주기 위해서입니다.

`hello.c`는 처음에는 사람이 읽을 수 있는 source file입니다. 이 source file은 컴퓨터 내부에 문자 그대로 저장되는 것이 아니라, 각 문자가 정해진 숫자 코드로 바뀌어 저장됩니다. 예를 들어 `#include <stdio.h>` 같은 문자열도 결국 문자 하나하나가 ASCII 값으로 저장된 byte sequence입니다. 따라서 시스템을 이해하려면 파일이 “문장”처럼 보인다는 인간의 관점과, 파일이 “byte들의 배열”이라는 기계의 관점을 동시에 가져야 합니다.

## 2. Text file, binary file, ASCII

PDF는 source file을 설명하면서 text file과 binary file을 구분합니다. Text file은 ASCII characters만으로 이루어진 파일입니다. ASCII standard는 각 character를 고유한 8-bit integer value로 표현합니다. 즉 `A`, `a`, `0`, `\n`, space 같은 문자들이 각각 숫자 코드로 저장됩니다.

Binary file은 사람이 바로 읽을 수 있는 문자만으로 구성되지 않는 파일입니다. 실행 파일, object file, 이미지, 압축 파일 등은 내부적으로 byte들의 조합이지만, 그 byte들이 ASCII 문자로만 해석되지 않습니다. 중요한 점은 text file도 결국 byte이고 binary file도 결국 byte라는 것입니다. 차이는 그 byte들을 어떤 규칙으로 해석할 수 있느냐입니다.

5쪽의 ASCII code table은 이 관점을 보여줍니다. 문자는 추상적인 기호가 아니라 숫자이고, 숫자는 다시 bit pattern입니다. 그래서 뒤 보충자료에서 `'v' -> 0x76 -> 01110110`, `'K' -> 0x4B -> 01001011` 같은 변환이 나옵니다. 시험/퀴즈에서는 정확한 ASCII 표를 통째로 외우기보다는, character가 integer code로 표현된다는 구조와 16진수/2진수 변환 감각이 중요할 수 있습니다.

## 3. Translation: source에서 executable까지

6쪽의 핵심은 `gcc -o hello hello.c`입니다. 이 명령 하나가 내부적으로 네 단계를 거칩니다.

| 단계 | 입력 | 출력 | 역할 |
| --- | --- | --- | --- |
| Preprocessing | `hello.c` | `hello.i` | 전처리 지시문 처리, pure C-language program 생성 |
| Compilation | `hello.i` | `hello.s` | C program을 assembly-language program으로 변환 |
| Assembly | `hello.s` | `hello.o` | assembly를 relocatable object program으로 변환 |
| Linking | `hello.o`와 library object | `hello` | 실행 가능한 binary executable 생성 |

Preprocessing은 `#include`, `#define`, `#ifdef` 같은 preprocessor directives를 처리합니다. `#include <stdio.h>`가 있으면 header file의 선언이 반영되고, macro가 있으면 치환되고, 조건부 컴파일도 이 단계에서 처리됩니다. 결과는 여전히 C 언어 형태이지만, 전처리 지시문이 처리된 modified source program입니다.

Compilation은 C 코드를 assembly-language program으로 바꿉니다. PDF의 예시에는 `main:`, `subq $8, %rsp`, `movl $.LC0, %edi`, `call puts`, `ret` 같은 형태가 나옵니다. 여기서부터는 CPU instruction에 가까워집니다. C의 `printf`가 내부적으로 더 단순한 호출 형태로 바뀌고, stack pointer 조정, return value 설정 같은 기계 수준 동작이 드러납니다.

Assembly는 assembly program을 machine instruction의 binary 형태로 바꿉니다. 이 결과가 object file입니다. 하지만 object file 하나만으로 완전한 실행 파일이 아닐 수 있습니다. 예를 들어 `printf`나 `puts` 같은 함수는 library 쪽 코드와 연결되어야 합니다.

Linking은 여러 object file과 library code를 합쳐 최종 executable object program을 만듭니다. PDF의 그림에서 `printf.o`가 linking에 들어가는 이유가 여기에 있습니다. 시스템프로그램을 공부할 때 linking은 나중에 symbol, relocation, static/dynamic linking, library 이해로 확장될 수 있는 매우 중요한 주제입니다.

## 4. Shell에서 `./hello`가 실행되는 흐름

10쪽은 executable을 실행하는 순간을 설명합니다. 사용자가 shell에 다음과 같이 입력합니다.

```bash
./hello
```

그러면 shell은 command line을 해석합니다. 먼저 command line의 첫 번째 word가 shell built-in command인지 확인합니다. 예를 들어 `cd` 같은 명령은 shell이 직접 처리하는 built-in일 수 있습니다. 하지만 `./hello`가 built-in이 아니라면 shell은 해당 executable file을 load하고 실행합니다. 실행된 program이 종료될 때까지 기다린 뒤, 다시 prompt를 보여줍니다.

이 흐름에서 중요한 것은 “프로그램 실행”이 그냥 파일을 여는 일이 아니라는 점입니다. executable file이 disk에 있고, OS가 이를 memory에 load하며, CPU가 instruction을 fetch/decode/execute하는 흐름으로 넘어갑니다. 사용자의 입력, shell의 해석, OS의 로딩, hardware의 실행이 모두 연결됩니다.

## 5. Hardware organization과 실행

11쪽 그림은 실행 과정의 하드웨어 배치를 보여줍니다. CPU 안에는 register file, PC(program counter), ALU가 있고, CPU는 system bus와 memory bus를 통해 main memory와 연결됩니다. I/O bridge는 system bus와 I/O bus 사이를 이어주고, I/O bus에는 USB controller, graphics adapter, disk controller, network adapter 같은 장치가 붙습니다.

`hello` executable은 처음에 disk에 저장되어 있습니다. 사용자가 keyboard로 `./hello`를 입력하면 이 입력도 I/O 장치를 통해 시스템으로 들어옵니다. 실행 파일은 disk에서 main memory로 로드되고, CPU는 memory의 instruction을 실행합니다. 출력 문자열 `hello, world\n`은 결국 display 장치 쪽으로 전달되어 사용자에게 보입니다.

12쪽의 notes에서는 bus와 word, controller와 adapter, instruction cycle이 정리됩니다. Word size는 1/2/4/8 bytes처럼 CPU가 자연스럽게 처리하는 데이터 단위입니다. Controller는 device 자체 또는 main PCB에 있는 chipset이고, adapter는 PCB slot에 꽂히는 card 형태를 말합니다. 둘 다 CPU와 I/O device 사이의 구체적인 연결 장치로 이해하면 됩니다.

Instruction cycle은 instruction fetch, instruction decode, operand fetch, execute, interrupt check/handling으로 이어집니다. 첫 주에는 각 단계의 세부 회로를 외우기보다, CPU가 “명령어 하나를 가져오고, 해석하고, 필요한 피연산자를 가져오고, 실행하고, interrupt를 확인한다”는 반복 구조를 기억하면 됩니다.

## 6. Running 단계의 세부 그림

13쪽은 사용자가 입력한 `./hello` 문자열이 memory에 저장되는 장면입니다. 여기서 키보드 입력도 결국 byte sequence가 되어 memory로 들어온다고 이해할 수 있습니다. shell은 그 문자열을 command로 해석합니다.

14쪽은 executable loading입니다. disk에 있던 `hello` code와 data가 main memory로 올라옵니다. 프로그램은 disk에 가만히 있을 때는 그냥 file이지만, 실행되려면 memory에 적재되고 process라는 실행 단위가 되어야 합니다.

15쪽은 execution입니다. `hello` code와 data가 memory에 있고, 문자열 `hello, world\n`이 출력됩니다. 이 단계에서 CPU는 instruction을 실행하고, OS와 I/O system을 통해 화면 출력이 이루어집니다. 작은 프로그램 하나가 실제로는 CPU, memory, disk, display, OS를 모두 지나갑니다.

## 7. Cache와 locality

16쪽의 cache memory는 processor-memory gap을 해결하기 위한 장치입니다. CPU는 매우 빠르지만 main memory는 상대적으로 느립니다. CPU가 매번 main memory에서 데이터를 직접 가져오면 속도 차이 때문에 성능이 크게 떨어집니다. 그래서 CPU 가까이에 L1 cache, L2 cache, L3 cache 같은 빠른 저장 공간을 둡니다.

Cache는 locality를 이용합니다. Locality는 프로그램이 최근에 접근한 데이터나 그 근처 데이터를 다시 접근할 가능성이 높다는 성질입니다. 예를 들어 loop에서 같은 변수를 반복해서 쓰거나 배열을 순서대로 읽는 경우, cache에 올라온 data가 다시 사용될 가능성이 큽니다. 시스템프로그램에서는 코드를 어떻게 쓰느냐가 cache 성능에 영향을 줄 수 있다는 점도 나중에 중요해집니다.

17쪽은 multicore architecture를 보여줍니다. Intel Core i7 예시에서는 여러 core가 하나의 chip에 들어 있고, 각 core는 register와 L1 instruction/data cache를 가지며, L2 cache를 갖고, L3 unified cache는 모든 core가 공유합니다. 이 구조는 뒤에서 thread-level parallelism, synchronization, memory consistency 같은 주제로 이어질 수 있습니다.

## 8. Storage hierarchy

18쪽과 19쪽은 storage hierarchy입니다. 핵심 문장은 “storage at one level serves as a cache for storage at the next lower level”입니다. 즉 위 계층은 아래 계층의 일부 내용을 더 빠르게 보관하는 cache 역할을 합니다.

| 계층 | 무엇을 저장/캐시하는가 | 대략적 특징 |
| --- | --- | --- |
| CPU registers | word | 가장 빠르고 가장 작음 |
| L1 cache | cache line | CPU chip 가까움 |
| L2 cache | cache line | L1보다 크고 느림 |
| L3 cache | cache line | 여러 core가 공유 가능 |
| Main memory | disk block/page | OS와 hardware가 관리 |
| Local secondary storage | files | HDD/SSD, OS가 관리 |
| Remote secondary storage/web cache | remote files/web pages | 네트워크를 거침 |

19쪽 표에는 latency 차이가 매우 크게 나옵니다. CPU register는 0~1 cycle 수준이고, L1은 4 cycles, L2는 10 cycles, L3는 50 cycles, main memory는 200 cycles 정도로 제시됩니다. HDD는 10,000,000 cycles, web cache는 1,000,000,000 cycles처럼 완전히 다른 스케일입니다. 이 표는 시스템 성능에서 “어디에 있는 데이터를 접근하느냐”가 매우 중요하다는 것을 보여줍니다.

## 9. Operating System의 역할

20쪽부터는 OS가 등장합니다. OS의 기능은 user interface, resource management, process management, networking, security and protection 등입니다. Kernel의 주요 기능은 hardware resource management와 software resource management입니다. Hardware resource에는 processor, memory, I/O devices가 있고, software resource에는 files, messages 등이 있습니다.

21쪽은 OS가 application과 hardware 사이에 있다는 그림입니다. Application program이 복잡하고 서로 다른 low-level hardware device를 직접 다루게 하면 너무 어렵고 위험합니다. OS는 application에게 simple and uniform interface를 제공하고, application이 hardware를 misuse하지 못하도록 보호합니다.

이 관점은 시스템프로그램에서 매우 중요합니다. 사용자 프로그램은 직접 disk controller를 조작하지 않고 file interface를 사용합니다. 직접 physical memory 전체를 만지지 않고 virtual memory를 사용합니다. 직접 CPU를 영구 점유하지 않고 process scheduling의 대상이 됩니다. 이 모든 것이 OS가 제공하는 abstraction입니다.

## 10. Process, virtual memory, file

22쪽은 OS abstraction 세 가지를 정리합니다.

- Process: running program에 대한 OS abstraction
- Virtual memory: process가 자기만의 memory를 가진 것처럼 보이게 하는 abstraction
- File: I/O device와 저장 데이터를 byte sequence로 보는 abstraction

23쪽의 process 그림에서는 context switching이 나옵니다. Process A가 user code를 실행하다가 `read` 같은 system call을 호출하면 kernel code로 들어가고, disk interrupt나 I/O 대기 상황에서 OS가 다른 Process B로 context switch할 수 있습니다. 시간이 흐르면서 CPU는 A와 B를 번갈아 실행하지만, 각 process 입장에서는 자기 프로그램이 실행되고 있는 것처럼 보입니다.

24쪽은 virtual address space입니다. 각 process는 program code/data, heap, shared libraries, user stack, kernel virtual memory 영역을 포함하는 address space view를 가집니다. Heap은 `malloc`으로 runtime에 만들어지고, stack은 함수 호출과 local variable에 쓰입니다. Shared library 영역은 library functions가 memory-mapped되는 곳입니다. Kernel virtual memory는 user code에는 보이지 않습니다.

25쪽의 file abstraction은 “sequence of bytes”입니다. OS는 다양한 I/O device에 대해 application이 uniform view를 갖게 해줍니다. Disk file, terminal input/output, network socket도 넓게 보면 byte stream이나 file-like interface로 다뤄질 수 있습니다. 이 관점은 뒤에 file I/O, pipe, socket으로 이어질 가능성이 높습니다.

26쪽에서는 virtual machine이 전체 시스템에 대한 abstraction이라고 설명됩니다. Process, instruction set architecture, virtual memory, files, OS, processor, memory, I/O devices까지 포함하는 abstraction입니다. VM은 단순히 VirtualBox 같은 프로그램만 의미하는 것이 아니라, 시스템 전체를 추상화한다는 컴퓨터 시스템 관점도 포함합니다.

## 11. Network도 I/O device처럼 보기

27쪽은 network를 individual system 관점에서 just another I/O device로 볼 수 있다고 말합니다. 이 문장이 중요합니다. 네트워크는 특별한 외부 세계처럼 보이지만, 한 컴퓨터 시스템 내부에서는 network adapter라는 I/O device를 통해 byte를 보내고 받는 구조입니다.

28쪽은 telnet으로 remote execution을 하는 5단계 예시입니다. 사용자가 local keyboard에서 `hello`를 입력하면 local telnet client가 그 문자열을 remote telnet server로 보냅니다. remote server는 shell에 문자열을 전달하고, shell은 remote machine에서 `hello` program을 실행합니다. program output인 `hello, world\n`은 telnet server에서 client로 돌아오고, client가 local display에 출력합니다.

이 예시는 remote execution도 결국 입력 문자열, network I/O, shell, program execution, output string 전달의 조합이라는 점을 보여줍니다. 나중에 socket programming을 할 때도 이 감각이 중요합니다.

## 12. Concurrency와 parallelism

29쪽은 concurrency와 parallelism을 구분합니다. Concurrency는 multiple processes가 processor를 번갈아 사용하면서 같은 시간에 실행되는 것처럼 보이는 상태입니다. 하나의 CPU만 있어도 OS가 빠르게 switch하면 여러 사용자가 동시에 시스템과 상호작용하거나, 한 사용자가 여러 task를 동시에 하는 것처럼 보입니다.

Parallelism은 실제로 multiple processors가 있어서 process들이 물리적으로 동시에 실행될 수 있는 구조입니다. Multiprocessor systems, multicore processors, hyperthreading이 여기에 들어갑니다.

30쪽은 thread-level parallelism으로 multicore processors를 보여줍니다. 각 core는 자기 register와 L1 cache를 갖고, L2 cache와 shared L3 cache를 통해 main memory와 연결됩니다. 여러 thread를 여러 core에 나누어 실행하면 실제 병렬 실행이 가능합니다.

31쪽은 hyperthreading입니다. 한 CPU가 multiple flows of control을 실행할 수 있게 하는 기법입니다. PC와 register file 같은 일부 hardware는 여러 copy를 두지만, floating-point arithmetic unit 같은 다른 부분은 single copy만 둘 수 있습니다. thread switching time이 매우 짧고, Intel Core i7 예시에서는 4 cores와 core당 2 threads로 총 8 threads를 실제 병렬처럼 실행할 수 있다고 설명합니다.

32쪽은 instruction-level parallelism과 SIMD입니다. Pipelining과 superscalar processors는 instruction-level parallelism에 해당합니다. SIMD는 single instruction multiple data로, 하나의 instruction이 여러 data에 대해 병렬 operation을 수행하게 합니다. 예시로 최근 Intel/AMD processor에서 8개의 single-precision floating-point numbers를 parallel하게 더할 수 있다고 나옵니다.

## 13. 보충 1: instruction cycle

35쪽과 36쪽은 instruction cycle을 복습합니다. 단계는 다음과 같습니다.

1. IF: Instruction Fetch
2. ID: Instruction Decode
3. OF: Operand Fetch
4. EX: Execute
5. IC&H: Interrupt Check and Handling

Sequential semantic model에서는 프로그램이 명령어를 하나씩 순서대로 실행하는 것처럼 보입니다. 하지만 machine level에서는 pipeline, cache, interrupt, out-of-order execution 같은 다양한 메커니즘이 개입할 수 있습니다. 첫 주차에서는 적어도 CPU가 instruction을 fetch/decode/execute하는 반복 장치라는 점을 잡아두면 됩니다.

## 14. 보충 2: virtual address space와 executable

37쪽은 virtual address space와 ELF executable 구조를 나란히 보여줍니다. Virtual address space에는 kernel virtual memory, user stack, memory-mapped region for shared libraries, runtime heap, read/write data, read-only code/data 등이 있습니다. ELF executable에는 ELF header, segment header table, `.init`, `.text`, `.rodata`, `.data`, `.bss`, `.symtab`, `.debug`, `.line`, `.strtab`, section header table 등이 보입니다.

핵심은 실행 파일 내부의 section/segment가 process address space에 load되어 실행된다는 것입니다. `.text`는 code, `.rodata`는 read-only data, `.data`는 initialized global/static data, `.bss`는 uninitialized global/static data로 이어질 수 있습니다. 지금 당장 ELF 세부 구조를 다 외울 필요는 없지만, “실행 파일의 내용이 process의 memory layout과 연결된다”는 점은 매우 중요합니다.

## 15. 보충 3: information representation

38~45쪽은 정보 표현 복습입니다. Character data, integer data, negative integer, float data, instruction까지 모두 bit pattern으로 표현됩니다.

예시는 다음과 같습니다.

- `'v' -> 0x76 -> 01110110`
- `'K' -> 0x4B -> 01001011`
- `'6' -> 0x36 -> 00110110`
- `'/' -> 0x2F -> 00101111`
- `"System" -> 0x53797374656D00`

`"System"` 뒤에 `00`이 붙는 것은 C string의 null terminator를 떠올리게 합니다. C에서 문자열은 보통 마지막에 `\0`이 붙어 끝을 표시합니다. 이 점은 시스템프로그래밍에서 pointer, memory, buffer를 다룰 때 매우 중요합니다.

Integer exercise에서는 short int의 binary representation이 나옵니다. `1`은 `0000000000000001`, `20`은 `0000000000010100`처럼 표현됩니다. 음수 `-1`, `-18476`은 물음표로 남겨져 있는데, 이는 two's complement 표현을 생각하게 만드는 문제입니다.

Float data에서는 `1.0`, `3.14`가 어떻게 표현되는지 묻고, 43쪽은 integer `1`과 float `1.0`의 bit pattern이 완전히 다르다는 점을 보여줍니다. Integer `1`은 `00000000000000000000000000000001`이고, float `1.0`은 `00111111100000000000000000000000`입니다. 사람이 보기에는 둘 다 1이지만, 컴퓨터 내부 표현은 type에 따라 달라집니다.

44쪽과 45쪽은 instruction도 bit pattern이라는 점을 보여줍니다. `movl $0,%eax` 같은 instruction도 `0x488903`, `010010001000100100000011`처럼 표현될 수 있습니다. 그리고 45쪽의 질문이 핵심입니다. `1000100100000011`이 memory에 있을 때, 이것이 무엇을 의미하는가? 답은 “문맥 없이는 모른다”입니다. 같은 bit pattern이 integer일 수도 있고, character일 수도 있고, floating-point data일 수도 있고, machine instruction일 수도 있습니다.

## 퀴즈/구술 대비 포인트

- `hello.c`는 왜 text file인가?
- ASCII standard가 character를 어떻게 표현하는가?
- `gcc -o hello hello.c` 내부의 4단계는 무엇인가?
- preprocessing과 compilation의 차이는 무엇인가?
- assembly와 linking은 각각 어떤 산출물을 만드는가?
- shell은 `./hello`를 입력받으면 어떤 순서로 처리하는가?
- built-in shell command와 external executable의 차이는 무엇인가?
- CPU의 instruction cycle은 어떤 단계로 이루어지는가?
- cache memory는 processor-memory gap을 어떻게 줄이는가?
- locality란 무엇이고 cache가 locality를 이용한다는 말은 무슨 뜻인가?
- storage hierarchy에서 위 계층이 아래 계층의 cache 역할을 한다는 말은 무슨 뜻인가?
- OS가 application에게 제공하는 simple and uniform interface의 예시는 무엇인가?
- process, virtual memory, file은 각각 어떤 abstraction인가?
- context switching은 왜 필요한가?
- virtual address space에는 어떤 영역들이 있는가?
- network를 I/O device처럼 볼 수 있다는 말은 무슨 뜻인가?
- concurrency와 parallelism의 차이는 무엇인가?
- multicore와 hyperthreading의 차이는 무엇인가?
- SIMD는 어떤 종류의 parallelism인가?
- integer `1`과 float `1.0`은 왜 bit pattern이 다른가?
- memory의 bit string은 왜 문맥 없이는 의미를 확정할 수 없는가?

## 헷갈리기 쉬운 점

- Source file이 사람이 읽을 수 있다고 해서 컴퓨터 내부에 “글자”로 저장되는 것은 아닙니다. 결국 byte sequence이고, text file은 그 byte들이 ASCII character로 해석될 수 있는 파일입니다.
- `gcc` 명령은 하나의 명령처럼 보이지만 내부적으로 전처리기, 컴파일러, 어셈블러, 링커를 차례로 거칩니다.
- Object file과 executable file은 다릅니다. Object file은 linking 전 중간 산출물이고, executable은 OS가 load해서 실행할 수 있는 최종 산출물입니다.
- Shell은 프로그램 자체가 아닙니다. Shell은 사용자의 command line을 해석하고, 필요하면 executable을 실행시키는 program입니다.
- Concurrency는 “동시에 진행되는 것처럼 보임”이고, parallelism은 “실제로 여러 처리 장치가 동시에 실행함”입니다.
- Virtual memory는 실제 physical memory가 무한히 생긴다는 뜻이 아니라, 각 process에게 독립적인 address space illusion을 제공한다는 뜻입니다.
- File은 문서 파일만 의미하지 않습니다. OS 관점에서는 다양한 I/O device를 byte sequence 또는 file-like abstraction으로 다룰 수 있습니다.
- Bit pattern 자체에는 의미가 없습니다. Type, instruction format, encoding rule 같은 해석 규칙이 있어야 의미가 생깁니다.

## 이번 주 공부 순서

1. `SP01-S16-33#` 온라인 강의를 끝까지 듣습니다.
2. 강의를 들으면서 PDF 페이지 6의 translation pipeline을 반드시 표시합니다.
3. 페이지 10~15의 실행 흐름을 `shell -> load -> memory -> CPU -> output`으로 정리합니다.
4. 페이지 16~19의 cache/storage hierarchy는 표 중심으로 봅니다.
5. 페이지 20~26의 OS abstraction은 process, virtual memory, file 세 단어로 묶습니다.
6. 페이지 29~32의 concurrency/parallelism은 차이 위주로 봅니다.
7. 페이지 38~45의 information representation은 “같은 bit, 다른 해석”이라는 한 문장으로 잡습니다.
8. 목요일 오프라인 수업 전 공지와 퀴즈 여부를 확인합니다.

## 목요일 전 최소컷

- `SP01-S16-33#` 수강 완료
- `gcc` 4단계 암기: preprocessing, compilation, assembly, linking
- OS abstraction 3개 암기: process, virtual memory, file
- concurrency vs parallelism 차이 설명 가능
- ASCII/text/binary 차이 설명 가능
- 목요일 15:05 퀴즈 가능성 확인

## 다음 업데이트

- 온라인 강의에서 교수님이 강조한 표현이나 예시가 있으면 이 파일에 추가
- 목요일 오프라인 수업에서 퀴즈 범위/문제풀이/과제 공지가 나오면 `notices.md`, `assignments.md`에 반영
- 전문 텍스트화가 필요하면 `transcript-SP01-FC.md`를 별도로 만들고, 이 `study.md`는 공부용 요약/정리 문서로 유지
