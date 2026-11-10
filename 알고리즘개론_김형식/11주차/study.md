# 알고리즘개론 11주차

기간: **2026-11-09~11-15** · iCampus API 확인일: **2026-11-09**, 한국 시간.

이번 주 연결 주제: **Graph·BFS·DFS·Bellman-Ford**

## 목차

1. 이번 주 자료·공부 연결
2. iCampus 11주차 항목
3. 과제·퀴즈와 제출 상태
4. 정리 범위

## 이번 주 자료·공부 연결

- 11주차 API 목록은 아래 9개 항목이다. Graph 두 개, BFS 관련 두 개, DFS 관련 두 개, Unweighted Graph Search, MIT 12. Bellman-Ford, Bellman-Ford 퀴즈를 각각 보존한다.
- 기본 학습 순서: 그래프 표현·탐색 개요 → BFS → DFS → 무가중치 경로 탐색 → Bellman-Ford. 이는 목록을 연결한 학습 동선이며 모든 강의 내용을 이미 읽고 검증한 상세 교안은 아니다.
- **Bellman-Ford algorithm 퀴즈: 11/15(일) 23:59**, 4문항·4점. 조회 시 미제출, 문제 본문은 이번 작업에서 응시·수집하지 않았다.
- **[Programming] K DNA Sequences: 11/15(일) 23:59:59**, 10점. 10주차 모듈에 있는 과제지만 이번 주 마감이므로 함께 관리한다. API는 코드와 설명문서 제출을 안내한다.
- [K DNA 명세](../10주차/Assignment_kDNA.pdf), [공개 입력 msa.in](../10주차/msa.in), [공개 출력 msa.out](../10주차/msa.out)을 다운로드해 원본 보관했다. C로 k개 DNA 서열 정렬을 구현하는 과제이며 자세한 요구사항 분석은 아직 별도다.
- [9주차 DP 상세 교안](../9주차/study.md), [10주차 Greedy 퀴즈·참고 답변](../10주차/study.md)과 새 진도를 구분한다.
- DP2·Greedy는 API에서 pending_review와 제출 시각을 확인했다. 이전의 제출 미확인 표시는 당시 기록이고 현재는 제출 확인·평가 대기다. 사용자의 학습 완료 여부까지 대신 판정하지 않는다.

- [과목 홈](../README.md) · [공지 원문](../notices.md) · [과제/퀴즈](../assignments.md)

## iCampus 11주차 항목

| 순서 | API에 표시된 제목 | 종류 | 접근·완료 표시 |
| --- | --- | --- | --- |
| 1 | [Graph](https://canvas.skku.edu/courses/76112/modules/items/5845032) | ExternalTool | API 완료 정보 없음 |
| 2 | [Graph](https://canvas.skku.edu/courses/76112/modules/items/5929673) | ExternalTool | API 완료 정보 없음 |
| 3 | [Graph search - BFS](https://canvas.skku.edu/courses/76112/modules/items/5845034) | ExternalTool | API 완료 정보 없음 |
| 4 | [BFS](https://canvas.skku.edu/courses/76112/modules/items/5929685) | ExternalTool | API 완료 정보 없음 |
| 5 | [Graph search - DFS](https://canvas.skku.edu/courses/76112/modules/items/5845036) | ExternalTool | API 완료 정보 없음 |
| 6 | [DFS](https://canvas.skku.edu/courses/76112/modules/items/5929723) | ExternalTool | API 완료 정보 없음 |
| 7 | [Unweighted Graph Search](https://canvas.skku.edu/courses/76112/modules/items/5845037) | ExternalTool | API 완료 정보 없음 |
| 8 | [12. Bellman-Ford](https://canvas.skku.edu/courses/76112/modules/items/5845038) | ExternalTool | API 완료 정보 없음 |
| 9 | [Bellman-Ford algorithm](https://canvas.skku.edu/courses/76112/modules/items/5845039) | Quiz | API 완료 정보 없음 |

ExternalTool은 LearningX 등 별도 도구의 연결 항목이다. 표준 API의 제목만으로 PDF/영상 구분·재생시간·개별 수강 마감·출결 완료를 확정하지 않는다. 같은 제목의 항목도 서로 다른 ID이면 별도로 보관했다.

## 과제·퀴즈와 제출 상태

| 항목 | 마감(KST) | 현재 조회 상태 |
| --- | --- | --- |
| [[Programming] K DNA Sequences](https://canvas.skku.edu/courses/76112/assignments/2051467) | 2026-11-15 23:59:59 | API 미제출 |
| [Dynamic Programming 2](https://canvas.skku.edu/courses/76112/assignments/2051457) | 2026-11-08 23:59:00 | 제출 확인·평가 대기 / 2026-11-04 11:39:10 |
| [Greedy Algorithm](https://canvas.skku.edu/courses/76112/assignments/2051458) | 2026-11-08 23:59:00 | 제출 확인·평가 대기 / 2026-11-08 12:07:55 |
| [Bellman-Ford algorithm](https://canvas.skku.edu/courses/76112/assignments/2051455) | 2026-11-15 23:59:00 | API 미제출 |

API 미제출은 확인 시점의 상태다. 아직 마감 전인 항목을 지각·누락으로 표시하지 않는다. 외부 UI 서버 코드 제출, 별도 보고서, 팀 과제의 개인 업로드는 같은 상태가 아니다.

## 정리 범위

이번 작업은 주차 세팅·API 목록·확정 일정·기존 원본 연결을 준비한 것이다. PDF 전체의 그림·수식·코드까지 읽어 작성한 전문 교안이 아니므로 1만자 미만이다. 새로운 상세 study는 실제 교안·영상 대본을 읽고 출처별로 확장한다. 파일 존재·API 모듈 상태·사용자 이해·제출 완료를 각각 구분한다.

조회 제한: 과목 파일 목록은 이번 API로 조회되지 않았다. 퀴즈 목록 조회가 되지 않은 과목도 있다. 제한을 자료가 없다는 뜻으로 바꾸지 않는다. 원본 PDF와 VMShare 제출물은 기존 경로에서 보존했다.
