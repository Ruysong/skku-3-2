# 알고리즘개론 - 김형식 5주차 스터디

## 빠른 목차

- Part A. 이번 주 iCampus 묶음과 마감
- Part B. Linear Sorting: Counting Sort와 Radix Sort
- Part C. Comparison Sorting의 Lower Bound
- Part D. 제출 항목과 공부 순서
- Part E. 자료 수령 후 추가할 상세 교안

## Part A. 5주차 진도와 변경된 iCampus 이용 기간

이 자료는 이번 **5주차 진도**로 파악했으나, 최신 iCampus 화면에서는 묶음 전체가 `6주차`에 들어가 있고 2026-10-05 00:00에 열려 2026-10-11 23:59에 종료된다. 한 주 연기된 것으로 보이지만 변경 이유는 확인되지 않았다. `Linear Sorting` 영상의 내부 출결 표기는 여전히 `5주차 2차시`다.

| 콘텐츠 | 길이/형식 | 의미 | 마감 |
| --- | --- | --- | --- |
| Count sort | PDF + 교수 영상 19:18 | key 범위가 제한된 정수 정렬 | 10/11 23:59 |
| Radix sort | PDF + 교수 영상 09:50 | stable sort를 자릿수별로 반복 | 10/11 23:59 |
| Linear Sorting | 외부 강의 51:57 | comparison lower bound를 넘는 정렬의 조건 | 10/11 23:59 |
| Lecture 7: Counting Sort, Radix Sort, Lower Bounds for ... | 외부 강의 52:09 | Count/Radix와 lower bound 연결 | 10/11 23:59 |
| Linear Sorting quiz | 4문제, 4점 | 정렬 내용 확인 퀴즈 | 10/11 23:59 |
| Prove that `log(n!) = Theta(n log n)` | 파일/서술 제출, 1점 | 비교정렬 lower bound의 핵심 보조정리 | 10/11 23:59 |
| Programming: Min-Max-Median Priority Queue | 프로그래밍 과제, 10점 | priority queue 응용 구현 | 10/11 23:59 |

---

## Part B. Linear Sorting: Counting Sort와 Radix Sort

이번 진도의 질문은 다음과 같다.

> 비교만 하는 정렬은 왜 `Omega(n log n)`보다 빨라질 수 없고, key의 구조를 이용하면 언제 더 빨라질 수 있는가?

### 1. Counting Sort

- 원소끼리 비교하지 않고, 각 key가 몇 번 등장하는지 count한다.
- key의 범위가 `0 ... k`처럼 제한돼 있을 때 `O(n + k)` 시간으로 동작한다.
- prefix sum을 이용해 각 key가 출력 배열에서 차지할 위치를 계산한다.
- 같은 key의 원소 순서를 유지하도록 구현하면 stable sort가 된다.

### 2. Radix Sort

- 한 번에 전체 key를 비교하지 않고, 일의 자리부터 높은 자리까지 안정 정렬을 반복한다.
- 각 자리 정렬에 stable counting sort를 쓰는 것이 대표적이다.
- 하위 자릿수의 순서가 보존돼야 다음 자릿수 정렬 결과가 전체 key 정렬로 이어진다.

### 3. 비교 정렬과의 차이

```text
comparison sort
  -> 비교 결과만으로 순서를 판단
  -> decision tree leaf가 n!개 필요
  -> worst case Omega(log(n!)) = Omega(n log n)

counting / radix sort
  -> key 값 또는 자릿수에 직접 접근
  -> comparison model 밖의 정보 사용
  -> key 범위와 자릿수 조건이 맞으면 linear 또는 near-linear time 가능
```

---

## Part C. Comparison Sorting의 Lower Bound

`n`개의 서로 다른 원소는 가능한 순서가 `n!`개다. comparison sorting algorithm은 입력 순서를 판별해 올바른 permutation을 선택해야 하므로 decision tree의 leaf가 적어도 `n!`개 필요하다.

```text
tree height >= log2(n!)
             = Theta(n log n)
```

이번 증명 과제 `log(n!) = Theta(n log n)`는 이 lower bound를 formal하게 완성하는 부분이다. 상세 증명과 Count/Radix의 정확성·복잡도 분석은 자료를 받은 뒤 이 문서에 추가한다.

---

## Part D. 제출 항목과 공부 순서

1. Count Sort와 Radix Sort 자료를 보고 입력 조건, stable 여부, 시간/공간 복잡도를 정리한다.
2. Linear Sorting 외부 강의로 decision tree와 `log(n!)` lower bound 연결을 확인한다.
3. `log(n!) = Theta(n log n)` 증명 과제를 먼저 마감 전 제출한다.
4. Linear Sorting 퀴즈를 응시한다.
5. Min-Max-Median Priority Queue 과제는 별도 설계·구현·테스트 시간을 확보한다.

## Part E. 자료 수령 후 추가할 상세 교안

2026-10-10 전달받은 Jason Ku `5. Linear Sorting` 대본의 상세 교안은 [6주차 study의 Part B](../6주차/study.md#part-b-mit-linear-sorting-교안)에 정리했다. 해싱 복습, 결정 트리 하한, tuple 예제, 안정성, 버킷 방식 Counting Sort, Radix Sort를 포함한다. 이 파일의 초기 학습지도와 원본 PDF의 별도 전문 검수 여부는 구분한다.

- Count Sort의 count/prefix sum/output placement 단계와 stable 조건
- Radix Sort의 LSD/MSD 방식, base와 digit 수에 따른 복잡도
- `log(n!)`의 upper/lower bound를 각각 보이는 증명
- comparison decision tree의 leaf 수와 sorting lower bound
- Linear Sorting 퀴즈 복기
- Min-Max-Median Priority Queue 과제 명세, 설계, 테스트 기록
