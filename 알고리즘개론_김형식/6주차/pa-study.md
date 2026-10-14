# PA 구현 교안: Min-Max-Median Priority Queue

**한 줄 결론:** 작은 절반 LOW와 큰 절반 HIGH를 나누고 각 그룹을 min/max 두 heap으로 관리하여, 세 종류의 조회는 `O(1)`, 삽입·삭제는 worst-case `O(log n)`으로 구현했다.

근거: [pq.c](./pq.c), [과제 PDF](./Assignment_Min_Max_Median_PQ.pdf), [제출 보고서](./Min_Max_Median_PQ_Report_2024315329.pdf), [공식 예제 압축파일](./PQ_Examples.zip). 강의 정리는 [6주차 study](./study.md), 제출 완료 상태는 사용자 확인 기준이다.

## 목차

1. [요구 연산과 입출력](#요구-연산과-입출력)
2. [왜 네 개의 heap인가](#왜-네-개의-heap인가)
3. [Node ID와 위치 추적](#node-id와-위치-추적)
4. [실제 함수의 연결](#실제-함수의-연결)
5. [불변식과 동작 예시](#불변식과-동작-예시)
6. [복잡도와 테스트](#복잡도와-테스트)
7. [복습 질문](#복습-질문)

## 요구 연산과 입출력

프로그램은 고정 파일 `pq.in`에서 명령 수와 명령들을 읽고, `pq.out`에 조회 결과만 쓴다. 사용자의 일반적인 터미널 표준입출력 프로그램과 다르므로 실행 전 파일명과 작업 디렉터리가 중요하다.

| 명령 | 의미 | 출력 |
| --- | --- | --- |
| `I value` | 정수 삽입 | 없음 |
| `D M` | minimum 삭제 | 없음 |
| `D X` | maximum 삭제 | 없음 |
| `D E` | lower median 삭제 | 없음 |
| `F M` | minimum 조회 | 해당 정수 또는 `NULL` |
| `F X` | maximum 조회 | 해당 정수 또는 `NULL` |
| `F E` | lower median 조회 | 해당 정수 또는 `NULL` |

빈 큐에서 삭제는 아무것도 하지 않고, 조회는 `NULL`이다. 원소가 짝수 개이면 두 가운데 값의 평균이 아니라 **작은 쪽 가운데 값**을 median으로 사용한다. 저장 중인 값은 서로 다르다는 과제 입력 조건을 전제로 한다. 코드가 중복 값을 별도로 검사·거부하는 기능을 구현한 것은 아니다.

## 왜 네 개의 heap인가

```text
전체 원소의 정렬 순서

   LOW: 작은 ceil(n/2)개       HIGH: 큰 floor(n/2)개
  [ 1  3  5 ]                 [ 7  9 ]
       |                            |
  min-heap + max-heap          min-heap + max-heap
       |        |                  |        |
    minimum   median         재균형 경계   maximum
```

위 배열 표기는 정렬 순서를 설명하기 위한 것이다. 실제 LOW/HIGH의 heap 배열 자체가 오름차순 배열인 것은 아니다. heap은 root와 부모·자식의 우선순위만 보장한다.

작은 쪽만 max-heap, 큰 쪽만 min-heap으로 두면 median 경계 이동에는 좋지만 전체 minimum이나 maximum을 바로 읽을 수 없다. 그래서 그룹마다 두 heap을 두어 양 끝값을 동시에 얻는다. 같은 그룹의 두 heap에는 **동일한 Node ID 집합**이 들어간다.

## Node ID와 위치 추적

`nodes[]`의 각 원소는 실제 정수 `value`, 소속 `side`, min-heap 위치 `min_pos`, max-heap 위치 `max_pos`를 가진다. heap 배열에는 정수 값의 사본이 아니라 Node ID가 들어간다. 값을 비교할 때 ID를 통해 `nodes[id].value`를 읽는다.

```text
nodes[id] -> value, side, min_pos, max_pos
                        |          |
group.min_heap[index] ---+          |
group.max_heap[index] --------------+
```

왜 위치가 필요한가? minimum을 삭제하면 min-heap root에서 ID를 알 수 있지만, 같은 ID가 max-heap에서는 어디 있는지 바로 알 수 없다. 매번 선형 탐색하면 삭제가 `O(n)`으로 느려진다. 위치를 저장하면 두 번째 heap에서도 즉시 삭제 위치를 찾는다.

이 방식은 heap에서 swap할 때마다 위치 정보도 함께 갱신해야 한다. `heap_swap()`이 해당 min/max 위치를 바꾸는 이유다. 배열은 맞게 정렬되었는데 위치가 오래된 값이면, 다음 삭제가 엉뚱한 Node를 지울 수 있다.

## 실제 함수의 연결

| 함수 | 하는 일 | 다른 함수와의 관계 |
| --- | --- | --- |
| `comes_first` | min/max 모드에 맞는 우선순위 비교 | 모든 heap 비교의 기준 |
| `heap_swap` | heap ID 교환과 위치 갱신 | sift와 삭제의 기반 |
| `sift_up` | 부모보다 우선인 원소를 위로 이동 | 삽입 또는 대체 원소의 위쪽 위반 복구 |
| `sift_down` | 더 우선인 자식 방향으로 내려가기 | 아래쪽 위반 복구 |
| `group_add` | 한 ID를 해당 그룹의 두 heap에 넣기 | insert·경계 이동에 사용 |
| `heap_remove_at` | 지정 위치의 ID를 제거하고 마지막 ID로 빈자리 대체 | 선형 탐색 없는 임의 위치 삭제 |
| `group_remove` | 같은 ID를 min/max 두 heap에서 제거 | 그룹 크기는 두 삭제가 끝난 뒤 감소 |
| `move_boundary` | 한 그룹의 경계 ID를 다른 그룹으로 이동 | 전체 live 원소 수는 바뀌지 않음 |
| `rebalance` | LOW/HIGH 크기 조건 복원 | 매 업데이트 후 적용 |
| `insert` | 새 ID 생성, 경계 값과 비교해 그룹 선택 | `group_add` 후 재균형 |
| `remove_node` | 실제 원소 삭제와 전체 개수 감소 | `group_remove` 후 재균형 |
| `delete_min/max/median` | 각 root의 ID를 선택해 삭제 | 삭제의 공통 처리는 `remove_node` |
| `find_min/max/median` | 해당 root의 value 반환 | 큐 상태를 바꾸지 않음 |
| `main` | 명령 해석, 빈 큐 처리, 파일 출력 | 비어 있을 때 root를 읽지 않도록 분기 |

### 임의 위치 삭제

`heap_remove_at()`은 마지막 ID를 삭제 위치로 옮긴다. 이 대체 ID가 부모보다 우선이면 sift-up, 그렇지 않으면 sift-down으로 복구한다. 한 위치에서 위반될 수 있는 방향을 확인해 움직인다.

`group_remove()`는 먼저 제거할 ID의 두 위치를 확보한다. min-heap을 수정한다고 max-heap 배열까지 같은 방식으로 움직이는 것은 아니므로, 둘은 각각 복구한다. 두 heap이 공유하는 `size`는 양쪽 삭제가 끝난 뒤 한 번 줄인다.

### 경계 이동과 재균형

LOW가 너무 크면 LOW의 최대값을 HIGH로, HIGH가 너무 크면 HIGH의 최소값을 LOW로 옮긴다. 임의 원소를 옮기면 작은 절반/큰 절반의 순서 조건이 깨질 수 있으므로 반드시 경계값을 선택한다.

한 번의 insert/delete 전에 크기 조건이 맞고 업데이트가 원소 하나만 바꾸므로, 복원에 필요한 경계 이동은 최대 한 번이다. 한 번 이동하는 동안 고정 개수의 heap 삽입·삭제를 수행하므로 전체 업데이트는 여전히 `O(log n)`이다.

## 불변식과 동작 예시

항상 유지할 조건은 다음 세 개다.

1. LOW의 모든 값은 HIGH의 모든 값보다 작다.
2. LOW 크기는 HIGH와 같거나 정확히 1 크다.
3. 한 그룹의 min/max heap은 같은 ID들을 가지며, 모든 저장 위치가 현재 heap 배열과 일치한다.

예시에서 그룹 안의 값은 설명을 위해 정렬해서 표시했다.

```text
insert 5: LOW [5]      HIGH []       median 5
insert 1: LOW [1]      HIGH [5]      median 1
insert 9: LOW [1,5]    HIGH [9]      median 5
insert 3: LOW [1,3]    HIGH [5,9]    median 3
delete median:
          LOW [1,5]    HIGH [9]      median 5
```

짝수 원소에서 LOW의 최대값이 작은 가운데 값이다. 삭제 후 HIGH의 최소값이 LOW로 이동해 크기를 맞추는 것을 볼 수 있다. maximum은 HIGH가 비어 있지 않으면 HIGH max root, 비어 있으면 LOW max root다. 후자의 대표 사례가 원소 한 개일 때다.

이 설계는 삭제를 뒤로 미루는 lazy deletion이 아니다. 삭제한 ID를 두 active heap에서 즉시 제거한다. 다만 `nodes[]`의 ID 슬롯은 재사용하지 않고 `next_id`가 계속 증가한다. **heap에서 빠짐**과 **ID 저장 슬롯 재사용**은 다른 문제다.

## 복잡도와 테스트

| 연산 | worst-case 시간 | 이유 |
| --- | --- | --- |
| insert | `O(log n)` | 두 heap 삽입, 최대 한 경계 이동 |
| delete min/max/median | `O(log n)` | 위치로 직접 제거, 두 heap 복구, 재균형 |
| find min/max/median | `O(1)` | root 접근 |

코드는 최대 명령 수 500,000에 맞춘 고정 배열을 예약한다. 4바이트 int 환경에서는 Node 배열과 네 ID 배열이 약 16MB다. 공간은 현재 live 원소 수 n이 아니라 **최대 명령 한도 N에 대한 `O(N)`**이다. 삭제해도 이전 Node ID를 재사용하지 않으므로 `O(n)` 공간이라고만 쓰면 실제 구현과 맞지 않는다.

보고서가 기록한 검증은 공식 예제 두 개와 sorted-list 기준 5,000개 random 연산 비교다. 성능은 Linux gcc 14.2.0, C89 설정에서 insertion-heavy/delete-heavy/mixed workload를 각각 7회 실행한 중앙값으로 기록되어 있다. 500,000명령의 값은 각각 104.240ms, 137.019ms, 101.213ms다.

위 수치는 **제출 보고서의 실험 기록**이며 이번 정리 작업에서 재측정한 값은 아니다. 파일 I/O를 포함한 전체 프로그램 시간이고 입력 생성 시간은 제외한 것으로 설명되어 있다. 상수 시간 측정값과 점근 복잡도는 서로 다른 종류의 근거다.

재실행할 때에는 `pq.in`과 `pq.out`을 기존 제출 폴더와 분리한 임시 디렉터리에서 다룬다. empty, 원소 한 개, 홀짝 크기 전환, 연속 median 삭제, 음수·0, 삭제한 값을 다시 삽입하는 사례가 특히 위치·재균형 검증에 중요하다.

## 복습 질문

1. median이 평균이 아니라 LOW의 최대값인 이유는?
2. min/max heap 두 개에 같은 Node ID를 넣어도 실제 원소가 두 개가 되는 것은 아닌 이유는?
3. 위치 정보 없이 두 번째 heap에서 삭제하면 어떤 비용이 생기는가?
4. 마지막 ID를 빈자리에 옮긴 뒤 왜 sift-up과 sift-down을 구분하는가?
5. LOW에서 HIGH로 이동할 때 최대값을 고르는 이유는?
6. find와 delete가 같은 root를 읽는데 복잡도가 다른 이유는?
7. live 원소가 1개여도 예약된 메모리가 작아지지 않는 이유는?

[알고리즘 PA 목차](../pa-study.md)
