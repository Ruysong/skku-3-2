# 알고리즘개론 7주차 (2026-10-12~10-18)

이번 주는 Median and Order Statistics를 확인하고, 10/19 중간고사를 위해 기존 알고리즘의 설명·복잡도·정확성 증명을 복습한다.

2026-10-15 자료 확인: 교수님 PDF 25페이지를 렌더링해 전체 확인했고, 제공된 MIT 영상 대본도 끝까지 읽었다. **교수 PDF는 시험 범위인 선택 알고리즘**, **MIT 대본은 시험 제외된 DP Part 1**이다. 교수 영상 두 개의 실제 대본까지 받은 것은 아니다.

## 목차

1. [자료와 출처](#자료와-출처)
2. [수강 묶음과 일정](#수강-묶음과-일정)
3. [학습 구조](#학습-구조)
4. [중간고사와 제출 구분](#중간고사와-제출-구분)
5. [정리 상태](#정리-상태)
6. [교수 PDF 페이지별 목차](#교수-pdf-페이지별-목차)
7. [선택 알고리즘의 실제 내용](#선택-알고리즘의-실제-내용)
8. [PDF 기준 증명 체크리스트](#pdf-기준-증명-체크리스트)
9. [MIT DP 영상 대본](#mit-dp-영상-대본)
10. [Dynamic Programming 퀴즈 문항과 영어 답변](#dynamic-programming-퀴즈-문항과-영어-답변)

## 자료와 출처

| 구분 | 자료 | 상태 |
| --- | --- | --- |
| 교수 자료 | [Median and Order Statistics.pdf](<./Median and Order Statistics.pdf>) | 25페이지. 10/15 전체 페이지의 텍스트·수식·그림 확인 |
| 교수 영상 | Median_Order (18:27), Worst_case_linear_time (20:33) | 사용자 화면에 수강 완료. 대응 PDF는 확인했지만 이 두 영상의 실제 음성·대본은 미확인 |
| 교수 영상 | Q5_LinearSorting (04:57) | 10/15 전달받은 화면에서 수강 완료. 10/18 마감. 중간고사 범위 포함 |
| MIT 영상 | 15. Dynamic Programming, Part 1: SRTBOT, Fib, DAGs, Bowling (57:18) | 10/15 화면에서 수강 완료. [제공 대본 전문](./MIT-DP-Part1-script.txt) 확인·원본 보관. 중간고사 제외 |
| 퀴즈 | Dynamic Programming, 4문항·4점 | 10/15 18:41 시작 화면 확인. 아래 문항·영어 답변 보관, 제출 완료 미확인. 10/18 23:59 마감. 중간고사 제외이나 제출 면제 공지는 아님 |
| 지난 주 교안 | [6주차 study](../6주차/study.md) | 두 MIT 선형 정렬 강의 및 Linear Sorting 퀴즈·사용자 답변 |

자료 제목·수강 화면 기록과 강의 내용을 직접 읽고 정리한 결과를 구분한다. 각 영상 대본이 들어오면 교수 영상과 MIT 영상의 독립된 목차 아래 추가한다.

## 수강 묶음과 일정

- 10/12(월): 저장된 [조교 공지](../notices.md)에 따르면 알고리즘 대면 강의 없음. 변경 공지가 있으면 새 공지를 우선한다.
- 10/18(일) 23:59: 위 7주차 콘텐츠와 DP 퀴즈의 전달받은 마감.
- 10/19(월) **10:30~11:45**: 중간고사. **산학협력센터 6층 85613**, 학번 끝자리 9인 본인 배정. 10/14 최종 공지 기준이며 시작 전에 도착한다.
- 지난 주 PQ·로그 팩토리얼 증명·Linear Sorting 퀴즈는 10/11 이용 종료 묶음이다. 파일 검수와 실제 제출 완료는 다르며, 이 주차로 마감을 옮기지 않는다.

## 학습 구조

```text
정렬된 순서에서 k번째 원소를 찾는 문제
  -> Median and Order Statistics
  -> Median_Order 영상
  -> Worst_case_linear_time 영상
  -> RandomizedSelect: rank 갱신, 기대 O(n) 대입 증명
  -> Median-of-Medians: 5개 그룹, 3n/10-6 보장
  -> T(ceil(n/5)) + T(7n/10+6) + O(n), 대입법
  -> 선형 median black box로 selection / balanced quicksort

별도 수강 묶음
  -> Q5_LinearSorting: 기존 정렬 내용과 연결
  -> MIT Dynamic Programming: 별도 강의 교안
  -> DP 퀴즈: 수강·제출 조건 별도 확인

시험 복습
  -> 기존 퀴즈와 의사코드
  -> assumptions / correctness / running time
  -> 영어로 설명하고 증명하는 연습
```

## 중간고사와 제출 구분

사용자 제공 TA [중간고사 공지](../notices.md)의 **10/14 최종 업데이트** 기준, **7주차 Median and Order Statistics까지 모든 자료·온라인 강의·퀴즈가 포함**된다. **DP 내용과 DP 온라인 퀴즈(SRTBOT, Fibonacci, bowling)만 제외**다. 대면 수업에서 다루지 않은 내용도 포함한다. 이전 10/05 구두 안내에서 변경되었다고 명시하므로 **Counting/Radix/Linear Sorting과 Q5_LinearSorting도 포함**으로 확정한다.

DP가 중간고사에서 제외되어도 수강·퀴즈 제출이 면제된 것으로 해석하지 않는다. 새 프로그래밍 과제의 명세·마감은 아직 확인되지 않았다. [과제 기록](../assignments.md)에 있는 지난 주 PQ를 이번 주 신규 PA로 세지 않는다.

## 정리 상태

이 문서는 자료 확인·목차·증명 준비 목록이다. 10/15에는 실제 PDF 25페이지와 MIT DP 대본을 확인하여 기존의 자료 미보관 표시를 해제했다. 이번 요청은 자료 파악이므로 모든 증명·예제를 풀어쓴 1만자 상세 교안 완성으로 표시하지 않는다. 교수 영상의 실제 대본과 PDF, 시험 포함 단원과 제외 단원을 분리한다.

## 교수 PDF 페이지별 목차

| PDF 페이지 | 내용 | 확인한 핵심 |
| --- | --- | --- |
| 1 | 표지 | Median and Order Statistics |
| 2 | Order statistics | i번째 작은 값, minimum·maximum·median |
| 3 | Min/max 동시 탐색 | pair 비교 후 작은 쪽은 min, 큰 쪽은 max와 비교 |
| 4 | Selection problem | 정렬 후 선택 O(n log n), 기대 선형·최악 선형 방법 |
| 5~6 | Randomized partition | 무작위 pivot, 선택은 partition 후 한쪽만 탐색 |
| 7 | RandomizedSelect 의사코드 | pivot rank k=q-p+1, 오른쪽에서는 목표 rank i-k |
| 8 | 4번째 원소 예제 | [3,6,2,8,4,7,1,5], pivot 2, 오른쪽의 2번째로 변경 |
| 9 | 최악·균형·99:1 질문 | 최악 O(n^2), 균형 O(n), 고정 비율 한쪽 재귀 |
| 10~12 | 평균/기대 분석 | 큰 쪽으로만 간다는 상한, 기대 합, T(k)<=ck 대입 |
| 13~14 | Worst-case linear selection | 5개 그룹, 그룹 중앙값, 중앙값들의 중앙값 |
| 15~17 | 28개 원소의 14번째 | pivot 17, 오른쪽의 3번째로 변경 |
| 18~20 | 단계별 비용·분할 보장 | 적어도 3n/10-6개 제거, 다음 크기 최대 7n/10+6 |
| 21~22 | 점화식·대입 증명 | T(ceil(n/5))+T(7n/10+6)+O(n), n0>70과 c 조건 |
| 23 | 선형 median black box | 중앙값을 구할 수 있으면 i번째 선택도 O(n) |
| 24 | 정렬로 응용 | 정확한 중앙값으로 양쪽 정렬하면 worst-case O(n log n) |
| 25 | Questions | 마무리 |

## 선택 알고리즘의 실제 내용

### 순서통계량과 비교 횟수

i번째 order statistic은 정렬 순서에서 i번째 값이다. i는 1-based rank이며 배열 index와 다르다. 홀수 n의 median rank는 (n+1)/2, 짝수 n은 n/2와 n/2+1이다. 교안 p.2의 n/2 표현은 홀짝 구분과 함께 읽는다. 두 가운데 값의 평균을 구하는 문제로 바꾸지 않는다.

최소 또는 최대 하나만 찾는 비교 횟수는 n-1이다. 두 번 탐색하면 2n-2지만, pair를 먼저 비교하면 두 원소당 세 비교로 양쪽 극값을 갱신한다. 교안 p.3은 최대 3n/2의 상한을 제시한다. 표준 초기화를 전개한 정확한 횟수는 짝수 n에서 3n/2-2, 홀수 n에서 3(n-1)/2다. 정확한 식은 pair 방식을 계산한 보강 설명이며 교안의 인쇄된 식과 구분한다.

### RandomizedSelect

```text
partition(A[p..r]) -> pivot at q
k = q-p+1                현재 구간 안에서 pivot의 1-based rank
i == k -> return pivot
i < k  -> select left with rank i
i > k  -> select right with rank i-k
```

오른쪽에서는 pivot과 왼쪽 k-1개, 총 k개가 빠지므로 i-k가 된다. p.8 예제는 목표 i=4, pivot rank k=2이므로 오른쪽의 두 번째다. base case는 p==r이다. p.6 의사코드는 반환을 축약했지만 p.7에서 q를 받으므로 Partition 결과의 pivot index를 반환해야 한다. 정확성은 partition의 순서 성질과 작은 구간의 귀납 가정으로 설명한다.

한쪽만 재귀한다고 무조건 선형은 아니다. 최악에는 T(n)=T(n-1)+O(n)=O(n^2)이고 균형이면 T(n)=T(n/2)+O(n)=O(n)이다. 매번 99% 쪽으로 가도 고정 비율이면 비용은 n+0.99n+0.99^2 n+...로 O(n)이다.

기대 분석은 균일 무작위 pivot의 각 rank가 확률 1/n이라는 점에서 시작한다. 목표가 언제나 더 큰 쪽에 있다고 상한을 잡는다. 짝수 n으로 표기를 단순화하면 다음 합을 쓴다.

```text
E[T(n)] <= (1/n) sum_{j=0}^{n-1} E[T(max(j,n-j-1))] + a*n
        <= (2/n) sum_{j=n/2}^{n-1} E[T(j)] + a*n
```

E[T(j)]<=c*j를 대입하고 등차합을 풀면 3cn/4-c/2+an이 된다. c를 충분히 크게 정하고 작은 입력의 base case를 처리하면 cn 이하로 닫힌다. 홀수 n은 floor/ceil 보정이 필요하다. p.11의 옆 설명에는 T(k)를 cn으로 대입한다고 적혀 있지만 실제 수식은 ck이며 **T(k)<=ck**를 사용한다.

### Median-of-Medians

```text
n elements
  -> groups of 5
  -> sort each tiny group, take median
  -> recursively select median x of group medians
  -> partition original input around x
  -> return x or select only the required side
```

각 그룹 크기가 최대 5라 내부 정렬은 그룹당 O(1), 전체 O(n)이다. 재귀는 **중앙값들 중 x를 찾는 호출**과 **원래 배열에서 답이 있는 쪽을 찾는 호출** 두 개다. 원래 배열 양쪽을 모두 선택하는 구조가 아니다.

p.19~20 그림은 그룹을 열로 그리고 중앙값 순서대로 배치한다. x 이상인 중앙값을 가진 완전한 그룹에서 x 이상인 원소 세 개를 센다. 불완전 그룹과 x가 있는 그룹을 제외하여 3n/10-6이라는 보수적 bound를 얻는다. 다음 선택 구간은 최대 7n/10+6이다.

엄밀한 >/< 개수 증명은 서로 다른 key 또는 동점의 일관된 순서 가정을 적는다. 중복값이 있으면 >=와 >를 같게 취급하지 않는다. 동점 순서를 구분하거나 <x/=x/>x 구간을 나누는 방식으로 조건을 명확히 해야 한다. 교안 예제 자체에도 중복값은 있다.

```text
T(n) <= T(ceil(n/5)) + T(7n/10+6) + a*n
     <= c*(n/5+1) + c*(7n/10+6) + a*n
      = 9cn/10 + 7c + a*n
```

cn 이하가 되려면 c>=10a*n/(n-70)이면 된다. n0>70을 고정하고 모든 n>=n0에 충분한 c와 base case를 정하는 것이 p.22 대입 증명의 마무리다. 균등한 aT(n/b) 형식이 아니므로 기본 Master 정리를 바로 적용하지 않는다.

### 예제·응용과 표기 주의

p.15~17의 그룹 중앙값은 12,17,34,12,21,30이고 정렬하면 12,12,17,21,30,34다. 교안은 lower median 17을 pivot으로 사용한다. 작은 값 10개와 pivot을 제외하면 전체 14번째는 오른쪽 3번째다. p.16 일부 열의 위·아래 값은 완전한 정렬 순서가 아니므로 그림 전체를 정렬 결과로 그대로 암기하지 않는다. 중앙값과 p.17 rank 갱신을 기준으로 확인한다.

O(n) median black box로 필요한 한쪽만 선택하면 T(n)=T(n/2)+O(n)=O(n)이다. 반면 양쪽을 정렬하면 T(n)=2T(n/2)+O(n)=O(n log n)이다. 같은 좋은 pivot이라도 한쪽/양쪽 재귀가 비용을 바꾼다. median 자체가 답이면 먼저 종료하고 홀짝 rank를 정확히 처리한다. p.24는 이론적 worst-case 보장과 달리 실제 상수 때문에 일반 Quicksort보다 느릴 수 있다고 덧붙인다.

## PDF 기준 증명 체크리스트

| 페이지 | 보지 않고 설명·증명할 것 |
| --- | --- |
| 3 | min/max 별도 탐색 비용과 pair 방식의 비교 횟수 |
| 7~8 | 세 분기와 i-k 갱신, 종료·정확성 |
| 9 | 최악 O(n^2), 균형 및 고정 99:1 분할 O(n) |
| 10~12 | 기대시간 합을 세우고 대입법으로 O(n)을 닫는 계산 |
| 14~18 | 5개 그룹·중앙값 재귀·원래 구간 선택의 동작과 단계별 비용 |
| 19~20 | 세 개씩 세는 이유, 두 그룹 제외, 3n/10-6과 7n/10+6 |
| 21~22 | ceiling·상수항 포함 점화식, c·n0와 base case |
| 23 | 선형 median을 사용한 i번째 선택의 정확성·O(n) |
| 24 | median pivot Quicksort의 worst-case O(n log n) |

## MIT DP 영상 대본

출처: Erik Demaine, MIT 6.006 Spring 2020, `15. Dynamic Programming, Part 1: SRTBOT, Fib, DAGs, Bowling`. 제공 대본의 마지막 강의 발화는 57:01이고 57:10 이후는 영상 제목·배포 정보다. [대본 파일](./MIT-DP-Part1-script.txt)은 첨부 텍스트를 수정 없이 복사했다. 원문에 MIT OCW의 CC BY-NC-SA 표기가 있다. 영상 판서·음성까지 검수한 것은 아니다.

**Median_Order/Worst_case_linear_time의 대본이 아니라 DP 강의다. DP 내용과 DP 온라인 퀴즈는 중간고사 제외다.** 이 대본에 Merge Sort·Word-RAM 복습이 나온다고 기존 Merge Sort·RAM 단원까지 제외하는 것은 아니다.

| 대본 시간 | 내용 |
| --- | --- |
| 0:12~5:56 | 재귀 설계, SRTBOT: Subproblems, Relations, Topological order, Base cases, Original problem, Time |
| 6:03~8:32 | Merge Sort를 SRTBOT으로 표현 |
| 8:41~13:28 | Fibonacci 단순 재귀와 지수 증가 |
| 13:40~20:35 | memoization·dictionary·중복 subproblem 재사용·선형 개수 덧셈 |
| 20:43~24:11 | Fibonacci 큰 정수 비트 비용, 단위 연산 가정과 O(n+n^2/w) 구분 |
| 24:17~26:39 | 모든 가능한 구간으로 과대계산하면 Merge Sort의 느슨한 O(n^3) 상한. 실제 Θ(n log n)과 구분 |
| 26:58~36:53 | DP 일반 구조, DAG shortest paths, incoming-edge recurrence·위상 순서·DFS·O(V+E) |
| 37:02~41:25 | Bowling 규칙: 하나는 v_i점, 인접 둘은 곱, 중복 사용 불가·건너뛰기 허용 |
| 41:32~46:11 | prefix·suffix O(n), substring O(n^2), subsequence 2^n 선택. subproblem 설계 |
| 46:18~52:00 | suffix B(i), skip/single/pair, 계산 순서·base case·원래 답·시간 |
| 52:07~57:01 | bottom-up DP, 끝 경계 검사, local brute force와 subproblem 재사용 |

Bowling은 B(i)를 i부터 끝까지 남았을 때 최대 점수로 정의한다. B(i+1), v_i+B(i+1), v_i*v_(i+1)+B(i+2) 중 최댓값을 택하고 pair 항은 i+1<n일 때만 허용한다. B(n)=0, 마지막 하나는 max(0,v_(n-1)), 답은 B(0)이다. 역방향으로 각 suffix를 한 번 계산하면 word 연산 가정하에 O(n)이다. 마지막 위치에서 pair를 허용해 배열 밖을 읽지 않도록 한다.

대본 전문은 보관했지만 이 DP 절은 자료 파악용 목차·핵심 요지이며 모든 발화를 다시 작성한 전문 교안은 아니다. 시험 우선 학습 대상과 분리한다.

## Dynamic Programming 퀴즈 문항과 영어 답변

### 출처와 상태

- 출처: 사용자가 2026-10-15에 전달한 iCampus `Dynamic Programming` 퀴즈 화면 전문.
- 시작 표시: 2026-10-15 18:41. 전달된 화면에는 네 문항 모두 아직 답변하지 않은 상태로 표시된다.
- 배점: 각 1점, 총 4문항·4점.
- 제출 마감: 2026-10-18 23:59, 한국 시간.
- 아래 영어 답변은 이 대화에서 작성한 공부용 참고 답변이다. 공식 해설·채점 결과·실제 제출 답안으로 간주하지 않는다. 제출 완료는 미확인이다.
- 이 퀴즈는 10/14 공식 공지에 따라 중간고사에서 제외된다. 퀴즈 제출 의무와 시험 제외는 별개다.

### Q1. SRTBOT

문제 원문:

SRTBOT is a paradigm to solve a problem recursively. What is SRTBOT?

영어 답변:

SRTBOT is a framework for solving problems recursively. S stands for Subproblems: define the smaller problems. R stands for Relations: explain how a subproblem depends on smaller subproblems. T stands for Topological order: choose an order so that the needed smaller subproblems are solved first. B stands for Base cases: give answers to the smallest problems. O stands for Original problem: explain how to obtain the final answer. T stands for Time: analyze the number of subproblems and the work for each one.

### Q2. Merge Sort와 Memoization

문제 원문:

Describe how merge sort can be represented by using the SRTBOT framework and explain why memorization is not helpful.

영어 답변:

In merge sort, each subproblem is to sort a subarray. The relation is to sort the left and right halves and then merge them. The topological order solves the smaller halves before merging them. The base case is a subarray with at most one element, which is already sorted. The original problem is to sort the entire array. The recurrence is T(n) = 2T(n/2) + O(n), so the running time is O(n log n). Memoization is not helpful because each recursive call handles a different subarray. There are no repeated subproblems whose saved answers could be reused.

용어 메모: 문제의 `memorization`은 문맥상 계산 결과를 저장해 재사용하는 `memoization`을 가리킨다. 문제 원문은 수정하지 않았다.

### Q3. Fibonacci의 Word-RAM 시간 분석

문제 원문:

Show that we can compute the nth Fibonacci number Fn in O(n + n^2/w) time where w is the word size of the compuation model.

영어 답변:

We compute Fibonacci numbers using F0 = 0, F1 = 1, and Fi = F(i-1) + F(i-2). We calculate them in increasing order, so each value is computed only once. However, large Fibonacci numbers do not fit in a single machine word. Since Fi is at most 2^i, it needs O(i) bits, or O(1 + i/w) words when each word contains w bits. Therefore, the addition needed to compute Fi takes O(1 + i/w) time. Summing this cost for all i up to n gives O(n + n²/w) total time.

### Q4. Bowling Dynamic Programming

문제 원문:

Describe the dynamic programming algorithm to compute the Bowling problem and analyze its time complexity.

영어 답변:

Let B[i] be the maximum score obtainable from pin i to the end. At each position, we can skip the pin, take it alone, or take it together with the next pin. These choices give B[i+1], v[i] + B[i+1], and v[i] × v[i+1] + B[i+2], respectively. The third choice is allowed only when the next pin exists. We choose the maximum of the valid choices. The base case is B[n] = 0, meaning no pins remain. We compute the values from right to left, and the final answer is B[0]. There are n subproblems with constant work each, assuming constant-time arithmetic, so the algorithm takes O(n) time and O(n) space.

### 대본 교안과의 연결

| 문항 | 복습할 대본 구간 | 핵심 |
| --- | --- | --- |
| Q1 | 0:12~5:56 | SRTBOT 여섯 요소와 답안 구조 |
| Q2 | 6:03~8:32, 13:40~20:35, 24:17~26:39 | Merge Sort의 부분문제, 중복 재계산과 memoization의 차이 |
| Q3 | 8:41~24:11 | Fibonacci 재사용, 큰 정수의 비트 수, word 단위 덧셈 비용 합산 |
| Q4 | 37:02~57:01 | suffix 정의, skip/single/pair 선택, 경계 조건, 역순 계산 |
