# 알고리즘개론 — 정당성·복잡도와 C 구현

정당성 증명·복잡도 분석과 C 구현 과제를 연결한다. 교수님 PDF, 외부 강의 노트·대본, 본인 과제 코드·보고서를 분리해 읽는다.

## 대표 구현

| 과제 | 실제 구현에서 볼 점 | 근거 |
|---|---|---|
| hw1 stable merge sort | 레코드 key 추출, 연결리스트 next relink, equal key 순서 | [소스·불변식 설명](./Assingmnet/hw1/study.md) |
| hw2 Min-Max-Median PQ | LOW/HIGH, 네 heap, Node ID·위치 갱신, 재균형 | [pq.c](./Assingmnet/hw2/pq.c), [구현·복잡도](./Assingmnet/hw2/pa-study.md) |
| hw3 K DNA Sequences | k차원 DP 평탄화, stride, 공통 부분수열 점수·역추적 | [msa.c](./Assingmnet/hw3/msa.c), [보고서](./Assingmnet/hw3/K_DNA_Report_2024315329_Junhyuk_Song.pdf) |

hw3는 11/13 LMS 제출 확인 기록이 있다. 공식 채점과 자체 검증은 별도다. 자소서에서는 [A1/A2/A3 경험 카드](../경험-근거지도.md#a1a2a3--자료구조의-불변식과-dp-상태-구현), 전체 제출물은 [pa-study.md](./pa-study.md)로 연결한다.

## 학습·증명 자료

- [증명 제출물](./proof-study.md): loop invariant, 점근 표기, 점화식, log factorial 등.
- 귀납법·분석 → sort·heap·hashing → order statistics → DP·greedy → graph search·shortest path.
- [MIT 원본 노트 목차](./MIT%20교안/목차.md), [9주차 DP 외부 영상별 정리](./9주차/study.md), [11주차 Bellman-Ford 상세 정리](./11주차/study.md#mit-bellman-ford).
- 외부 영상 대본과 학습 정리는 개인의 알고리즘 발명·독립 증명 완료를 뜻하지 않는다. 학습·제출 상태는 날짜가 있는 최신 기록을 따른다.

## 수업 자료 찾기

[1주차](./1주차/study.md) · [2주차](./2주차/study.md) · [3주차](./3주차/study.md) · [4주차](./4주차/study.md) · [5주차](./5주차/study.md) · [6주차](./6주차/study.md) · [7주차](./7주차/study.md) · [9주차](./9주차/study.md) · [10주차](./10주차/study.md) · [11주차](./11주차/study.md) · [12주차](./12주차/study.md)

- [공지 원문](./notices.md) · [과제·퀴즈·제출 기록](./assignments.md) · [전체 일정](../tasks.md)
- [전체 경험·근거 지도](../경험-근거지도.md) · [저장소 홈](../README.md)

최신 정리: 2026-11-17. 소스·보고서의 보관 상태와 수강·개인 학습 완료는 별도로 관리한다.
