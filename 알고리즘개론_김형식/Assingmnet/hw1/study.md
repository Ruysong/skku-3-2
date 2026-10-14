# hw1 구현 교안: 연결리스트 Stable Merge Sort

**한 줄 결론:** `*` 표시가 붙은 attribute를 문자열 key로 선택하고, 레코드 Node들의 `next`를 바꾸어 안정적인 오름차순 정렬을 수행했다.

근거: [실제 hw1.c](./Project1/Project1/hw1.c), [과제 PDF](./Assignment01-3.pdf), [최종 제출 보고서](./Junhyuk%20Song_Assignment1_Report_Stable_Merge_Sort.pdf). 같은 폴더의 다른 보고서 초안과 Visual Studio 빌드 산출물은 최종 구현 근거와 구분한다.

보관된 예제: [hw1_input.txt](./hw1_input.txt), [예상 출력 파일](./hw1_output-1.txt). 실행 코드는 결과를 `hw1_output.txt`에 쓰므로 예상 출력 파일명을 그대로 결과 파일명으로 혼동하지 않는다.

## 목차

1. [전체 흐름](#전체-흐름)
2. [입력과 Node](#입력과-node)
3. [분할·재귀·병합](#분할재귀병합)
4. [정확성과 안정성](#정확성과-안정성)
5. [복잡도·테스트 근거](#복잡도테스트-근거)
6. [복습할 질문](#복습할-질문)

## 전체 흐름

```text
hw1_input.txt
  n -> $ -> attribute header -> $ -> n records
                  |
          find '*' -> keyslot
                  |
      extract_key() + malloc(Node)
                  |
      head -> Node -> Node -> ... -> NULL
                  |
             merge_sort(head)
          split / recurse / merge
                  |
      sorted list -> hw1_output.txt
                  |
              free all Nodes
```

파일을 읽고 정렬하는 프로그램이지, 배열의 정수만 정렬하는 예제는 아니다. 원래 레코드 문자열과 정렬 key를 나누어 저장한 덕분에 결과 파일에는 원래 레코드를 그대로 쓸 수 있다.

## 입력과 Node

실제 Node는 다음 세 정보를 가진다.

```text
Node
  key[16]    정렬에 사용할 attribute 문자열
  next       다음 Node 주소
  data[1024] 원래 레코드 한 줄
```

`key[16]`은 최대 15문자와 문자열 종료문자를 담는 크기다. 파일 읽기용 `line[2048]`과 Node의 `data[1024]`는 서로 다른 배열이다. 큰 읽기 버퍼가 곧 Node 저장 한도를 늘려주는 것은 아니다. 과제의 정상 레코드 길이 제한을 전제로 한다.

header의 `*` 앞에 있는 `:` 개수를 세어 `keyslot`을 정한다. `P_Id*`라면 0, `P_Id:LastName*`이라면 1이다. 정렬 key를 첫 번째 필드로 고정하지 않는다.

`extract_key()`는 현재 field 번호를 추적하다가 `keyslot`에 도달하면 다음 `:` 또는 문자열 끝까지 복사한다. 따라서 key 앞의 다른 field가 비어 있어도 `:`를 기준으로 field 번호가 올라간다. 비교에는 `strcmp()`를 쓰므로 숫자처럼 보이는 key도 문자열 사전순이다. 예를 들어 일반적으로 `"10"`은 `"2"`보다 먼저 온다.

각 줄의 CR/LF를 제거한 뒤 `malloc(sizeof(Node))`, 원문 저장, key 추출, `next = NULL`을 수행한다. `head`는 첫 Node, `tail`은 마지막 Node다. 뒤에 붙일 때 `tail->next`와 `tail`을 갱신하므로 입력 순서대로 리스트가 만들어진다.

## 분할·재귀·병합

### 1. 분할: slow / fast

```text
slow = head
fast = head->next

slow: 한 칸씩
fast: 두 칸씩

fast가 끝에 도달하면 slow는 왼쪽 마지막 Node
second = slow->next
slow->next = NULL
```

`second`를 저장하고 실제 연결을 끊는 것이 중요하다. 연결을 끊지 않으면 왼쪽 재귀가 같은 긴 리스트를 계속 받아 길이가 줄지 않을 수 있다. 두 하위 리스트는 각각 기존 Node를 가지고 있으며 내부 상대 순서는 유지된다.

### 2. 재귀: merge_sort

빈 리스트 또는 Node 한 개이면 이미 정렬되어 있으므로 그대로 반환한다. 그 외에는 왼쪽·오른쪽을 각각 재귀 정렬하고 `merge(left, right)`의 결과를 반환한다. 매번 작은 리스트로 내려가므로 종료 조건에 도달한다.

### 3. 병합: merge

`a`, `b`는 두 정렬된 리스트에서 아직 처리하지 않은 첫 Node를 가리킨다. `strcmp(a->key, b->key) <= 0`이면 왼쪽 Node를 선택한다. 선택한 Node를 `stash`에 보관하고 원래 리스트의 시작 포인터를 한 칸 옮긴 뒤, 선택 Node의 연결을 정리하여 결과 tail에 붙인다.

```text
left :  [Amy] -> [Kim] -> [Park]
right:  [Han] -> [Kim] -> [Yu]
         |
compare first remaining keys
         |
smaller Node -> stash -> result tail
```

둘 중 한쪽이 소진되면 남은 정렬 리스트 전체를 결과 tail 뒤에 붙인다. 이미 정렬된 suffix이므로 다시 하나씩 비교할 필요가 없다. 병합 과정에서 레코드를 새 배열로 복사하거나 새 Node를 할당하지 않는다.

## 정확성과 안정성

정렬의 목표는 **오름차순 + 원래 원소들의 보존**이다. 키만 정렬되어도 원문 레코드를 잃거나 중복 생성했다면 올바른 결과가 아니다.

보고서의 split invariant는 slow가 k칸, fast가 가능한 범위에서 2k+1칸 이동했다는 관계다. 종료 시 리스트를 거의 같은 크기로 나눌 수 있음을 설명한다. merge invariant는 결과 prefix가 정렬되어 있고, 지금까지 뽑은 Node들을 정확히 한 번 포함하며, `a`·`b`가 남은 suffix의 시작이라는 것이다.

**안정성의 핵심은 `<=`다.** 동일 key가 양쪽 리스트에 있으면 왼쪽 Node를 먼저 뽑는다. 분할이 원래 순서를 보존하고 각각의 재귀도 stable하므로, 병합 후에도 같은 key의 원래 입력 순서가 유지된다. 비교를 `<`로 바꾸어 오른쪽을 먼저 선택하면 두 리스트를 가로지르는 동점 순서가 바뀔 수 있다.

예를 들어 입력 순서가 `Kim(record A)`, `Kim(record B)`라면 정렬 뒤에도 A가 B보다 먼저여야 한다. stable이라는 말은 모든 레코드를 원래 자리에 둔다는 뜻이 아니라 **동점끼리의 상대 순서**를 보존한다는 뜻이다.

재귀 전체의 정확성은 Node 수에 대한 귀납법으로 연결된다. 크기 0·1은 맞고, 더 작은 두 리스트의 정렬을 가정한 뒤 올바른 merge를 적용하면 현재 크기의 리스트도 맞는다. 이때 정렬됨, Node 보존, 안정성을 함께 확인한다.

## 복잡도·테스트 근거

한 호출의 중간 지점 탐색과 병합은 각각 선형이다. 균형 분할이므로 대략 `T(n) = 2T(n/2) + O(n)`이며 정렬 시간은 `O(n log n)`이다. key 길이가 과제에서 제한되어 있으므로 `strcmp` 비용을 상수로 보는 분석이다. 임의 길이 key까지 일반화하면 문자열 비교 비용도 따로 포함해야 한다.

리스트 입력 보관은 `O(n)` 공간이고, 정렬의 재귀 stack은 `O(log n)`이다. merge 자체는 기존 Node를 재연결하므로 별도의 `O(n)` 병합 배열이 없다. 따라서 **입력 저장 공간**과 **정렬 추가 공간**을 구분해야 한다.

보고서 4페이지 Testing은 테스트해야 할 사례를 설명한다. 실제 실행 로그나 각 사례의 통과 표가 있는 형태는 아니므로 아래를 검증 완료 목록으로 읽지 않는다.

| 복습·재실행 사례 | 확인할 성질 |
| --- | --- |
| `P_Id*`와 `LastName*` | key 위치가 header에 따라 바뀜 |
| 뒤쪽 field에 `*` | `keyslot`이 고정되지 않음 |
| 동일 key 두 레코드 | 원래 입력 순서 유지 |
| 빈 non-key field | field 번호 추적이 유지됨 |
| 0·1개, 이미 정렬, 역순 | 종료·분할·병합 경계 |
| 큰 입력 | 배열 복사 없이 `O(n log n)` 흐름 유지 |

이번 아카이브 점검은 코드와 보고서 대조이며 테스트를 새로 실행한 기록은 아니다. Ubuntu gcc 빌드와 hidden test 결과도 이 문서만으로 확정하지 않는다.

## 복습할 질문

1. `data`와 `key`를 따로 두는 이유는?
2. `keyslot`과 key의 값은 어떻게 다른가?
3. slow/fast 분할에서 `slow->next = NULL`을 빼면 어떻게 되는가?
4. 안정성을 보장하는 비교 조건은 어디에 있는가?
5. `stash`로 옮기는 것은 데이터 복사인가, Node 재연결인가?
6. 한쪽 리스트가 비었을 때 suffix를 바로 붙여도 되는 이유는?
7. 리스트 `O(n)` 공간과 추가 `O(log n)` 공간은 왜 모순이 아닌가?

[알고리즘 PA 목차](../../pa-study.md)
