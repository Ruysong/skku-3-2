# 알고리즘개론 10주차

기간: 2026-11-02~11-08

Greedy·Huffman 교안과 실제 Greedy Algorithm 퀴즈를 구분해 보관한다. 퀴즈 반영: 2026-11-08.

## 목차

1. [자료 연결](#자료-연결)
2. [실제 퀴즈 - Greedy Algorithm](#실제-퀴즈---greedy-algorithm)
3. [Q1. Huffman Encoding 최적성 증명](#q1-huffman-encoding-최적성-증명)
4. [Q2. Activity Selection 최적성 증명](#q2-activity-selection-최적성-증명)
5. [Q3. 0-1 Knapsack의 비율 Greedy 반례](#q3-0-1-knapsack의-비율-greedy-반례)
6. [복습할 증명 구조](#복습할-증명-구조)
7. [정리 상태](#정리-상태)

## 자료 연결

- [공지](../notices.md) · [과제 및 퀴즈](../assignments.md)
- 교수님 [Greedy Algorithm 교안](./Greedy%20Algorithm.pdf): Activity Selection, Huffman, Fractional/0-1 Knapsack 비교.
- [Huffman 최적성 증명 보충자료](./huffman.pdf): 교환 논증의 핵심 주장과 축약·귀납 증명.
- [9주차 정리](../9주차/study.md) · [MIT 교안 목차](../MIT%20교안/목차.md)
- 11/7 반영: [9주차 DP 상세 정리](../9주차/study.md)에 외부 영상 세 개의 원문·개별 교안·DP 2 실제 문항과 제출 답안을 보관했다. 해당 영상·퀴즈의 전달된 마감은 11/8 23:59이다.
- 아래 Greedy 퀴즈 외의 추가 영상·새 과제·마감은 전달되지 않은 항목까지 추정하지 않는다.
- 이전 주차의 미확인 상태를 이번 주 미제출로 단정하지 않는다.

## 실제 퀴즈 - Greedy Algorithm

### 출처와 상태

- 출처: 사용자가 붙여넣은 iCampus [Greedy Algorithm](https://canvas.skku.edu/courses/76112/quizzes/84703) 응시 화면.
- 시작 표시: **2026-11-08 12:04**.
- 제출 마감: **2026-11-08(일) 23:59**.
- 3문항, 각 1점, 총 3점.
- 전달된 화면은 세 문항 모두 아직 답변하지 않음, 저장 안 됨 상태다. 아래 답변을 대화에서 작성했다는 사실과 LMS에 입력·제출했다는 사실은 다르다. **제출 완료는 미확인**이다.
- 시간 제한·재시도 횟수는 제공 화면에 명시되지 않았다. 0분 0초라는 경과 시간 표시만으로 제한이 없다고 추정하지 않는다.
- 아래 영어 답변은 이 대화에서 작성한 **학습용 참고 답변**을 보관한 것이다. 사용자가 실제 제출한 답안이나 공식 정답·채점 결과로 표시하지 않는다.

## Q1. Huffman Encoding 최적성 증명

### 문제 원문

Prove that the greedy approach in Huffman Encoding, which combines the two least frequent symbols iteratively, produces an optimal encoding that minimizes the total bit length.

### 대화에서 작성한 쉬운 영어 참고 답변

We consider binary prefix-free codes. Each symbol is a leaf in a binary tree. Its code length is its depth. The total bit length is the sum of each symbol’s frequency multiplied by its code length.

Let x and y be the two symbols with the smallest frequencies. There is an optimal tree in which x and y are siblings at the deepest level. To see why, take an optimal tree and choose two deepest sibling leaves. We can move x and y to these positions by swapping symbol labels. This cannot increase the total cost because placing less frequent symbols at greater depths cannot increase the weighted sum. Therefore, the resulting tree is also optimal.

Now combine x and y into one symbol z with frequency f(x) + f(y). This reduces the problem from n symbols to n−1 symbols. Expanding z back into x and y increases the total cost by exactly f(x) + f(y), because both symbols become one level deeper.

The reduced tree must be optimal for the reduced problem. Otherwise, we could replace it with a better reduced tree and then expand z, producing a better tree for the original problem. This would contradict optimality.

We now use induction. For two symbols, Huffman coding is clearly optimal. Assume it is optimal for n−1 symbols. For n symbols, combining the two least frequent symbols is safe, and Huffman coding solves the reduced problem optimally by the induction hypothesis. Expanding the combined symbol therefore gives an optimal tree for the original problem.

Thus, repeatedly combining the two least frequent symbols minimizes the total bit length.

## Q2. Activity Selection 최적성 증명

### 문제 원문

Prove that selecting activities with the earliest end time always yields an optimal solution for maximizing the number of non-overlapping activities.

### 대화에서 작성한 쉬운 영어 참고 답변

Assume each activity has a fixed start time and finish time. Two activities are compatible if one starts at or after the other finishes.

Let g be the activity with the earliest finish time. Take an optimal solution O, and let a be its first activity. Since g finishes earliest, its finish time is no later than a’s finish time.

Replace a with g. Every later activity in O starts at or after a finishes. Therefore, it also starts at or after g finishes. The replacement creates no overlap and keeps the same number of activities. Thus, there is an optimal solution that starts with g.

After choosing g, we only need to consider activities that start at or after g finishes. The remaining activities in the optimal solution must form an optimal solution to this smaller problem. Otherwise, replacing them with a better solution would improve the original solution, which is a contradiction.

We can apply the same argument repeatedly. The empty problem is trivially optimal, and each earliest-finish choice safely reduces the problem to a smaller one. Therefore, by induction, repeatedly selecting the compatible activity with the earliest finish time gives the maximum number of non-overlapping activities.

## Q3. 0-1 Knapsack의 비율 Greedy 반례

### 문제 원문

Prove that a greedy approach that selects items by value/weight ratio cannot guarantee optimal solutions for the 0/1 Knapsack problem.

### 대화에서 작성한 쉬운 영어 참고 답변

A counterexample is enough to show that selecting items by value/weight ratio does not always give an optimal solution.

Suppose the knapsack capacity is 50. There are three items:

Item A: weight 10, value 60, ratio 6.
Item B: weight 20, value 100, ratio 5.
Item C: weight 30, value 120, ratio 4.

The greedy algorithm chooses A first because it has the highest ratio. It then chooses B. The total weight is 30 and the total value is 160. The remaining capacity is 20, so C cannot fit.

However, choosing B and C gives a total weight of 50 and a total value of 220. This is better than the greedy result of 160.

In the 0/1 Knapsack problem, items cannot be divided. Choosing an item with a high ratio can leave insufficient space for a better combination of whole items. Therefore, selecting items by value/weight ratio cannot guarantee an optimal solution.

## 복습할 증명 구조

| 문항 | 증명 방법 | 혼자 재현할 핵심 |
| --- | --- | --- |
| Huffman | 교환 논증 + 최적 부분 구조 + 귀납법 | 최소 빈도 두 문자를 가장 깊은 형제로 둘 수 있음 → 하나로 축약 → 비용 차이 `f(x)+f(y)` → 축약 문제의 최적성 → 귀납 |
| Activity Selection | 교환 논증 + 남은 문제에 대한 귀납 | 최적해 첫 활동을 가장 일찍 끝나는 활동으로 바꿔도 나머지 활동이 유지됨 → 개수 유지 → 남은 구간에 반복 |
| 0-1 Knapsack | 구체적인 반례 | 용량 50, 비율 순 A·B 선택은 160, B·C는 220 → 항상 최적이라는 주장이 깨짐 |

Huffman의 목적함수는 `Σ frequency × depth`다. 최적 prefix-code tree는 불필요한 단일 자식 내부 노드를 줄인 full binary tree로 잡을 수 있어 가장 깊은 sibling leaf 쌍을 선택할 수 있다. 라벨 교환과 트리 축약은 서로 다른 단계다. 같은 빈도에서는 동률을 어떻게 처리하든 최적 코드가 여러 개 생길 수 있으며 코드 문자열 자체가 유일할 필요는 없다.

Activity Selection의 기준은 가장 짧은 활동이 아니라 **종료 시각이 가장 빠른 활동**이다. 표준 문제는 시작 시각이 종료 시각보다 이른 활동들을 다루며, 종료와 다음 시작이 같으면 양립 가능하다. “greedy니까 최적”이라는 말 대신 교체해도 이후 선택들이 사라지지 않는다는 부등식 `finish(g) ≤ finish(a)`를 설명한다.

배낭 문제의 반례는 실제 입력 조건을 만족하고 greedy보다 나은 해 하나만 제시해도 충분하다. fractional 버전이라면 A·B 이후 남은 용량 20에 C의 2/3을 넣어 값 240을 얻을 수 있지만, 0-1에서는 물건을 나눌 수 없어 이 선택을 허용하지 않는다. 이 차이는 9주차 DP 배낭과 연결해서 복습한다.

## 정리 상태

이번 갱신은 사용자가 전달한 실제 퀴즈 3문항과 대화의 영어 참고 답변을 보관하는 작업이다. 교수님 교안 전체를 페이지별로 다시 작성한 1만자 상세 study 완성본은 아니므로 분량을 맞추기 위한 내용을 추가하지 않았다. 교안과 퀴즈 복습 링크·핵심 증명 구조를 연결하며, 원본 PDF는 수정하지 않는다.

<!-- kdna-download-20261109:start -->
## K DNA 과제 원본 보관 (11/9 확인)

10주차 모듈의 [K DNA 과제](https://canvas.skku.edu/courses/76112/assignments/2051467)는 **11/15 23:59:59 마감**, 코드와 설명문서 제출이다. [명세](./Assignment_kDNA.pdf) · [공개 입력](./msa.in) · [공개 출력](./msa.out)을 원본으로 보관했다. 이번 주 관리는 [11주차](../11주차/study.md)에서 본다. 자세한 명세 분석·구현 완료를 의미하지 않는다.

DP2와 Greedy는 11/9 API에서 제출 확인·평가 대기다. 기존 미확인 표시는 당시 화면 기록이다.
<!-- kdna-download-20261109:end -->
