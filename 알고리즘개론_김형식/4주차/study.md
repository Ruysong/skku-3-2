# 알고리즘개론 - 김형식 4주차 스터디

## 빠른 목차

- Part A. 이번 주 iCampus 묶음과 마감
- Part B. Sorting: Sorting Basic, Quicksort, Average Case, Challenges, Decision Tree
- Part C. Hashing 외부 강의: set interface부터 universal hashing까지
- Part D. Hashing quiz 준비
- Part E. 복습 질문과 영어 서술 포인트

## Part A. 이번 주 콘텐츠와 마감

| 콘텐츠 | 길이/형식 | 관리 |
| --- | --- | --- |
| Sorting PDF | 원본 자료 | Sorting unit 기준 |
| 09 SortingBasic | 14:28 | 강의 |
| 10 Quicksort | 26:23 | 강의 |
| 11 Quicksort average | 32:55 | 강의 |
| 12 Sorting Challenges | 07:47 | 강의 |
| 13 Decision Tree | 09:41 | 강의 |
| 4. Hashing | 52:55 | MIT 6.006 외부 강의 |
| Hashing quiz | 4문제, 4점 | 2026-09-27 23:59 마감 |

화면상 5주차 묶음으로 표시되지만 여러 item에는 4주차 표기가 남아 있다. 이번에는 주차 숫자보다 9월 27일 마감의 Sorting/Hashing 묶음으로 관리한다.

---

# Part C. Hashing - MIT 6.006 Lecture 4 교안

## 1. 이 강의의 질문

Hashing은 set data structure에서 다음 세 operation을 빠르게 만들려는 방법이다.

- find(k): key k를 가진 item이 있는지 찾기
- insert(x): item을 넣기
- delete(k): key k의 item을 지우기

이전 set 구현의 출발점은 unsorted array였다. 모든 item을 linear scan하면 find, insert, delete 중 적어도 여러 operation이 linear time이 된다. Array를 정렬해 binary search를 쓰면 static find는 logarithmic time까지 줄일 수 있지만, build와 dynamic update가 부담스럽다.

    unsorted array
        -> find: Theta(n)
    sorted array + binary search
        -> static find: Theta(log n)
    hashing
        -> expected find/insert/delete: Theta(1)

강의의 핵심은 왜 comparison만으로는 log n보다 빠를 수 없는지 보이고, RAM의 random access를 이용하면 어떻게 expected constant time을 얻는지 설명하는 것이다.

## 2. Comparison model과 decision tree lower bound

Comparison model에서는 item/key를 black box처럼 다룬다. Key의 실제 bit나 number를 주소로 쓰지 못하고, 두 key를 비교해서 less-than, equal, greater-than 같은 결과만 얻는다. 강의는 이 결과를 true/false 두 갈래 branch로 단순화해 설명한다.

    comparison
      |
      +-- true
      |
      +-- false

Comparison algorithm은 decision tree로 표현할 수 있다.

- Internal node: key comparison
- Edge: comparison outcome
- Leaf: algorithm이 더 이상 branch하지 않고 output을 반환하는 지점
- Root-to-leaf path: 한 execution이 실제로 수행한 comparison sequence

n개 item을 저장한 correct find algorithm은 최소 n+1개의 output을 구분해야 한다.

1. 저장된 n개 item 각각을 return할 수 있어야 한다.
2. searched key가 set에 없다는 결과도 return할 수 있어야 한다.

따라서 decision tree에는 최소 n+1 leaf가 필요하다. Binary tree height h는 최대 2^h leaf를 가지므로,

    2^h >= n + 1
    h >= log2(n + 1)

Worst-case comparison 수는 가장 긴 root-to-leaf path, 즉 tree height다. 따라서 comparison model의 correct find는 worst case Omega(log n)보다 빨라질 수 없다. Balanced binary search가 Theta(log n)을 달성하므로 이 model 안에서는 log n이 맞는 경계다.

중요한 해석은 이렇다. Binary search가 느려서 log n인 것이 아니라, key를 비교하는 것만 허용하는 model 전체가 log n lower bound를 가진다. 더 빠르려면 comparison보다 강한 operation이 필요하다.

## 3. RAM random access와 direct access array

Word-RAM은 number를 address로 사용해 memory의 임의 위치로 constant time에 접근할 수 있다. 이 one-step random access는 comparison의 constant branching과 다르게, 많은 위치 중 하나를 직접 고를 수 있다.

Integer key k가 있을 때 direct access array A에서 item을 A[k]에 둔다.

    key 10 -> A[10]
    find(10)   -> A[10] 확인
    insert(x)  -> A[key(x)]에 저장
    delete(10) -> A[10] 비움

Set semantics에서는 동일 key의 새 insert가 기존 item을 replace할 수 있으므로 slot 하나에 item 하나만 두는 것이 맞다.

| operation | direct access array time |
| --- | --- |
| find | Theta(1) worst case |
| insert | Theta(1) worst case |
| delete | Theta(1) worst case |

문제는 space다. n은 현재 저장된 item 수, u는 가능한 key universe의 크기라고 하자. 예를 들어 학생 300명을 MIT ID 같은 9-digit integer로 저장하면 n은 수백인데 u는 대략 10^9일 수 있다. Direct access array는 Theta(u) space를 요구하므로 u가 n보다 훨씬 크면 대부분이 빈 slot인 거대한 array가 된다.

또한 direct access array는 set interface에는 맞지만 ordered iteration 같은 sequence operation에는 좋지 않다. Sparse하게 흩어진 key의 다음 element를 찾으려면 빈 slot을 scan할 수 있기 때문이다.

## 4. Integer key와 word-RAM 가정

Direct access와 hashing은 key를 array index calculation에 사용하므로 integer key가 필요하다. Comparison model은 comparison만 가능하면 arbitrary object key도 다룰 수 있었지만, 여기서는 key를 numeric address로 해석할 수 있어야 한다.

Machine word size를 w bit라고 하면 one word에 담기는 integer와 address range는 2^w와 관계가 있다. Key가 one word에 들어가고 u < 2^w라고 가정해야 arithmetic과 array access를 constant time으로 볼 수 있다.

시험 문제에서 integer나 string length에 별도 bound가 주어지지 않으면, 이 수업의 RAM model에서는 input key가 one word에 들어간다고 가정한다. 이 가정이 없으면 매우 큰 integer의 arithmetic 자체가 constant time이 아닐 수 있다.

## 5. Hash function: 큰 universe를 작은 table로 압축

Direct access의 space 문제를 피하려면 U = {0, ..., u-1}의 큰 key space를 table index {0, ..., m-1}로 map한다.

    h: U -> {0, 1, ..., m-1}

여기서 m은 hash table length이며 n과 비슷한 크기, 정확히는 linear in n으로 잡고 싶다.

    large universe U
            |
            | h(k)
            v
    small table of m slots

Item x는 A[h(key(x))]에 접근해 저장하거나 찾는다. h 계산과 array access가 constant time이면 direct access의 speed를 유지하면서 array size를 u가 아니라 m으로 줄일 수 있다.

## 6. Collision은 피할 수 없다

u > m이면 pigeonhole principle상 서로 다른 key a, b가 같은 index로 map되는 경우가 반드시 있다.

    a != b
    h(a) = h(b)

이것이 collision이다. 더 강하게, universe가 table보다 훨씬 크면 어떤 h를 택해도 특정 input set은 한 index에 많은 key를 몰아넣을 수 있다. 단순히 h를 하나 고정했다고 모든 input에서 evenly distributed된다는 보장은 없다.

따라서 hash table의 진짜 문제는 collision이 생기지 않게 하는 것이 아니라, collision이 생겨도 find/insert/delete가 빠르게 유지되도록 처리하는 것이다.

## 7. Collision resolution 두 가지

### Open addressing

m을 n보다 크게 두고 collision이 난 item을 table 안의 다른 empty slot에서 찾는 방식이다. Python의 hash table 구현은 이런 계열의 scheme을 사용한다. Table 밖에 별도 container를 만들지 않고 array 내부의 다른 위치를 probe한다.

장점은 implementation에서 pointer chain을 두지 않아도 된다는 점이다. 그러나 어떤 probe sequence를 쓰는지, load factor가 어떻게 영향을 주는지에 따라 analysis가 복잡하다. 강의는 open addressing을 소개만 하고 formal analysis는 하지 않는다.

### Chaining

각 table slot에는 하나의 item 대신 collision item을 담을 secondary data structure의 pointer를 둔다. 이 secondary container를 chain이라고 부른다. 흔한 선택은 linked list지만 dynamic array, sorted array 등 sequence data structure라면 무엇이든 가능하다.

    table index i
        |
        +-> chain: [a] -> [b] -> [c]

find(k)는 다음 순서다.

1. i = h(k)를 계산한다.
2. table i에 연결된 chain으로 간다.
3. chain 안에서 key k를 linear search한다.
4. 일치하면 return, 끝까지 없으면 not found다.

Insert/delete도 해당 chain에서 수행한다. Chain을 어떤 structure로 구현하느냐보다 chain length가 짧다는 성질이 더 중요하다. Chain length가 constant면 그 안을 linear scan해도 constant time이다.

초기화 때 모든 slot에 empty chain을 두는 방법과, 첫 item은 slot에 직접 두고 collision 때만 secondary structure를 만드는 방법이 있다. 이는 constant factor와 implementation detail의 차이이며, m = Theta(n)이면 전체 space는 여전히 Theta(n)이다.

## 8. Division method와 fixed deterministic hash의 한계

가장 쉬운 hash function은 division method다.

    h(k) = k mod m

k를 m으로 modulo하면 결과는 0부터 m-1 범위에 들어간다. Input key가 h와 무관하게 uniformly distributed되어 있다면 잘 작동할 수 있다. 실제 implementation은 bit mixing을 더한 뒤 modulo하는 식으로 사용되기도 한다.

그러나 h가 fixed deterministic이면 h를 알고 있는 adversarial input 또는 상관된 input이 모든 key를 같은 slot에 보내도록 만들 수 있다. 그러면 chain length가 n이 되고 operation이 Theta(n)으로 퇴화한다.

여기서 중요한 구분:

- 좋은 average-looking input에서 잘 돌아감
- 모든 possible input에 대해 performance guarantee가 있음

강의가 원하는 것은 두 번째다. Key distribution을 신뢰하지 않고도 expected running time을 증명하려면 input이 아니라 hash function 선택에 randomness를 둔다.

## 9. Universal hashing family

Hash function 하나를 고정하는 대신, family H에서 hash function 하나를 random하게 고른다. User가 key set을 결정한 뒤 table이 a, b를 random하게 골라 h를 정하므로 user가 특정 fixed h에 맞춘 worst input을 미리 구성하기 어렵다.

강의의 universal family 예시는 다음 form이다.

    h_a,b(k) = ((a*k + b) mod p) mod m

Parameters:

| symbol | 의미 |
| --- | --- |
| m | hash table length |
| u | key universe size |
| p | u보다 큰 fixed prime |
| a | 1부터 p-1 사이에서 random 선택 |
| b | 0부터 p-1 사이에서 random 선택 |
| k | integer key |

a는 0이면 모든 key information을 잃을 수 있으므로 0이 아니어야 한다. Table을 만들 때 p를 정하고, a와 b를 random하게 골라 family의 한 function을 instance로 사용한다.

Universal property는 서로 다른 두 key x != y에 대해 다음을 보장한다.

    Pr[h(x) = h(y)] <= 1/m

Probability는 random choice of h, 즉 random choice of a and b에 대한 것이다. 모든 pair가 충돌할 확률이 table slot 하나를 무작위로 고른 수준인 1/m 이하라는 뜻이다. Universal은 특정 하나의 formula 이름이 아니라 이 property를 가진 hash family를 설명하는 형용사다.

## 10. Chaining expected time 증명

Stored key를 k_i, k_j라고 하자. Indicator random variable X_ij를 정의한다.

    X_ij = 1  if h(k_i) = h(k_j)
           0  otherwise

k_i가 들어간 chain length는 그 key와 collision하는 stored key의 수를 모두 더한 값이다.

    C_i = sum over j of X_ij

j = i인 항은 key가 자기 자신과 항상 충돌하므로 1이다. j != i인 항은 universality에 의해 expectation이 최대 1/m이다.

Linearity of expectation을 쓰면,

    E[C_i]
      = 1 + sum over j != i E[X_ij]
      <= 1 + (n - 1) / m

여기서 random variable들이 서로 independent일 필요는 없다. 기대값의 linearity는 independence 없이도 적용된다. 이 점은 시험에서 자주 헷갈린다.

m이 n의 constant factor 이상, 즉 m = Theta(n)이면,

    E[C_i] <= 1 + (n - 1)/m = Theta(1)

따라서 chained hash table의 find, insert, delete는 expected Theta(1) time이다. Worst case가 사라진 것은 아니다. 특정 random h 선택에서는 긴 chain이 생길 수 있지만, random hash function choice에 대한 expectation이 constant라는 guarantee다.

## 11. Dynamic hash table과 rebuild

n은 insert/delete에 따라 변하는데 m을 고정하면 load factor n/m이 커질 수 있다. 그러면 expected chain length 1 + (n-1)/m도 더 이상 constant가 아니다.

해결은 dynamic array와 비슷하다.

1. n이 커져 m이 n에 비례하지 않게 되면 larger m을 고른다.
2. 새 table을 만든다.
3. 새 random a, b도 골라 새로운 universal hash function을 정한다.
4. 기존 item 전부를 새 table에 rehash한다.

Rebuild 한 번은 많은 item을 옮기므로 비싸지만, table size를 적절히 geometric하게 조정하면 rebuild가 모든 operation마다 일어나지 않는다. Amortized analysis로 update당 expected constant time을 유지할 수 있다.

## 12. 전체 흐름 한 장

    set interface
      |
      +-- unsorted array: linear scan
      +-- sorted array: comparison find Theta(log n)
      |
      +-- comparison model lower bound
      |       n+1 outputs -> decision tree height Omega(log n)
      |
      +-- RAM random access
              |
              +-- direct access array
              |       O(1) operations, Theta(u) space
              |
              +-- hash table h: U -> [0,m)
                      |
                      +-- collision unavoidable
                      |
                      +-- open addressing: table 내부 다른 slot
                      |
                      +-- chaining: slot -> short secondary chain
                              |
                              +-- fixed h: adversarial input 가능
                              |
                              +-- universal random h
                                      |
                                      +-- pair collision probability <= 1/m
                                      +-- expected chain length <= 1+(n-1)/m
                                      +-- m = Theta(n) -> expected Theta(1)

## Part D. Hashing quiz 준비

이 퀴즈에서 답으로 바로 연결되는 포인트:

- Priority queue와 hash table은 목적이 다르다. Heap은 max/min priority를, hash table은 key 기반 set lookup을 빠르게 한다.
- Direct access array는 find/insert/delete worst-case Theta(1)이지만 Theta(u) space다.
- Hashing은 large universe를 m slots로 compress해서 space를 Theta(m)으로 줄인다.
- Collision은 bug가 아니라 unavoidable property다.
- Chaining은 slot마다 secondary sequence를 두는 방식이다.
- Universal hashing의 constant time은 worst-case deterministic guarantee가 아니라 random h 선택에 대한 expected guarantee다.
- m = Theta(n)을 유지해야 chain expectation이 constant다.
- Expected value linearity에는 independence가 필요 없다.

영어 서술형 뼈대:

A hash table maps each integer key k to an array index h(k) in the range 0 to m-1. Since different keys can map to the same index, collisions are handled by chaining, where each slot stores a collection of items. With a universal hash family, any two distinct keys collide with probability at most 1/m. Therefore, the expected chain length is at most 1 + (n-1)/m. If m is proportional to n, find, insert, and delete take expected O(1) time.

## Part E. 복습 질문

1. Comparison model에서 find가 Omega(log n)보다 빨라질 수 없는 이유는?
2. Decision tree leaf가 n+1개 필요한 이유는?
3. Direct access array가 constant-time인데도 일반적으로 쓰기 어려운 이유는?
4. n, u, m, w는 각각 무엇인가?
5. Collision이 왜 pigeonhole principle상 unavoidable한가?
6. Open addressing과 chaining은 충돌을 각각 어떻게 처리하는가?
7. Fixed division method가 모든 input에서 좋은 performance를 보장하지 못하는 이유는?
8. Universal property는 어떤 probability를 bound하는가?
9. X_ij는 무엇을 나타내며 E[X_ij]는 j != i일 때 왜 at most 1/m인가?
10. Expected chain length에 self term 1이 포함되는 이유는?
11. 왜 linearity of expectation에 independence가 필요 없는가?
12. Dynamic hash table이 rebuild할 때 왜 새 hash function까지 골라도 되는가?

## 출처와 범위

- Hashing 강의: 사용자가 제공한 MIT 6.006 Lecture 4 전체 transcript, 0:00-52:48
- Quiz 정보: iCampus Hashing, 4문제 4점, 2026-09-27 23:59
- Sorting 원본: Sorting.pdf 및 iCampus 영상 목록
- 다음 내용: 강의 말미의 안내대로 이후 faster sorting unit으로 연결