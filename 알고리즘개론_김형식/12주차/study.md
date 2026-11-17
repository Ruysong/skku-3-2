# 알고리즘개론 12주차

기간: **2026-11-16(월)~11-22(일)** · API 확인: **2026-11-16 10:31 KST 전후**.

이번 주 주제: **위상정렬·최단경로**

## 바로가기·정리 범위

- [과목 홈](../README.md) · [공지](../notices.md) · [과제·퀴즈](../assignments.md) · [원본·조회 manifest](./source-manifest.json)

이번 작업은 **주차 세팅·원본 다운로드·목차·학습 연결·마감 및 제출 상태 정리**다. 공개 PDF의 텍스트와 핵심 그림·요구사항을 대조했으며, 모든 페이지를 전문 교안화하거나 영상 전체를 수강·대본 검수한 것은 아니다. 따라서 이 study는 1만자보다 짧은 학습지도이고 수강·학습 완료를 대신 표시하지 않는다.

## 받은 원본

| 파일 | 위치 | 상태 |
| --- | --- | --- |
| Topological Sort.pdf | [원본](Topological%20Sort.pdf) | 다운로드·SHA-256 기록 |
| Single-Source Shortest Path.pdf | [원본](Single-Source%20Shortest%20Path.pdf) | 다운로드·SHA-256 기록 |
| All-Pairs Shortest Paths.pdf | [원본](All-Pairs%20Shortest%20Paths.pdf) | 다운로드·SHA-256 기록 |

## 오늘 수업 복습과 이번 주 온라인 진도 구분

**오늘 11/16 수업에서 Greedy를 복습할 것 같다는 것은 사용자 예상**이다. 수업 복습 설명은 사용자 요청에 따라 채팅으로만 제공한다. Greedy 원본·기존 퀴즈는 10주차에 보존하며 12주차 신규 과제로 옮기지 않는다.

이번 12주차 온라인 새 진도는 **위상정렬 → 단일 출발 최단경로 → 모든 쌍 최단경로**다. 교수님 영상 4개 약 97분 35초, PDF 3개를 확인했다. 새 MIT 영상이 12주차에 추가됐다고 추정하지 않는다.

```text
지난주 DFS --------------------> DAG -> 위상정렬
지난주 MIT Bellman-Ford -------> relaxation -> BF / Dijkstra
                                      |
                               출발점이 모든 정점이면?
                                      |
                               Floyd-Warshall DP
```

## A. Topological Sort.pdf — 20쪽

- p.2~4: DAG의 선후 의존성을 만족하는 정점 나열. 옷 입기 예제로 `u→v`면 u가 먼저라는 조건을 이해한다.
- p.5~15: DFS가 정점을 끝낼 때 리스트 앞에 삽입한다. 결과는 **종료 시각 내림차순**이다. 인접 리스트를 쓸 때 O(V+E)다.
- p.16: 간선 `(u,v)` 탐색 시 v가 WHITE면 자손이 되어 먼저 종료하고, BLACK이면 이미 종료했다. GREY면 조상으로 가는 back edge여서 DAG 가정에 모순이다. 따라서 모든 간선에 대해 f[u]>f[v]이고 결과가 위상 순서다.
- p.17~19: DFS 코드, WHITE/GREY/BLACK, discovery/finish time과 back edge 복습.

정점 나열이 유일할 필요는 없다. 위상정렬은 임의 유향 그래프에 항상 존재하지 않고 DAG라는 조건이 필요하다. 이번 교안의 주 구현은 DFS 종료순이므로 indegree 방식은 추가 구현 예시와 구분한다.

## B. Single-Source Shortest Path.pdf — 49쪽

| 페이지 | 목차 | 설명·연습 |
| --- | --- | --- |
| 2~8 | 문제·성질 | 가중치 합, 최단경로의 최적 부분 구조, 삼각부등식, 음수 사이클 |
| 9 | Relaxation | `d[v] > d[u]+w(u,v)`이면 거리와 predecessor 갱신 |
| 10~25 | Bellman-Ford 실행 | 초기화, V−1회 전체 간선 완화, 추가 완화 검사 |
| 26~29 | BF 정확성 | upper-bound property와 최단경로 간선 수에 대한 귀납 |
| 30~44 | Dijkstra | 최소 tentative distance 정점 추출·확정, 인접 간선 완화 |
| 45~47 | 복잡도·힙 | ExtractMin O(V)회, DecreaseKey O(E)회, 힙별 비용 |
| 48~49 | 마무리 | Dijkstra 관련 인용·질문 |

BF는 음수 간선을 허용한다. 출발점에서 도달 가능한 음수 사이클이 있으면 그 영향을 받는 정점들의 유한 최단거리가 존재하지 않음을 별도로 처리한다. 도달할 수 없는 음수 사이클을 출발점의 거리 초기화만으로 검출한다고 쓰지 않는다.

BF 증명은 어떤 순간에도 d[v]가 실제 최단거리보다 작아지지 않는다는 성질과, i번째 pass 뒤 최단경로 앞 i개 간선을 따라 정확한 거리가 전파된다는 성질을 합친다. 표준 in-place 완화는 간선 순서에 따라 한 pass에서 여러 간선만큼 전파될 수 있다. **항상 정확히 i개 이하 간선만 사용한 값**이라는 MIT의 두 배열 DP 정의와 무조건 동일시하지 않는다. [11주차 MIT 상세 교안](../11주차/study.md#mit-bellman-ford)과 비교한다.

Dijkstra는 **간선 가중치가 비음수**라는 전제가 중요하다. 현재 최소인 정점 u로 더 싸게 가는 길이 나중에 생기지 않음을 설명해야 한다. 음수 간선이 있을 때 모든 입력에서 실패한다는 뜻은 아니지만 일반적인 정확성 보장이 깨진다.

이진 힙의 일반 표기는 `O((V+E)log V)`다. 교안의 `O(E log V)`는 E가 V 정도 이상인 통상 조건에서 축약한 표기로 이해한다. Fibonacci heap은 `O(V log V+E)`이며 표의 amortized와 worst-case를 구분한다. 배열 기반 구현의 O(V²)와도 비교할 수 있다.

## C. All-Pairs Shortest Paths.pdf — 23쪽

- p.2~5: 모든 정점 쌍의 거리 행렬. BF를 V번 수행하는 방법과 비교하고, 초기 W[i][i]=0, 간선이 있으면 가중치, 없으면 ∞로 둔다.
- p.6~8: `D^(k)[i][j]`는 중간 정점을 1..k에서만 허용한 최단거리다. k를 안 쓰거나 쓰는 두 경우로 분해한다.
- p.9~14: 경로 복원을 위한 P 행렬, 3정점 예제와 k별 행렬 갱신.
- p.15~20: 여러 행렬 → 두 행렬 → 하나의 행렬로 공간 절약. 음수 사이클이 없으면 대각선 0, kth row/column이 유지되어 in-place 갱신이 가능하다.
- p.21~22: P가 기록한 중간 정점에서 양쪽 경로를 재귀 출력. 시간 O(V³), 공간 O(V²).

```text
D^(k)[i][j] = min(
    D^(k-1)[i][j],
    D^(k-1)[i][k] + D^(k-1)[k][j]
)

for k in 1..V:         # 허용하는 중간 정점의 단계
    for i in 1..V:
        for j in 1..V:
            D[i][j] = min(D[i][j], D[i][k]+D[k][j])
```

k를 가장 바깥에 두는 이유를 DP 상태 정의로 설명한다. 음수 간선은 허용하지만 이 교안의 유한 거리·경로 복원 설명은 음수 사이클이 없는 조건이다. 구현에서는 ∞를 정수 최대값으로 둘 때 덧셈 오버플로를 피한다. P의 0은 “중간 정점 없음”이므로 도달 불가능과 직접 간선 존재를 P 하나만 보고 구별하지 않는다.

## 공부할 증명·문제

1. DFS back edge와 DAG 관계, 종료 역순의 위상정렬 정확성.
2. 최단경로의 부분 경로도 최단이라는 교체·모순 논증.
3. BF upper-bound property와 V−1 pass 정확성, 음수 사이클 추가 검사.
4. Dijkstra 최소 거리 정점을 확정해도 되는 이유와 비음수 전제.
5. 힙 연산 횟수에 따른 실행 시간 도출.
6. Floyd의 “k를 거침/거치지 않음” 점화식과 귀납, in-place 근거.
7. 주어진 그래프의 relaxation·predecessor와 Floyd 행렬을 직접 채우기.

이번 API에는 **12주차 새 퀴즈·프로그래밍 과제가 없다**. 강의 4개는 11/22 23:59 마감이다. 목록 상태는 이후 추가될 수 있으므로 이번 조회 시점의 사실로 기록한다. K DNA와 BF 퀴즈는 아래 제출 확인표에서 지난주 완료로 구분한다.


## iCampus 공개 항목

| 항목 | 종류 | 재생시간 | 공개/수강 마감(KST) |
| --- | --- | --- | --- |
| [Topological sort](https://canvas.skku.edu/courses/76112/modules/items/5936953) | 영상 | 9:48 | 공개 / 마감 2026-11-22 23:59:00 |
| [Topological Sort](https://canvas.skku.edu/courses/76112/modules/items/5845041) | PDF | - | 공개 / 마감 2026-11-22 23:59:00 |
| [Shortest path (Bellman-Bord)](https://canvas.skku.edu/courses/76112/modules/items/5936957) | 영상 | 25:44 | 공개 / 마감 2026-11-22 23:59:00 |
| [Shortest path (Dijkstra)](https://canvas.skku.edu/courses/76112/modules/items/5936961) | 영상 | 21:17 | 공개 / 마감 2026-11-22 23:59:00 |
| [Single-Source Shortest Path](https://canvas.skku.edu/courses/76112/modules/items/5845042) | PDF | - | 공개 / 마감 2026-11-22 23:59:00 |
| [All-pairs shortest paths](https://canvas.skku.edu/courses/76112/modules/items/5936962) | 영상 | 40:47 | 공개 / 마감 2026-11-22 23:59:00 |
| [All-Pairs Shortest Paths](https://canvas.skku.edu/courses/76112/modules/items/5845044) | PDF | - | 공개 / 마감 2026-11-22 23:59:00 |

## 과제·퀴즈 제출 확인

| 항목 | 정규 마감(KST) | 현재 조회 상태·제출 시각 |
| --- | --- | --- |
| [Dynamic Programming 2](https://canvas.skku.edu/courses/76112/assignments/2051457) | 2026-11-08 23:59:00 | 제출 확인·평가 대기 / 2026-11-04 11:39:10 |
| [Greedy Algorithm](https://canvas.skku.edu/courses/76112/assignments/2051458) | 2026-11-08 23:59:00 | 제출 확인·평가 대기 / 2026-11-08 12:07:55 |
| [[Programming] K DNA Sequences](https://canvas.skku.edu/courses/76112/assignments/2051467) | 2026-11-15 23:59:59 | 제출 확인 / 2026-11-13 16:36:29 |
| [Bellman-Ford algorithm](https://canvas.skku.edu/courses/76112/assignments/2051455) | 2026-11-15 23:59:00 | 제출 확인·평가 대기 / 2026-11-12 21:08:03 |

LMS 미제출은 현재 사용자 계정의 제출 상태다. 서버 코드 제출·팀 내부 전달·학습 완료 여부와 구분하며 잠금 시각을 정규 마감으로 바꾸지 않는다. 퀴즈를 시작하거나 답안을 입력·제출하지 않았다.

## 공지·확인 한계

- 11/9 이후 게시된 새 공지 없음. 기존 운영 공지와 현재 과제 일정을 함께 확인했다.
- Canvas 전체 파일 목록은 HTTP 401이므로 학생에게 제공된 LearningX 강의콘텐츠·수업자료실·과제 첨부 경로로 원본을 확인했다. 파일 목록 오류를 자료 부재로 처리하지 않았다.
