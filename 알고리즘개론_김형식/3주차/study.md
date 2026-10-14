# 알고리즘개론 - 김형식 3주차 스터디

제출물 연결: [recursion tree·master method 증명 PDF 목차](../proof-study.md).

## 빠른 목차

이 문서는 3주차 원본 콘텐츠를 한 파일에 모은 학습용 교안이다. 아래 표에서 먼저 **어떤 수업/외부자료의 내용인지** 확인하고, 필요한 Part만 골라 읽으면 된다.

| Part | 원본 콘텐츠 | 이 문서의 범위 | 공부할 때 쓰는 용도 |
| --- | --- | --- | --- |
| A | iCampus: Solving recurrences, 04_SolvingRecurrences | 점화식의 의미와 공통 꼴 | 이번 주 전체 출발점 |
| B | iCampus: 05_Substitution, 06_Recursion, 07_Master | Substitution, Recursion Tree, Master Method 선택 | 점화식 문제 풀이 |
| C | 외부 영상: 3.2.1 Asymptotic Notation | ~, little-o, Big-O의 기본 관계 | 점근 표기 정의와 증명 |
| D | 외부 읽을거리 + 점근표기 보강 | Big-O의 엄밀한 정의, limsup, Theta 관계 | O와 o가 헷갈릴 때 |
| E | 외부 영상: Proving the Master Theorem | 재귀트리와 등비급수로 Master 정리 유도 | Master 공식이 왜 맞는지 |
| F | iCampus 강의 + 이번 주 과제 | 일반 Master Method, regularity, 과제 2개 연결 | 제출물 복습 |
| G | 이번 주 퀴즈 대비 | 흔한 함정과 영어 답안 뼈대 | 시험·퀴즈 직전 |
| H | iCampus: 8. Binary Heaps | Priority Queue, Heap, Heap Sort, build heap | Binary Heap 영상·퀴즈 |

## 권장 학습 순서

1. 처음 공부: Part A → Part B → Part C → Part E
2. 점근 표기가 헷갈릴 때: Part C → Part D
3. 과제나 서술형 복습: Part F → Part G
4. Binary Heap 공부: Part H만 독립적으로 읽기

---

## 0. 이번 주 범위와 자료 상태

이번 주는 재귀 알고리즘의 실행 시간을 분석하는 방법을 한 덩어리로 배우는 주차다. Merge Sort에서 이미 본 “재귀로 쪼개고 다시 합친다”는 구조를 일반화해, 어떤 점화식이 어떤 성장률을 갖는지 설명한다.

```text
Divide and Conquer 알고리즘
        ↓
실행 시간을 T(n)으로 표현
        ↓
점화식(recursion / recurrence) 작성
        ↓
Substitution 또는 Recursion Tree로 풀이
        ↓
Master Theorem으로 특정 꼴을 빠르게 판정
        ↓
O, Theta, o, ~ 로 결과의 성장 관계를 정확히 표현
```

이번 문서는 다음 자료를 함께 반영한다.

- iCampus의 `Solving recurrences`, `04_SolvingRecurrences`, `05_SolvingRecurrences_substitution`, `06_SolvingRecurrences_Recursion`, `07_SolvingRecurrences_Master`
- 외부 영상 `3.2.1 Asymptotic Notation: Video` 전문 스크립트
- 외부 읽을거리 `big-O and little-o`
- 외부 영상 `Proving the Master Theorem` 전문 스크립트
- 이번 주에 푼 Recursion Tree / Substitution 과제와 Master Method 과제

Binary Heap과 Heap 퀴즈는 아직 이 문서에 합치지 않는다. 그건 재귀 분석의 연장이 아니라 다음 자료구조 단원의 새 내용이므로, 실제 강의자료를 본 뒤 이 문서 뒤에 별도 장으로 추가한다.

---

# Part A. [iCampus 강의] Solving Recurrences

## 1. 왜 실행 시간을 “점화식”으로 쓰는가

재귀 알고리즘은 자기보다 작은 입력에 대해 같은 문제를 다시 푼다. 그래서 한 번의 호출만 세어서는 전체 시간을 알 수 없다. 현재 호출이 하는 일과 더 작은 호출들이 하는 일을 합쳐야 한다.

예를 들어 Merge Sort는 길이 `n` 배열을 절반 두 개로 나누고, 각각을 정렬한 다음 선형 시간에 merge한다.

```text
T(n) = 2T(n / 2) + Theta(n)
```

- `2T(n/2)`: 크기 `n/2`인 부분문제를 두 개 푼다.
- `Theta(n)`: 두 정렬 결과를 합치는 merge 작업이다.
- 보통 `T(1)=Theta(1)` 같은 base case도 함께 둔다.

점화식의 목적은 정확히 몇 밀리초가 걸리는지를 예측하는 것이 아니다. 하드웨어, 컴파일러, 구현 상수에 흔들리지 않고 입력 크기가 커질 때 어느 항이 실행 시간을 지배하는지 보는 것이다. 그래서 결론은 대개 `Theta(n log n)`, `Theta(n^2)`처럼 쓴다.

### 1.1 Divide and Conquer 점화식의 공통 꼴

가장 자주 만나는 형태는 아래다.

```text
T(n) = aT(n / b) + f(n)
```

| 기호 | 의미 |
| --- | --- |
| `a` | 한 번의 호출이 만드는 부분문제 개수 |
| `b` | 입력이 줄어드는 비율. 각 부분문제 크기는 `n/b` |
| `f(n)` | 재귀 호출을 제외하고 현재 호출이 직접 하는 일 |
| `T(1)` | 더 이상 쪼개지지 않는 base case 비용 |

예를 들어:

- Binary Search: `T(n)=T(n/2)+Theta(1)`
- Merge Sort: `T(n)=2T(n/2)+Theta(n)`
- 재귀 호출 3개, 각 문제 크기 1/4, 현재 작업 `n^2`: `T(n)=3T(n/4)+Theta(n^2)`

`a`, `b`, `f(n)`을 분리해 읽는 습관이 중요하다. 문제에서 주어진 의사코드를 보면 “재귀 호출이 몇 개인가”, “각 호출의 입력이 얼마나 줄어드는가”, “루프/merge/복사 등 현재 호출의 추가 비용은 무엇인가”를 각각 표시한다.

---

# Part B. [iCampus 강의] Substitution / Recursion Tree / Master Method

## 2. 점화식을 푸는 세 가지 관점

### 2.1 Substitution Method: 답을 가정하고 귀납으로 검증

Substitution Method는 원하는 상한 또는 하한을 먼저 추측한 뒤, 수학적 귀납법처럼 성립을 보이는 방식이다.

기본 순서는 다음과 같다.

1. 작은 입력에서의 base case를 확인한다.
2. 예를 들어 `T(n) <= c n^2` 같은 가설을 세운다.
3. 재귀항 `T(n/b)`에 귀납 가설을 대입한다.
4. 상수 `c`와 충분히 큰 `n`을 골라 부등식이 닫히게 한다.
5. 상한과 하한이 모두 같은 차수로 나오면 `Theta` 결론을 쓴다.

예를 들어 `T(n)=2T(n/2)+n^2`에 대해 `T(n)=O(n^2)`를 보이고 싶다면, `T(n/2) <= c(n/2)^2`를 가정해

```text
T(n)
<= 2 * c(n/2)^2 + n^2
= (c/2)n^2 + n^2
```

를 얻는다. `c >= 2`처럼 충분히 큰 상수를 택하면 우변은 `c n^2` 이하가 된다. 이것이 “가정한 차수 안에서 재귀항이 다시 닫힌다”는 뜻이다.

Substitution은 답을 잘 추측해야 한다는 부담이 있지만, 정확한 증명 문장으로 쓰기에 좋다. 과제에서 “show that” 또는 “prove”가 나오면 단순히 Master Theorem 결과만 적지 말고, 어떤 귀납 가설을 대입했는지 보이는 편이 안전하다.

### 2.2 Recursion Tree: 각 레벨의 비용을 눈으로 분해

Recursion Tree는 재귀 호출을 트리로 펼쳐서 깊이별 총 비용을 더하는 방법이다.

```text
root:                    T(n)
                         own work

level 1:          T(n/b)       T(n/b)
                  own work     own work

level 2:       ... 여러 개의 더 작은 부분문제 ...
```

매 레벨에서 할 일은 네 가지다.

1. 깊이 `i`에서 노드가 몇 개인지 센다.
2. 각 노드의 입력 크기와 비재귀 비용을 쓴다.
3. 둘을 곱해 해당 레벨 총비용을 구한다.
4. 트리 높이와 leaf 비용까지 포함해 모든 레벨을 합친다.

Merge Sort를 예로 들면 깊이 `i`에서 노드는 `2^i`개, 각 노드의 merge 관련 비용은 `n/2^i`다.

```text
level i cost
= 2^i * (n / 2^i)
= n
```

모든 레벨의 비용이 `Theta(n)`이고 트리 높이가 `Theta(log n)`이므로 전체는 `Theta(n log n)`이다.

Recursion Tree는 Master Theorem이 바로 적용되지 않는 점화식에도 쓸 수 있다. 대표적으로 다음은 자식 크기가 서로 달라서 표준 Master Theorem의 `aT(n/b)` 꼴이 아니다.

```text
T(n) = T(n/3) + T(2n/3) + n
```

하지만 각 레벨에서 부분문제 크기의 합은 `n`이므로 해당 레벨의 선형 작업 총합은 `Theta(n)`으로 볼 수 있다. 깊이는 `Theta(log n)` 범위이므로 결과는 `Theta(n log n)`이다.

### 2.3 Master Theorem: 반복되는 균등 분할 꼴의 빠른 판정

Master Theorem은 `aT(n/b)+f(n)` 꼴에만 쓰는 도구다. 매우 편하지만 모든 점화식에 강제로 대입하면 안 된다.

적용 전 체크:

- 모든 재귀 호출의 크기가 같은가? 즉 모두 `n/b` 꼴인가?
- 재귀 호출 개수가 일정한 `a`개인가?
- `a`, `b`, `f(n)`을 분명히 읽었는가?
- Case 3이면 regularity condition까지 확인했는가?

`T(n/3)+T(2n/3)+n`처럼 부분문제 크기가 다르면 Master Theorem을 쓰지 않고 Recursion Tree나 다른 경계 방법을 택한다. 도구 선택 자체가 문제 풀이의 일부다.

---

# Part C. [외부 보조영상] 3.2.1 Asymptotic Notation

## 3. 점근 표기법: 알고리즘 성장률을 비교하는 네 관계

외부 `3.2.1 Asymptotic Notation` 영상은 함수의 성장률 관계 네 가지를 소개한다. 이들은 특정 `n`에서의 숫자를 비교하는 표기가 아니라, `n -> infinity`에서 함수 전체가 어떻게 비교되는지를 말한다.

```text
~        거의 같은 성장
o        엄밀히 더 느린 성장
O        결국 상수배 이내의 상한
Theta    같은 차수의 성장
```

### 3.1 Asymptotic Equivalence: `f ~ g`

정의:

```text
f(n) ~ g(n)
iff lim[n -> infinity] f(n) / g(n) = 1
```

두 함수의 비율이 1로 가면, 큰 입력에서는 서로의 선도항까지 사실상 같은 크기라는 뜻이다.

예시:

```text
n^2 + n ~ n^2
```

왜냐하면

```text
(n^2 + n) / n^2 = 1 + 1/n -> 1
```

이기 때문이다. 낮은 차수항 `n`은 `n^2`에 비해 점점 영향이 사라진다.

#### `~`의 성질

`~`는 함수들 사이의 equivalence relation(등가 관계)이다.

- Reflexive: `f ~ f`. 비율 `f/f`는 1이다.
- Symmetric: `f ~ g`이면 `g ~ f`. `g/f = 1/(f/g)`이고 극한은 `1/1=1`이다.
- Transitive: `f ~ g`, `g ~ h`이면 `f ~ h`. `f/h = (f/g)(g/h)`이고 두 비율의 극한이 모두 1이므로 곱의 극한도 1이다.

따라서 `~`는 “같은 성장률 묶음”을 만드는 관계다. 여기서 `f(n) ~ g(n)`이라고 자주 쓰지만, 엄밀하게는 특정 입력값 두 개가 아니라 함수 `f`, `g` 사이의 관계다.

#### `~`와 `Theta`는 다르다

`f ~ g`이면 반드시 `f=Theta(g)`지만, 반대는 성립하지 않는다.

```text
2n = Theta(n)
2n is not asymptotically equivalent to n
```

비율 `2n/n`은 2로 가므로 같은 차수이지만 1로 가지 않는다. 즉 `~`가 `Theta`보다 더 강한 주장이다.

### 3.2 Little-o: `f=o(g)`

정의:

```text
f(n) = o(g(n))
iff lim[n -> infinity] f(n) / g(n) = 0
```

이는 `f`가 `g`보다 엄밀히 느리게 성장한다는 뜻이다.

예시:

```text
n^2 = o(n^3)
```

```text
n^2 / n^3 = 1/n -> 0
```

Little-o의 `o`는 Big-O의 작은 글자 표기가 아니다. “상수배를 아무리 작게 잡아도, 충분히 큰 입력부터 `f`가 그 상수배 `g`보다 작아진다”는 더 강한 의미다.

양의 함수에 대한 정량적 정의는 다음과 같다.

```text
f = o(g)
iff for every c > 0,
    there exists n0 such that
    for all n >= n0, f(n) <= c g(n)
```

여기서 핵심은 `for every c > 0`다. `c`를 100으로 잡는 것도, 0.01로 잡는 것도 가능해야 한다. 물론 `c`가 바뀌면 필요한 `n0`도 달라질 수 있다.

#### `o`의 성질

- `f=o(f)`는 성립하지 않는다. 비율 `f/f`는 1이기 때문이다.
- `f=o(g)`, `g=o(h)`이면 `f=o(h)`이다.
- 이런 의미에서 Little-o는 strict partial order(엄격한 부분 순서)처럼 동작한다.
- `f=o(g)`이면 `f=O(g)`다. 반대는 성립하지 않는다. 예를 들어 `n=O(n)`이지만 `n`은 `o(n)`이 아니다.

### C3. Big-O: `f=O(g)` (외부 영상 개요)

Big-O의 표준 정의는 극한이 아니라 상수 `c`, `n0`를 사용한다.

```text
f(n) = O(g(n))
iff there exist c > 0 and n0 such that
    for all n >= n0, f(n) <= c g(n)
```

즉 충분히 큰 모든 입력부터 `f`가 `g`의 어떤 고정 상수배를 넘지 않는다는 뜻이다. 알고리즘 분석에서 상수배가 하드웨어·구현·컴파일러에 따라 달라질 수 있으므로, 이 관계가 특히 중요하다.

예시:

```text
3n^2 = O(n^2)
```

```text
3n^2 <= 3 * n^2
```

이므로 `c=3`, `n0=1`을 고를 수 있다. “상수 3을 무시한다”는 말은 3이 없어졌다는 뜻이 아니라, 성장 차수 분류에서 고정 상수배를 따로 구분하지 않는다는 뜻이다.

#### Big-O와 보통 극한의 함정

비율의 보통 극한이 존재하고 유한하면 Big-O를 보이기에 충분하다.

```text
if lim f(n)/g(n) = L < infinity,
then f(n) = O(g(n))
```

예를 들어 비율이 3으로 가면 결국 어떤 상수보다 작게 묶을 수 있다.

하지만 “비율의 보통 극한이 유한하게 존재한다”는 것은 Big-O의 필요조건이 아니다. 극한이 아예 존재하지 않아도 Big-O일 수 있다.

반례:

```text
f(n) = 0  if n is even
       1  if n is odd

g(n) = 1
```

`f(n)/g(n)`은 0과 1을 계속 오가므로 일반적인 극한이 없다. 그래도 모든 `n`에 대해 `f(n) <= g(n)`이므로 `f=O(g)`다.

따라서 Big-O를 “비율의 limit이 유한하면 되는 관계”라고만 외우면 틀릴 수 있다.

# Part D. [외부 보조자료 심화] Big-O / limsup / Theta

## D1. limsup: 극한이 흔들릴 때 Big-O를 정확히 말하는 법

외부 읽을거리는 위 문제를 해결하려고 limit superior, 즉 `limsup`를 설명한다. 함수 `h(n)`에 대해

```text
limsup[n -> infinity] h(n)
= lim[n -> infinity] sup { h(j) : j >= n }
```

이다.

- `sup`는 least upper bound(최소 상계)다.
- `sup {h(j): j>=n}`는 `n` 이후 모든 값 중 가장 높은 경계다.
- `max`는 실제 최대값이 없을 수 있지만, `sup`는 존재한다. 예를 들어 `1-2^(-n)`은 1에 가까워지지만 1을 실제로 찍지 않으므로 최대값은 없고 supremum은 1이다.
- `n`을 크게 할수록 살펴보는 tail이 줄어드므로, 이 tail supremum들의 극한이 limsup다.

양의 함수에 대해 다음이 정확하다.

```text
f(n) = O(g(n))
iff limsup[n -> infinity] f(n)/g(n) < infinity
```

한 방향은 쉽다. `f(n) <= c g(n)`가 충분히 큰 모든 `n`에 대해 성립하면, 그 이후 tail의 모든 비율이 `c` 이하이므로 limsup도 유한하다.

반대 방향도 성립한다. limsup가 유한하면 그보다 조금 큰 상수 `c`를 골랐을 때, 충분히 뒤에서는 모든 비율이 `c` 이하가 된다. 이것이 바로 Big-O 정의의 `c`, `n0`다.

또한 Little-o는 다음처럼도 표현된다.

```text
f(n) = o(g(n))
iff limsup[n -> infinity] f(n)/g(n) = 0
```

실전 과제에서는 보통 `c`, `n0` 증명이나 간단한 극한 계산이 더 자연스럽다. limsup는 “왜 보통 극한이 없다고 Big-O가 실패하는 것은 아닌가”를 정확하게 설명해 주는 개념이다.

### 3.5 Theta: `f=Theta(g)`

정의:

```text
f(n) = Theta(g(n))
iff f(n) = O(g(n)) and g(n) = O(f(n))
```

즉 `f`가 `g`의 상수배보다 커지지 않을 뿐 아니라, `g`도 `f`의 상수배보다 커지지 않는다. 둘은 위아래에서 같은 차수로 묶인다.

정량적으로는 양의 상수 `c1`, `c2`, `n0`가 존재해서 충분히 큰 모든 `n`에 대해

```text
c1 g(n) <= f(n) <= c2 g(n)
```

가 되는 관계다.

예시:

```text
2n^2 + n log n + 1 = Theta(n^2)
```

상한에서는 `n log n <= n^2`가 충분히 큰 `n`에서 성립하므로 전체를 어떤 `n^2` 상수배로 묶을 수 있다. 하한에서는 `2n^2 + n log n + 1 >= 2n^2`이므로 이미 `n^2`의 상수배보다 크다.

`Theta` 역시 reflexive, symmetric, transitive한 등가 관계다. 알고리즘 실행 시간을 정확한 차수까지 말했다면 가능하면 `O`보다 `Theta`를 쓰는 편이 정보량이 크다.

---

# Part E. [외부 보조영상] Proving the Master Theorem

## 4. Master Theorem을 재귀트리에서 유도하기

외부 `Proving the Master Theorem` 영상의 핵심은 공식을 외우는 것이 아니라, 모든 레벨 비용이 하나의 등비급수가 된다는 점이다.

대상은 다음과 같은 단순화된 꼴이다.

```text
T(n) = aT(n/b) + Theta(n^d)
```

편의를 위해 현재 호출의 비재귀 작업을 `c n^d`라고 생각하자. 트리의 깊이 `i`에서는

- 노드 수: `a^i`
- 각 노드의 입력 크기: `n/b^i`
- 각 노드의 비재귀 비용: `c(n/b^i)^d`

이다. 그러므로 깊이 `i` 전체의 비용은

```text
a^i * c(n/b^i)^d
= c n^d * (a/b^d)^i
```

가 된다.

```text
level 0:       c n^d
level 1:       c n^d (a / b^d)
level 2:       c n^d (a / b^d)^2
...
level L:       c n^d (a / b^d)^L
```

입력이 1이 될 때까지 `b`로 나누므로

```text
n / b^L = 1
b^L = n
L = log_b n
```

이다.

이제 `r=a/b^d`라고 놓으면 내부 레벨 비용의 합은

```text
c n^d (1 + r + r^2 + ... + r^L)
```

이다. 즉 Master Theorem의 세 경우는 등비급수의 공비 `r`이 1보다 작은지, 같은지, 큰지를 구분한 결과다.

### 4.1 r < 1: 위쪽, 즉 루트 근처가 지배

```text
a < b^d
r = a/b^d < 1
```

등비급수 `1+r+...+r^L`은 상수에 수렴하는 방향이다. 따라서 전체 비용은

```text
Theta(n^d)
```

가 된다. 현재 호출에서 하는 비재귀 작업이 아래쪽 재귀 호출들을 모두 합친 것보다 지배적이라는 뜻이다.

예시:

```text
T(n) = 2T(n/2) + n^2
```

여기서는 `a=2`, `b=2`, `d=2`이므로 `b^d=4`이고 `2<4`다. 따라서 루트 비용 `n^2` 쪽이 지배해 `Theta(n^2)`이다.

### 4.2 r = 1: 모든 레벨이 균등하게 기여

```text
a = b^d
r = 1
```

각 레벨 비용이 모두 `Theta(n^d)`다. 레벨 수가 `L+1=Theta(log n)`이므로

```text
Theta(n^d log n)
```

가 된다.

Merge Sort가 대표 예시다.

```text
T(n) = 2T(n/2) + n
a=2, b=2, d=1
a=b^d=2
=> Theta(n log n)
```

Binary Search도 같은 관점으로 볼 수 있다.

```text
T(n) = T(n/2) + Theta(1)
a=1, b=2, d=0
a=b^d=1
=> Theta(log n)
```

여기서는 각 레벨의 비용이 상수이고, 그 상수 비용이 `Theta(log n)`개 쌓인다.

### 4.3 r > 1: 아래쪽, 즉 leaf 근처가 지배

```text
a > b^d
r = a/b^d > 1
```

아래 레벨로 갈수록 레벨 비용이 커진다. 따라서 마지막 레벨 근처가 지배한다.

```text
n^d r^L
= n^d (a/b^d)^(log_b n)
= a^(log_b n)
= n^(log_b a)
```

그래서 결론은

```text
Theta(n^(log_b a))
```

가 된다. 이 단계에서 필요한 로그 성질은

```text
a^(log_b n) = n^(log_b a)
```

이며, 이는 양변에 로그를 취하거나 지수 규칙으로 확인할 수 있다. 영상의 말로 된 전개가 복잡하게 느껴져도, 이 식 하나가 leaf 비용을 표준 결과로 바꾸는 핵심이다.

### 4.4 영상의 case 번호와 교재의 case 번호를 섞지 않기

붙여준 영상은 `a<b^d`, `a=b^d`, `a>b^d` 순서로 사례를 설명한다. 일반 교재의 Master Theorem은 보통 `f(n)`과 `n^(log_b a)`를 비교하며 Case 1, 2, 3을 부르기 때문에 번호 순서가 달라 보일 수 있다.

시험에서는 “Case 몇 번”이라는 단어만 믿지 말고 항상 다음을 적는다.

```text
a = ?
b = ?
f(n) = ?
n^(log_b a) = ?
f(n)과 기준항 중 무엇이 더 큰가?
```

그 다음에 루트 지배, 전 레벨 균등, 리프 지배 중 어느 그림인지 확인하면 된다.

---

# Part F. [iCampus 강의와 과제 연결] Master Method 적용

## 5. 일반 Master Theorem의 시험용 형태

일반형은 다음이다.

```text
T(n) = aT(n/b) + f(n)
```

기준항은 항상

```text
n^(log_b a)
```

이다.

| 경우 | 비교 | 결론 |
| --- | --- | --- |
| 재귀항 지배 | `f(n)=O(n^(log_b a-epsilon))` | `Theta(n^(log_b a))` |
| 균형 | `f(n)=Theta(n^(log_b a))` | `Theta(n^(log_b a) log n)` |
| 비재귀항 지배 | `f(n)=Omega(n^(log_b a+epsilon))` 및 regularity | `Theta(f(n))` |

여기서 `epsilon>0`은 다항식 차이(polynomial gap)가 있어야 한다는 뜻이다. 예를 들어 `n^3`은 `n`보다 다항식 차이로 크지만, `n log n`과 `n`은 단순히 로그가 하나 더 있는 관계라 어떤 확장 정리를 써야 하는지 주의해야 한다. 이번 과제의 `3T(n/4)+n log n`은 비재귀항 지배로 다뤘고 regularity condition을 함께 확인했다.

### 5.1 Regularity Condition은 왜 필요한가

Case 3에서는 단지 `f(n)`이 커 보인다는 말로 충분하지 않다. 보통 다음과 같은 조건을 확인한다.

```text
a f(n/b) <= c f(n)
for some c < 1 and all sufficiently large n
```

뜻은 한 레벨 아래의 비재귀 총비용이 현재 레벨 비용의 고정된 작은 비율 이하라는 것이다. 그러면 아래로 갈수록 비용이 충분히 줄어들어 루트 근처가 정말 지배한다고 말할 수 있다.

이번 예시:

```text
T(n) = 3T(n/4) + n log n
```

```text
3 f(n/4)
= 3 * (n/4) log(n/4)
= (3/4)n log(n/4)
```

이고 충분히 큰 `n`에서 `log(n/4) < log n`이므로 이는 `(3/4)n log n`보다 작다. 따라서 `c=3/4`에 해당하는 감소가 보이고, 결론은 `Theta(n log n)`이다.

---

## 6. 이번 주 과제 해설과 연결

### 6.1 Recursion Tree / Substitution 과제

| 점화식 | 레벨 비용 관찰 | 결론 |
| --- | --- | --- |
| `T(n)=4T(n/2)+100n` | 깊이 `i` 비용이 `100n*2^i`로 증가, 마지막 레벨이 지배 | `Theta(n^2)` |
| `T(n)=2T(n/2)+n^2` | 깊이 `i` 비용이 `n^2/2^i`로 감소, 루트가 지배 | `Theta(n^2)` |
| `T(n)=3T(n/4)+2n^2` | 깊이 `i` 비용이 `2n^2(3/16)^i`로 감소 | `Theta(n^2)` |
| `T(n)=T(n/3)+T(2n/3)+n` | 자식 크기는 달라도 각 완전 레벨의 크기 합이 `n` | `Theta(n log n)` |

첫 문제에서 leaf 수는 `4^(log_2 n)=n^2`이고, 두 번째 문제는 root의 `n^2`에서 아래로 갈수록 비용이 반씩 줄어든다. 세 번째의 `3/16`은 Master Theorem에서 말한 `a/b^d`와 동일한 역할을 한다.

네 번째는 Master Theorem의 균등 분할 꼴이 아니다. 이 차이를 보고 Recursion Tree를 고른 것이 핵심이다.

### 6.2 Master Method 과제

| 점화식 | 기준항 `n^(log_b a)` | 판단 | 결론 |
| --- | --- | --- | --- |
| `2T(n/2)+n` | `n` | 균형 | `Theta(n log n)` |
| `2T(n/2)+n^3` | `n` | 비재귀항 지배 + regularity | `Theta(n^3)` |
| `3T(n/4)+n log n` | `n^(log_4 3)` | 비재귀항 지배 + regularity | `Theta(n log n)` |

답안을 쓸 때는 결과만 적지 않고 `a`, `b`, `f(n)`, 기준항, case 판단, 최종 `Theta`를 순서대로 쓰면 영어 문제에서도 논리가 드러난다.

---

# Part G. [복습 · 퀴즈 대비] 함정과 영어 서술형

## 7. 이 주차에서 특히 헷갈리기 쉬운 것

### 7.1 Big-O는 정확한 실행 시간이 아니다

`O(n^2)`은 “항상 `n^2`초 걸린다”는 뜻이 아니다. 충분히 큰 입력에서 어떤 상수배 `n^2`보다 빨리 증가하지 않는다는 성장률 주장이다. 기계가 바뀌어 상수배가 달라도 알고리즘 자체의 차수 비교는 남는다.

### 7.2 O와 Theta를 섞지 않기

`Theta(n^2)`라면 당연히 `O(n^2)`이기도 하고 `O(n^3)`이기도 하다. 하지만 가장 정확한 차수를 물을 때 `O(n^3)`만 쓰면 너무 느슨하다. 상한과 하한을 함께 안다면 `Theta`를 쓴다.

### 7.3 “극한이 없으니 Big-O도 아니다”는 틀리다

홀짝에 따라 값이 흔들리는 함수처럼 비율의 보통 극한은 없어도, 비율 전체가 어느 상수 아래에 있으면 Big-O다. 이때 limsup 또는 원래의 `c,n0` 정의로 판단한다.

### 7.4 Master Theorem을 모든 재귀식에 쓰지 않기

`T(n/3)+T(2n/3)+n`처럼 부분문제 크기가 다르면 표준 Master Theorem 조건을 만족하지 않는다. 이때 억지로 `a=2`, `b=?`를 정하면 안 된다.

### 7.5 Case 3은 regularity까지

`f(n)`이 기준항보다 커 보여도, 표준 Master Theorem의 Case 3 결론을 쓰려면 regularity condition을 확인한다. 수업 과제에서 이 부분을 적어 둔 이유가 있다.

---

## 8. 시험·퀴즈에서의 최소 답안 뼈대

### 점근 표기 증명

```text
To show f(n) is O(g(n)),
choose c = ...
For all n >= n0 = ...,
f(n) <= c g(n).
Therefore, f(n) is O(g(n)).
```

```text
To show f(n) is Theta(g(n)),
show both:
c1 g(n) <= f(n)
and
f(n) <= c2 g(n)
for all n >= n0.
Therefore, f(n) is Theta(g(n)).
```

### Master Theorem

```text
T(n) = aT(n/b) + f(n)

a = ...
b = ...
f(n) = ...
n^(log_b a) = ...

Since ...
[case and, when needed, regularity condition]

Therefore, T(n) = Theta(...).
```

### Recursion Tree

```text
At level i:
number of nodes = ...
cost per node = ...
total level cost = ...

The height is ...
Summing all levels gives ...
Therefore, T(n) = Theta(...).
```

이 형태를 영어로 바로 쓸 수 있게 익혀 두면, 지난 퀴즈처럼 의사코드·증명·시간복잡도를 한 문제에 섞어 물어도 답의 뼈대가 먼저 잡힌다.

---

## 9. 현재 시점의 남은 학습

재귀 분석과 점근 표기 보조자료는 이 문서에 반영 완료다. 실제 iCampus 진행률은 영상 재생 완료 여부와 별개이므로, `3.2.1 Asymptotic Notation: Video`와 `Proving the Master Theorem`은 화면에서 완료 표시를 확인한다.

이번 주에 새로 공부할 내용은 다음이다.

1. `8. Binary Heaps` 영상: 배열 기반 complete binary tree, heap-order property, insert와 extract 동작을 확인한다.
2. `[Quiz] Binary Heaps`: 영상 정리 직후 4문제를 푼다.
3. Heap 자료를 받으면 이 문서의 다음 장에 heap structure와 연산 흐름을 추가한다.

즉 지금 이 노트의 재귀·점근표기 파트는 닫혔고, Binary Heap이 들어오면 3주차 전체 노트가 완성된다.


---

# Part H. [iCampus 강의] 8. Binary Heaps

## 10. Binary Heap: Priority Queue와 In-place Heap Sort

### 10.1 이 단원은 무엇을 해결하는가

Binary Heap은 우선순위 큐(priority queue)를 구현하는 자료구조다. 우선순위 큐는 모든 원소의 전체 정렬 순서를 항상 유지할 필요는 없고, 가장 우선순위가 높은 원소를 빠르게 꺼내는 것이 목표다.

이번 강의에서는 key가 큰 원소의 우선순위가 더 높다고 두는 max-priority queue를 사용한다.

~~~text
Priority Queue interface

build(A)       여러 원소로 우선순위 큐를 만든다.
insert(x)      key를 가진 항목 x를 넣는다.
find_max()     최대 key 항목을 확인한다.
delete_max()   최대 key 항목을 제거하면서 반환한다.
~~~

우선순위 큐가 필요한 예시는 다음과 같다.

- Router: 우선순위가 높은 packet을 먼저 전송한다.
- Operating System scheduler: 실행 대기 process 중 우선순위가 높은 것을 먼저 고른다.
- Event simulation: 발생 시간이 가장 이른 event를 다음으로 처리한다.
- Graph algorithm: 이후 다룰 최단 경로와 최소 신장 트리 계열에서 다음 후보를 선택한다.

Set interface 전체를 모두 빠르게 지원하려면 AVL tree처럼 강한 자료구조가 필요하다. 하지만 priority queue는 최댓값을 찾고 삭제하는 일만 중요하므로, 더 단순한 구조를 선택할 수 있다. Binary Heap은 이 제한된 인터페이스에 맞춰 배열과 트리의 장점을 결합한 구조다.

---

### 10.2 Priority Queue 관점에서 기존 구조 비교

강의는 Heap으로 바로 들어가기 전에, 같은 priority queue interface를 여러 구조로 구현해 본다.

| 구현 | build | insert | delete_max | 핵심 특징 |
| --- | ---: | ---: | ---: | --- |
| unsorted dynamic array | Theta(n) | Theta(1) amortized | Theta(n) | 넣기는 쉽지만 최댓값을 찾으려면 전부 훑는다 |
| sorted dynamic array | Theta(n^2) 방식으로 유지 | Theta(n) | Theta(1) amortized | 최댓값은 끝에 있지만 삽입 때 shift가 필요하다 |
| Set AVL | Theta(n log n) | Theta(log n) | Theta(log n) | 강력하지만 구조가 복잡하고 in-place가 아니다 |
| Binary Heap | Theta(n) bottom-up build | Theta(log n) | Theta(log n) | 배열 하나로 구현하며 Heap Sort에 쓸 수 있다 |

Unsorted array에서는 새 원소를 맨 뒤에 append하면 되므로 insert가 빠르다. 하지만 max가 어디 있는지 모르므로 delete_max 때 전체 배열을 scan해야 한다. max를 찾은 뒤에는 마지막 원소와 swap하고 마지막 위치를 삭제하면 된다.

Sorted array는 반대다. 오름차순 배열이라면 max는 항상 맨 뒤에 있으므로 find_max와 delete_max는 빠르다. 그러나 새 원소의 위치를 찾고 그 뒤 원소들을 shift해야 하므로 insert가 선형 시간이다. Binary search로 위치를 빨리 찾아도 shift 비용은 사라지지 않는다.

Heap의 목표는 이 둘의 중간을 잡는 것이다.

~~~text
unsorted array:  insert fast, delete_max slow
sorted array:    insert slow, delete_max fast
binary heap:     insert and delete_max both log n
~~~

상수 시간 두 개를 동시에 얻을 수는 없지만, 둘 다 Theta(log n)이면 충분히 좋다.

---

### 10.3 Priority Queue Sort가 기존 정렬을 하나로 묶는 방식

어떤 priority queue 구현이든 다음 방식으로 정렬을 만들 수 있다.

~~~text
PriorityQueueSort(A)
    Q = build(A)
    repeat n times
        output delete_max(Q)
    reverse output
~~~

delete_max는 큰 것부터 반환하므로, 그대로 받으면 내림차순이다. 마지막에 reverse하면 오름차순 정렬이 된다. Heap Sort에서는 원배열의 뒤쪽에 최대값을 차례로 놓아 reverse 과정 없이 오름차순을 만든다.

이 틀에 자료구조를 끼우면 이미 아는 정렬들이 나온다.

| priority queue 구현 | insert all + delete_max 반복이 만드는 정렬 |
| --- | --- |
| unsorted array | Selection Sort |
| sorted array | Insertion Sort |
| Set AVL | AVL Sort |
| binary heap | Heap Sort |

#### Unsorted array가 Selection Sort인 이유

원소를 입력 순서대로 배열에 넣은 뒤, 매번 전체 배열을 훑어서 가장 큰 값을 찾는다. 그 max를 현재 범위의 맨 뒤 원소와 swap하고 범위를 하나 줄인다. 이것이 Selection Sort의 동작과 같다.

#### Sorted array가 Insertion Sort인 이유

원소를 하나씩 넣을 때마다 현재 배열의 정렬 상태를 유지한다. 새 원소를 제자리까지 왼쪽으로 움직이는 과정이 Insertion Sort와 같다.

#### Heap이 만드는 Heap Sort의 목표

Heap은 n log n 비교 시간과 O(1) extra space를 함께 얻는다. Merge Sort는 Theta(n log n)이지만 보통 보조 배열이 필요하고, AVL Sort는 트리 노드와 pointer가 필요하다. Heap Sort는 입력 배열 자체를 Heap 저장공간과 최종 결과 저장공간으로 재사용한다.

---

### 10.4 Complete Binary Tree: 배열로 표현할 수 있는 트리 모양

Heap은 임의의 binary tree가 아니다. Complete Binary Tree의 모양을 강제한다.

정의:

1. 마지막 레벨을 제외한 모든 레벨은 완전히 채워져 있다.
2. 마지막 레벨의 노드는 왼쪽부터 빈틈없이 채운다. 이를 left-justified라고 한다.

~~~text
complete binary tree

depth 0:                 A
                        / \
depth 1:                B   C
                       / \ / \
depth 2:              D  E F  G
                     / \
depth 3:            H   I
~~~

마지막 레벨에 H, I까지만 있어도 왼쪽부터 채웠으므로 complete다. 반대로 마지막 레벨의 왼쪽 자리는 비어 있는데 오른쪽에 노드가 있으면 complete가 아니다.

Complete Binary Tree는 노드 n개가 주어졌을 때 가능한 모양이 사실상 하나다. 위에서 아래로, 각 레벨에서 왼쪽에서 오른쪽으로 채우면 된다. 그래서 포인터를 저장하지 않아도 배열의 위치만으로 부모와 자식 관계를 계산할 수 있다.

트리 높이는 Theta(log n)이다. AVL처럼 rotation으로 균형을 맞출 필요가 없다. complete 조건 자체가 최선에 가까운 균형을 자동으로 보장한다.

---

### 10.5 Level Order Array Representation

Complete Binary Tree의 노드를 level order, 즉 깊이가 얕은 것부터 왼쪽에서 오른쪽으로 배열에 적는다.

~~~text
tree:                   A
                       / \
                      B   C
                     / \ / \
                    D  E F  G
                   / \
                  H   I

array index:           0 1 2 3 4 5 6 7 8
array value:           A B C D E F G H I
~~~

배열과 complete binary tree 사이에는 일대일 대응이 있다.

- 길이 n인 배열 하나가 주어지면, 그 위치를 level order로 읽어 unique한 complete binary tree를 만들 수 있다.
- n개 노드를 가진 complete binary tree 하나가 주어지면, level order로 읽어 unique한 배열을 만든다.

이 때문에 Heap은 implicit data structure라고 부른다. 노드 안에 left, right, parent pointer를 저장하지 않고 원소 배열 하나만 저장한다.

#### 0-based index 공식

이 강의와 일반적인 Python/C 배열은 0부터 인덱싱한다고 두면 다음이 성립한다.

~~~text
parent(i)      = (i - 1) // 2       for i > 0
left_child(i)  = 2i + 1
right_child(i) = 2i + 2
~~~

예를 들어 index 1의 left child는 3, right child는 4다. index 4의 parent는 (4-1)//2=1이다.

자식이 실제로 존재하는지는 배열 범위로 검사한다.

~~~text
left_child(i) < heap_size
right_child(i) < heap_size
~~~

1-based 배열을 쓰는 교재에서는 left=2i, right=2i+1, parent=i//2가 된다. 두 공식을 섞으면 off-by-one 오류가 난다. 지금 강의의 배열 설명은 0-based 기준이다.

---

### 10.6 Max-Heap Property: 정렬된 배열이 아니라 부모가 더 큼

Complete Binary Tree의 모양만으로는 priority queue가 되지 않는다. key에 대한 조건을 하나 더 둔다.

~~~text
Max-Heap Property

For every node i:
key(parent(i)) >= key(i)
~~~

즉 부모의 key는 각 자식의 key보다 크거나 같다.

~~~text
          20
        /    \
      15      17
     /  \    /  \
    8   10  12   6
~~~

이 트리는 max-heap이다.

- 20은 15, 17보다 크거나 같다.
- 15는 8, 10보다 크거나 같다.
- 17은 12, 6보다 크거나 같다.

그러나 형제끼리의 순서는 전혀 요구하지 않는다. 15와 17 중 어느 쪽이 큰지는 상관없고, 왼쪽 subtree의 모든 원소가 오른쪽 subtree의 모든 원소보다 작아야 한다는 조건도 없다.

따라서 Heap은 Binary Search Tree가 아니다.

| 구조 | 핵심 조건 |
| --- | --- |
| Binary Search Tree | left subtree key <= node key <= right subtree key 같은 전역적 검색 순서 |
| Max-Heap | 부모 key >= 두 자식 key라는 지역적 순서 |

Heap 배열은 완전히 정렬되어 있지 않다. 예를 들어 [20, 15, 17, 8, 10, 12, 6]은 valid max-heap이지만 배열 자체가 내림차순도 오름차순도 아니다.

#### Root가 maximum인 이유

Max-heap property가 각 edge에서만 성립해도, root는 전체 max다. 어떤 descendant로 가는 path를 따라가면

~~~text
root >= child >= grandchild >= ... >= descendant
~~~

가 된다. 부등식의 transitivity 때문에 어떤 노드도 root보다 클 수 없다. 더 일반적으로, 어떤 노드는 자신의 subtree 안의 모든 descendant보다 크거나 같다.

그래서 find_max()는 Q[0]이고 시간은 Theta(1)이다.

#### Min-Heap

Min-heap은 모든 부등호 방향을 뒤집은 구조다.

~~~text
key(parent(i)) <= key(i)
~~~

그러면 root가 minimum이므로 find_min, delete_min에 적합하다. Max-heap으로 delete_min을 빠르게 할 수 없고, min-heap으로 delete_max를 빠르게 할 수 없다. 필요한 priority 방향에 맞춰 heap 종류를 정한다.

---

### 10.7 Insert: 끝에 추가하고 Heapify Up

새 원소는 배열의 맨 끝에 append해야 complete binary tree 모양을 유지할 수 있다. 이것은 새 노드를 마지막 레벨의 가장 오른쪽 빈 칸에 넣는 것과 같다.

하지만 새 key는 임의이므로 부모보다 커서 max-heap property를 깰 수 있다. 새 원소 외의 기존 노드들은 insertion 전부터 heap이었으므로, 문제는 새 원소에서 root로 올라가는 한 경로에만 있을 수 있다.

#### 알고리즘

~~~text
insert(x)
    append x at the end of Q
    max_heapify_up(last index)

max_heapify_up(i)
    while i > 0 and Q[parent(i)].key < Q[i].key
        swap Q[i] and Q[parent(i)]
        i = parent(i)
~~~

예를 들어 valid heap의 마지막에 key 22를 append했다고 하자.

~~~text
before bubbling:
          20
        /    \
      15      17
     /  \    / \
    8   10  12  6
   /
  22
~~~

22는 parent 8보다 크므로 swap한다. 그 뒤 22는 15보다 크므로 다시 swap한다. 마지막으로 20보다 크므로 root까지 올라간다. 각 swap은 현재 parent-child 관계를 고치고, 새 원소만 한 단계 위로 이동시킨다.

#### 왜 다른 부분은 망가지지 않는가

새 원소가 위로 올라갈 때, 아래로 내려가는 원래 parent는 새 원소를 제외한 자기 subtree의 원소보다 이미 크거나 같았다. 따라서 그 parent를 한 단계 아래로 내려도 그 subtree의 자식들과의 관계는 유지된다. 위쪽에는 새 원소만 새롭게 검사하면 된다.

이 불변식 때문에 rotation이 필요 없다. 삽입 원소 하나만 ancestor path를 따라 bubble up하면 된다.

#### 시간복잡도

한 번의 swap에서 원소는 정확히 한 level 올라간다. complete tree의 높이가 Theta(log n)이므로 insert는 Theta(log n)이다. 동적 배열을 일반적으로 쓰면 resize 때문에 amortized라고 부르지만, Heap Sort처럼 고정 입력 배열을 쓰면 resize 자체가 없다.

---

### 10.8 Delete Max: root를 마지막과 바꾸고 Heapify Down

Max-heap의 maximum은 root Q[0]에 있다. 그러나 배열에서 효율적으로 삭제할 수 있는 위치는 맨 마지막뿐이다.

따라서 delete_max는 다음의 두 문제를 동시에 해결한다.

1. root의 max를 반환해야 한다.
2. complete binary tree 모양을 유지해야 한다.

해법은 root와 마지막 원소를 swap한 다음, 마지막을 삭제하는 것이다.

~~~text
delete_max()
    swap Q[0] and Q[heap_size - 1]
    remove and return the old maximum at the last position
    max_heapify_down(0)
~~~

마지막 leaf를 root로 가져오면 complete shape는 유지된다. 다만 원래 아래쪽에 있던 비교적 작은 key가 root에 올라왔을 수 있으므로 max-heap property가 root에서 깨질 수 있다.

#### Heapify Down에서 왜 더 큰 자식과 바꾸는가

현재 위치 i의 두 child 중 key가 더 큰 child를 j라고 하자.

- Q[i] >= Q[j]라면 Q[i]는 두 child 모두보다 크거나 같으므로 local property가 성립한다.
- Q[i] < Q[j]라면 Q[i]와 Q[j]를 swap한다.
- 반드시 큰 child를 골라야 한다. 작은 child와 바꾸면 다른 child가 새 parent보다 더 클 수 있어 property가 남는다.

#### 알고리즘

~~~text
max_heapify_down(i)
    while i has at least one child
        j = index of the existing child with maximum key
        if Q[i].key >= Q[j].key
            stop
        swap Q[i] and Q[j]
        i = j
~~~

right child가 없을 수 있으므로, 실제 구현에서는 left child 존재 여부를 먼저 검사하고 right child가 존재할 때만 둘을 비교한다.

예를 들어 root가 0이고 children이 5와 10이면, 0은 10과 swap해야 한다.

~~~text
before:              0
                     / \
                    5  10

swap with 10:       10
                     / \
                    5   0
~~~

5와 swap하면 root가 5가 되어 10보다 작으므로 여전히 heap property가 깨진다. 10을 올리면 root 주변의 두 edge가 한 번에 맞고, 0만 그 아래에서 계속 검사하면 된다.

##### 시간복잡도와 정확성

처음에는 last leaf 하나만 root로 이동했으므로, 다른 위치는 이미 올바른 heap이다. swap할 때마다 문제 원소 하나만 아래로 이동한다. leaf에 도달하거나 child들보다 크거나 같아지는 순간 멈추며 heap이 복구된다.

한 번에 한 level씩만 내려가므로 delete_max는 Theta(log n)이다.

---

### 10.9 Heap Sort: 배열 자체를 Heap과 출력 공간으로 같이 쓰기

Heap Sort는 priority queue sort를 진짜 in-place로 만드는 방법이다. 별도의 dynamic array를 새로 만들지 않고, 원래 배열 A의 prefix를 현재 heap Q로 해석한다.

~~~text
A = [ heap prefix | already sorted suffix ]
~~~

초기에는 heap size가 0이고, prefix를 늘리며 모든 원소를 heap에 넣을 수 있다. 더 효율적으로는 배열 전체를 bottom-up build해서 바로 heap으로 만든다.

그 뒤 delete_max를 반복하면 다음 일이 일어난다.

1. heap root의 maximum을 heap 마지막 위치와 swap한다.
2. heap size를 하나 줄인다.
3. 줄어든 heap 영역에서 heapify down한다.
4. 방금 빠진 maximum은 배열의 맨 뒤 fixed sorted suffix에 남는다.

~~~text
initial:      [ heap of all n items ]

after 1st delete_max:
              [ heap of n-1 items | largest ]

after 2nd delete_max:
              [ heap of n-2 items | second largest | largest ]

final:        [ smallest ... largest ]
~~~

Max-heap을 쓰면 largest부터 배열의 오른쪽 끝에 확정되므로 최종 배열은 오름차순이다. Min-heap으로 같은 방식으로 끝에 밀어내면 내림차순이 되므로, 원하는 결과 방향을 생각해야 한다.

#### Heap Sort pseudocode

~~~text
heapsort(A)
    build_max_heap(A)
    heap_size = length(A)

    while heap_size > 1
        swap A[0], A[heap_size - 1]
        heap_size = heap_size - 1
        max_heapify_down(A, 0, heap_size)
~~~

- heap 영역: A[0 ... heap_size-1]
- 이미 확정된 정렬 영역: A[heap_size ... n-1]
- heapify_down은 반드시 줄어든 heap_size 안에서만 수행한다. 뒤쪽 sorted suffix를 heap의 일부로 보면 안 된다.

#### Heap Sort 복잡도

- build: bottom-up 방식이면 Theta(n)
- delete_max: n-1번, 각 Theta(log n)
- 전체: Theta(n log n)
- 추가 공간: index, temporary swap 변수 정도만 필요하므로 O(1)

Comparison model에서 정렬은 일반적으로 Omega(n log n) 비교가 필요하다. Heap Sort는 그 차수에 도달하면서도 in-place라는 장점이 있다.

---

### 10.10 Build Heap을 Theta(n)에 하는 Bottom-Up Trick

원소를 하나씩 insert하면서 heapify up하면 각 삽입이 최대 Theta(log n)이므로 전체는 Theta(n log n)이다.

~~~text
insert all one by one
= sum of insertion depths
= Theta(n log n)
~~~

더 좋은 방법은 배열을 처음에는 그냥 complete binary tree 모양으로 두고, 마지막 internal node부터 root 방향으로 heapify down을 하는 것이다.

~~~text
build_max_heap(A)
    for i = parent(last index) down to 0
        max_heapify_down(A, i)
~~~

0-based 배열에서 마지막 internal node는 parent(n - 1) = (n - 2) // 2다. leaf는 자식이 없으므로 이미 heap property를 만족한다. leaf의 부모부터 차례로 heapify down하면, 어떤 node를 처리할 때 그 children의 subtree는 이미 heap이다. 따라서 그 node만 내려 보내면 전체 subtree가 heap이 된다.

#### 왜 총합이 선형인가

root 근처 node는 멀리 내려갈 수 있어 비싸지만 개수가 매우 적다. 반대로 leaf 근처 node는 매우 많지만 내려갈 높이가 거의 없다.

높이 h인 node 수는 대략 n / 2^(h+1)개이고, 각 node의 heapify down 비용은 최대 O(h)다. 따라서 총합은 대략

~~~text
sum over h >= 0 of (n / 2^(h+1)) * h
= n * sum over h >= 0 of h / 2^(h+1)
= O(n)
~~~

이다. 무한 등비급수 계열의 합이 상수로 수렴하므로 build_max_heap은 Theta(n)이다.

이 부분은 모든 node가 log n만큼 내려간다고 세면 틀린다는 것을 보여 준다. 대부분의 node는 leaf에 가깝고 실제로는 거의 이동하지 않는다.

---

### 10.11 Heap의 핵심 불변식 두 개

Heap을 구현·추적할 때는 아래 둘을 분리해서 본다.

#### 1. Shape invariant

배열의 처음 heap_size개 원소는 complete binary tree를 나타낸다.

- insert는 맨 뒤에 추가한다.
- delete는 맨 뒤에서 제거한다.
- 그래서 중간에 빈 칸이 생기지 않는다.

#### 2. Order invariant

각 parent의 key는 child의 key보다 크거나 같다.

- insert 뒤에는 새 leaf에서 위로 올라가며 복구한다.
- delete_max 뒤에는 root에서 아래로 내려가며 복구한다.
- heapify 중 문제가 될 수 있는 원소는 하나뿐이다.

이 두 불변식을 함께 지키기 때문에 Heap이 배열인데도 tree 높이의 시간으로 작동한다.

---

### 10.12 공식 퀴즈: [Quiz] Binary Heaps

**상태:** 2026-09-20 21:00 시작됨. 4문항, 총 4점, 마감은 2026-09-27 23:59다. 현재 답안은 아직 작성하지 않은 상태로 기록한다.

이 아래 네 문항은 iCampus에 실제로 열린 퀴즈 원문이다. 나중에 복습할 때는 일반 예상 포인트보다 이 질문을 기준으로 답안을 준비한다.

1. **Priority queue can be implemented by using an array. Explain how “insert” and “delete_max” operations work and analyze their time complexity.**
2. **Describe a sorting algorithm using a priority queue and analyze its time complexity.**
3. **Describe how the “insert” operation works in a max binary heap.**
4. **Describe how the “delete_max” operation works in a max binary heap.**

### 10.13 퀴즈 문항과 교안의 연결

| 실제 문항 | 먼저 볼 교안 절 |
| --- | --- |
| 1. Array priority queue의 insert / delete_max | 10.2, 10.3 |
| 2. Priority queue를 이용한 sorting | 10.3, 10.9 |
| 3. Max binary heap insert | 10.7 |
| 4. Max binary heap delete_max | 10.8 |

### 10.14 퀴즈에서 나올 법한 확인 포인트

1. 배열을 트리로 읽기
   - 0-based index에서 parent, left child, right child를 계산할 수 있는가?
   - 어떤 index가 leaf인지 판단할 수 있는가?

2. Heap 여부 판단
   - complete shape와 max-heap property를 구분할 수 있는가?
   - sibling 순서가 자유롭다는 점을 아는가?
   - Heap은 BST나 sorted array가 아니라는 점을 아는가?

3. insert 추적
   - 끝에 먼저 append한다.
   - parent보다 큰 동안만 위로 swap한다.
   - 새 key 하나만 ancestor path를 따라 움직인다.

4. delete_max 추적
   - root와 마지막을 swap한다.
   - 마지막을 heap 영역 밖으로 보낸다.
   - 두 child 중 큰 child와 swap하며 내려간다.

5. 시간복잡도
   - find_max=Theta(1)
   - insert=Theta(log n)
   - delete_max=Theta(log n)
   - bottom-up build=Theta(n)
   - heapsort=Theta(n log n), extra space O(1)

6. Master Theorem과의 연결
   - Heap은 이번 주 후반의 새 자료구조 단원이다.
   - 그러나 bottom-up build가 선형이라는 설명은 앞에서 배운 레벨별 비용 합, 즉 많은 node는 얕은 비용만 낸다는 분석 관점과 이어진다.

---

### 10.15 한 장 구조도

~~~text
Priority Queue
    │
    ├─ insert(x)
    ├─ find_max()
    └─ delete_max()
            │
            ▼
      Binary Max-Heap
            │
    ┌───────┴────────┐
    ▼                ▼
Complete shape    Max-heap order
array level-order parent >= children
    │                │
    ▼                ▼
index arithmetic  root = maximum
no pointers       local property -> descendant property
    │                │
    └───────┬────────┘
            ▼
     Insert: append + heapify up
            │
            ▼
Delete max: root/last swap + heapify down
            │
            ▼
      Theta(log n) per operation
            │
            ▼
Bottom-up build: Theta(n)
            │
            ▼
Heap Sort: Theta(n log n), in-place
~~~

### 10.16 이번 주 3주차의 최종 연결

~~~text
Week 2
Insertion Sort Theta(n^2)
Merge Sort Theta(n log n)
        ↓
Week 3 front
점화식 / Recursion Tree / Master Theorem
성장률을 증명하고 읽는 법
        ↓
Week 3 back
Priority Queue / Binary Heap
Heap operations와 Heap Sort
        ↓
next
Heap을 기반으로 한 priority queue 활용과 이후 알고리즘
~~~

이제 3주차 내용은 재귀 알고리즘의 시간을 분석하는 법에서 새 자료구조로 정렬을 구현하는 법으로 넘어간다. Binary Heap은 모든 원소를 정렬된 상태로 유지하지 않으면서도 max를 즉시 확인할 수 있게 설계된 구조라는 한 문장으로 잡으면 된다.
