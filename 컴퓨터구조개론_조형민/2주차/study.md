# 컴퓨터구조개론 - 조형민 2주차 학습지도

## 상태

- 2026-09-07 기준 `CA_Lecture_02.pdf`를 반영함.
- 원본 자료는 44페이지짜리 `Introduction to Computer Architecture - Chapter 1 Performance` 강의자료임.
- 이번 정리는 PDF의 흐름을 기준으로, 성능을 어떻게 정의하고 계산하는지 한 번에 복습할 수 있게 만든다.
- 수업 중 교수님이 추가로 강조한 예제나 PA 공지가 나오면 이 파일의 해당 절에 덧붙인다.

## 원본 자료

- `CA_Lecture_02.pdf`: Chapter 1 Performance, 44 pages

## 한 줄 결론

2주차의 핵심은 컴퓨터구조에서 "빠르다"는 말을 감으로 쓰지 않고, 실행 시간, 처리량, 클럭, 명령어 수, CPI, 벤치마크, 전력 한계, 병렬화 한계까지 포함해서 정량적으로 설명하는 것이다. 1주차가 컴퓨터 시스템의 큰 구성과 기술 배경을 잡았다면, 2주차는 그 시스템을 평가하는 언어를 만든다.

## 2주차 전체 구조도

이번 2주차는 `Performance`가 중심이고, 앞의 반도체 제조 파트부터 뒤의 Amdahl's Law까지 사실 하나의 이야기로 이어진다. 자료 전체 흐름은 그대로 `반도체 -> 성능 정의 -> CPU 성능 공식 -> benchmark -> power wall -> multicore -> Amdahl's Law` 순서다.

```text
┌───────────────────────────────────────────────────────────┐
│                  WEEK 2 : PERFORMANCE                     │
│                                                           │
│  핵심 질문: "컴퓨터가 빠르다"를 어떻게 숫자로 설명하지?    │
└──────────────────────────┬────────────────────────────────┘
                           │
                           ▼

                 [1. Semiconductor Technology]
                실제 CPU는 반도체 위에 만들어짐
                           │
                           ▼
        Old System ──────────────── Modern SoC
        여러 chip 사용              기능을 한 chip에 통합
        느린 chip 통신              빠름 / 전력↓ / 신뢰성↑
                           │
                           ▼

                 [2. IC Manufacturing]
       Silicon → Wafer → Die → Test → Package
                           │
              ┌────────────┴────────────┐
              ▼                         ▼
            Yield                    Die Area
      정상 동작 die의 비율          chip 하나의 크기
              │                         │
              └────────────┬────────────┘
                           ▼
                     Cost per Die
        = Wafer Cost / (Dies per Wafer × Yield)

        큰 Die
          ↓
        wafer에서 적게 나옴
          +
        defect 맞을 확률 증가
          ↓
        Yield ↓
          ↓
        Cost ↑↑
                           │
                           ▼

                       [Chiplet]
             거대한 die 하나 대신
          여러 개의 작은 die를 연결
             → yield / cost 개선

════════════════════════ PERFORMANCE ════════════════════════

                           ▼

              [3. What does "FAST" mean?]

                 Performance는 두 관점

          Response Time                  Throughput
          = Latency                      = tasks / time
          하나 끝나는 시간               일정 시간에 몇 개 처리?

          PC / 개인 사용자               Server에 중요
                           │
                           ▼

                [4. Relative Performance]

                Performance = 1 / Execution Time

                    실행시간 ↓
                       ↓
                    성능 ↑

      "A가 B보다 몇 배 빠른가?"

           Performance_A / Performance_B
                 =
             Time_B / Time_A

                           │
                           ▼

              [5. What affects Performance?]

      Algorithm
      "얼마나 많은 computation이 필요한가?"
                  │
                  ▼
      Language / Compiler / ISA
      "몇 개 instruction으로 바뀌는가?"
                  │
                  ▼
      Processor / Memory
      "각 instruction이 얼마나 빨리 실행되는가?"
                  │
                  ▼
      I/O
      "입출력은 얼마나 빠른가?"

                           │
                           ▼

════════════════════ CPU PERFORMANCE CORE ═══════════════════

                  [6. CPU Clock]

             ┌───────┐     ┌───────┐
Clock ───────┘       └─────┘       └────

     Clock Cycle Time = 한 cycle에 걸리는 시간
     Clock Rate       = 1초에 몇 cycle?

            Cycle Time = 1 / Clock Rate

예:
4 GHz
= 1초에 4 × 10^9 cycles
= 1 cycle = 0.25 ns

                           │
                           ▼

                [7. CPU Time]

        CPU Time
        = Clock Cycles × Cycle Time

또는

        CPU Time
        = Clock Cycles / Clock Rate

즉 빠르게 하려면

       cycles ↓       또는       clock rate ↑

BUT 둘 사이에는 trade-off가 있음.

"GHz만 높으면 무조건 빠르다"는 말은 틀림.

                           │
                           ▼

              [8. IC + CPI]

IC
= Instruction Count
= 프로그램이 실행한 instruction 개수

CPI
= Cycles Per Instruction
= instruction 하나당 평균 몇 cycle?

                Clock Cycles
                      =
                    IC × CPI

                           │
                           ▼

            이번 주 최중요 공식

      CPU Time = IC × CPI × Cycle Time

                    또는

      CPU Time = IC × CPI / Clock Rate

             ┌────────┬────────┬───────────┐
             │        │        │
             ▼        ▼        ▼
            IC       CPI    Cycle Time
           명령수    명령당    한 cycle
                    cycle      시간

        이 3개가 합쳐져 실제 CPU Time 결정

                           │
                           ▼

                [9. Average CPI]

Instruction마다 CPI가 다를 수 있음

ADD : CPI 1
LOAD: CPI 2
MUL : CPI 4

그래서 단순히 `(1 + 2 + 4) / 3`이 아니라 실행 빈도를 반영함.

Average CPI
= Σ(CPI_i × IC_i) / Total IC

→ 많이 실행되는 instruction이 더 중요함.

                           │
                           ▼

══════════════════════ MEASUREMENT ══════════════════════════

                   [10. Benchmark]

"그래서 실제 컴퓨터는 뭘 돌려서 비교하지?"

                   Benchmark
        = 성능 측정용 프로그램 / workload

예:
SPEC      → CPU
TPC       → Database
LINPACK   → Supercomputer
3DMark    → Graphics

                           │
                           ▼

                    [SPECratio]

       SPECratio = Reference Time / Measured Time

             높을수록 빠름

예:
Reference = 100 sec
Computer  = 50 sec

SPECratio = 100 / 50 = 2

→ reference보다 2배 빠름

                           │
                           ▼

                [11. Geometric Mean]

여러 benchmark ratio를 하나로 합칠 때

Arithmetic Mean보다 Geometric Mean을 사용

        (ratio_1 × ratio_2 × ... × ratio_n)^(1/n)

→ 상대적 performance ratio를 평균낼 때 사용

                           │
                           ▼

════════════════════ WHY NOT JUST GHz? ══════════════════════

                    [12. Power Wall]

예전:
transistor ↓
clock ↑
CPU 빨라짐 ↑

하지만...

Power ∝ C × V^2 × Frequency

Frequency ↑
     ↓
Power ↑
     ↓
Heat ↑
     ↓
Cooling / Reliability 문제

          → clock을 계속 올릴 수 없음

                           │
                           ▼

                [13. Multicore]

        "한 core를 계속 더 빠르게"
                    ↓ 한계

        "core를 여러 개 쓰자"

          Core 1      Core 2
             \         /
              \       /
              Parallel Work

하지만

cores ↑ ≠ 프로그램 자동으로 빨라짐

Software가 일을 병렬화해야 함.

Task division
Load balancing
Communication
Synchronization

                           │
                           ▼

                  GPU / Accelerator

GPU
= 많은 비슷한 계산을 동시에 처리

Accelerator
= 특정 workload에 특화

NPU
= Neural Processing Unit
= Neural Network 계산 특화

                           │
                           ▼

══════════════════ PARALLELISM HAS A LIMIT ══════════════════

                 [14. Amdahl's Law]

프로그램 전체를 빨리 만들 수 있는 게 아님.

Program
┌───────────────────────────────────┐
│ Parallelizable │ Serial Part      │
│ 80%            │ 20%              │
└───────────────────────────────────┘

80%를 아무리 빨리 만들어도
20%는 그대로 남음.

        New Time
        =
        Improved Part / Speedup
        +
        Unaffected Part

따라서 일부를 무한히 빠르게 해도 전체 speedup에는 한계가 있다.

                 ↓

       Make the Common Case Fast
```

## 이번 주 정중앙 공식

앞의 `Wafer / Die / Yield / Chiplet`은 CPU라는 물건을 현실적으로 어떻게 만드는지에 대한 배경이고, 그 이후부터 성능 분석 본체가 시작된다. 이번 주 정중앙은 아래 그림이다.

```text
               PERFORMANCE

                   │
                   ▼
          Execution Time
                   │
         Performance = 1/T
                   │
                   ▼
              CPU Time
                   │
          ┌────────┴────────┐
          ▼                 ▼
     Clock Cycles        Cycle Time
          │
          ▼
       IC × CPI
          │
          ▼

CPU Time = IC × CPI × Cycle Time
```

이 그림 하나가 이번 주의 중심이다. 자료에서도 CPU Time을 `IC × CPI × Clock Cycle Time`, 또는 `IC × CPI / Clock Rate`로 정리한다.

## 1주차와 연결

지난주에는 컴퓨터 시스템이 여러 계층으로 내려간다는 큰 그림을 봤다.

```text
C/C++
 ↓
Assembly
 ↓
Machine Code
 ↓
ISA
 ↓
CPU
```

이번 주에는 그 계층들을 성능 숫자로 연결한다.

```text
C/C++
 ↓
Compiler / ISA
 ↓
"몇 instruction 나오지?"
        ↓
       IC

CPU Microarchitecture
        ↓
"instruction 하나에 몇 cycle?"
        ↓
       CPI

Circuit / Clock
        ↓
"한 cycle이 몇 초?"
        ↓
   Cycle Time
```

결국 아래 식으로 모인다.

```text
                  IC
                   ×
                  CPI
                   ×
              Cycle Time
                   =
                CPU Time
```

즉 2주차는 1주차에서 배운 각 계층을 성능 숫자로 연결하는 주차다. ISA는 IC/CPI에 영향을 주고, processor와 memory system은 instruction 실행 속도에 영향을 주고, 반도체 공정과 clock은 cycle time과 power limit에 영향을 준다.

## 예습/복습용 8문장

간단한 영어 설명으로도 말할 수 있게 이 정도는 바로 튀어나오면 좋다.

```text
Performance
= how fast the computer finishes work.

Response time
= time for one task.

Throughput
= how many tasks finish in a given time.

Clock rate
= how many clock cycles happen per second.

IC
= how many instructions the program executes.

CPI
= average cycles needed for one instruction.

CPU Time
= IC × CPI × Cycle Time.

Amdahl's Law
= speeding up only one part has a limit
  because the unchanged part still remains.
```

마지막 큰 스토리는 이렇다.

```text
We want a faster CPU
        ↓
reduce IC / CPI / Cycle Time
        ↓
but increasing clock rate causes power and heat problems
        ↓
Power Wall
        ↓
use Multicore / GPU / Accelerator
        ↓
but not all programs can run in parallel
        ↓
Amdahl's Law
```

예습 우선순위는 `CPU Time 공식 > IC/CPI/Clock 이해 > Response Time vs Throughput > Amdahl > Yield/Chiplet > Benchmark` 순서로 잡으면 된다.

## 1. Semiconductor Technologies

2주차 PDF는 성능 공식부터 바로 시작하지 않고 반도체 기술 이야기로 들어간다. 이유는 간단하다. 컴퓨터구조의 성능은 추상적인 알고리즘이나 소프트웨어만으로 결정되지 않는다. 실제로 명령어를 실행하는 프로세서는 반도체 공정 위에 만들어지고, 회로를 한 칩 안에 얼마나 많이, 얼마나 안정적으로, 얼마나 싸게 넣을 수 있는지가 시스템 성능과 비용을 크게 바꾼다.

초기 컴퓨터 시스템 예시로 1980년대 IBM AT, 즉 흔히 말하는 286 보드가 나온다. 이 시기의 보드는 기능별 칩이 여러 개로 흩어져 있었다. CPU, 메모리 관련 로직, 입출력 제어 로직 같은 기능들이 한 칩 안에 다 들어가지 못하고 여러 칩 사이에 나뉘어 있었다. 당시에는 하나의 IC에 집적할 수 있는 트랜지스터 수가 지금보다 훨씬 적었기 때문이다.

이 구조의 단점은 네 가지로 정리된다.

- 칩과 칩 사이 통신이 느리다.
- 전력 소모가 크다.
- 여러 칩을 써야 하므로 비용이 높다.
- 부품 수가 많아질수록 시스템 신뢰성이 낮아진다.

여기서 중요한 포인트는 "칩 사이를 왔다 갔다 하는 비용"이다. 컴퓨터 내부에서 데이터가 이동하는 거리와 경로는 성능에 직접 영향을 준다. 같은 기능이 한 칩 안에 가까이 있으면 더 빠르고 전력도 덜 든다. 반대로 여러 칩 사이를 오가면 신호 전달 지연, 전력, 보드 복잡도, 고장 가능성이 모두 커진다.

이후 자료는 현대 시스템 예시로 Intel i7 Ivy Bridge SoC와 iPhone 13 Pro를 보여준다. SoC는 System-on-Chip의 의미로, 예전에는 여러 칩으로 나뉘어 있던 기능들이 하나의 칩 안에 많이 통합된 형태다. 스마트폰 SoC는 CPU 코어뿐 아니라 GPU, 메모리 컨트롤러, 영상 처리, AI 가속기, 통신 관련 블록 등 다양한 기능을 한 칩 또는 매우 가까운 패키지 안에 넣는다.

따라서 이 파트의 핵심은 "현대 컴퓨터구조는 반도체 집적 기술의 발전과 함께 칩 내부로 기능을 모아 성능, 전력, 비용, 신뢰성을 개선해왔다"는 것이다. 이것은 뒤에서 나오는 power wall, multicore, accelerator 이야기와도 연결된다.

## 2. Integrated Circuit Manufacturing

다음 파트는 IC 제조 과정이다. PDF는 웨이퍼와 다이, 그리고 수율을 설명한다. 여기서 용어가 헷갈리기 쉬우므로 먼저 구분한다.

- Wafer: 반도체 칩을 만들기 위한 둥근 원판이다.
- Die: 웨이퍼 위에 반복적으로 찍혀 있는 개별 칩 하나의 단위다.
- Defect: 제조 과정에서 생기는 결함이다.
- Yield: 만들어진 die 중 정상 작동하는 die의 비율이다.

완제품 칩 하나의 비용을 생각할 때 단순히 "웨이퍼 하나가 얼마인가"만 보면 안 된다. 웨이퍼에서 몇 개의 die를 얻을 수 있는지, 그중 몇 개가 정상 작동하는지가 같이 들어간다. PDF의 핵심 공식은 다음이다.

```text
Cost per die = Cost per wafer / (Dies per wafer × Yield)
```

즉 칩 하나의 비용은 웨이퍼 비용을 정상 작동하는 die 개수로 나눈 값이다. 웨이퍼 비용이 같아도 die가 작으면 한 웨이퍼에서 더 많은 칩을 얻을 수 있고, 결함에 영향을 덜 받아 yield가 높아진다. 반대로 die가 커지면 한 웨이퍼에서 얻는 개수가 줄고, 하나의 die 안에 결함이 들어갈 확률도 높아져 yield가 낮아진다.

Dies per wafer는 대략 다음처럼 생각한다.

```text
Dies per wafer ≈ Wafer area / Die area
```

정확한 값은 원형 웨이퍼와 사각형 die의 가장자리 손실 때문에 더 복잡하지만, 시험/복습에서는 die area가 커질수록 dies per wafer가 줄어든다는 직관이 중요하다.

Yield는 PDF에서 다음 꼴로 나온다.

```text
Yield = 1 / (1 + (Defects per area × Die area))^N
```

여기서 `Defects per area × Die area`는 해당 die 안에 결함이 들어갈 가능성과 연결된다. die area가 커질수록 결함을 포함할 확률이 커지고, yield는 비선형적으로 떨어진다. 그래서 die 면적이 조금 커졌다고 비용이 조금만 오르는 것이 아니라, 정상 칩 수가 빠르게 줄면서 cost per die가 크게 증가할 수 있다.

이 부분의 진짜 메시지는 "큰 칩은 비싸다"보다 더 구체적이다. 큰 칩은 한 웨이퍼에서 적게 나오고, 결함을 맞을 확률도 높기 때문에, 비용이 비선형적으로 증가한다. 그래서 현대 고성능 칩 설계에서는 단순히 하나의 거대한 die를 만드는 대신 chiplet 구조가 중요해졌다.

## 3. Die Size와 Chiplet

PDF는 NVIDIA GeForce RTX 3050과 NVIDIA Tesla A100을 예시로 든다. RTX 3050은 276 mm^2, Samsung 8nm 공정, 가격은 약 250달러 수준으로 제시된다. Tesla A100은 826 mm^2, TSMC 7nm 공정, 가격은 약 10,000달러 수준으로 제시된다.

이 숫자들을 외우는 것보다 중요한 것은 크기와 비용의 관계다. A100은 단순히 면적이 세 배 정도 크니 가격도 세 배라는 식으로 가지 않는다. 고성능 데이터센터 GPU는 더 큰 die, 더 어려운 공정, 더 낮은 yield, 고급 패키징, HBM 같은 메모리 구성, 서버 시장 가격 구조 등이 얹히면서 일반 소비자 GPU와 완전히 다른 비용 구조를 갖는다.

여기서 chiplet이 등장한다. Chiplet은 큰 칩 하나를 통째로 만드는 대신, 여러 개의 작은 칩 조각을 패키지 안에서 연결하는 방식이다. 작은 die는 yield가 높고 제조가 상대적으로 쉽다. 여러 chiplet을 고속 인터커넥트로 연결하면, 하나의 거대한 단일 die를 만드는 것보다 비용과 수율 면에서 유리할 수 있다.

PDF의 예시로 AMD Ryzen Threadripper 64-core CPU가 나온다. 이 제품군은 여러 chiplet을 묶어 많은 코어를 제공하는 방향을 보여준다. 컴퓨터구조 관점에서 chiplet은 성능만의 문제가 아니라 제조 비용, 수율, 확장성, 패키징 기술의 절충이다.

복습할 때는 다음 질문에 답할 수 있어야 한다.

- 왜 die가 커지면 cost per die가 비선형적으로 증가하는가?
- yield는 무엇이며 defect와 die area가 왜 중요한가?
- chiplet은 왜 등장했는가?
- 예전 다중 칩 보드와 현대 chiplet은 둘 다 여러 칩을 쓰지만 무엇이 다른가?

마지막 질문이 꽤 중요하다. 예전 보드는 기능이 분산되어 느리고 비효율적인 한계 때문에 여러 칩을 썼다. 현대 chiplet은 한계 때문에 쪼개는 면도 있지만, 고급 패키징과 빠른 연결 기술을 이용해 비용과 수율을 최적화하려는 전략적 분할에 가깝다.

## 4. Performance의 시작

15페이지부터 본격적으로 Performance 파트가 시작된다. 컴퓨터구조에서 성능은 그냥 "빠르다"가 아니다. 어떤 관점에서 빠른지 먼저 정해야 한다. 같은 시스템도 개인 사용자에게는 빠르게 느껴질 수 있지만 서버 운영자에게는 부족할 수 있고, 반대로 처리량은 좋은데 개별 요청 응답이 느릴 수도 있다.

PDF는 항공사 예시를 통해 이 차이를 보여준다. 항공편 하나가 목적지까지 걸리는 시간은 response time에 가깝고, 하루에 얼마나 많은 승객이나 항공편을 처리할 수 있는지는 throughput에 가깝다. 컴퓨터 시스템도 마찬가지다.

```text
Response Time = Latency
Throughput = Bandwidth or number of tasks per time
```

Response time은 작업이 시작된 시점부터 완료될 때까지 걸리는 시간이다. 사용자가 프로그램을 실행하고 결과를 기다릴 때, 웹 페이지가 열리길 기다릴 때, 컴파일이 끝나길 기다릴 때 중요한 지표다. 개인용 컴퓨터, 임베디드 시스템, 실시간성에 민감한 시스템은 response time이 중요하다.

Throughput은 일정 시간 안에 완료한 작업의 총량이다. 서버, 데이터센터, 배치 처리 시스템, 다수 요청을 처리하는 웹 서비스에서는 throughput이 중요하다. 예를 들어 요청 하나의 응답 시간이 조금 느려도, 동시에 매우 많은 요청을 안정적으로 처리하면 throughput은 높다고 볼 수 있다.

시험에서 자주 헷갈릴 수 있는 포인트는 둘의 관계다. 어떤 개선은 response time과 throughput을 둘 다 좋게 만들 수 있다. 예를 들어 CPU가 빨라지면 개별 작업도 빨라지고 단위 시간당 처리 작업 수도 늘 수 있다. 하지만 항상 같은 방향은 아니다. 병렬 서버를 늘리면 throughput은 올라가지만 개별 작업의 response time이 반드시 줄지는 않을 수 있다. 반대로 캐시 최적화는 특정 작업의 response time을 줄이는 데 강하지만 전체 시스템 throughput에는 제한적일 수 있다.

## 5. Relative Performance

성능을 정량화하기 위해 PDF는 다음 정의를 둔다.

```text
Performance = 1 / Execution Time
```

실행 시간이 짧을수록 성능이 크다. 그래서 "X가 Y보다 n배 빠르다"는 말은 다음처럼 쓴다.

```text
n = Performance_X / Performance_Y
  = ExecutionTime_Y / ExecutionTime_X
```

예시에서 컴퓨터 A가 어떤 프로그램을 10초에 실행하고, 컴퓨터 B가 같은 프로그램을 15초에 실행한다면 다음과 같다.

```text
Performance_A / Performance_B = ExecutionTime_B / ExecutionTime_A = 15 / 10 = 1.5
```

따라서 A는 B보다 1.5배 빠르다. 여기서 분자와 분모를 반대로 놓으면 해석이 완전히 뒤집히므로 조심해야 한다. 성능 비교는 항상 "비교하려는 성능의 비"를 먼저 쓰고, 성능은 실행 시간의 역수이므로 실행 시간은 반대로 들어간다.

복습용으로는 다음처럼 외우면 된다.

```text
A가 B보다 몇 배 빠른가?
= B의 실행 시간 / A의 실행 시간
```

A가 더 빠르면 A의 실행 시간이 더 작으므로 결과가 1보다 커진다. 만약 값이 1보다 작게 나오면 질문의 방향이나 분자/분모를 다시 확인해야 한다.

## 6. What Affects Performance

PDF는 성능에 영향을 주는 계층을 나눠서 설명한다. 이 페이지가 좋은 이유는 컴퓨터구조가 어디에 끼어 있는지 보여주기 때문이다.

- Algorithm: 수행해야 하는 계산량과 메모리 접근량을 결정한다.
- Programming Language, Compiler, ISA: 프로그램이 몇 개의 명령어로 번역되는지에 영향을 준다.
- Processor and Memory System: 각 명령어가 얼마나 빠르게 실행되는지에 영향을 준다.
- I/O System: 입출력 작업의 속도에 영향을 준다.

즉 같은 문제라도 알고리즘이 나쁘면 계산량이 많아진다. 같은 알고리즘이라도 프로그래밍 언어와 컴파일러, ISA에 따라 생성되는 instruction count가 달라진다. 같은 명령어 수라도 프로세서 구조, 파이프라인, 캐시, 메모리 시스템에 따라 각 명령어의 실행 비용이 달라진다.

컴퓨터구조 수업에서 특히 중요한 것은 ISA, processor, memory system 쪽이다. 알고리즘 수업이 "작업량 자체를 어떻게 줄일까"에 집중한다면, 컴퓨터구조는 "주어진 프로그램을 하드웨어가 어떤 명령어와 사이클로 실행하는가"를 본다. 그래서 뒤에서 CPU Time 공식을 instruction count, CPI, clock rate로 쪼갠다.

이 관점은 시프, 운체와도 연결된다. 시스템프로그램은 C, assembly, linker, memory layout처럼 프로그램이 기계 수준으로 내려가는 길을 본다. 운영체제는 process, virtual memory, file system처럼 하드웨어 자원을 추상화하고 보호하는 계층을 본다. 컴퓨터구조는 그 아래에서 instruction, datapath, control, memory hierarchy가 실제 성능을 어떻게 만드는지 본다.

## 7. CPU Time 공식

CPU 성능 분석의 중심 공식은 다음이다.

```text
CPU Time = CPU Clock Cycles × Clock Cycle Time
CPU Time = CPU Clock Cycles / Clock Rate
```

CPU Clock Cycles는 프로그램을 실행하는 데 필요한 클럭 사이클 수다. Clock Cycle Time은 한 사이클에 걸리는 시간이다. Clock Rate는 1초에 몇 사이클이 도는지, 즉 주파수다. Clock Cycle Time과 Clock Rate는 역수 관계다.

```text
Clock Cycle Time = 1 / Clock Rate
Clock Rate = 1 / Clock Cycle Time
```

CPU Time을 줄이려면 두 방향이 있다.

- 필요한 clock cycles를 줄인다.
- clock rate를 높인다.

하지만 둘은 독립적으로 마음대로 줄이고 늘릴 수 없다. clock rate를 높이는 설계는 한 사이클 안에 할 수 있는 일을 줄이거나 파이프라인을 깊게 만들 수 있고, 그 결과 전체 clock cycles가 늘어날 수 있다. 반대로 한 사이클에 많은 일을 하게 만들면 cycle time이 길어져 clock rate가 낮아질 수 있다. 그래서 컴퓨터구조는 항상 trade-off를 본다.

여기서 "클럭이 높은 CPU가 무조건 빠르다"가 틀린 말이 된다. 클럭이 높아도 같은 프로그램을 실행하는 데 필요한 사이클 수가 더 많거나, CPI가 더 높거나, 메모리 병목이 심하면 실제 실행 시간은 나빠질 수 있다.

## 8. CPU Clocking

디지털 회로는 clock signal에 맞춰 상태를 바꾼다. 자료에서는 clock이 1에서 0으로, 다시 1로 돌아오는 반복 신호로 표현된다. Clock period는 한 번의 주기가 걸리는 시간이고, clock frequency 또는 clock rate는 1초에 몇 번의 주기가 반복되는지를 뜻한다.

예시로 다음 숫자가 나온다.

```text
Clock period = 250 ps = 0.25 ns = 250 × 10^-12 seconds
Clock rate = 4.0 GHz = 4.0 × 10^9 Hz
```

둘은 서로 맞물린다. 4GHz는 1초에 40억 사이클이므로 한 사이클 시간은 1 / 4GHz = 0.25ns = 250ps다.

PDF는 `A × B + C` 계산이 설계에 따라 1 cycle 또는 2 cycles가 될 수 있음을 보여준다. 이것은 같은 계산이라도 하드웨어 설계 방식에 따라 clock cycle count가 달라질 수 있다는 뜻이다. 한 사이클 안에 곱셈과 덧셈을 모두 처리하려면 cycle time이 길어질 수 있고, 두 사이클로 나누면 cycle time을 짧게 잡을 수 있다. 결국 CPU Time은 cycle count와 cycle time의 곱으로 봐야 한다.

## 9. CPU Time Example

PDF의 예제는 공식 사용법을 확인하기 좋다.

조건은 다음이다.

```text
Computer A: clock rate = 2 GHz, CPU time = 10 seconds
Computer B: A보다 더 빠른 clock frequency로 설계
Computer B: clock cycles는 A보다 1.2배 증가
목표: Computer B의 CPU time = 6 seconds
질문: Computer B의 clock rate는 얼마여야 하는가?
```

먼저 A가 사용한 clock cycles를 구한다.

```text
CPU Time = Clock Cycles / Clock Rate
Clock Cycles = CPU Time × Clock Rate
Clock Cycles_A = 10 sec × 2 GHz
               = 10 × 2 × 10^9
               = 20 × 10^9 cycles
```

B는 cycle count가 1.2배 증가한다.

```text
Clock Cycles_B = 1.2 × 20 × 10^9
               = 24 × 10^9 cycles
```

B가 이 프로그램을 6초에 끝내려면 필요한 clock rate는 다음이다.

```text
Clock Rate_B = Clock Cycles_B / CPU Time_B
             = 24 × 10^9 / 6
             = 4 × 10^9 Hz
             = 4 GHz
```

따라서 B는 4GHz가 필요하다. 이 예제의 포인트는 clock rate를 2GHz에서 4GHz로 두 배 올려도 실행 시간이 10초에서 5초가 되는 것이 아니라는 점이다. cycle count가 1.2배 늘어났기 때문에 목표 6초를 맞추려면 4GHz가 필요하다. 성능 개선을 계산할 때는 clock rate만 보면 안 되고, cycles 변화까지 같이 봐야 한다.

## 10. Instruction Count와 CPI

다음 단계는 clock cycles를 더 세밀하게 쪼개는 것이다. PDF는 Instruction Count와 CPI를 도입한다.

```text
Instruction Count, IC = 프로그램이 실행한 명령어 수
Cycles Per Instruction, CPI = 명령어 하나당 평균 clock cycle 수
```

둘을 이용하면 다음 공식이 된다.

```text
Clock Cycles = Instruction Count × CPI
CPU Time = Instruction Count × CPI × Clock Cycle Time
CPU Time = Instruction Count × CPI / Clock Rate
```

이 공식이 컴퓨터구조 성능 분석의 핵심이다.

```text
CPU Time = IC × CPI × Clock Cycle Time
```

또는

```text
CPU Time = IC × CPI / Clock Rate
```

각 항이 의미하는 바는 다르다.

- IC는 주로 알고리즘, 프로그래밍 언어, 컴파일러, ISA의 영향을 받는다.
- CPI는 프로세서 구현, 명령어 종류, 파이프라인, 메모리 접근, 캐시 등의 영향을 받는다.
- Clock Cycle Time 또는 Clock Rate는 회로 설계와 공정 기술의 영향을 받는다.

성능을 개선한다는 것은 결국 IC, CPI, cycle time 중 하나 이상을 줄이는 것이다. 하지만 하나를 줄이면 다른 하나가 늘 수 있다. 예를 들어 복잡한 명령어를 제공하는 ISA는 instruction count를 줄일 수 있지만, 해당 명령어의 CPI나 cycle time을 증가시킬 수 있다. 반대로 단순한 명령어는 CPI나 clock 설계에 유리하지만 instruction count가 늘 수 있다.

이것이 RISC와 CISC, compiler optimization, pipeline 설계 등을 배울 때 계속 반복되는 관점이다. 2주차에서는 아직 깊게 들어가지는 않지만, 앞으로의 모든 성능 논의는 이 공식으로 돌아온다고 보면 된다.

## 11. CPI Example 1

PDF 예제에서 컴퓨터 A와 B는 같은 ISA를 사용하므로 같은 프로그램에 대해 instruction count가 같다고 가정한다.

```text
Computer A: cycle time = 250 ps, average CPI = 2.0
Computer B: cycle time = 500 ps, average CPI = 1.2
```

Instruction count를 `I`라고 두면 다음과 같다.

```text
CPU Time_A = I × 2.0 × 250 ps = 500 ps × I
CPU Time_B = I × 1.2 × 500 ps = 600 ps × I
```

따라서 A가 더 빠르다.

```text
Performance_A / Performance_B = CPU Time_B / CPU Time_A
                              = 600 / 500
                              = 1.2
```

즉 A는 B보다 1.2배 빠르다. 이 예제는 clock cycle time만 보면 B가 느리지만 CPI가 낮아서 어느 정도 만회한다는 구조다. 그러나 최종 곱을 비교하면 A가 더 빠르다. 성능 비교는 항상 각 항을 따로 보고 끝내지 말고 최종 CPU time을 계산해야 한다.

## 12. CPI in More Detail

실제 CPU에서 모든 명령어가 같은 cycle 수를 쓰지는 않는다. 단순한 정수 연산은 적은 cycle로 끝날 수 있고, 메모리 접근이나 곱셈 같은 명령어는 더 많은 cycle이 필요할 수 있다. 그래서 평균 CPI를 계산할 때는 명령어 종류별 CPI와 개수를 가중합해야 한다.

공식은 다음이다.

```text
Clock Cycles = Σ (CPI_i × Instruction Count_i)
Average CPI = Clock Cycles / Total Instruction Count
```

즉 평균 CPI는 단순히 각 명령어 종류 CPI의 산술 평균이 아니다. 프로그램에서 많이 실행되는 명령어가 더 큰 영향을 준다. 이것은 뒤의 Amdahl's Law와도 연결된다. 자주 실행되는 부분을 빠르게 만드는 것이 전체 성능에 더 중요하다.

PDF의 instruction type 예제는 세 종류의 명령어가 있다고 둔다.

```text
Type A CPI = 1
Type B CPI = 2
Type C CPI = 3
```

Sequence 1의 총 instruction count는 5이고, clock cycles는 10으로 계산된다.

```text
Clock Cycles_1 = 2 × 1 + 1 × 2 + 2 × 3 = 10
Average CPI_1 = 10 / 5 = 2.0
```

Sequence 2의 총 instruction count는 6이고, clock cycles는 9로 계산된다.

```text
Clock Cycles_2 = 4 × 1 + 1 × 2 + 1 × 3 = 9
Average CPI_2 = 9 / 6 = 1.5
```

여기서 흥미로운 점은 Sequence 2가 명령어 수는 더 많지만 평균 CPI가 낮고 총 cycles도 더 적다는 것이다. 그래서 instruction count만으로 성능을 판단하면 틀릴 수 있다. 반대로 CPI만 봐도 안 된다. 총 실행 시간은 IC, CPI, clock cycle time의 곱이다.

## 13. Performance Summary

PDF는 큰 그림 공식을 다음처럼 정리한다.

```text
CPU Time = Seconds / Program
         = Instructions / Program
         × Clock Cycles / Instruction
         × Seconds / Clock Cycle
```

이 공식은 단위가 깔끔하게 소거된다.

```text
Instructions / Program × Cycles / Instruction = Cycles / Program
Cycles / Program × Seconds / Cycle = Seconds / Program
```

단위로 보면 왜 공식이 맞는지 이해하기 쉽다. 시험에서 공식을 외웠는데 헷갈리면 단위를 써보면 된다.

이 공식은 컴퓨터구조의 세 가지 성능 손잡이를 보여준다.

- Instructions per program: 프로그램이 실행하는 명령어 수
- Clock cycles per instruction: 명령어 하나가 평균적으로 쓰는 사이클 수
- Seconds per clock cycle: 한 사이클의 실제 시간

컴파일러는 instruction count를 줄일 수 있고, 프로세서 구조는 CPI를 줄일 수 있고, 회로/공정은 cycle time을 줄일 수 있다. 하지만 어느 한 항만 최적화하면 전체가 자동으로 좋아지는 것은 아니다. 앞으로 배우는 pipeline, cache, memory hierarchy, parallelism은 대부분 이 세 항 중 하나 이상을 건드린다.

## 14. Benchmarks와 SPEC

성능을 계산하는 공식이 있어도, 실제 컴퓨터를 비교하려면 어떤 프로그램을 돌릴지 정해야 한다. 그래서 benchmark가 필요하다. Benchmark는 성능 측정을 위해 정해진 프로그램 모음이다. 현실적인 workload를 대표하도록 만들어진 프로그램을 실행하고 시간을 측정한다.

PDF에서는 SPEC CPU benchmark가 나온다. SPEC은 CPU, I/O, Web 등 여러 분야의 벤치마크를 개발하는 단체다. SPEC CPU2006은 프로그램 실행의 elapsed time을 측정하고, 기준 시스템의 시간과 비교해 SPECratio를 계산한다.

```text
SPECratio = Reference Time / Measured Elapsed Time
```

Measured elapsed time이 작을수록 SPECratio는 커진다. 즉 SPECratio가 높을수록 기준 시스템보다 빠르다는 뜻이다.

예를 들어 reference time이 100초이고 어떤 컴퓨터의 measured time이 50초라면 SPECratio는 2다. 기준 시스템보다 두 배 빠르다고 볼 수 있다. measured time이 200초라면 SPECratio는 0.5다. 기준 시스템보다 느린 것이다.

PDF에는 다른 benchmark suite도 나온다.

- EEMBC: embedded benchmark
- TPC: database transaction processing
- LINPACK: supercomputer 성능 측정에 많이 쓰이는 선형대수 benchmark
- 3DMark: graphics benchmark
- Geekbench: cross-platform benchmark

중요한 점은 벤치마크는 목적에 맞게 골라야 한다는 것이다. CPU 정수 연산 성능이 궁금한데 그래픽 벤치마크만 보면 부정확하다. 데이터베이스 서버 성능을 보려면 TPC 같은 트랜잭션 중심 benchmark가 더 관련 있다.

## 15. Performance Ratios와 Geometric Mean

여러 benchmark 프로그램의 결과를 하나로 합치려면 평균을 내야 한다. 그런데 성능 ratio의 평균에는 산술평균이 잘 맞지 않는다. 자료는 Computer 1과 Computer 2의 예시를 통해 산술평균이 기준 선택에 따라 해석을 흔들 수 있음을 보여준다.

성능 ratio를 합칠 때는 geometric mean을 쓴다.

```text
Geometric Mean = (Π SPECratio_i)^(1/n)
```

즉 모든 ratio를 곱한 뒤 n제곱근을 취한다. 기하평균의 장점은 상대적 비율을 다룰 때 기준 시스템이 바뀌어도 비교 관계가 안정적으로 유지된다는 점이다.

PDF의 예시를 정리하면 다음과 같다.

```text
Computer 1 ratios: 1, 2, 4
Geometric mean = (1 × 2 × 4)^(1/3) = 8^(1/3) = 2

Computer 2 ratios: 2, 8, 4
Geometric mean = (2 × 8 × 4)^(1/3) = 64^(1/3) = 4
```

따라서 Computer 2가 Computer 1보다 2배 빠르다.

Reference가 바뀐 예시에서도 결과는 유지된다.

```text
Computer 1 ratios: 2, 5, 1/3
Geometric mean = (10/3)^(1/3) ≈ 1.49

Computer 2 ratios: 4, 20, 1/3
Geometric mean = (80/3)^(1/3) ≈ 2.98
```

여기서도 Computer 2는 Computer 1보다 약 2배 빠르다. 이것이 성능 ratio를 합칠 때 geometric mean을 쓰는 이유다.

시험에서 물을 수 있는 포인트는 다음이다.

- SPECratio가 무엇인지 설명할 수 있는가?
- ratio가 높으면 좋은지 낮으면 좋은지 아는가?
- 여러 ratio를 합칠 때 왜 arithmetic mean이 아니라 geometric mean을 쓰는가?
- geometric mean 계산 방법을 아는가?

## 16. SPEC CPU 예시와 벤치마크 해석

PDF 후반에는 Intel Core i7 920 기반 SPEC CPU 결과, SPEC CPU 2017, benchmark application 예시가 나온다. 여기서 모든 세부 benchmark 이름을 외울 필요는 없다. 대신 다음 정도가 중요하다.

SPEC CPU는 실제 응용 프로그램에 가까운 workload를 모아 CPU 성능을 측정하려는 시도다. 단순한 toy program 하나만 돌리는 것이 아니라, 정수 연산, 부동소수점 연산, 압축, 컴파일, 시뮬레이션 등 다양한 작업을 포함한다. 그래서 한 프로그램에서만 빠른 컴퓨터가 전체적으로 빠른지 확인할 수 있다.

하지만 benchmark도 완벽하지 않다. 어떤 프로그램을 선택하느냐, 컴파일러 옵션을 어떻게 주느냐, 메모리 시스템이 workload와 얼마나 잘 맞느냐에 따라 결과가 달라진다. 따라서 benchmark 숫자는 성능 논의의 출발점이지, 모든 상황의 절대 진리는 아니다.

컴퓨터구조 수업에서 benchmark를 배우는 이유는 하드웨어 설계를 평가하려면 대표 workload가 필요하기 때문이다. 새로운 cache 구조, branch predictor, pipeline 설계, multicore 구조를 만들었을 때 실제로 빨라졌는지 확인하려면 벤치마크를 돌려야 한다. 그리고 여러 프로그램 결과를 합칠 때는 ratio와 geometric mean을 조심해서 다뤄야 한다.

## 17. Uniprocessor Performance와 Power Wall

자료는 이후 uniprocessor performance 흐름을 보여준다. 과거에는 단일 프로세서 성능이 빠르게 증가했다. 공정이 좋아지고, 트랜지스터가 작아지고, 클럭을 높이고, 더 깊은 파이프라인과 더 복잡한 마이크로아키텍처를 넣으면서 단일 코어 성능을 많이 끌어올릴 수 있었다.

하지만 power consumption trend가 한계로 등장한다. CMOS IC technology에서 전력은 대략 다음 관계를 가진다.

```text
Power ∝ Capacitive Load × Voltage^2 × Frequency
```

전압은 제곱으로 들어간다. 그래서 전압을 낮추면 전력 감소 효과가 크다. 과거에는 전압을 낮추면서 frequency를 크게 올릴 수 있었다. 하지만 전압은 이미 약 1V 근처까지 내려와 더 낮추기 어려워졌고, frequency를 계속 올리면 전력이 커지고 열 문제가 심해진다.

Power Wall의 핵심은 다음이다.

- 높은 전력은 높은 열을 만든다.
- 높은 열은 칩 안정성과 수명, 냉각 비용 문제를 만든다.
- 전압은 이미 많이 낮아져 더 줄이기 어렵다.
- frequency를 낮추면 성능이 떨어진다.
- 트랜지스터 scaling도 물리적, 경제적 한계에 부딪힌다.

그래서 단순히 clock rate를 계속 올려서 성능을 높이는 방식은 한계에 닿았다. 이 지점이 multicore와 parallelism으로 넘어가는 배경이다.

## 18. Multiprocessors, GPU, Accelerator

Power wall 이후 컴퓨터 시스템은 단일 코어 frequency를 무한히 올리는 대신 여러 코어를 한 칩에 넣는 방향으로 발전했다. Multicore microprocessor는 하나의 칩에 둘 이상의 processor core가 들어 있는 구조다.

하지만 코어 수를 늘린다고 프로그램이 자동으로 빨라지는 것은 아니다. 병렬 프로그래밍이 필요하다. PDF는 병렬 프로그램에서 필요한 것들을 정리한다.

- 계산을 독립적인 task로 나눈다.
- 여러 코어에 작업을 적절히 분배한다.
- load balancing을 맞춘다.
- task 사이 통신과 synchronization을 최적화한다.

이 내용은 운영체제의 process/thread, concurrency, lock/semaphore와도 이어진다. 하드웨어가 여러 코어를 제공해도 소프트웨어가 그 병렬성을 잘 사용하지 못하면 성능 향상은 제한된다.

GPU는 병렬 계산에 최적화된 프로세서다. CPU가 복잡한 제어 흐름과 다양한 작업을 유연하게 처리하는 데 강하다면, GPU는 많은 데이터를 비슷한 연산으로 동시에 처리하는 데 강하다. 그래픽 처리뿐 아니라 머신러닝, 과학 계산, 행렬 연산에서 GPU가 중요한 이유다.

자료는 domain-specific accelerators와 Neural Processing Units도 언급한다. Accelerator는 특정 종류의 연산을 매우 빠르고 효율적으로 처리하도록 설계된 하드웨어다. NPU는 신경망 연산에 특화된 가속기다. 범용 CPU보다 유연성은 낮을 수 있지만, 특정 workload에서는 성능과 전력 효율이 훨씬 좋다.

큰 흐름은 이렇게 잡으면 된다.

```text
Clock frequency만 올리기 어려움
        ↓
여러 core를 사용
        ↓
소프트웨어 병렬화 필요
        ↓
GPU처럼 많은 연산을 병렬 처리
        ↓
특정 workload는 accelerator로 더 효율화
```

## 19. Amdahl's Law

PDF 마지막 핵심은 Amdahl's Law다. 병렬화나 특정 부분 최적화가 전체 성능을 얼마나 개선할 수 있는지 알려주는 법칙이다.

공식은 다음이다.

```text
T_improved = T_affected / Improvement Factor + T_unaffected
```

어떤 부분을 n배 빠르게 만들면, 그 부분의 시간은 n으로 나뉜다. 하지만 개선하지 않은 부분의 시간은 그대로 남는다. 전체 시간은 개선된 부분과 개선되지 않은 부분의 합이다.

이 법칙의 결론은 자료에 나오는 문장처럼 "make the common case fast"다. 자주 실행되는 부분, 전체 시간에서 큰 비중을 차지하는 부분을 빠르게 만들어야 전체 성능이 의미 있게 오른다.

PDF 예시 1은 다음과 같다.

```text
Add: 30 seconds
Sub: 20 seconds
Mul: 25 seconds
Div: 25 seconds
Total: 100 seconds
Add를 10배 빠르게 개선
```

Add 부분은 30초에서 3초가 된다.

```text
T_improved = 30 / 10 + 70
           = 3 + 70
           = 73 seconds
```

전체 시간은 100초에서 73초로 줄어든다. Add를 10배나 빠르게 만들었지만 전체 개선은 27% 정도다. 왜냐하면 나머지 70초는 그대로 남아 있기 때문이다.

예시 2는 더 강한 한계를 보여준다.

```text
Multiply operations = 전체 시간의 80%
Original execution time = 100 seconds
Multiply time = 80 seconds
Other time = 20 seconds
목표 전체 speedup = 5x
목표 실행 시간 = 20 seconds
```

Multiply를 n배 빠르게 한다고 하면 다음이다.

```text
T_improved = 80 / n + 20
```

5배 speedup을 하려면 전체 시간이 20초가 되어야 한다.

```text
80 / n + 20 = 20
80 / n = 0
```

이것은 n이 무한대여야 가능하다. 즉 multiply 부분을 아무리 빠르게 만들어도, 개선되지 않는 20초가 남아 있기 때문에 전체 실행 시간을 20초보다 작게 만들 수 없다. 목표 5배 speedup은 불가능하다.

이 예시는 Amdahl's Law의 핵심을 잘 보여준다. 최적화 대상이 전체 시간에서 큰 비중을 차지하더라도, 개선되지 않는 부분이 남아 있으면 전체 speedup에는 상한이 생긴다. 병렬화도 마찬가지다. 프로그램의 80%만 병렬화 가능하고 20%가 반드시 순차 실행되어야 한다면, 무한히 많은 프로세서를 써도 전체 speedup은 5배를 넘을 수 없다.

## 20. 공식 모음

| 개념 | 공식 | 의미 |
| --- | --- | --- |
| Performance | `1 / Execution Time` | 실행 시간이 짧을수록 성능이 높음 |
| Relative Performance | `Perf_X / Perf_Y = Time_Y / Time_X` | X가 Y보다 몇 배 빠른지 |
| CPU Time | `Clock Cycles × Clock Cycle Time` | 총 사이클 수와 한 사이클 시간의 곱 |
| CPU Time | `Clock Cycles / Clock Rate` | clock rate 기준 표현 |
| Clock Cycle Time | `1 / Clock Rate` | 한 사이클에 걸리는 시간 |
| Clock Rate | `1 / Clock Cycle Time` | 초당 사이클 수 |
| Clock Cycles | `Instruction Count × CPI` | 명령어 수와 명령어당 사이클 수의 곱 |
| CPU Time | `IC × CPI × Clock Cycle Time` | 컴퓨터구조 성능 공식의 핵심 |
| CPU Time | `IC × CPI / Clock Rate` | clock rate 기준 성능 공식 |
| Weighted CPI | `Σ(CPI_i × IC_i) / IC_total` | 명령어 종류별 실행 비중 반영 |
| Cost per die | `Cost per wafer / (Dies per wafer × Yield)` | 정상 작동 칩 하나의 비용 |
| Dies per wafer | `Wafer area / Die area` 근사 | die가 작을수록 한 웨이퍼에서 많이 나옴 |
| SPECratio | `Reference Time / Measured Time` | 높을수록 기준 시스템보다 빠름 |
| Geometric Mean | `(Π ratio_i)^(1/n)` | 여러 성능 ratio를 합칠 때 사용 |
| Amdahl's Law | `T_affected / factor + T_unaffected` | 부분 최적화 후 전체 실행 시간 |

## 21. 헷갈릴 포인트 정리

첫째, response time과 throughput은 같은 말이 아니다. Response time은 하나의 작업이 끝나는 데 걸리는 시간이고, throughput은 일정 시간 동안 끝낸 작업 수다. 한쪽이 좋아지면 다른 쪽도 좋아질 수 있지만, 항상 자동으로 같이 좋아지는 것은 아니다.

둘째, performance와 execution time은 역수 관계다. "A가 B보다 몇 배 빠른가"를 묻는 문제에서는 `Time_B / Time_A`를 계산한다. A가 빠르면 A 시간이 작기 때문에 결과가 1보다 커진다.

셋째, clock rate만 보면 안 된다. CPU time은 cycles를 clock rate로 나눈 값이다. clock rate가 높아져도 cycles가 많이 늘면 실제 실행 시간 개선은 작을 수 있다.

넷째, instruction count만 봐도 안 된다. 명령어 수가 적어도 CPI가 높으면 느릴 수 있고, 명령어 수가 많아도 CPI가 낮으면 더 빠를 수 있다.

다섯째, average CPI는 명령어 종류별 CPI의 단순 평균이 아니다. 실행된 명령어 개수로 가중평균을 내야 한다.

여섯째, SPECratio는 measured time이 아니라 reference time을 measured time으로 나눈 비율이다. 숫자가 클수록 빠르다.

일곱째, 여러 benchmark ratio를 합칠 때는 geometric mean을 쓴다. 산술평균은 기준에 따라 비교가 이상해질 수 있다.

여덟째, Amdahl's Law는 "일부를 아무리 빠르게 해도 나머지가 남으면 전체 speedup은 제한된다"는 뜻이다. 그래서 common case를 빠르게 만드는 것이 중요하다.

아홉째, power wall은 단순히 전기를 많이 먹는 문제가 아니다. 열, 안정성, 냉각, 전압 scaling 한계, frequency scaling 한계가 모두 연결된 구조적 한계다.

열째, multicore는 하드웨어만의 해결책이 아니다. 병렬 프로그램 작성, task 분할, load balancing, synchronization이 같이 필요하다.

## 22. 1주차와 연결

1주차에서 컴퓨터구조는 하드웨어와 소프트웨어 사이의 interface, abstraction, technology trend를 다뤘다. 2주차는 그 추상화된 시스템을 실제로 평가하는 방법을 배운다.

1주차에서 ISA가 중요하다고 했다면, 2주차에서는 ISA가 instruction count와 CPI에 영향을 준다는 식으로 연결된다. 1주차에서 technology가 중요하다고 했다면, 2주차에서는 공정, die size, yield, power wall이 성능과 비용의 현실적 제약으로 등장한다. 1주차에서 computer system은 계층적으로 구성된다고 했다면, 2주차에서는 algorithm, compiler, ISA, processor, memory, I/O가 각각 성능에 영향을 주는 계층으로 나온다.

그래서 2주차는 앞으로 배울 내용을 위한 성능 언어를 만든다. 이후 수업에서 pipeline이 나오면 CPI를 줄이려는 장치로 보면 되고, cache가 나오면 memory stall을 줄여 CPI를 낮추는 장치로 보면 된다. 병렬처리가 나오면 Amdahl's Law로 전체 speedup 한계를 같이 봐야 한다.

## 23. 시험/복습 우선순위

### 1순위 - 반드시 계산 가능해야 함

- `Performance = 1 / Execution Time`
- `CPU Time = Clock Cycles × Clock Cycle Time`
- `CPU Time = Clock Cycles / Clock Rate`
- `Clock Cycles = IC × CPI`
- `CPU Time = IC × CPI × Clock Cycle Time`
- `CPU Time = IC × CPI / Clock Rate`
- Relative performance 계산
- Amdahl's Law 계산

### 2순위 - 말로 설명 가능해야 함

- Response time과 throughput 차이
- Die area, defects, yield, cost per die 관계
- Chiplet이 등장한 이유
- Clock rate만으로 성능을 판단하면 안 되는 이유
- Average CPI가 가중평균인 이유
- SPECratio와 geometric mean을 쓰는 이유
- Power wall이 multicore로 이어진 이유

### 3순위 - 큰 흐름 파악

- Old computer systems에서 modern SoC로 이동한 이유
- Benchmark suite의 종류와 목적
- GPU와 accelerator가 필요한 배경
- Common case를 빠르게 만들라는 말의 의미

## 24. 연습 문제

1. 컴퓨터 A는 프로그램을 8초에 실행하고, 컴퓨터 B는 12초에 실행한다. A는 B보다 몇 배 빠른가?

풀이 방향: `Time_B / Time_A = 12 / 8 = 1.5`, A가 B보다 1.5배 빠르다.

2. 어떤 프로그램이 3GHz CPU에서 9초 걸린다. 총 clock cycles는 얼마인가?

풀이 방향: `Clock Cycles = CPU Time × Clock Rate = 9 × 3 × 10^9 = 27 × 10^9 cycles`.

3. 어떤 CPU의 clock rate는 2GHz, instruction count는 10^9, CPI는 2다. CPU time은 얼마인가?

풀이 방향: `CPU Time = IC × CPI / Clock Rate = 10^9 × 2 / (2 × 10^9) = 1 second`.

4. 명령어 A, B, C의 CPI가 각각 1, 2, 4이고 실행 개수가 각각 50, 30, 20이면 평균 CPI는 얼마인가?

풀이 방향: 총 cycles = `50×1 + 30×2 + 20×4 = 190`, 총 IC = 100, 평균 CPI = 1.9.

5. 전체 실행 시간이 100초인 프로그램에서 60초를 차지하는 부분을 3배 빠르게 만들면 전체 실행 시간은 얼마인가?

풀이 방향: `T_improved = 60/3 + 40 = 60 seconds`.

6. 전체 프로그램의 75%만 병렬화 가능하다. 병렬 부분을 무한히 빠르게 만들 수 있다고 해도 최대 speedup은 얼마인가?

풀이 방향: 개선되지 않는 25%가 남으므로 최소 실행 시간은 원래의 25%, 최대 speedup은 4배.

7. SPECratio가 4라는 것은 어떤 뜻인가?

풀이 방향: 기준 시스템보다 해당 benchmark를 4배 빠르게 실행한다는 뜻이다. 공식은 `Reference Time / Measured Time`이다.

8. 왜 geometric mean을 쓰는가?

풀이 방향: benchmark 결과는 절대 시간이 아니라 비율이므로, 여러 ratio를 합칠 때 기준 선택에 덜 흔들리는 기하평균이 적절하다.

## 25. 오늘 기준 체크리스트

- [x] `CA_Lecture_02.pdf` 원본 반영
- [x] Chapter 1 Performance 전체 흐름 정리
- [x] 반도체 제조, cost per die, yield 정리
- [x] response time과 throughput 정리
- [x] CPU Time, clock, IC, CPI 공식 정리
- [x] SPEC benchmark와 geometric mean 정리
- [x] power wall, multicore, GPU, accelerator 정리
- [x] Amdahl's Law 예제 정리
- [ ] 수업 중 교수님이 강조한 PA 1, RISC-V simulator, discussion 관련 공지 추가 확인
- [ ] 수요일 수업 후 예제 풀이나 강조점을 추가 반영

## 26. 아주 짧은 암기 카드

- 성능은 실행 시간의 역수다.
- A가 B보다 몇 배 빠른지 보려면 `B 시간 / A 시간`을 계산한다.
- CPU time은 `IC × CPI × Cycle Time`이다.
- Clock rate만 높다고 무조건 빠른 것이 아니다.
- CPI는 명령어 종류와 실행 비중에 따라 달라진다.
- 큰 die는 yield가 낮아지고 cost per die가 비선형적으로 증가한다.
- SPECratio는 높을수록 좋다.
- 여러 ratio 평균은 geometric mean을 쓴다.
- Power wall 때문에 단일 코어 클럭 상승만으로는 한계가 있다.
- Multicore 성능은 병렬화 가능한 부분에 의해 제한된다.
- Amdahl's Law는 부분 최적화의 한계를 보여준다.
- Common case를 빠르게 만들어야 전체 성능이 의미 있게 좋아진다.