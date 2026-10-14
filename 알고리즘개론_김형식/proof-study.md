# 증명 과제 복습목차

제출했던 PDF를 읽고 **어떤 증명 구조를 썼는지** 연결한 목차다. 새 제출 답안을 작성하거나 기존 문서를 수정하지 않았다. 제출 완료는 사용자 확인 기준이다.

## 제출물과 구조

| 과제 | 최종 보관 파일 | 실제 문서의 구조 |
| --- | --- | --- |
| Insertion Sort correctness | [2페이지 제출 PDF](./2주차/JunhyukSong_Prove%20the%20correctness%20of%20the%20Insertion%20Sort%20algorithm%20using%20Loop%20Invariant.pdf) | inner invariant로 큰 값 이동을 설명하고 outer invariant의 초기화·유지·종료, permutation을 연결 |
| 점근 표기 5문제 | [3페이지 제출 PDF](./2주차/Junhyuk%20Song_Prove%20the%20following%20statements.pdf) | 정의의 부등식 제시, c·n0 지정, 상한/하한을 각각 비교 |
| Recursion tree 4문제 | [2페이지 제출 PDF](./3주차/Junhyuk_Song_Solve_the_recurrences.pdf) | level 비용, 깊이, leaf 비용을 더하고 불균형 분할은 상·하한을 따로 설명 |
| Master method 3문제 | [1페이지 제출 PDF](./3주차/Junhyuk_Song_Solve_the_recurrences_Master_Method.pdf) | a·b·f(n), 기준 성장률, case 판정, Case 3 regularity 확인 |
| log(n!) | [1페이지 제출 PDF](./6주차/Proof_log_factorial_2024315329_Song_Junhyeok.pdf) | upper bound와 lower bound를 만든 뒤 Theta로 결론 |

## 각 문서가 다룬 범위

점근 표기 문서는 `5n^2-3n+20`, `2n^3-7n+1`, `2n^2+n log n+1`, `3n+10^10`의 bound를 다룬다. 같은 함수라도 상한만 증명한 것과 상·하한을 모두 증명한 것은 다른 주장이다. 상수의 크기보다 모든 `n >= n0`에 대해 부등식이 성립하는지가 중요하다.

recursion tree 문서는 다음 네 점화식을 다룬다.

```text
4T(n/2) + 100n
2T(n/2) + n^2
3T(n/4) + 2n^2
T(n/3) + T(2n/3) + n
```

처음 세 식은 level 비용의 등비수열과 leaf 비용을 비교한다. 마지막 식은 같은 크기로 나누지 않으므로 모든 level이 끝까지 완전히 찬 것처럼 계산하지 않는다. full level이 유지되는 깊이로 lower bound, 가장 긴 경로의 깊이로 upper bound를 설명했다.

master 문서는 `2T(n/2)+n`, `2T(n/2)+n^3`, `3T(n/4)+n lg n`이다. 기준은 `n^(log_b a)`이며, Case 3에서 regularity 조건을 별도로 확인한다. 단순히 f(n)이 크다는 관찰만으로 조건 검사를 대체하지 않는다.

log factorial 문서는 로그 밑 2를 명시하고, 전체 인수의 상한과 큰 쪽 절반 인수의 하한으로 연결한다. 근사 공식이나 수업 밖 정리를 새로 도입한 제출물은 아니다.

## 프로그래밍 과제와 연결

```text
Invariant: 반복 중 무엇이 유지되는가?
   -> hw1 merge의 sorted prefix / 원소 보존 / 안정성
   -> PQ의 그룹 순서 / 크기 / heap 위치

Recurrence: 재귀 비용은 어떻게 합쳐지는가?
   -> hw1 split + two recursive sorts + linear merge

Upper + lower bounds: 정확한 성장률은 무엇인가?
   -> comparison sorting decision tree / log(n!)
```

복습할 때는 PDF의 결론만 외우기보다 **가정, invariant 또는 부등식, 유지 논리, 종료 후 목표**를 순서대로 다시 설명한다. 이 문서는 원본 PDF를 대신하는 전문 전사가 아니라 제출물별 찾아보기다.
