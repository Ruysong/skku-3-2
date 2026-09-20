# 알고리즘개론 - 김형식 3주차 학습지도

## 진행 현황 (2026-09-20)

완료한 콘텐츠는 다음 8개다.

- `Solving recurrences` 자료
- `04_SolvingRecurrences`
- `05_SolvingRecurrences_substitution`
- `06_SolvingRecurrences_Recursion`
- `07_SolvingRecurrences_Master`
- `Solve the recurrences using either the substitution method or the recursion tree method.` 과제
- `big-O and little-o` 보조자료
- `Solve the recurrences using the master method.` 과제

남은 항목은 `3.2.1 Asymptotic Notation: Video`, `Proving the Master Theorem`, `8. Binary Heaps`, `[Quiz] Binary Heaps`다. 화면상의 마감은 모두 9월 27일 23:59다.

## 이번 주의 큰 흐름

```text
재귀 알고리즘의 실행 시간
        ↓
점화식 T(n)
        ↓
Substitution / Recursion Tree / Master Method
        ↓
asymptotic notation으로 결과 표현
        ↓
Binary Heap으로 다음 자료구조 단원 진입
```

즉 이번 주 전반은 Merge Sort에서 만난 재귀 실행 시간을 일반적인 점화식으로 푸는 법이고, 후반은 그 결과를 `O`, `Theta`, `o` 같은 표기로 정확히 읽는 법이다. Heap은 다음으로 우선순위 큐와 Heap Sort를 다루기 위한 출발점이다.

## 외부 보조자료 반영

### `3.2.1 Asymptotic Notation: Video`

붙여준 강의 스크립트 기준으로 네 관계를 한 세트로 잡는다.

| 관계 | 뜻 | 비율 관점 |
| --- | --- | --- |
| `f ~ g` | 거의 같은 성장 | `lim f(n)/g(n) = 1` |
| `f = o(g)` | `f`가 엄밀히 더 느림 | `lim f(n)/g(n) = 0` |
| `f = O(g)` | `f`가 결국 상수배 안에 갇힘 | `limsup f(n)/g(n) < infinity` |
| `f = Theta(g)` | 같은 차수 | `f = O(g)` 이고 `g = O(f)` |

- `n^2 + n ~ n^2`: 비율이 1로 간다.
- `n^2 = o(n^3)`: 비율 `1/n`이 0으로 간다.
- `3n^2 = O(n^2)`: 상수 3은 성장 차수를 바꾸지 않는다.
- `2n^2 + n log n + 1 = Theta(n^2)`: 위에서도 아래에서도 `n^2`의 상수배로 묶인다.

`~`와 `Theta`는 같은 성장률을 묶는 등가 관계이고, `o`는 엄격하게 더 느린 성장을 뜻한다. 점화식을 푼 뒤에는 단순히 식을 끝내는 것이 아니라, 이 표기 중 무엇으로 결론을 써야 하는지까지 연결해야 한다.

### `big-O and little-o` 읽을거리

이 자료는 Big-O 정의에서 자주 생기는 혼동 하나를 정확히 짚는다.

- Big-O의 표준 정의는 충분히 큰 모든 `n`에 대해 `f(n) <= c g(n)`가 되게 하는 상수 `c`, `n0`가 존재한다는 것이다.
- `f = o(g)`는 어떤 양의 상수 `c`를 잡아도, 충분히 큰 `n`부터 `f(n) <= c g(n)`가 되는 더 강한 관계다.
- `lim f(n)/g(n)`가 존재하고 유한하면 Big-O임을 보이기 쉽지만, Big-O의 필요조건은 아니다.
- 예를 들어 비율이 홀짝에 따라 흔들려 일반적인 극한이 존재하지 않아도, 전체 비율이 상수로 제한되면 Big-O일 수 있다.
- 이런 경우까지 포함해 Big-O를 비율로 말할 때는 `limsup f(n)/g(n) < infinity`가 정확한 표현이다.

시험이나 과제의 일반 증명에서는 극한보다 `c`, `n0`를 직접 잡는 방식이 가장 안전하다. 외부 읽을거리는 그 정의가 왜 필요한지, 그리고 `O`와 `o`를 말로만 비슷하게 취급하면 왜 틀리는지를 보강하는 자료로 둔다.

## 이번 주 과제와 연결

이미 푼 점화식 과제는 다음 결론으로 정리된다.

| 점화식 | 핵심 관찰 | 결과 |
| --- | --- | --- |
| `4T(n/2) + 100n` | 마지막 레벨의 `n^2` 비용이 지배 | `Theta(n^2)` |
| `2T(n/2) + n^2` | 루트의 `n^2` 비용이 지배 | `Theta(n^2)` |
| `3T(n/4) + 2n^2` | 레벨 비용이 `(3/16)^i`로 감소 | `Theta(n^2)` |
| `T(n/3) + T(2n/3) + n` | 각 완전 레벨 비용은 `n`, 높이는 `Theta(log n)` | `Theta(n log n)` |

Master Method 과제에서는 `aT(n/b) + f(n)`에서 `n^(log_b a)`와 `f(n)`을 비교하는 구조를 사용했다. 특히 Case 3에서는 `f(n)`이 더 큰 것만으로 끝내지 않고 regularity condition도 확인한다.

## 이후 통합할 것

- `Proving the Master Theorem`의 증명 흐름
- Binary Heap의 배열 표현, heap-order property, insert/extract 동작
- Binary Heaps 퀴즈의 실제 문제와 답안 포인트

이 자료들이 추가되면 이 문서를 재귀 분석에서 Heap까지 이어지는 완성형 3주차 스터디로 확장한다.
