# 알고리즘개론 - 김형식 2주차 학습지도

## 한줄 결론

2주차는 알고리즘 분석의 첫 본격 단원입니다. 지난주가 `Problem -> Algorithm -> Correctness -> Efficiency -> Model`이라는 기본 언어를 세팅한 주차였다면, 이번 주는 그중 `Efficiency`를 실제 정렬 문제에 적용합니다. 흐름은 `Sorting problem -> Insertion Sort -> Running Time -> Worst/Average/Best Case -> Asymptotic Analysis -> O/Omega/Theta/little-o -> Insertion Sort 분석 -> 더 빠른 정렬이 필요한 이유 -> Merge Sort -> Recurrence -> Recursion Tree -> Theta(n log n)`입니다.

이번 주 교수님 자료는 세 파일입니다. `Performance Analysis.pdf`가 메인 앞부분이고, `Merge Sort.pdf`가 메인 뒷부분입니다. `big_o.pdf`는 새로운 큰 단원이라기보다 Big O와 code complexity를 보충하는 참고자료로 보면 됩니다. MIT 강의는 수요일에 따로 듣고 추가 업데이트하는 것으로 둡니다.

## 상태

- 2026-09-07 월요일 알고리즘 수업 기준 2주차 자료 3개 반영.
- `Performance Analysis.pdf`, `Merge Sort.pdf`, `big_o.pdf` 기준으로 study 정리 완료.
- MIT 6.006 관련 강의/스크립트/노트는 수요일에 별도 반영 예정.
- 이 문서는 전문 transcript가 아니라, PDF 목차와 페이지 흐름을 따라 공부/시험/퀴즈용으로 다시 엮은 정리본입니다.

## 원본 자료

| 파일 | 역할 | 페이지 | 성격 |
| --- | --- | ---: | --- |
| `Performance Analysis.pdf` | 2주차 앞부분 메인 자료 | 47p | Sorting, insertion sort, running time, asymptotic notation, insertion sort analysis |
| `Merge Sort.pdf` | 2주차 뒷부분 메인 자료 | 39p | Divide and conquer, merge sort, recurrence, recursion tree, properties |
| `big_o.pdf` | Big O 보조자료 | 9p | Big O 정의, growth classes, performance vs complexity, code complexity 분석 |

## 2주차 전체 흐름

```text
Sorting
  ↓
Insertion Sort
  ↓
Running Time T(n)
  ↓
Worst / Average / Best Case
  ↓
Machine-independent Time
  ↓
Asymptotic Analysis
  ↓
O / Omega / Theta / little-o
  ↓
Insertion Sort = Theta(n^2)
  ↓
Can we do better?
  ↓
Divide and Conquer
  ↓
Merge Sort
  ↓
T(n) = 2T(n/2) + Theta(n)
  ↓
Recursion Tree
  ↓
Merge Sort = Theta(n log n)
```

2주차의 핵심 질문은 단순합니다. 정렬이라는 아주 기본적인 문제를 놓고, 왜 어떤 알고리즘은 작은 입력에서는 괜찮지만 큰 입력에서는 안 되는지, 그리고 왜 `n^2`보다 `n log n`이 좋은지 설명할 수 있어야 합니다. 정렬 자체보다 중요한 것은 정렬을 통해 알고리즘 분석의 언어를 배우는 것입니다.

## Part 1. Performance Analysis

### 1. The Problem of Sorting

`Performance Analysis.pdf`는 sorting problem 정의로 시작합니다. 입력은 숫자들의 sequence입니다. 예를 들어 `8 2 4 9 3 6`이 입력이면, 출력은 같은 원소들을 오름차순으로 재배열한 `2 3 4 6 8 9`입니다.

정렬 문제의 specification은 다음처럼 볼 수 있습니다.

```text
Input: sequence <a1, a2, ..., an> of numbers
Output: permutation <a'1, a'2, ..., a'n>
Condition: a'1 <= a'2 <= ... <= a'n
```

여기서 중요한 말은 `permutation`입니다. 출력은 그냥 오름차순 숫자 아무거나가 아니라, 입력에 있던 원소들을 빠뜨리거나 새로 만들지 않고 순서만 바꾼 결과여야 합니다. 즉 sorting의 correctness는 두 조건을 동시에 만족해야 합니다.

- 출력이 오름차순이어야 함.
- 출력이 입력의 permutation이어야 함.

이 두 번째 조건을 놓치면 `1 2 3 4 5 6`처럼 정렬되어 보이지만 입력과 다른 값을 내놓는 이상한 알고리즘도 통과해버립니다. 알고리즘 문제를 정의할 때 input/output 조건을 정확히 잡아야 하는 이유가 여기 있습니다.

### 2. Why Sorting?

PDF는 sorting이 왜 중요한지도 보여줍니다. obvious application으로는 MP3 library 정리, student records 유지가 나옵니다. 더 중요한 것은 정렬해두면 쉬워지는 문제들입니다.

- closest pairs 찾기
- binary search
- statistical outliers 식별
- duplicates 찾기
- computer graphics에서 front-to-back rendering

정렬은 그 자체가 목적일 때도 있지만, 다른 알고리즘의 전처리 단계로 쓰이는 경우가 많습니다. 예를 들어 binary search는 배열이 정렬되어 있을 때만 의미가 있습니다. 중복 탐지도 정렬 후 인접 원소만 비교하면 쉬워질 수 있습니다. 그래서 sorting은 알고리즘 수업에서 가장 자주 쓰이는 분석 예제입니다.

### 3. Insertion Sort의 기본 아이디어

Insertion sort는 이미 정렬된 부분을 유지하면서 새 원소를 적절한 위치에 삽입하는 알고리즘입니다. 자료의 첫 예시는 이미 정렬된 `2, 4, 5, 10`에 `7`을 넣는 상황입니다.

```text
2, 4, 5, 10   insert 7
2, 4, 5, 7, 10
```

동작은 뒤에서부터 비교하며 진행됩니다.

1. 새 원소 `7`과 마지막 원소 `10`을 비교합니다.
2. `7 < 10`이므로 `10`을 오른쪽으로 한 칸 shift합니다.
3. 다음 원소 `5`와 비교합니다.
4. `7 >= 5`이므로 그 뒤에 `7`을 넣습니다.

이때 핵심은 insertion sort가 매 순간 “앞쪽 구간은 이미 정렬되어 있다”는 invariant를 유지한다는 것입니다. `a[0:i-1]`이 정렬되어 있다고 보고, `t`를 이 구간 안의 알맞은 위치에 넣어 `a[0:i]`가 정렬되게 만듭니다.

PDF에 나온 insertion code는 다음 구조입니다.

```c
/* insert t into a[0:i-1] */
int j;
for (j = i - 1; j >= 0 && t < a[j]; j--)
    a[j + 1] = a[j];
a[j + 1] = t;
```

여기서 `j`는 새 원소가 들어갈 위치를 찾기 위해 왼쪽으로 움직입니다. `t < a[j]`인 동안 기존 원소를 오른쪽으로 밀어내고, 조건이 깨지는 지점 바로 뒤에 `t`를 넣습니다.

### 4. Insertion Sort 전체 동작

Insertion sort 전체는 크기 1짜리 sequence에서 시작합니다. 원소 하나짜리 배열은 이미 정렬되어 있습니다. 그 다음 남은 원소를 하나씩 정렬된 prefix에 삽입합니다.

예제 배열은 다음입니다.

```text
8 2 4 9 3 6
```

정렬 흐름은 다음처럼 보면 됩니다.

```text
[8] 2 4 9 3 6
[2 8] 4 9 3 6
[2 4 8] 9 3 6
[2 4 8 9] 3 6
[2 3 4 8 9] 6
[2 3 4 6 8 9]
```

중요한 것은 매 단계에서 왼쪽 prefix가 정렬되어 있다는 점입니다. Insertion sort의 correctness를 증명한다면 loop invariant를 사용할 수 있습니다.

```text
Invariant: i번째 반복이 시작될 때, A[0:i-1]은 원래 앞 i개 원소의 sorted permutation이다.
```

이 invariant가 유지되면 마지막에 `i = n`이 되었을 때 전체 배열이 sorted permutation이라는 결론이 나옵니다. 1주차에 배운 correctness proof가 여기서 바로 연결됩니다.

### 5. Running Time

자료는 running time이 input에 따라 달라진다고 말합니다. 이미 정렬된 sequence는 insertion sort에게 쉽습니다. 반대로 reverse sorted input은 매우 어렵습니다.

그래서 major simplifying convention으로 running time을 input size로 parameterize합니다.

```text
T_A(n) = time of algorithm A on length n inputs
```

여기서 `A`는 알고리즘이고, `n`은 입력 크기입니다. 같은 알고리즘이라도 입력 값의 배열 상태에 따라 실제 연산 횟수가 달라질 수 있지만, 일단 크기 `n`을 기준으로 함수를 잡습니다.

이번 주에서 정말 중요한 말은 “we seek upper bounds on the running time, to have a guarantee of performance”입니다. 알고리즘 분석에서 upper bound를 중요하게 보는 이유는 보장을 주기 때문입니다. 최악의 경우에도 이 정도 안에 끝난다고 말할 수 있어야 합니다.

### 6. Worst-case, Average-case, Best-case

PDF는 세 가지 분석을 구분합니다.

| 분석 | 의미 | 수업에서의 비중 |
| --- | --- | --- |
| Worst-case | size n의 모든 입력 중 최대 시간 | 보통 가장 중요 |
| Average-case | size n 입력들의 기대 시간 | 때때로 사용, 입력 분포 가정 필요 |
| Best-case | 가장 운 좋은 입력의 시간 | 거의 쓰지 않음 |

Worst-case는 안정적인 보장을 제공합니다. “어떤 입력이 와도 이 정도 시간 안에는 끝난다”를 말할 수 있습니다. Average-case는 현실적인 평균 성능을 설명할 수 있지만, 입력 분포를 어떻게 가정하느냐가 어렵습니다. Best-case는 느린 알고리즘도 운 좋은 입력 하나에서는 빨라 보일 수 있으므로 분석 기준으로 거의 의미가 없습니다.

Insertion sort를 예로 들면 이미 정렬된 배열은 best case입니다. 각 원소가 자기 자리에 있으므로 거의 비교만 하고 shift가 없습니다. 반대로 reverse sorted input은 worst case입니다. 새 원소마다 앞의 거의 모든 원소를 밀어야 합니다.

### 7. Machine-independent Time

실제 초 단위 실행시간은 기계에 의존합니다. CPU, 메모리, cache, compiler, 언어, OS 상태에 따라 같은 알고리즘도 다르게 측정됩니다. 그래서 알고리즘 수업에서는 machine dependent constants를 무시하고, `T(n)`의 growth를 봅니다.

자료의 핵심 표현은 다음입니다.

```text
Ignore machine dependent constants.
Look at growth of T(n) as n -> infinity.
```

이것이 asymptotic analysis입니다. 실제로 `n`이 작은 경우에는 상수나 낮은 차수 항이 더 중요할 수 있습니다. 하지만 알고리즘 수업의 목표는 입력이 커질 때 어떤 알고리즘이 구조적으로 더 나은지 판단하는 것입니다.

### 8. Asymptotic Performance

Asymptotic analysis는 중요한 factor만 남기고 lower order term과 constant를 무시합니다. 예를 들어 다음 함수가 있다고 합시다.

```text
T(n) = 5n^2 + 100n + 3000
```

작은 `n`에서는 `3000`이나 `100n`도 영향이 있을 수 있습니다. 하지만 `n`이 커질수록 `n^2` 항이 지배합니다. 그래서 이 함수의 order of growth는 `n^2`라고 봅니다.

이번 주에는 이 감각이 매우 중요합니다. 단순히 공식을 외우는 것이 아니라, “큰 입력에서 어떤 항이 최종적으로 지배하는가”를 봐야 합니다.

### 9. Order of Growth

자료는 성장률 분류를 다룹니다. 정리하면 다음 순서입니다.

```text
O(1)
O(log n)
O(n)
O(n log n)
O(n^2)
O(n^3)
O(2^n)
O(n!)
```

직관은 다음과 같습니다.

- `O(1)`: 입력 크기와 상관없이 일정.
- `O(log n)`: 입력이 커져도 매우 천천히 증가. binary search가 대표적.
- `O(n)`: 입력을 한 번 훑는 정도.
- `O(n log n)`: 좋은 비교 정렬에서 자주 등장. merge sort가 여기에 해당.
- `O(n^2)`: 이중 반복문이나 단순 정렬에서 자주 등장. insertion sort worst/average가 여기에 해당.
- `O(2^n)`, `O(n!)`: 입력이 조금만 커져도 급격히 폭발.

PDF는 `Theta(n^2)`와 `Theta(n^3)`를 비교하며, 충분히 큰 `n`에서는 `Theta(n^2)` 알고리즘이 항상 `Theta(n^3)`보다 낫다고 설명합니다. 여기서 “충분히 큰 `n`”이 asymptotic analysis의 핵심입니다.

### 10. Big O

Big O는 asymptotic upper bound입니다.

```text
f(n) = O(g(n))
iff there exist positive constants c and n0 such that
f(n) <= c * g(n) for all n >= n0
```

쉽게 말하면 `g(n)`이 `f(n)`의 위쪽 성장률 제한이라는 뜻입니다. 하지만 Big O는 bound가 얼마나 tight한지는 말하지 않습니다.

예를 들어 `n = O(n^2)`도 참이고, `n = O(n^3)`도 참입니다. 왜냐하면 충분히 큰 `n`에 대해 `n`은 `n^2`나 `n^3`보다 작게 묶일 수 있기 때문입니다. 하지만 알고리즘 분석에서는 가능한 한 작은 upper bound를 말하는 것이 좋습니다.

PDF의 예시는 다음입니다.

```text
f(n) = 25n
g(n) = (1/3)n^2
25n = O(n^2/3)
```

`c = 1`, `n0 = 75`를 잡으면 `25n <= n^2/3`이 `n >= 75`에서 성립합니다. 여기서 중요한 점은 `c`와 `n0`를 하나라도 찾으면 된다는 것입니다. 값이 유일할 필요는 없습니다.

### 11. Polynomial의 order

자료에는 다항식의 최고차항이 order를 결정한다는 정리가 나옵니다.

```text
If f(n) = ak n^k + ... + a1 n + a0,
then f(n) = O(n^k).
```

증명 아이디어는 각 낮은 차수 항을 `n^k`로 묶는 것입니다. `n >= 1`이면 `n^{k-1}`, `n`, `1` 같은 항들은 모두 `n^k` 이하로 볼 수 있습니다. 그래서 계수 절댓값들을 모두 더한 상수 `c`를 잡아 전체를 `c n^k`로 묶습니다.

예시:

```text
2n^2 = O(n^3)       true but loose
n^2 = O(n^2)        true and tight upper bound
1000n^2 + 1000n = O(n^2)
n = O(n^2)          true but loose
```

이 부분은 시험에서 “둘 다 참일 수 있는 이유”를 묻기 좋습니다. `O(n^3)`가 참이라고 해서 `O(n^2)`가 거짓이라는 뜻이 아닙니다. Big O는 상한이므로 더 느슨한 상한도 참이 될 수 있습니다.

### 12. No Uniqueness

Big O 증명에서 `c`와 `n0`는 유일하지 않습니다. PDF 예시는 `100n + 5 = O(n^2)`입니다.

한 방법은 `n0 = 5`, `c = 101`을 잡는 것입니다.

```text
100n + 5 <= 100n + n = 101n <= 101n^2   for all n >= 5
```

다른 방법은 `n0 = 1`, `c = 105`를 잡는 것입니다.

```text
100n + 5 <= 100n + 5n = 105n <= 105n^2   for all n >= 1
```

둘 다 맞습니다. 중요한 것은 “some constants `c` and `n0`”를 찾는 것입니다. 가장 예쁜 상수를 찾는 문제가 아니라, 정의를 만족하는 상수쌍이 존재함을 보이는 문제입니다.

### 13. Big Omega

Big Omega는 asymptotic lower bound입니다.

```text
f(n) = Omega(g(n))
iff there exist positive constants c and n0 such that
f(n) >= c * g(n) for all n >= n0
```

Big O가 “이보다 더 빨리 커지지는 않는다”라면, Big Omega는 “적어도 이 정도는 커진다”입니다. PDF는 `g(n)`이 lower bound이고, 가능한 한 큰 function을 잡는 것이 좋다고 설명합니다.

예를 들어 `n^2`는 `Omega(n)`도 맞지만, 더 정확하게는 `Omega(n^2)`라고 말할 수 있습니다. Lower bound도 너무 느슨하게 말할 수 있지만, 분석에서는 tight한 정보를 주는 쪽이 좋습니다.

### 14. Theta

Theta는 upper bound와 lower bound가 동시에 맞는 경우입니다.

```text
f(n) = Theta(g(n))
iff there exist positive constants c1, c2, n0 such that
c1 * g(n) <= f(n) <= c2 * g(n) for all n >= n0
```

즉 `g(n)`이 `f(n)`의 위와 아래를 동시에 묶습니다. 그래서 Big O와 Big Omega보다 더 precise합니다.

예를 들어 `n^2/2 - n/2 = Theta(n^2)`입니다. 충분히 큰 `n`에서는 이 함수가 어떤 상수배 `n^2`의 아래와 위 사이에 들어갑니다. 반대로 `n != Theta(n^2)`입니다. `n`은 `n^2`보다 훨씬 느리게 자라므로 아래쪽 bound가 성립하지 않습니다.

이 부분은 이렇게 외우면 됩니다.

```text
O: <= upper bound
Omega: >= lower bound
Theta: both, tight bound
```

### 15. Little o

Little o는 non-tight upper bound를 나타냅니다. PDF 표현을 정리하면 다음입니다.

```text
f(n) = o(g(n)) iff f(n) = O(g(n)) and f(n) is not Omega(g(n))
```

즉 `f`가 `g`보다 asymptotically strictly smaller하다는 뜻입니다.

예시:

```text
3n + 2 = o(n^2)
3n + 2 != o(n)
```

`3n + 2`는 `n^2`보다 훨씬 느리게 자라므로 `o(n^2)`입니다. 하지만 `n`과는 같은 선형 성장률이므로 `o(n)`은 아닙니다.

### 16. Polynomial vs Exponential Time

자료는 polynomial time과 exponential time도 비교합니다.

```text
Polynomial time: O(n^k)
Exponential time: O(k^n)
```

`O(n^3)`과 `O(2^n)`을 비교하면, 작은 입력에서는 `2^n`이 그럭저럭 보일 수 있어도 큰 입력에서는 지수 함수가 압도적으로 커집니다. 그래서 알고리즘 개선이 하드웨어 개선보다 더 중요하다는 메시지가 나옵니다.

PDF의 표현은 `Faster Computer vs Better Algorithm`입니다. 예를 들어 `2^n` 알고리즘을 더 빠른 컴퓨터에서 돌리는 것보다, 알고리즘 자체를 `n^3`으로 바꾸는 것이 훨씬 큰 개선입니다. 컴퓨터가 100배 빨라져도 exponential growth를 근본적으로 막지는 못합니다.

### 17. Insertion Sort Analysis

Insertion sort의 worst case는 reverse-sorted input입니다. 매번 새 원소를 맨 앞까지 넣어야 하므로, 앞의 원소들을 거의 전부 오른쪽으로 shift해야 합니다.

각 단계에서 비용을 대략 보면 다음과 같습니다.

```text
1 + 2 + 3 + ... + (n-1) = Theta(n^2)
```

따라서 worst-case running time은 `Theta(n^2)`입니다.

Average case도 모든 permutation이 equally likely라고 가정하면 평균적으로 각 삽입에서 절반 정도를 움직인다고 볼 수 있습니다. 그래도 합은 여전히 `Theta(n^2)`입니다.

```text
Average shift count ≈ j/2
sum_{j=1}^{n} j/2 = Theta(n^2)
```

그래서 insertion sort는 작은 `n`에서는 moderately fast일 수 있지만, 큰 `n`에서는 fast sorting algorithm이라고 보기 어렵습니다.

### 18. Insertion Sort가 O(n^3)인가?

PDF에 quiz로 “Is Insertion Sort O(n^3)? Is Insertion Sort O(n^2)?”가 나옵니다. 답은 둘 다 yes입니다. 다만 의미가 다릅니다.

- Insertion sort worst/average running time은 `Theta(n^2)`입니다.
- `Theta(n^2)`이면 당연히 `O(n^2)`입니다.
- 그리고 `n^2`는 `n^3`보다 느리게 자라므로 `O(n^3)`도 참입니다.

하지만 좋은 답은 `O(n^2)`입니다. `O(n^3)`은 참이지만 너무 느슨한 upper bound입니다. 시험에서 이 질문이 나오면 “Big O는 upper bound라서 둘 다 참이지만, tight하게 말하면 `Theta(n^2)`이고 upper bound로는 `O(n^2)`가 더 적절하다”고 답하면 됩니다.

### 19. Binary Insertion Sort

마지막에 binary insertion sort가 소개됩니다. Normal insertion sort에서 새 원소가 들어갈 위치를 찾을 때 선형 탐색을 하는 대신 binary search를 쓰면 comparison 수를 줄일 수 있습니다.

하지만 중요한 질문은 time complexity입니다. 위치를 찾는 comparison은 `O(log n)`으로 줄일 수 있지만, 배열에 실제로 삽입하려면 원소들을 오른쪽으로 shift해야 합니다. 이 shift 비용은 여전히 최악의 경우 `O(n)`입니다.

따라서 binary insertion sort의 전체 시간은 일반적인 array 구현에서는 여전히 `Theta(n^2)`입니다. 비교 횟수는 줄어들 수 있지만, 원소 이동 비용이 지배하기 때문입니다.

```text
Search position: O(log n) each
Shift elements: O(n) each in worst case
Total over n insertions: Theta(n^2)
```

이건 아주 좋은 함정 포인트입니다. “binary search를 썼으니 정렬 전체가 `O(n log n)`인가?”라고 생각하면 안 됩니다. insertion에서 expensive한 부분이 비교인지 이동인지 구분해야 합니다.

## Part 2. Merge Sort

### 1. Merge Sort Idea

`Merge Sort.pdf`는 divide and conquer의 대표 예제로 merge sort를 설명합니다. 기본 아이디어는 세 단계입니다.

```text
Divide: 배열을 midpoint에서 둘로 나눈다.
Conquer: 각 절반을 재귀적으로 정렬한다.
Merge: 정렬된 두 절반을 하나의 정렬된 배열로 합친다.
```

Insertion sort가 “정렬된 prefix에 하나씩 삽입”하는 방식이라면, merge sort는 “큰 문제를 절반으로 나누고, 작은 문제의 정답을 합쳐 큰 문제의 정답을 만든다”는 방식입니다.

### 2. Merge Sort Pseudocode

PDF의 pseudocode는 다음 구조입니다.

```text
MERGE-SORT A[1..n]
1. If n = 1, done.
2. Recursively sort A[1..ceil(n/2)] and A[ceil(n/2)+1..n].
3. Merge the 2 sorted lists.
```

Base case는 `n = 1`입니다. 원소 하나짜리 배열은 이미 정렬되어 있으므로 더 나눌 필요가 없습니다. 그 외에는 왼쪽 절반과 오른쪽 절반을 정렬하고, 마지막에 두 sorted list를 merge합니다.

여기서 key subroutine은 `MERGE`입니다. Merge sort의 전체 효율은 merge가 얼마나 빠르게 되는지에 크게 의존합니다.

### 3. Merging Two Sorted Arrays

PDF는 두 sorted arrays를 합치는 과정을 여러 페이지에 걸쳐 보여줍니다. 예시는 숫자들이 조금 추출상 붙어 보이지만, 핵심은 두 배열의 맨 앞 원소를 비교해 더 작은 값을 auxiliary/output array에 차례로 넣는 것입니다.

```text
Left sorted array:  [2, 7, 13, 20]
Right sorted array: [1, 9, 11, 12]

Compare left[0] and right[0]
Take smaller one
Advance that pointer
Repeat
```

매 단계에서 둘 중 작은 front element를 꺼내므로 결과 배열은 계속 정렬 상태를 유지합니다. 두 배열의 모든 원소는 정확히 한 번씩 output으로 이동합니다. 그래서 총 `n`개 원소를 merge하는 비용은 `Theta(n)`입니다.

이 부분에서 merge의 correctness도 볼 수 있습니다. 두 입력 배열이 이미 정렬되어 있다면, 아직 남은 원소 중 최솟값은 항상 두 배열의 맨 앞 원소 중 하나입니다. 그래서 둘 중 작은 것을 고르면 sorted order가 깨지지 않습니다.

### 4. Auxiliary Array

Merge는 보통 auxiliary array를 필요로 합니다. 두 sorted halves를 원래 배열 안에서 바로 합치기는 쉽지 않습니다. 그래서 임시 배열에 결과를 만들고, 필요하면 다시 원래 배열로 복사합니다.

PDF는 auxiliary array를 명시하고, merge sort의 properties에서 `Not in-place`, `Requires an auxiliary array (O(n) extra space)`라고 정리합니다.

즉 merge sort는 시간은 좋지만 공간을 더 씁니다.

```text
Time: Theta(n log n)
Extra space: O(n)
In-place: No, 일반적 구현 기준
```

### 5. Merge Sort Running Time

Merge sort 분석은 recurrence로 갑니다. PDF는 `MERGE-SORT A[1..n]`의 각 줄 비용을 다음처럼 잡습니다.

```text
If n = 1: O(1)
Recursively sort two halves: 2T(n/2)
Merge two sorted lists: O(n)
```

그래서 recurrence는 다음입니다.

```text
T(n) = O(1), if n = 1
T(n) = 2T(n/2) + O(n), if n > 1
```

엄밀히는 `T(ceil(n/2)) + T(floor(n/2))`이지만, asymptotic하게는 `2T(n/2)`로 단순화해도 결과가 달라지지 않습니다. 이 단순화는 이번 주 asymptotic analysis의 사고방식과 연결됩니다. 정확한 작은 차이보다 큰 성장률을 봅니다.

### 6. Recursion Tree

PDF는 `T(n) = 2T(n/2) + cn`을 recursion tree로 풉니다. 여기서 `c > 0`은 merge에 들어가는 선형 비용의 상수입니다.

레벨별 비용을 보면 다음과 같습니다.

```text
Level 0: cn
Level 1: 2 * c(n/2) = cn
Level 2: 4 * c(n/4) = cn
...
```

각 level의 총 merge 비용은 `cn`입니다. 배열이 반씩 줄어들기 때문에 height는 `log n`입니다. 정확히는 `lg n`으로 표기됩니다.

```text
Height = lg n
Cost per level = cn
Number of leaves = n
Leaf total = Theta(n)
```

전체 비용은 level마다 `cn`이 `log n`번 반복되므로 `Theta(n log n)`입니다. leaf 비용 `Theta(n)`은 `n log n`에 비해 낮은 차수라서 전체에 흡수됩니다.

```text
T(n) = Theta(n log n)
```

이 결과가 중요한 이유는 insertion sort의 `Theta(n^2)`보다 asymptotically 좋기 때문입니다. 충분히 큰 `n`에서 `n log n`은 `n^2`보다 훨씬 느리게 증가합니다.

### 7. Iterative Merge Sort

Merge sort는 재귀로만 구현되는 것이 아닙니다. PDF는 iterative merge sort도 소개합니다.

```text
Merge by 1
Merge by 2
Merge by 4
Merge by 8
Merge by 16
...
```

처음에는 길이 1짜리 sorted runs를 서로 merge합니다. 그 다음 길이 2짜리 runs, 길이 4짜리 runs, 길이 8짜리 runs를 차례로 merge합니다. 이 방식은 bottom-up merge sort입니다.

하지만 PDF는 iterative version에서도 `O(n)` auxiliary space가 여전히 필요하다고 말합니다. 재귀 호출을 줄이거나 copying을 줄이는 구현상의 장점은 있을 수 있지만, merge 자체가 보통 extra array를 필요로 한다는 성질은 남습니다.

### 8. Properties of Merge Sort

PDF의 properties는 세 가지로 정리됩니다.

1. Not in-place
2. Stable
3. Iterative merge sort reduces copying

Stable sorting은 같은 key를 가진 원소들의 상대적 순서가 유지되는 정렬입니다. PDF는 equal values에서 left를 target으로 먼저 보내야 stable하다고 말합니다. 즉 merge할 때 `left_value <= right_value`이면 left를 먼저 넣어야 합니다. 같은 값에서 right를 먼저 넣으면 원래 왼쪽에 있던 원소의 상대 순서가 바뀔 수 있습니다.

```text
Stable merge rule:
if left[i] <= right[j], take left[i] first.
```

이건 구현 디테일처럼 보이지만, stable sort 여부를 묻는 문제에서는 매우 중요합니다.

### 9. Insertion Sort vs Merge Sort

PDF는 `Theta(n log n)`이 `Theta(n^2)`보다 천천히 자란다고 정리합니다. 따라서 merge sort는 worst case에서 insertion sort를 asymptotically 이깁니다.

자료에는 practice에서 `n > 30` 정도면 merge sort가 insertion sort를 이긴다고 되어 있습니다. 이 숫자는 절대 법칙이라기보다, 작은 입력에서는 insertion sort의 단순함과 낮은 constant factor가 유리할 수 있다는 감각을 주는 예시입니다.

정리하면 다음입니다.

| 알고리즘 | Worst case | Average case | Extra space | Stable |
| --- | ---: | ---: | ---: | --- |
| Insertion Sort | Theta(n^2) | Theta(n^2) | O(1) | Yes, 구현에 따라 |
| Merge Sort | Theta(n log n) | Theta(n log n) | O(n) | Yes |

Insertion sort는 작은 입력이나 거의 정렬된 입력에서 좋을 수 있습니다. Merge sort는 큰 입력에서 안정적인 `n log n` 성능을 보장합니다. 그래서 실제 라이브러리 정렬도 한 알고리즘만 쓰기보다 입력 크기와 상태에 따라 여러 기법을 섞는 경우가 많습니다.

### 10. Divide and Conquer

Merge sort는 divide and conquer의 대표 예시입니다. PDF 마지막 부분은 divide and conquer를 일반 전략으로 정리합니다.

```text
Divide problem into smaller parts.
Independently solve the parts.
Combine these solutions to get overall solution.
```

Merge sort에서는 배열을 두 절반으로 나누고, 각각을 정렬한 뒤, 두 sorted lists를 merge합니다. Quick sort는 다른 방식의 divide and conquer입니다. 배열을 small items와 large items로 partition한 뒤 두 부분을 재귀적으로 정렬합니다.

이번 주에는 quicksort를 깊게 배우는 것이 아니라, divide and conquer라는 사고방식이 merge sort에서 어떻게 나타나는지를 잡으면 됩니다.

## Part 3. Big O Supplementary Material

### 1. Big O의 의미

`big_o.pdf`는 Big O notation이 함수의 asymptotic behavior를 설명하는 표기라고 설명합니다. 쉽게 말하면 함수가 얼마나 빨리 자라거나 줄어드는지 나타냅니다. 알고리즘 분석에서는 보통 시간 또는 step 수를 `T(n)`으로 두고, 큰 입력에서 이 함수가 어떤 order로 자라는지 봅니다.

예시는 다음입니다.

```text
T(n) = 4n^2 - 2n + 2
T(n) = O(n^2)
```

상수와 낮은 차수 항을 무시하면 `n^2`가 지배합니다. 이 설명은 `Performance Analysis.pdf`의 asymptotic analysis와 같은 이야기입니다.

### 2. Growth classes

보조자료는 흔한 class를 다음처럼 정리합니다.

```text
O(1): constant
O(log n): logarithmic
O((log n)^c): polylogarithmic
O(n): linear
O(n^2): quadratic
O(n^c): polynomial
O(c^n): exponential
```

특히 `O(n^c)`와 `O(c^n)`이 매우 다르다는 점을 강조합니다. 전자는 polynomial이고 후자는 exponential입니다. `c`가 어떤 상수든, 큰 `n`에서는 exponential이 훨씬 빠르게 폭발합니다.

또한 log의 base는 Big O에서 상수 차이이므로 보통 무시됩니다.

```text
O(log_2 n) = O(log_10 n) = O(ln n)
```

왜냐하면 로그 밑 변환은 상수배 차이만 만들기 때문입니다.

### 3. Performance vs Complexity

보조자료는 performance와 complexity를 구분합니다.

- Performance: 실제 프로그램이 실행될 때 시간/메모리/디스크/네트워크를 얼마나 쓰는가.
- Complexity: 문제 크기가 커질 때 자원 요구량이 어떻게 scale하는가.

Performance는 machine, compiler, implementation에 영향을 받습니다. Complexity는 알고리즘의 구조적 성장률을 봅니다. Complexity가 performance에 영향을 주지만, performance 측정값만으로 complexity를 바로 알 수는 없습니다.

이 구분은 1주차 퀴즈 Q3와 그대로 이어집니다. 실제 실행시간을 직접 측정하는 대신 basic operations를 세는 이유가 바로 이것입니다.

### 4. Basic operations

보조자료는 basic operation 예시를 듭니다.

- arithmetic operation: `+`, `*`
- assignment: `x := 0`
- test: `x = 0`
- read of primitive type
- write of primitive type

시간은 대체로 basic operation 수에 비례한다고 봅니다. 물론 현실에서는 모든 operation 비용이 완전히 같지 않지만, 알고리즘 분석에서는 큰 성장률을 보기 위해 이런 단순화가 필요합니다.

### 5. Code complexity 분석

보조자료 뒤쪽은 간단한 코드 구조의 complexity를 다룹니다.

Sequence of statements는 각 statement 시간을 더합니다. 각 statement가 constant time이면 전체도 `O(1)`입니다.

```text
statement 1;
statement 2;
...
statement k;
```

If-then-else는 worst case에서 더 느린 branch를 봅니다.

```text
if condition:
    block 1
else:
    block 2

T = max(time(block1), time(block2))
```

Loop는 반복 횟수에 body 비용을 곱합니다.

```text
for i in 1..N:
    O(1) work

Total = O(N)
```

Nested loop는 바깥 반복과 안쪽 반복을 곱합니다.

```text
for i in 1..N:
    for j in 1..M:
        O(1) work

Total = O(NM)
```

특수하게 둘 다 `N`번 돌면 `O(N^2)`입니다. 함수 호출이 들어가면 호출된 함수의 complexity까지 포함해야 합니다. 예를 들어 `g(k)`가 `O(k)`라면 loop 안에서 `g(j)`를 부를 때는 그 비용을 합산해야 합니다.

이 보조자료는 실제 코드를 보고 Big O를 계산하는 연습용입니다. 교수님 메인 PDF가 notation과 sorting 분석 중심이라면, `big_o.pdf`는 “코드 조각을 보고 복잡도를 어떻게 세는가”를 보강합니다.

## 1주차와의 연결

1주차에서는 알고리즘 공부의 세 목표가 나왔습니다.

```text
Solve computational problems
Prove correctness
Argue efficiency
```

2주차는 이 중 `Argue efficiency`가 본격화되는 주차입니다. 다만 correctness가 사라진 것은 아닙니다. Insertion sort와 merge sort 모두 “왜 정렬 결과가 맞는가”를 말할 수 있어야 합니다. 하지만 수업의 무게중심은 running time 분석, asymptotic notation, recurrence 분석입니다.

1주차 birthday problem에서는 record를 유지하면서 같은 birthday를 찾았습니다. 거기서는 `record`를 어떻게 구현하느냐가 efficiency와 연결되었습니다. 2주차 sorting에서는 같은 문제를 푸는 여러 알고리즘, 즉 insertion sort와 merge sort를 비교하면서 알고리즘 설계 차이가 성장률을 어떻게 바꾸는지 봅니다.

## 이번 주 시험/퀴즈 대비 핵심

### 꼭 설명 가능해야 하는 것

- Sorting problem의 input/output 조건.
- Sorting output이 input의 permutation이어야 하는 이유.
- Insertion sort의 “sorted prefix에 새 원소 삽입” 아이디어.
- Insertion sort의 worst case가 reverse sorted input인 이유.
- Insertion sort worst/average running time이 `Theta(n^2)`인 이유.
- Best case를 분석 기준으로 거의 쓰지 않는 이유.
- Average case에는 input distribution assumption이 필요하다는 점.
- Machine-independent time이 필요한 이유.
- Asymptotic analysis에서 constant와 lower-order terms를 무시하는 이유.
- Big O, Omega, Theta의 formal definition과 직관.
- `O(n^3)`가 참이어도 tight한 설명은 아닐 수 있다는 점.
- Little o가 non-tight upper bound라는 점.
- Polynomial time과 exponential time의 차이.
- Merge sort의 divide/conquer/merge 구조.
- Merge cost가 `Theta(n)`인 이유.
- Merge sort recurrence `T(n) = 2T(n/2) + O(n)`.
- Recursion tree에서 각 level 비용이 `cn`, 높이가 `log n`이라 전체가 `Theta(n log n)`이 되는 이유.
- Merge sort가 not in-place이고 `O(n)` auxiliary space를 쓰는 이유.
- Stable sort에서 equal values일 때 left를 먼저 보내야 하는 이유.
- Binary insertion sort가 비교 수는 줄여도 전체 배열 이동 때문에 여전히 `Theta(n^2)`인 이유.

### 헷갈리기 쉬운 점

1. `O`와 `Theta`를 섞어 쓰기 쉽습니다.

`Insertion sort is O(n^2)`는 맞습니다. 하지만 더 정확한 worst/average 분석은 `Theta(n^2)`입니다. `O(n^3)`도 맞지만 너무 약한 말입니다.

2. 실제 초 단위 실행시간과 complexity를 혼동하기 쉽습니다.

같은 insertion sort도 빠른 컴퓨터에서는 짧게 걸릴 수 있습니다. 하지만 입력이 커질 때 `n^2`로 커지는 구조 자체는 변하지 않습니다.

3. Binary insertion sort를 `O(n log n)`으로 착각하기 쉽습니다.

Binary search는 위치 찾기만 빠르게 합니다. 배열에서 삽입하려면 원소 이동이 필요하고, 이 이동이 여전히 `Theta(n^2)` 총비용을 만듭니다.

4. Merge sort가 항상 모든 면에서 insertion sort보다 좋은 것은 아닙니다.

Merge sort는 asymptotic time이 좋지만 extra space가 필요하고 구현이 더 복잡합니다. 작은 입력에서는 insertion sort가 실제로 더 나을 수 있습니다.

5. Average-case는 그냥 “보통”이 아닙니다.

Average-case를 말하려면 모든 입력이 어떤 확률로 들어오는지 분포 가정이 필요합니다. Insertion sort average case에서 all permutations equally likely라고 하는 이유가 이것입니다.

## 빠른 암기표

| 개념 | 한 줄 정의 |
| --- | --- |
| Sorting | 입력 sequence의 permutation을 오름차순으로 재배열하는 문제 |
| Insertion Sort | 정렬된 prefix에 새 원소를 적절한 위치로 삽입하는 정렬 |
| Running Time | 입력 크기 `n`에 따른 알고리즘 수행 비용 함수 |
| Worst-case | 크기 `n` 입력 중 가장 오래 걸리는 경우 |
| Average-case | 입력 분포를 가정한 기대 수행시간 |
| Best-case | 가장 운 좋은 입력에서의 수행시간, 분석 기준으로 약함 |
| Big O | asymptotic upper bound |
| Big Omega | asymptotic lower bound |
| Big Theta | tight asymptotic bound |
| little-o | non-tight upper bound, strictly smaller growth |
| Polynomial time | `O(n^k)` 형태의 시간 |
| Exponential time | `O(k^n)` 형태의 시간 |
| Merge Sort | 절반으로 나누고 재귀 정렬한 뒤 merge하는 정렬 |
| Recurrence | 재귀 알고리즘의 수행시간을 자기 자신으로 표현한 식 |
| Recursion Tree | recurrence를 level별 비용으로 펼쳐 보는 분석 방법 |
| Stable Sort | 같은 key의 상대적 순서를 유지하는 정렬 |
| In-place | 추가 배열 없이 입력 배열 내부에서 주로 처리하는 성질 |

## 직접 말해보는 복습 질문

1. Sorting problem의 output 조건을 permutation과 sorted order로 설명할 수 있는가?
2. Insertion sort는 매 반복에서 어떤 invariant를 유지하는가?
3. Reverse sorted input에서 insertion sort가 왜 최악인가?
4. `1 + 2 + ... + n`이 왜 `Theta(n^2)`인가?
5. 왜 best-case analysis는 알고리즘 성능 보장으로 거의 쓰지 않는가?
6. Average-case analysis에는 왜 input distribution assumption이 필요한가?
7. `f(n) = O(g(n))`의 formal definition에서 `c`와 `n0`의 역할은 무엇인가?
8. `100n + 5 = O(n^2)`를 증명할 때 `c`, `n0`가 유일하지 않은 이유는 무엇인가?
9. `n = O(n^2)`는 참인데 `n = Theta(n^2)`는 거짓인 이유는 무엇인가?
10. `3n + 2 = o(n^2)`이지만 `3n + 2 != o(n)`인 이유는 무엇인가?
11. Merge sort에서 merge 비용이 왜 `Theta(n)`인가?
12. `T(n) = 2T(n/2) + cn`의 recursion tree에서 각 level 비용이 왜 `cn`인가?
13. Merge sort의 tree height가 왜 `log n`인가?
14. Merge sort가 stable하려면 equal values에서 어떤 쪽을 먼저 보내야 하는가?
15. Binary insertion sort가 왜 전체적으로 `Theta(n^2)`인가?

## 이번 주 공부 순서 추천

1. `Performance Analysis.pdf` 2-6쪽: sorting 문제 정의와 insertion operation을 먼저 봅니다.
2. 7-18쪽: insertion sort 예제 배열 `8 2 4 9 3 6`이 어떻게 정렬되는지 손으로 따라갑니다.
3. 19-24쪽: running time, worst/average/best case, asymptotic analysis의 필요성을 잡습니다.
4. 25-40쪽: Big O, Omega, Theta, little-o 정의를 예제와 같이 봅니다.
5. 42-47쪽: polynomial/exponential, insertion sort analysis, binary insertion sort 질문을 정리합니다.
6. `Merge Sort.pdf` 2-20쪽: divide/conquer/merge 구조와 merge 과정을 따라갑니다.
7. 21-32쪽: recurrence와 recursion tree를 직접 그려 `Theta(n log n)`을 말로 설명합니다.
8. 33-38쪽: in-place 여부, auxiliary space, stable property, insertion sort와 비교를 정리합니다.
9. `big_o.pdf` 5-9쪽: sequence, if-else, loop, nested loop complexity 계산을 연습합니다.
10. 수요일 MIT 강의 수강 후 MIT 표현과 교수님 PDF 표현이 어디서 겹치는지 이 파일에 추가합니다.

## 수요일 MIT 업데이트 대기

이번 주 교수님 수업 자료 기준 정리는 여기까지입니다. MIT 강의는 수요일에 따로 듣고, 해당 강의가 sorting/performance/merge sort 중 어디와 연결되는지 확인한 뒤 추가합니다. 특히 MIT에서 같은 내용을 다른 notation이나 예제로 설명하면 이 파일의 해당 섹션 아래에 `MIT 강의 수강 후 정리`로 병합합니다.

## 최소컷

오늘 수업 전후로 시간이 없으면 아래만 외우면 됩니다.

```text
Insertion Sort = sorted prefix에 삽입, worst/average Theta(n^2)
Merge Sort = divide/conquer/merge, recurrence T(n)=2T(n/2)+Theta(n), total Theta(n log n)
Big O = upper bound, Omega = lower bound, Theta = tight bound, little-o = non-tight upper bound
Binary insertion sort = search는 log n이어도 shift 때문에 전체 Theta(n^2)
```
