# 알고리즘개론 1주차 학습지도

## 한줄 결론

1주차는 알고리즘 과목의 방향을 잡는 주차입니다. `lec00_overview`는 이 수업을 왜 듣는지, 무엇을 평가하는지, 알고리즘을 왜 AI 시대에도 배워야 하는지 보여주는 개요 자료이고, `lec01_preliminaries`는 앞으로 알고리즘을 증명하고 분석할 때 필요한 기초 언어를 정리한 자료입니다. `induction_교수님추천 참고예제.pdf`는 약한 귀납법과 강한 귀납법을 헷갈리지 않게 예제로 보라는 보조자료입니다.

현재 이 문서는 교수님 PDF/공지 기반의 1주차 학습지도에 MIT 6.006 `Lecture 1: Algorithms and Computation` 수강 정리까지 병합한 최종 복습본입니다. 이번 주 퀴즈 대비는 이 파일의 `MIT 강의 수강 후 정리`와 `퀴즈 대비 체크포인트`를 먼저 보면 됩니다.

## 자료 파일

- `./lec00_overview.pdf`: 과목 오리엔테이션 + 알고리즘의 의미 + 최대 부분배열 예시 + 수업 운영/평가 방식
- `./lec01_preliminaries.pdf`: 귀납법, 알고리즘의 조건, 계산 모델, 수행시간, 점근적 성능, 입력 크기, 경우 분석
- `./induction_교수님추천 참고예제.pdf`: Cornell CS 2800의 귀납법 예제 노트. weak induction과 strong induction 비교용

## MIT OCW 참고 자료

교수님이 수업 참고 강의로 안내한 것으로 보이는 큰 원천은 MIT OCW `6.006 Introduction to Algorithms, Spring 2020`입니다. iCampus 1주차의 `[Quiz] Algorithms and Computation`과 직접 맞물리는 자료는 MIT OCW `Lecture 1: Algorithms and Computation`입니다.

- MIT OCW course: <https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-spring-2020/>
- Lecture videos 목록: <https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-spring-2020/video_galleries/lecture-videos/>
- Lecture 1: Algorithms and Computation: <https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-spring-2020/resources/lecture-1-algorithms-and-computation/>
- Lecture notes 목록: <https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-spring-2020/pages/lecture-notes/>
- Resource index: <https://ocw.mit.edu/courses/6-006-introduction-to-algorithms-spring-2020/pages/resource-index/>

정리 방식은 `미리 전부 복사`가 아니라 `주차별로 필요한 MIT 자료를 study.md에 연결하고, 실제로 본 뒤 핵심 내용을 누적`하는 쪽으로 갑니다. 그래야 저장소가 안 복잡해지고, 교수님 수업 범위와 MIT 참고 범위가 섞여서 과장되는 일을 막을 수 있습니다.

## MIT 강의 수강 후 정리: Lecture 1. Algorithms and Computation

이번 MIT 6.006 Lecture 1의 핵심은 알고리즘 공부가 단순 코딩이 아니라는 점입니다. 강의는 처음부터 끝까지 `Problem -> Algorithm -> Correctness -> Efficiency -> Model of Computation -> Data Structure` 순서로 전개됩니다. 즉 먼저 무엇을 풀어야 하는지 정확히 정의하고, 그 문제를 푸는 절차를 만들고, 그 절차가 모든 valid input에서 맞는지 증명하고, 얼마나 효율적인지 분석하며, 마지막으로 그 분석을 가능하게 하는 계산 모델과 자료구조로 넘어갑니다.

```text
Computational Problem
        |
        v
Algorithm
        |
        v
Correctness Proof
        |
        v
Efficiency Analysis
        |
        v
Model of Computation: Word RAM
        |
        v
Data Structures
        |
        v
Efficient Algorithms
```

## 1. 알고리즘 수업의 목표

MIT 강의가 제시하는 알고리즘 수업의 목표는 세 가지입니다.

- Solve computational problems
- Prove correctness
- Argue efficiency

여기서 중요한 것은 `solve`만으로 끝나지 않는다는 점입니다. 알고리즘은 어떤 문제를 푸는 방법이지만, 알고리즘 수업에서는 그 방법이 왜 항상 맞는지와 왜 충분히 빠른지도 함께 말해야 합니다. 몇 개의 테스트 케이스를 맞혔다고 해서 알고리즘이 correct하다고 할 수 없고, 작은 입력에서 빨라 보인다고 해서 efficient하다고 할 수도 없습니다.

그래서 이 강의의 기본 사고 흐름은 다음입니다.

```text
무엇을 계산해야 하는가?
        |
어떻게 계산할 것인가?
        |
왜 항상 올바른가?
        |
얼마나 효율적인가?
```

이 흐름은 교수님 PDF의 `preliminaries`와도 잘 맞습니다. 교수님 자료가 귀납법, 알고리즘 조건, RAM 모델, 입력 크기, 점근적 분석을 먼저 깔아두는 이유도 결국 이 네 질문에 답하기 위해서입니다.

## 2. Computational Problem

Computational problem은 “입력에 대해 어떤 출력이 올바른지”를 정의하는 것입니다. 아직 어떻게 계산할지는 말하지 않습니다. 예를 들어 “배열에서 값 5가 들어 있는 index 하나를 반환하라”는 problem이고, 왼쪽부터 찾을지, binary search를 쓸지, hash table을 쓸지는 algorithm의 영역입니다.

강의에서는 problem을 input space와 output space 사이의 binary relation으로 봅니다.

```text
Input Space I
Output Space O
Problem R subset I x O
```

`(x, y) in R`이라는 말은 입력 `x`에 대해 출력 `y`가 올바른 답이라는 뜻입니다. Problem을 function이 아니라 relation으로 정의하는 이유는 하나의 input에 correct output이 여러 개 있을 수 있기 때문입니다. 예를 들어 배열 `[2, 5, 7, 5, 9]`에서 값 `5`의 index 하나를 반환하라는 문제라면, index `1`도 맞고 index `3`도 맞습니다.

그래서 problem specification은 보통 predicate로 씁니다. Predicate는 입력과 출력이 주어졌을 때 그 출력이 올바른지 판정하는 조건입니다.

```text
Input  : A, target value 5
Output : i
Correct if A[i] == 5
```

알고리즘 문제를 풀 때는 제일 먼저 이 predicate를 명확히 해야 합니다. predicate가 흐리면 correctness proof도 흐려지고, algorithm이 무엇을 보장해야 하는지도 불분명해집니다.

## 3. Instance와 arbitrarily sized input

Problem instance는 특정한 하나의 입력입니다. 예를 들어 `A = [10, 7, 4, 3]`은 하나의 instance입니다. 하지만 알고리즘 수업은 특정 instance 하나를 손으로 푸는 데 관심이 있는 것이 아니라, 임의 크기의 입력을 처리하는 general problem을 푸는 데 관심이 있습니다.

강의에서 이 점은 arbitrarily sized input으로 연결됩니다. 알고리즘의 설명 자체는 finite하고 고정되어 있지만, 입력 크기 `n`은 100일 수도 있고, 1000일 수도 있고, 훨씬 클 수도 있습니다. 따라서 loop, recursion, induction, asymptotic analysis가 자연스럽게 등장합니다.

## 4. Birthday Problem

Lecture 1의 대표 예제는 birthday problem입니다. 학생들을 조사하면서 같은 birth time을 가진 pair가 있는지 찾는 문제입니다. 일반적인 생일만 생각하면 가능한 생일이 365개이고 학생이 365명을 넘으면 pigeonhole principle에 의해 반드시 같은 생일이 존재합니다. 하지만 강의에서는 확률 문제가 아니라 실제 입력을 받아 match가 있는지 확실히 판정하는 deterministic algorithm을 원합니다.

문제의 핵심은 다음입니다.

```text
Input  : students and their birth times
Output : matching pair if one exists, otherwise NONE
Correct if returned pair has the same birth time, or NONE is returned only when no pair exists
```

이 예제가 좋은 이유는 problem, algorithm, correctness, efficiency, data structure가 모두 연결되기 때문입니다. 같은 birth time이 있는지 확인하려면 이전에 본 birth time들을 record에 저장해야 하고, 그 record를 어떻게 구현하느냐에 따라 효율성이 달라집니다.

## 5. Algorithm

Problem이 “무엇이 올바른 답인가”라면, algorithm은 “그 답을 어떻게 계산할 것인가”입니다. 강의에서는 algorithm을 수학적으로 function처럼 봅니다.

```text
Algorithm A: I -> O
```

Problem은 하나의 input에 여러 correct output을 허용할 수 있지만, algorithm을 실제로 한 번 실행하면 하나의 output이 나옵니다. 따라서 algorithm `A`가 problem `R`을 solve한다는 것은 모든 valid input `x`에 대해 `(x, A(x)) in R`이라는 뜻입니다.

여기서 function이라는 말은 functional programming의 pure function을 뜻하는 것이 아닙니다. 내부에서 상태 변경이나 I/O가 일어나는지 여부가 핵심이 아니라, 최종적으로 input 하나에 output 하나를 대응시키는 수학적 관점이 핵심입니다.

## 6. Birthday Algorithm

Birthday problem의 가장 직접적인 알고리즘은 record를 유지하면서 학생을 하나씩 보는 방식입니다.

```text
Maintain an initially empty record R.

For each student s:
    b = birthday of s

    if b is already in R:
        return matching pair

    add (b, s) to R

return NONE
```

이 알고리즘은 record를 어떻게 구현하는지 아직 정하지 않은 high-level procedure입니다. 하지만 같은 수업을 듣는 사람이 구현할 수 있을 만큼 충분히 명확합니다. 알고리즘 답안은 항상 실제 C++ 코드일 필요는 없지만, “적절히 찾아서 반환한다”처럼 모호해서는 안 됩니다.

## 7. Correctness와 induction

알고리즘을 제시했다고 끝이 아닙니다. 다음 질문은 “왜 이 알고리즘이 항상 맞는가?”입니다. 테스트는 proof가 아닙니다. 입력 10개, 100개, 10000개에서 맞아도 그 다음 입력에서 틀릴 수 있습니다. 모든 valid input에서 맞는다는 주장은 논리적 증명이 필요합니다.

강의는 loop와 recursion이 arbitrarily large input을 finite code로 처리하는 방식이라고 설명하고, 이런 반복 구조의 correctness를 보이는 자연스러운 도구가 induction이라고 연결합니다.

귀납법의 기본 구조는 다음입니다.

```text
Predicate P(k)를 정의한다.
Base case를 보인다.
Inductive hypothesis를 세운다.
Inductive step에서 P(k) -> P(k+1)을 보인다.
결론적으로 모든 k에 대해 P(k)가 성립한다.
```

Birthday algorithm의 proof에서는 다음과 같은 명제를 잡습니다.

```text
P(k): 첫 k명의 학생 안에 match가 존재하면,
      알고리즘은 k+1번째 학생을 조사하기 전에 이미 match를 반환한다.
```

Base case는 `k = 0`입니다. 첫 0명 안에는 match가 존재할 수 없으므로 implication의 전제가 거짓입니다. 따라서 vacuous truth로 참입니다.

Inductive step에서는 두 경우로 나눕니다.

- 첫 `k`명 안에 이미 match가 있다면, induction hypothesis에 의해 알고리즘은 이미 match를 반환했습니다.
- 첫 `k`명 안에는 match가 없지만 첫 `k+1`명 안에는 match가 있다면, 새로운 match는 반드시 `k+1`번째 학생을 포함합니다. 알고리즘은 `k+1`번째 학생의 birthday를 이전 record와 비교하므로 match를 발견합니다.

이렇게 `P(k) -> P(k+1)`을 보이고, 모든 학생을 다 본 뒤 match가 있으면 반환하고 없으면 `NONE`을 반환한다는 correctness 결론으로 갑니다.

## 8. Efficiency

Correct algorithm이라고 끝이 아닙니다. 같은 문제를 푸는 correct algorithm이 여러 개라면, 더 efficient한 algorithm을 원합니다. 실제 실행 시간을 초 단위로 직접 비교하는 것은 공정하지 않습니다. CPU, OS, compiler, cache, implementation, input distribution 같은 변수가 너무 많기 때문입니다.

그래서 알고리즘 분석에서는 fundamental operation을 세고, input size `n`에 따른 operation count의 growth를 봅니다. 이때 중요한 것은 정확한 초가 아니라 `T(n)`이 어떤 속도로 증가하는가입니다.

```text
T(n) = number of fundamental operations as a function of input size n
```

Input size도 문제마다 다릅니다. 배열은 보통 원소 개수 `n`, matrix는 `n x n`이면 대략 `n^2`, graph는 보통 `|V| + |E|`로 봅니다. 따라서 복잡도를 말하기 전에 무엇을 input size로 보는지 먼저 정해야 합니다.

## 9. Asymptotic notation

Lecture 1에서는 Big-O, Big-Omega, Big-Theta의 역할을 구분합니다.

| 표기 | 의미 | 직관 |
| --- | --- | --- |
| `O(f(n))` | upper bound | 이 정도보다 더 빨리 커지지는 않는다 |
| `Omega(f(n))` | lower bound | 적어도 이 정도는 든다 |
| `Theta(f(n))` | tight bound | 위아래가 같은 성장률로 묶인다 |

주의할 점은 Big-O가 정확히 몇 번 연산한다는 뜻이 아니라는 것입니다. 예를 들어 `3n + 100`도 `O(n)`입니다. 중요한 것은 큰 입력에서 dominant term이 무엇인지입니다.

대표 성장률은 다음 순서로 기억하면 됩니다.

```text
1 < log n < n < n log n < n^2 < polynomial higher powers < exponential
```

MIT 6.006 맥락에서 efficient algorithm은 대체로 polynomial time algorithm을 뜻합니다. Exponential time은 작은 입력에서는 돌아갈 수 있어도 입력이 조금 커지면 현실적으로 어렵습니다.

## 10. Word RAM model

Efficiency를 분석하려면 계산 모델이 필요합니다. Lecture 1의 모델은 Word RAM입니다. RAM은 Random Access Memory이고, memory의 임의 주소에 constant time으로 접근할 수 있다고 가정합니다. Memory는 bit들로 이루어져 있고, 보통 byte-addressable입니다.

```text
1 byte = 8 bits
word = CPU가 자연스럽게 한 번에 처리하는 고정 크기 데이터 단위
```

32-bit address와 byte-addressable memory를 가정하면 가능한 주소 수는 `2^32`개이고, 각 주소가 1 byte를 가리키므로 약 4 GiB의 address space가 됩니다. 64-bit에서는 address space가 훨씬 커집니다.

Word RAM에서는 word 단위의 기본 arithmetic, logical operation, comparison, memory access 등을 constant time으로 모델링합니다. 물론 현실에서는 cache나 memory hierarchy 때문에 모든 접근 비용이 완전히 같지는 않지만, 알고리즘의 큰 성장률을 보기 위해 단순화합니다.

## 11. Data Structure로 연결

CPU는 한 번에 constant amount of information만 처리한다고 봅니다. 따라서 `n`개의 데이터를 전부 읽어야 한다면 최소한 `Omega(n)`이 필요하고, 단순 순회로 `O(n)`에 가능하므로 `Theta(n)`입니다.

여기서 data structure가 등장합니다. 현실에서는 많은 데이터를 저장하고, 그 데이터에 대해 search, insert, delete, find minimum, access 같은 operation을 빠르게 해야 합니다. 자료구조는 데이터를 담는 통이 아니라, 필요한 operation들을 효율적으로 지원하도록 데이터를 조직하는 방식입니다.

Birthday algorithm에서 record를 단순 배열로 구현하면, 새 학생의 birth time이 이전 record에 있는지 확인하기 위해 이전 항목을 하나씩 검사해야 합니다. 최악의 경우 비교 횟수는 다음과 같습니다.

```text
0 + 1 + 2 + ... + (n - 1) = n(n - 1) / 2 = Theta(n^2)
```

하지만 더 좋은 data structure를 쓰면 record lookup을 더 빠르게 만들 수 있습니다. 앞으로 배우는 sorting, hash table 같은 도구가 바로 이런 효율성 개선과 연결됩니다.

## 12. 앞으로의 MIT 6.006 흐름

강의 마지막은 전체 과목의 큰 단원을 소개합니다.

```text
Data Structures / Sorting
        |
Graphs / Shortest Paths
        |
Dynamic Programming
```

알고리즘 문제를 해결하는 방식은 크게 두 가지입니다. 하나는 현재 문제를 이미 해결 방법을 아는 문제로 reduction하는 것이고, 다른 하나는 recursive structure 등을 이용해 새로운 algorithm design을 하는 것입니다. 이 첫 강의는 그 모든 내용을 시작하기 전에 problem, algorithm, correctness, efficiency라는 언어를 맞추는 역할입니다.

## Lecture 1 핵심 용어표

| 영어 | 한국어 | 핵심 의미 |
| --- | --- | --- |
| Computational Problem | 계산 문제 | input과 correct output 사이의 relation |
| Input Space | 입력 공간 | 가능한 모든 입력 |
| Output Space | 출력 공간 | 가능한 모든 출력 |
| Binary Relation | 이항 관계 | input과 올바른 output 사이의 관계 |
| Predicate | 술어/조건 | output이 correct인지 판정하는 조건 |
| Problem Instance | 문제 인스턴스 | 특정한 하나의 입력 |
| Algorithm | 알고리즘 | input에서 output을 계산하는 절차 |
| Correctness | 정확성 | 모든 valid input에서 올바른 답을 반환함 |
| Proof | 증명 | correctness를 논리적으로 보이는 것 |
| Induction | 귀납법 | 반복/재귀 구조의 정확성을 증명하는 방법 |
| Base Case | 기저 사례 | induction의 시작점 |
| Inductive Hypothesis | 귀납 가설 | `P(k)`가 참이라고 가정 |
| Inductive Step | 귀납 단계 | `P(k) -> P(k+1)` 증명 |
| Efficiency | 효율성 | 필요한 계산 자원의 정도 |
| Fundamental Operation | 기본 연산 | 계산 모델에서 기본 단위가 되는 연산 |
| Input Size | 입력 크기 | 입력을 표현하는 정보량 |
| Asymptotic Analysis | 점근적 분석 | input 증가에 따른 growth 분석 |
| Big-O | 빅오 | upper bound |
| Big-Omega | 빅오메가 | lower bound |
| Big-Theta | 빅세타 | tight bound |
| Model of Computation | 계산 모델 | 어떤 연산을 어떤 비용으로 볼지 정의 |
| Word RAM | 워드 램 | word 단위 기본 연산과 random access를 constant time으로 보는 모델 |
| Data Structure | 자료구조 | 데이터를 저장하고 필요한 operation을 효율적으로 지원하는 구조 |
| Reduction | 환원 | 문제를 이미 아는 다른 문제로 변환 |
| Dynamic Programming | 동적 계획법 | subproblem 결과를 재사용하는 설계 방식 |

## 반드시 구분할 것

### Problem vs Algorithm

```text
Problem   = 무엇이 정답인가?
Algorithm = 정답을 어떻게 찾는가?
```

### Problem Relation vs Algorithm Function

```text
Problem:
하나의 input에 여러 correct output 가능

Algorithm:
하나의 input에 실제 output 하나 생성
```

### Correctness vs Efficiency

```text
Correctness = 답이 항상 맞는가?
Efficiency  = 얼마나 적은 계산으로 답을 구하는가?
```

### Actual Running Time vs Asymptotic Complexity

```text
Actual running time:
CPU, compiler, OS, cache, implementation 등에 영향받음

Asymptotic complexity:
input size 대비 operation count의 growth를 추상화
```

### Word vs Byte

```text
1 byte = 8 bits
word = architecture가 자연스럽게 처리하는 고정 크기 단위
```

## 시험 직전 압축 노트

```text
Algorithmics
= Problem Definition
+ Algorithm Design
+ Correctness Proof
+ Efficiency Analysis

Computational Problem
= Input과 Correct Output 사이의 Binary Relation.

Algorithm A가 Problem R을 solve하려면
모든 valid x에 대해 (x, A(x)) in R.

Test는 proof가 아니다.
Loop/recursion의 correctness에는 induction이 자연스럽다.

Birthday proof의 핵심 P(k):
첫 k명 안에 match가 있으면
k+1번째 학생을 보기 전에 이미 match를 반환했다.

Efficiency는 seconds가 아니라
input size n에 따른 fundamental operations의 growth로 본다.

O(f(n))     = upper bound
Omega(f(n)) = lower bound
Theta(f(n)) = tight bound

1 < log n < n < n log n < n^2 < poly < exponential

Word RAM에서는 word 단위 기본 연산과 memory access를 constant time으로 본다.

Data Structure는 많은 데이터를 저장하면서
Search / Insert / Delete 같은 operation을 빠르게 지원하는 방법이다.
```

## MIT 강의 기반 이해 확인 문제

- 왜 computational problem은 function이 아니라 binary relation으로 정의하는가?
- Problem과 algorithm의 차이를 예 하나로 설명할 수 있는가?
- Algorithm을 mathematical function이라고 할 때, functional programming의 pure function과 무엇이 다른가?
- Birthday algorithm의 pseudocode를 말할 수 있는가?
- Birthday algorithm correctness proof에서 `P(k)`를 어떻게 잡는가?
- 왜 `k = 0` base case가 vacuous truth로 성립하는가?
- 첫 `k`명 안에는 match가 없는데 첫 `k+1`명 안에는 match가 있으면, 왜 새 match는 `k+1`번째 학생을 포함해야 하는가?
- 실제 초 단위 running time이 아니라 fundamental operation을 세는 이유는 무엇인가?
- Matrix와 graph에서 input size를 어떻게 잡는가?
- `O`, `Omega`, `Theta`의 차이를 설명할 수 있는가?
- Growth rate `1, log n, n, n log n, n^2, exponential`을 작은 순서대로 나열할 수 있는가?
- Word RAM에서 word, byte, address, random access가 각각 무엇을 뜻하는가?
- 32-bit byte-addressable memory에서 왜 약 4 GiB address space가 나오는가?
- Birthday record를 단순 배열로 구현하면 왜 `Theta(n^2)`가 되는가?
- Data structure 선택이 algorithm efficiency에 왜 직접 영향을 주는가?


## iCampus 강의콘텐츠

공통 기간: 2026-08-31 00:00 - 2026-09-13 23:59

| 항목 | 상태/분량 | 메모 |
| --- | --- | --- |
| lec00_overview | 완료 표시 | 1주차 개요 자료 |
| Preliminaries | 완료 표시 | 예비 개념 자료 |
| 01_Preliminaries | 19:39 | 1주차 1차시 |
| 1. Algorithms and Computation | 45:39 | 출결 대상 아님 |
| [Quiz] Algorithms and Computation | 4문제, 4점 | 2026-09-13 23:59까지 응시 필요 |
| weak induction vs strong induction | 0점, 외부 도구 | 퀴즈/개념 대비 참고자료 성격 |

## 이번 주에 뭘 하라는 건가

이번 주 할 일은 크게 네 묶음으로 보면 됩니다.

1. 과목 전체 구조 파악

`lec00_overview`를 통해 이 과목이 단순히 코딩을 많이 하는 과목이 아니라, 문제를 푸는 방법을 설계하고 그 방법이 얼마나 좋은지 분석하는 과목임을 파악해야 합니다. 교수님은 AI가 코드는 써줄 수 있어도, 어떤 방법이 맞고 빠른지 판단하는 능력은 여전히 사람이 가져야 한다는 흐름으로 수업의 필요성을 설명합니다.

2. 알고리즘의 기본 정의 이해

알고리즘은 입력을 받아 출력으로 바꾸는 명확한 계산 절차입니다. 그냥 코드 조각이 아니라, 유한한 단계 안에서 끝나야 하고, 각 단계가 모호하지 않아야 하며, 문제 해결을 위한 절차여야 합니다. 앞으로 이 과목에서 어떤 방법을 배우든, 그 방법이 실제 알고리즘이라고 부를 수 있는 조건을 만족하는지 계속 확인하게 됩니다.

3. 분석을 위한 예비 개념 준비

`lec01_preliminaries`의 핵심은 앞으로 알고리즘을 설명할 때 쓰는 언어를 미리 맞추는 것입니다. 귀납법으로 알고리즘의 정당성을 보이고, RAM 모델 같은 계산 모델 아래에서 수행시간을 세며, 입력 크기에 따라 시간/공간 복잡도가 어떻게 변하는지 봅니다. 이 부분을 대충 넘기면 나중에 증명이나 시간복잡도 분석이 다 흔들립니다.

4. 퀴즈 대비

`[Quiz] Algorithms and Computation`이 4문제 4점으로 열려 있으므로, 개요 자료와 알고리즘/계산 관련 영상 내용을 본 뒤 응시해야 합니다. 교수님이 MIT 강의를 보고 퀴즈를 풀고 준비해오라고 안내하셨으므로, iCampus 영상만이 아니라 MIT OCW 흐름도 함께 참고하는 것으로 보입니다.

## 파일별 역할 정리

## lec00_overview.pdf

이 파일은 과목의 첫 지도입니다. 크게 세 부분으로 나눌 수 있습니다.

첫째, 알고리즘이 무엇인지 설명합니다. 알고리즘은 어떤 입력을 받아 정해진 절차를 통해 출력을 만드는 방법입니다. 유튜브 추천 알고리즘 예시처럼, 현실에서 알고리즘은 사용자의 행동이나 데이터를 입력으로 받아 추천 결과 같은 출력을 만듭니다. 이 예시는 알고리즘이 추상적인 수학 문제가 아니라 실제 서비스와 시스템 안에서 작동하는 절차라는 점을 보여줍니다.

둘째, 왜 알고리즘을 배워야 하는지 설명합니다. 자료에서는 AI 도구가 코드를 빠르게 만들어줄 수 있다는 점을 인정합니다. 하지만 AI가 준 코드가 정말 좋은지, 입력이 커졌을 때 버틸 수 있는지, 더 나은 방법이 있는지 판단하려면 알고리즘 지식이 필요하다고 강조합니다. 특히 “돌아가는 코드”와 “좋은 알고리즘”은 다릅니다. 테스트를 통과하는 코드라도 입력 크기가 커지면 현실적으로 사용할 수 없을 수 있습니다.

셋째, 최대 연속 부분배열 합 문제를 통해 알고리즘 설계 전략을 비교합니다. 같은 문제를 두고 brute force, divide and conquer, dynamic programming 접근을 차례로 보여줍니다. 처음에는 모든 구간을 확인하는 단순한 방법을 생각할 수 있지만, 이 방법은 대략 `n^2` 이상의 비용으로 커질 수 있습니다. 그 다음 배열을 둘로 나누어 왼쪽, 오른쪽, 가운데를 가로지르는 경우를 고려하는 divide and conquer 방식으로 `n log n` 정도의 접근을 보여줍니다. 마지막으로 현재 위치에서 끝나는 최적 부분합을 유지하는 dynamic programming 아이디어를 통해 선형 시간 접근으로 개선합니다.

이 파일에서 중요한 공부 포인트는 “정답 코드 하나”가 아니라 “문제를 보는 관점의 개선”입니다. 같은 문제라도 무식하게 모두 확인할 수도 있고, 재귀적으로 쪼갤 수도 있고, 이전 계산 결과를 기억하면서 한 번만 훑을 수도 있습니다. 알고리즘 수업은 이 차이를 의식적으로 보는 훈련에 가깝습니다.

또한 후반부에는 수업 운영 방식이 들어 있습니다. 수업은 대면 수업과 iCampus 온라인 강의로 진행되며, MIT OCW의 selected topics와 iCampus 녹화 강의가 언급됩니다. 교재는 CLRS, 즉 `Introduction to Algorithms`입니다. 평가 비율은 PA 20%, 중간 30%, 기말 30%, 퀴즈 15%, 참여/출석 5%로 정리되어 있습니다. 표절 정책과 AI 사용 정책도 들어 있는데, 이 과목에서는 AI assistant 사용을 허용하되 사용 도구와 사용 부분을 명시해야 하며, 제출한 내용은 본인이 설명하고 방어할 수 있어야 한다는 취지입니다.

## lec01_preliminaries.pdf

이 파일은 이름 그대로 예비 지식입니다. `Preliminaries`는 “본격적인 내용을 들어가기 전에 깔아두는 기초 개념”이라는 뜻입니다. 여기서는 앞으로 알고리즘을 설계하고 분석할 때 계속 쓰는 기본 도구를 다룹니다.

첫 번째 핵심은 induction, 즉 수학적 귀납법입니다. 알고리즘 과목에서는 어떤 절차가 모든 입력 크기에서 맞게 동작한다는 것을 보여야 할 때가 많습니다. 이때 base case, inductive hypothesis, inductive step 구조가 자주 나옵니다. `n = k` 같은 가장 작은 경우에서 성립함을 보이고, 임의의 `n`에서 성립한다고 가정한 뒤 `n + 1`에서도 성립함을 보이면 전체에 대해 성립한다고 말할 수 있습니다.

자료에는 가우스 합 공식 예제가 나옵니다. `1 + 2 + ... + n = n(n+1)/2` 꼴의 공식을 귀납법으로 증명하는 예시입니다. 이 예시는 basis, hypothesis, step을 어떻게 써야 하는지 보여주는 표준 연습입니다. 이어서 등비수열 합 공식도 귀납법으로 증명합니다. 이 둘은 앞으로 시간복잡도 합을 다룰 때도 자주 만나는 형태라, 단순 증명 연습을 넘어 계산 패턴을 익히는 의미가 있습니다.

두 번째 핵심은 weak induction과 strong induction입니다. weak induction은 보통 바로 이전 단계가 맞다고 가정하고 다음 단계가 맞음을 보이는 방식입니다. strong induction은 `k` 이하의 모든 단계가 맞다고 가정하고 다음 단계를 보이는 방식입니다. 알고리즘에서는 어떤 문제가 바로 이전 크기만 참이면 되는 경우도 있지만, 여러 작은 크기의 문제가 모두 필요할 때도 있습니다. 재귀 알고리즘이나 분할정복 증명에서는 strong induction이 자연스럽게 쓰일 수 있습니다.

세 번째 핵심은 알고리즘의 조건입니다. 알고리즘은 입력, 출력, 명확성, 유한성, 효과성을 만족해야 합니다. 즉 입력이 없을 수도 있지만 출력은 있어야 하고, 단계가 모호하면 안 되며, 모든 경우에 대해 유한한 단계 뒤 종료해야 합니다. 또한 각 명령은 실제로 수행 가능한 기본 연산이어야 합니다.

네 번째 핵심은 분석 모델입니다. 수업에서는 보통 generic uniprocessor RAM 모델을 기준으로 분석합니다. 모든 메모리 접근 비용을 같다고 보고, 동시에 여러 연산이 일어나지 않는다고 보며, 합리적인 기본 명령은 단위 시간으로 취급합니다. 실제 컴퓨터는 캐시, 병렬성, 컴파일러 최적화 등으로 더 복잡하지만, 알고리즘 분석에서는 먼저 이런 단순 모델로 큰 그림을 잡습니다.

다섯 번째 핵심은 running time과 asymptotic performance입니다. 수행시간은 실행되는 primitive step의 수로 생각합니다. 정확히 몇 초가 걸리는지가 아니라 입력 크기가 커질 때 연산 수가 어떻게 증가하는지가 중요합니다. 그래서 이 과목에서는 점근적 성능, 즉 입력이 매우 커질 때의 성장률을 중요하게 봅니다. 시간뿐 아니라 공간, 저장공간, 대역폭, 전력, 회로 규모 같은 자원도 같은 방식으로 생각할 수 있습니다.

여섯 번째 핵심은 input size입니다. 복잡도는 입력 크기의 함수로 표현됩니다. 하지만 입력 크기를 무엇으로 볼지는 문제마다 다릅니다. 정렬 문제에서는 원소 개수가 입력 크기이고, 곱셈 문제에서는 비트 수가 중요할 수 있으며, 그래프 알고리즘에서는 정점 수와 간선 수가 함께 입력 크기가 됩니다. 따라서 시간복잡도를 말할 때는 먼저 입력 크기를 어떻게 정의했는지 분명히 해야 합니다.

마지막 핵심은 best case, average case, worst case입니다. best case는 가장 운 좋은 입력에서의 하한 느낌이고, worst case는 어떤 입력에서도 이 정도 안에는 끝난다는 보장입니다. average case는 기대 수행시간을 보지만, 무엇을 평균으로 볼 것인지가 중요합니다. 현실 입력이 균등 랜덤 입력과 다를 수 있기 때문에 average case는 조심해서 써야 합니다. 시험에서는 보통 worst case 분석이 가장 안정적인 기준으로 자주 쓰입니다.

## induction_교수님추천 참고예제.pdf

이 파일은 `weak induction vs strong induction` 항목에서 연결된 참고 노트입니다. 과제 점수는 0점이지만, 교수님이 예제를 참고하라고 올린 자료이므로 퀴즈나 증명 문제 대비용으로 보면 됩니다.

노트는 귀납법을 사다리 비유로 설명합니다. base case는 처음 딛는 가장 아래 계단이고, induction hypothesis는 이미 밟을 수 있다고 가정하는 계단입니다. weak induction은 현재 밟고 있는 한 단계에 기대어 다음 단계로 가는 느낌이고, strong induction은 이전에 밟아온 여러 단계 전체를 활용해 다음 단계로 가는 느낌입니다.

weak induction 예제로는 합 공식 증명이 나옵니다. 먼저 가장 작은 경우가 맞는지 확인하고, `n = k`에서 맞다고 가정한 뒤 `n = k + 1`에서도 맞음을 보입니다. 이 구조는 `lec01_preliminaries`의 가우스 합 공식 예제와 연결됩니다.

strong induction 예제로는 2 이상의 모든 정수가 소수들의 곱으로 분해될 수 있음을 보이는 문제가 나옵니다. 이 예제에서는 `k + 1`이 소수이면 바로 성립하고, 합성수이면 더 작은 두 인수로 나눈 뒤 induction hypothesis를 사용합니다. 이때 바로 직전 숫자 하나만 필요한 것이 아니라, `k` 이하의 여러 수에 대한 성립이 필요하므로 strong induction이 자연스럽습니다.

마지막에는 귀납법 증명 체크리스트가 나옵니다. 필요한 base case를 모두 확인했는지, base case/hypothesis/step을 분명히 구분했는지, induction hypothesis를 inductive step에서 실제로 제대로 사용했는지를 확인하라는 내용입니다.

## 1주차 개념 지도

## 1. 알고리즘이란 무엇인가

알고리즘은 문제를 풀기 위한 명확하고 유한한 절차입니다. 이 정의에서 중요한 단어는 `문제`, `입력`, `출력`, `절차`, `유한성`, `명확성`입니다. 앞으로 수업에서는 어떤 문제를 만나면 먼저 입력과 출력이 무엇인지 정의하고, 그 입력을 원하는 출력으로 바꾸는 절차를 설계합니다. 그 다음 절차가 맞는지 증명하고, 얼마나 빠른지 분석합니다.

## 2. 자료구조와 알고리즘의 관계

개요 자료에서는 algorithm과 data structure를 함께 언급합니다. 자료구조는 데이터를 담고 조직하는 방식이고, 알고리즘은 그 데이터를 이용해 문제를 푸는 절차입니다. 같은 문제라도 어떤 자료구조를 쓰는지에 따라 알고리즘의 성능이 크게 달라질 수 있습니다. 그래서 알고리즘 수업은 단순히 절차만 외우는 게 아니라, 데이터가 어떤 형태로 놓여 있고 어떤 연산이 자주 필요한지도 같이 봐야 합니다.

## 3. 알고리즘의 필수 조건

알고리즘의 조건은 시험에서 정의형 또는 판단형으로 나올 수 있습니다.

- 입력: 외부에서 주어지는 값. 0개 이상일 수 있음
- 출력: 알고리즘이 만들어내는 값. 최소 1개 이상이어야 함
- 명확성: 각 단계가 애매하지 않아야 함
- 유한성: 모든 경우에 대해 유한한 단계 후 종료해야 함
- 효과성: 각 단계가 실제로 수행 가능한 기본 연산이어야 함
- 효율성: 시간과 메모리 같은 자원을 고려해야 함

특히 유한성과 명확성은 “그럴듯한 설명”과 “알고리즘”을 가르는 기준입니다.

## 4. 왜 알고리즘을 배워야 하는가

AI가 코드를 만들 수 있어도 알고리즘을 알아야 하는 이유는, 생성된 코드의 품질을 판단해야 하기 때문입니다. 어떤 코드가 작은 입력에서는 잘 돌아가도 큰 입력에서는 쓸 수 없을 수 있습니다. 1주차 예시는 최대 부분배열 문제를 통해 이 점을 보여줍니다. 단순한 이중 반복문은 정답을 내지만 입력이 커지면 시간이 급격히 늘어납니다. 알고리즘 지식이 있으면 “이 문제는 더 빠른 방법이 있지 않을까?”라고 물을 수 있고, 실제로 divide and conquer나 dynamic programming으로 개선할 수 있습니다.

## 5. 최대 연속 부분배열 합 문제

문제는 정수 배열이 주어졌을 때, 연속된 구간 중 합이 가장 큰 구간의 합을 찾는 것입니다. 예를 들어 양수는 이익, 음수는 손실로 생각하면, 며칠 연속 투자했을 때 총 이익이 최대가 되는 기간을 찾는 문제입니다.

이 문제의 핵심은 “연속된 구간”이라는 제약입니다. 아무 원소나 골라 합치는 것이 아니라, 배열에서 끊기지 않는 하나의 구간을 골라야 합니다. 따라서 시작점과 끝점을 어떻게 고를지가 중요합니다.

## 6. Brute force 접근

가장 단순한 방법은 모든 가능한 시작점과 끝점을 확인하는 것입니다. 시작점은 `n`개 있을 수 있고, 각 시작점마다 가능한 끝점이 여러 개 있습니다. 가능한 구간 수는 대략 `n + (n-1) + ... + 1` 형태라서 `n^2` 규모입니다. 각 구간의 합을 매번 새로 계산하면 그보다 더 비싸질 수도 있습니다. 개요 자료에서는 이 접근이 정답은 맞지만 입력이 커질 때 너무 느려진다는 점을 보여줍니다.

## 7. Divide and conquer 접근

분할정복은 문제를 절반으로 나누고, 왼쪽 절반의 최적해와 오른쪽 절반의 최적해를 재귀적으로 구한 뒤 결합하는 방식입니다. 최대 부분배열 문제에서는 한 가지 경우가 더 필요합니다. 최적 구간이 왼쪽에만 있을 수도 있고, 오른쪽에만 있을 수도 있지만, 가운데 경계를 가로지를 수도 있습니다. 따라서 crossing subarray를 따로 계산해야 합니다.

경계를 가로지르는 최대합은 왼쪽에서 가운데로 끝나는 최댓값과 오른쪽에서 가운데 다음부터 시작하는 최댓값을 따로 구해 합치면 됩니다. 이 결합 비용은 선형 시간이고, 절반 크기의 문제 두 개를 재귀적으로 푸는 구조가 됩니다. 그래서 전체 시간은 대략 `T(n) = 2T(n/2) + n`이고, `n log n` 형태로 볼 수 있습니다.

## 8. Dynamic programming 접근

DP 접근에서는 각 위치에서 끝나는 최대 부분합을 유지합니다. 현재 원소를 이전 구간에 붙이는 게 나은지, 현재 원소에서 새로 시작하는 게 나은지를 비교합니다. 핵심 점화식은 `mem[n] = max(mem[n-1] + a[n], a[n])`입니다. 그리고 전체 답은 지금까지 본 `mem` 값 중 최댓값입니다.

이 방식은 배열을 한 번만 훑으면 되므로 선형 시간입니다. 1주차에서 이 예시는 brute force, divide and conquer, dynamic programming이 어떻게 같은 문제를 점점 더 효율적으로 푸는지 보여주는 대표 사례입니다.

## 9. 귀납법이 왜 필요한가

알고리즘을 배우면 “이 방법이 항상 맞는가?”를 자주 물어야 합니다. 몇 개의 예제에서 맞았다고 해서 모든 입력에서 맞는 것은 아닙니다. 귀납법은 입력 크기나 반복 단계가 커져도 어떤 성질이 계속 유지됨을 보이는 도구입니다. 특히 재귀 알고리즘이나 반복 알고리즘의 정당성을 설명할 때 유용합니다.

## 10. Weak induction과 strong induction

Weak induction은 보통 `n`에서 성립한다고 가정하고 `n+1`에서 성립함을 보입니다. 바로 이전 단계 하나만 있으면 충분한 문제에 잘 맞습니다. Strong induction은 `k` 이하의 모든 경우에서 성립한다고 가정하고 `k+1`에서 성립함을 보입니다. 다음 단계를 증명할 때 여러 작은 경우를 가져다 써야 하면 strong induction이 자연스럽습니다.

소인수분해 예제는 strong induction을 쓰는 이유를 잘 보여줍니다. 어떤 합성수 `k+1`을 두 인수로 나누면, 그 인수들이 꼭 `k`일 필요는 없습니다. 여러 작은 수 중 하나일 수 있습니다. 그래서 `k` 하나가 아니라 `k` 이하 전체에 대해 성립한다고 가정해야 증명이 편해집니다.

## 11. 알고리즘 분석 모델

수업에서는 RAM 모델을 기본 계산 모델로 사용합니다. 이 모델에서는 기본 연산 하나가 대략 같은 시간이라고 보고, 메모리 접근 비용도 단순화합니다. 현실과 완전히 같지는 않지만, 알고리즘끼리 큰 성장률을 비교하기에는 적합합니다. 중요한 것은 “분석은 어떤 모델 위에서 하는가”입니다. 모델을 정하지 않으면 수행시간을 말하기 어렵습니다.

## 12. 수행시간과 입력 크기

수행시간은 대개 입력 크기 `n`에 대한 함수로 표현합니다. 정렬에서는 원소 개수, 곱셈에서는 비트 수, 그래프에서는 정점 수와 간선 수가 입력 크기가 됩니다. 따라서 어떤 문제에서 `n`이 무엇을 뜻하는지 먼저 잡아야 합니다. `O(n)`, `O(n log n)`, `O(n^2)` 같은 표현은 그 다음에 의미가 생깁니다.

## 13. Best, average, worst case

Best case는 가장 운 좋은 입력에서의 수행시간입니다. Average case는 평균적인 입력에서의 기대 수행시간인데, 평균 입력을 어떻게 정의하는지가 어렵습니다. Worst case는 가장 나쁜 입력에서도 보장되는 수행시간입니다. 알고리즘 수업에서는 보통 worst case가 안정적인 분석 기준으로 중요합니다. 퀴즈나 시험에서도 “보장”이라는 단어가 나오면 worst case를 먼저 떠올리면 좋습니다.

## 퀴즈 대비 체크포인트

- 알고리즘의 정의를 입력, 출력, 계산 절차 관점에서 설명할 수 있는가
- 알고리즘의 조건인 input, output, definiteness, finiteness, effectiveness를 구분할 수 있는가
- 알고리즘과 자료구조의 차이를 말할 수 있는가
- 왜 AI가 있어도 알고리즘을 배워야 하는지 설명할 수 있는가
- 최대 연속 부분배열 합 문제가 무엇을 요구하는지 이해했는가
- brute force 방식이 왜 느린지 구간 개수 관점에서 설명할 수 있는가
- divide and conquer에서 왼쪽, 오른쪽, crossing case가 왜 모두 필요한지 설명할 수 있는가
- `T(n) = 2T(n/2) + n`이 왜 `n log n`으로 이어지는지 감을 잡았는가
- DP 점화식 `mem[n] = max(mem[n-1] + a[n], a[n])`의 의미를 설명할 수 있는가
- weak induction과 strong induction의 차이를 예제로 구분할 수 있는가
- base case, inductive hypothesis, inductive step을 빠뜨리지 않고 쓸 수 있는가
- 입력 크기가 문제마다 다르게 정의된다는 점을 알고 있는가
- best case, average case, worst case의 차이를 알고 있는가

## 이번 주 공부 순서 추천

1. `lec00_overview.pdf`를 먼저 훑어서 과목 목표, 평가 방식, AI/표절 정책을 확인합니다.
2. 최대 부분배열 예시에서 brute force, divide and conquer, DP가 어떻게 달라지는지 비교합니다.
3. `lec01_preliminaries.pdf`를 보면서 귀납법과 알고리즘 분석 용어를 정리합니다.
4. `induction_교수님추천 참고예제.pdf`로 weak/strong induction 예제를 확인합니다.
5. MIT 강의와 iCampus `Algorithms and Computation` 영상을 보고 퀴즈를 풉니다.
6. 퀴즈를 풀기 전, 위 체크포인트를 말로 설명해봅니다.

## 아직 모호한 점

- `weak induction vs strong induction`은 iCampus에서는 과제 및 평가 항목으로 보이지만 배점이 0점이고 외부 도구 유형입니다. 현재 정보만으로는 제출 과제라기보다 참고자료/학습활동에 가깝습니다.
- 1주차 콘텐츠 마감이 모두 2026-09-13 23:59로 잡혀 있으므로 첫 주 자료를 2주차 끝까지 처리하게 열어둔 것으로 보입니다. 단, 교수님이 별도로 말한 제출 방식이나 GradeScope 연동이 생기면 그 공지가 우선입니다.
- `Preliminaries`와 `01_Preliminaries`의 관계는 iCampus 표시만으로는 완전히 분리하기 어렵습니다. 현재 정리는 `Preliminaries`를 예비 개념 묶음, `01_Preliminaries`를 19:39짜리 1주차 1차시 영상/자료로 봅니다.

## weak induction vs strong induction 안내

출처: iCampus 과제 및 평가 - `weak induction vs strong induction`

- 마감: 2026-09-13 23:59
- 배점: 0점
- 제출물 유형: 외부 도구
- 이용 가능: 2026-08-31 00:00 이후
- 참고 링크: <http://www.cs.cornell.edu/courses/cs2800/2011fa/Lectures/induction.pdf>

원문 메모:

```text
weak induction은 (k-1)th step이 true라고 가정하고, kth step이 true임을 보이는 것입니다.
strong induction은 m < k step들이 true라고 가정하고, kth step이 true임을 보이는 것입니다.

위 노트의 예제를 참고하면 좋을 것 같습니다.
```

## 구두 안내/해석 메모

- 교수님이 MIT 강의를 보고 퀴즈를 풀고 준비해오라고 안내하셨음.
- MIT `Lecture 1: Algorithms and Computation` 수강 정리까지 `MIT 강의 수강 후 정리` 섹션에 반영했음.
- 1주차 콘텐츠 마감이 모두 2026-09-13 23:59로 잡혀 있으므로, 첫 주차 자료는 2주차 끝까지 처리하는 일정으로 보임.
- 다만 제출처나 추가 제출 방식은 아직 명확히 확인된 내용이 없으므로, iCampus 공지/과제 및 평가/GradeScope를 계속 확인해야 함.
