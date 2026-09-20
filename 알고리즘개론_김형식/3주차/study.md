# 알고리즘개론 - 김형식 3주차 스터디

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

### 3.3 Big-O: `f=O(g)`

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

### 3.4 limsup: 극한이 흔들릴 때 Big-O를 정확히 말하는 법

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
