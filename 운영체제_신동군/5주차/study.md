# 운영체제 - 신동군 5주차 스터디: Memory

## 빠른 목차

- Part A. 한 장 복습 지도
- Part B. Address Space와 C Memory API
- Part C. Address Binding과 Base/Bounds 주소 변환
- Part D. Contiguous Allocation과 Fragmentation
- Part E. Virtual Memory: 공간, 배치, 보호 문제의 해결
- Part F. Segmentation
- Part G. Free-Space Management
- Part H. 앞 단원과의 연결, 복습 질문

## Part A. 한 장 복습 지도

이번 주의 중심은 프로그램이 사용하는 **가상 주소(Virtual Address, VA)** 와 실제 RAM의 **물리 주소(Physical Address, PA)** 를 분리하는 것이다. 프로그램은 자기만의 크고 연속적인 메모리를 가진 것처럼 실행하고, OS와 하드웨어의 MMU가 실제 RAM 위치를 관리한다.

```text
                    PROGRAM
                       |
                Virtual Address
                       |
                       v
              +----------------+
              |   MMU / MAP    |
              | 주소 변환·검사 |
              +-------+--------+
                      |
                Physical Address
                      v
              +----------------+
              |      RAM       |<-----> DISK
              | A / B / FREE   |      공간 부족 시 이동
              +----------------+
```

```text
Address Space
     | 프로그램은 VA를 사용
Address Translation
     | MMU가 PA로 변환·검사
Contiguous Allocation
     | 연속 공간 요구 때문에 단편화 발생
Virtual Memory
     | mapping으로 물리 배치와 주소를 분리
Segmentation
     | segment별 변환·보호
Free-Space Management
     | 실제 물리 공간을 효율적으로 공급
```

시험 직전에 우선 복원할 그림은 세 개다.

1. `Base/Bounds`로 VA를 검사하고 `PA = VA + Base`를 만드는 흐름
2. 분산된 RAM hole도 mapping을 쓰면 여러 process에 배치할 수 있다는 그림
3. `(segment, offset) -> segment table -> physical address` 흐름

---

## Part B. Address Space와 C Memory API

### 1. Address space란 무엇인가

Address space는 실행 중인 process가 보는 메모리의 모습이다. 물리 메모리는 모든 process가 나눠 쓰지만, OS는 각 process에 **private하고 잠재적으로 큰 메모리 공간**이 있는 것처럼 보이게 한다. 이것이 memory virtualization의 출발점이다.

프로그램에서 `main`, `malloc(1)`, 지역 변수의 주소를 출력하면 모두 주소처럼 보이는 값을 얻는다. 하지만 이 값은 일반적으로 물리 주소가 아니라 process 관점의 virtual address다.

```text
낮은 주소
+-----------+
|   CODE    |  실행 명령어
+-----------+
|   DATA    |  전역/정적 데이터
+-----------+
|   HEAP    |  malloc/free, 아래 방향이 아닌 위쪽으로 성장
|           |
|  빈 공간  |
|           |
|   STACK   |  함수 호출, 지역 변수, 높은 주소 쪽에서 아래로 성장
+-----------+
높은 주소
```

주소 배치 자체는 플랫폼과 실행마다 달라질 수 있다. 여기서 외울 것은 정확한 숫자가 아니라 code, heap, stack이 서로 다른 역할과 lifetime을 가진다는 점이다.

### 2. Stack과 Heap

| 구분 | Stack | Heap |
| --- | --- | --- |
| 할당/해제 | compiler가 함수 호출·반환에 맞춰 자동 처리 | programmer가 `malloc`/`free`로 명시 처리 |
| 대표 데이터 | 지역 변수, 함수 call frame | 실행 중 크기가 정해지는 객체 |
| lifetime | 함수가 반환하면 끝남 | `free`하거나 process가 종료할 때까지 유지 가능 |
| 위험 | 반환 뒤 지역 변수 주소 사용 | leak, dangling pointer, double free |

`malloc()`이 돌려준 주소값 자체는 보통 stack의 pointer 변수에 들어가고, 실제 할당된 객체는 heap에 존재한다. 즉 stack에 있는 pointer가 heap 객체를 가리키는 구조가 흔하다.

### 3. 자주 나오는 memory bug

- **Forgetting to allocate**: 할당하지 않은 pointer에 접근한다.
- **Not allocating enough / buffer overflow**: 필요한 것보다 작은 공간을 확보해 경계를 넘겨 쓴다. 문자열은 끝의 `\0`도 필요하다.
- **Uninitialized memory**: `malloc` 직후의 값은 0이라고 가정할 수 없다. `calloc`은 할당 뒤 0으로 초기화한다.
- **Memory leak**: 더 이상 쓸 수 없는 heap 객체를 `free`하지 않아 장기 실행 프로그램의 memory가 계속 늘어난다.
- **Dangling pointer**: 아직 쓰려는 객체를 먼저 `free`했다.
- **Double free / invalid free**: 같은 객체를 두 번 해제하거나 heap 객체가 아닌 것을 `free`한다.

짧게 끝나는 process는 OS가 종료 시 page를 회수하므로 OS 전체 관점에서 leak을 남기지는 않는다. 그렇다고 프로그램 내부 leak이 괜찮다는 뜻은 아니다. 서버처럼 오래 실행되는 프로그램에서는 leak이 실제 장애가 된다. `valgrind` 같은 도구는 이러한 C memory 오류를 찾는 데 유용하다.

### 4. Memory library 아래의 OS 지원

C library의 `malloc/free`는 자체 allocator를 사용하지만 결국 OS 기능에 기대야 한다.

- `brk/sbrk`: program break, 즉 heap 끝 위치를 늘리거나 줄인다.
- `mmap`: 특정 file과 연결되지 않은 anonymous memory region을 만들 수도 있고, file mapping에도 사용된다.

```text
kernel space
stack
mmap area
heap
BSS / data
text
start_brk ---- program break
```

---

## Part C. Address Binding과 Base/Bounds 주소 변환

### 1. Logical/virtual address와 physical address

CPU가 명령어를 실행하며 만들어 내는 주소는 logical address 또는 virtual address다. Memory unit이 실제로 다루는 주소는 physical address다. VA를 PA로 바꾸는 일을 **address translation** 또는 relocation이라고 한다.

### 2. 주소를 언제 확정하는가: binding

| Binding | 주소 확정 시점 | process 위치를 바꾸려면 | 특징 |
| --- | --- | --- | --- |
| Compile-time | compile 시 | 재컴파일 | absolute code, 위치가 미리 정해질 때만 가능 |
| Load-time | load 시 | reload/relocation | relocatable code 필요 |
| Runtime | 실행 중 | base 등 mapping 변경 | 하드웨어 MMU가 매 access 때 변환 |

Compile-time과 load-time 방식은 소프트웨어 중심 relocation이다. 특히 load-time binding은 나중에 address space를 다른 위치로 옮기기 어렵고 protection도 충분히 제공하지 못한다. 현대 OS의 핵심은 runtime binding이다.

### 3. Dynamic relocation: Base와 Bounds

Runtime binding에서는 MMU 안의 base register와 bounds 또는 limit register를 사용한다.

```text
Virtual Address (VA)
        |
        v
  VA < Bounds ?
   | yes             | no
   v                 v
PA = Base + VA    exception
   |
   v
 physical memory
```

- **Base**: 이 process address space가 실제 RAM에서 시작하는 물리 주소
- **Bounds/Limit**: process가 접근 가능한 virtual address 범위
- **Translation**: 유효한 경우 `PA = VA + Base`

각 process는 0부터 시작하는 자기 주소 공간을 가진다고 생각해도, 실제로는 서로 다른 base에 놓인다. 따라서 Process A와 B가 모두 VA 1024를 사용해도 서로 다른 PA로 번역될 수 있다.

### 4. Protection과 context switch

base/bounds register를 user program이 바꿀 수 있으면 protection은 무너진다. 그래서 이를 변경하는 instruction은 privileged instruction이어야 하고, OS만 kernel mode에서 실행할 수 있다.

잘못된 VA 또는 bounds 밖 access가 생기면 hardware는 exception을 일으킨다. OS는 illegal access를 처리하고 process를 종료하거나 오류를 전달한다.

CPU virtualization에서 context switch가 register 상태를 저장·복원했던 것처럼, memory virtualization에서도 OS는 process 전환 시 각 process의 base/bounds pair를 저장·복원해야 한다. CPU state와 memory mapping state는 함께 process context의 일부다.

---

## Part D. Contiguous Allocation과 Fragmentation

### 1. 연속 할당

초기의 방식에서는 모든 free memory가 하나의 큰 hole에서 시작하고, process가 들어오면 그 process 전체를 수용할 수 있는 하나의 연속 partition을 준다. process가 끝나면 partition은 free hole이 된다.

```text
[ hole 100 KB ] [ Process A ] [ hole 150 KB ]

총 free = 250 KB
요청 = 200 KB
-> 어느 하나의 연속 hole도 200 KB가 아니므로 할당 실패
```

이 예제가 external fragmentation의 핵심이다. 총량이 아니라 **연속성**이 문제다.

### 2. 두 종류의 fragmentation

- **Internal fragmentation**: 이미 할당한 block 내부에서 실제로 쓰지 않는 낭비. 예를 들어 stack과 heap 사이의 미사용 영역을 포함해 크게 잡은 contiguous address space는 내부 낭비가 될 수 있다.
- **External fragmentation**: free space의 총합은 충분하지만 여러 작은 hole로 분산돼 큰 연속 요청을 만족시키지 못한다.

### 3. coalescing과 compaction

- **Coalescing**: 인접한 free hole 두 개를 하나의 더 큰 hole로 합친다. process 종료 뒤 가장 먼저 할 수 있는 완화책이다.
- **Compaction**: 실행 중 process 내용을 옮겨 free space를 한 덩어리로 모은다. 하지만 dynamic relocation이 가능해야 하고, 많은 memory copy와 CPU time을 써서 비용이 크다.

Compaction은 fragmentation을 없애는 만능 해결책이 아니라, 실행 중 data를 옮겨도 주소 의미를 보존할 수 있을 때만 가능한 비싼 작업이다.

### 4. placement strategy

| 전략 | 선택 방식 | 장점 | 주의점 |
| --- | --- | --- | --- |
| First-fit | 처음 만나는 충분히 큰 hole | 단순하고 search overhead가 작음 | 앞부분 hole 사용 편향 가능 |
| Best-fit | 충분한 hole 중 가장 작은 것 | 큰 hole 보존 기대 | 전체 search가 길고 작은 조각을 많이 만들 수 있음 |
| Worst-fit | 가장 큰 hole | 작은 조각 수를 줄이려는 시도 | 전체 search 필요, 항상 좋은 결과 보장 없음 |
| Next-fit | 직전 탐색 종료점부터 순환 search | first-fit과 유사한 낮은 overhead, 분산 사용 | 다음 search 위치를 유지해야 함 |

이 전략들은 모두 contiguous allocation 틀 안에서 hole을 선택하는 방법이다. 어떤 하나가 모든 workload에서 절대적으로 우월하다는 뜻은 아니다.

---

## Part E. Virtual Memory: 공간, 배치, 보호 문제의 해결

### 1. memory의 세 문제

강의는 virtual memory가 필요한 이유를 세 문제로 정리한다.

1. **Not enough space**: 32-bit program은 4 GB virtual address를 볼 수 있지만 실제 RAM이 1 GB일 수 있다.
2. **Holes in address space**: free RAM의 총량은 충분해도 contiguous placement가 안 될 수 있다.
3. **Protection**: 모든 program이 같은 physical address를 마음대로 쓰면 서로의 data를 망가뜨릴 수 있다.

### 2. Indirection과 mapping

virtual memory는 program address를 RAM address에 직접 같다고 보지 않고 map을 한 번 거친다.

```text
No virtual memory
program address == RAM address

Virtual memory
program VA -- mapping --> physical RAM address
```

이 indirection 하나가 공간 부족, physical placement, protection이라는 세 문제를 함께 풀 수 있게 만든다.

### 3. 공간 부족: disk를 backing store로 사용

program address space의 일부를 RAM이 아니라 disk에 두고, 필요한 data만 RAM으로 가져올 수 있다. RAM이 찼다면 최근에 덜 사용한 data를 내보내고 필요한 data를 읽는다. 후속 주차에서 page, page fault, replacement algorithm으로 구체화될 내용이다.

중요한 성능 직관은 disk access가 RAM access보다 압도적으로 느리다는 것이다. virtual memory가 address space를 크게 보이게 해도 자주 disk를 왕복하면 performance가 크게 떨어진다.

### 4. hole 문제: non-contiguous placement

각 process의 virtual address space는 연속적으로 보이지만, 그 내용은 physical RAM의 떨어진 곳들에 mapping할 수 있다. 그래서 1 GB hole 두 개가 있을 때도 Program 3의 서로 다른 부분을 각 hole에 둘 수 있다. contiguous allocation의 external fragmentation 제약을 완화하는 핵심이다.

### 5. protection과 sharing

각 process마다 VA-to-PA mapping이 다르다.

```text
Process A: VA 1024 --map A--> PA 1
Process B: VA 1024 --map B--> PA 5
```

같은 VA 값이라도 다른 physical memory를 가리키므로 process는 서로의 private data를 직접 훼손할 수 없다. 반대로 shared library처럼 공유가 필요한 data는 여러 process의 VA를 같은 PA에 deliberate하게 mapping할 수 있다. Virtual memory는 isolation만 만드는 기능이 아니라 controlled sharing도 가능하게 한다.

### 6. memory virtualization의 목표

- **Transparency**: program은 virtualized memory의 복잡함을 모르고 private memory가 있는 것처럼 동작한다.
- **Efficiency**: translation을 위한 시간과 metadata를 위한 공간을 과도하게 쓰지 않아야 한다. 후속 page table/TLB 단원에서 다룬다.
- **Protection**: process 간, 그리고 process와 OS 간 isolation을 보장한다.

---

## Part F. Segmentation

### 1. 기본 관점

Segmentation은 address space를 하나의 연속 덩어리로 보지 않고, code, global variable, heap, stack, library처럼 의미 있는 variable-sized segment들의 모음으로 본다.

```text
Logical address v = (s, d)

s: segment number
d: displacement / offset
```

각 segment는 physical memory의 서로 다른 위치에 놓일 수 있다. 따라서 heap과 stack 사이의 넓은 unused virtual space를 physical memory에 실제로 할당하지 않아도 된다.

### 2. segment address translation

```text
(s, d)
  |
  v
segment table[s]
  | Base, Limit, protection, growth direction
  |
  +-- d valid? -- no --> segmentation violation / fault
  |
  yes
  v
PA = Base[s] + adjusted offset
```

`d`가 해당 segment limit보다 작아야 한다. 그렇지 않으면 segmentation fault다. 각 segment에 read/write/execute 권한을 두면 code segment를 read-execute로, heap/stack을 read-write로 보호할 수 있다.

### 3. segment 판별 방식

- **Explicit segmentation**: virtual address의 상위 bit 일부가 segment 번호를 직접 나타낸다.
- **Implicit segmentation**: hardware가 address가 만들어진 방식을 보고 추론한다. program counter에서 나온 instruction fetch는 code, stack/base pointer 기반 주소는 stack, 그 외는 heap으로 판단할 수 있다.

### 4. stack의 성장 방향

일반적인 그림에서는 stack이 높은 virtual address에서 낮은 방향으로 성장한다. 따라서 stack segment offset을 해석할 때는 단순히 base에 양의 offset을 더하는 code/heap과 다를 수 있다. stack range와 growth direction을 함께 고려해야 한다.

### 5. coarse-grained와 fine-grained

| 분류 | 의미 | 예 |
| --- | --- | --- |
| Coarse-grained | 소수의 큰 segment | code, heap, stack |
| Fine-grained | 많은 작은 segment | object, procedure 등 다수 단위 |

Fine-grained segmentation은 많은 segment table entry를 관리해야 한다. OS는 context switch 시 segment register 상태를 저장·복원하고, `sbrk()` 등으로 heap이 커질 때 segment size/limit도 갱신해야 한다.

Segmentation은 non-contiguous allocation과 protection에 도움이 되지만, variable-sized segment이므로 physical memory free-space 관리와 external fragmentation 문제가 다시 나타난다. 이것이 후속 paging으로 이어지는 이유다.

---

## Part G. Free-Space Management

OS는 segment나 kernel object를 위한 실제 physical memory 공간을 찾아야 한다. 일반 allocator가 매번 긴 free list를 search하면 비용과 fragmentation이 커질 수 있다.

### 1. Segregated list

자주 요청되는 size별로 전용 pool을 둔다. 예를 들어 특정 크기의 object 요청은 그 size list에서 바로 받는다. 인기 size에 대한 search가 단순하고 fragmentation을 줄일 수 있지만, 각 pool에 어느 정도 공간을 미리 줄지 정하는 trade-off가 있다.

### 2. Slab allocator

Slab allocator는 lock, file-system inode 같은 자주 쓰는 kernel object type별 cache를 만든다.

```text
object request
    |
object-specific cache
    |
free object exists? -- yes --> allocate
    |
    no
    v
request a slab from general allocator
```

한 slab 안 object들의 reference count가 모두 0이 되면 general allocator가 slab을 reclaim할 수 있다. Object layout을 반복 초기화하지 않고 cache를 재사용한다는 점도 kernel workload에서 의미가 있다.

### 3. Buddy allocator

Buddy allocator는 power-of-two 크기 block을 order별로 관리한다.

```text
Block size = 4 KB * 2^order

order 0: 4 KB
order 1: 8 KB
order 2: 16 KB
```

큰 block을 필요에 따라 두 buddy로 반씩 split하고, 둘 다 free가 되면 merge한다. 크기 분할과 병합이 규칙적이라 관리가 단순하지만, 요청 크기를 power-of-two block으로 올려 잡으면서 internal fragmentation이 생길 수 있다.

---

## Part H. 앞 단원과의 연결과 복습 질문

### 1. Process/CPU virtualization과의 연결

- Process는 CPU register만이 아니라 address space와 memory mapping state도 가진다.
- Context switch는 CPU register만 교체하는 일이 아니다. Base/bounds나 segment register 같은 memory translation state도 맞는 process 것으로 복원해야 한다.
- User mode process가 kernel-only register를 바꾸지 못하게 하는 privileged instruction은 user/kernel mode와 protection 원칙을 다시 보여준다.

### 2. 다음 단원으로 이어지는 질문

- Base/bounds는 하나의 연속 region만 다루는데, 여러 불연속 physical page는 어떻게 map할까?
- virtual address마다 mapping 정보를 어디에 저장할까?
- mapping table이 너무 크면 어떻게 빠르게 찾을까?
- RAM이 가득 찼을 때 어느 page를 disk로 내보낼까?

이 질문들이 paging, page table, TLB, page replacement 단원으로 이어진다.

### 3. 빠른 자가 점검

1. `malloc` pointer가 stack에 있을 수 있는데 실제 object는 왜 heap에 있다고 하는가?
2. Base=1000, Bounds=200일 때 VA=199와 VA=200은 각각 어떻게 처리되는가?
3. total free space가 충분한데 allocation이 실패하는 현상은 internal/external fragmentation 중 무엇인가?
4. virtual memory가 hole 문제를 푸는 핵심은 compaction이 아니라 무엇인가?
5. Process A와 B가 모두 VA 1024를 써도 충돌하지 않는 이유는 무엇인가?
6. Segmentation에서 `(s, d)`의 `s`와 `d`는 각각 무엇인가?
7. stack segment가 다른 segment와 달리 주의할 growth direction은 무엇인가?
8. Slab allocator와 buddy allocator가 해결하려는 free-space 관리 문제는 무엇인가?

## 학습 완료 기록

- 상태: 2026-09-29 기준 5주차 Memory 강의 복습 완료
- 원본: `OS06-Memory.pdf` (51 pages)
- 개인 한 장 요약의 핵심: VA/PA 분리, base/bounds, fragmentation, virtual memory mapping, segmentation, free-space management
- 2026-10-01: 2주치 묶음 퀴즈 응시 완료 (사용자 확인)
- 다음: PA1 system call 구현·테스트, 10/7 마감
