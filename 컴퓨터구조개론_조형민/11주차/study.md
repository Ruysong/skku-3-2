# 컴퓨터구조개론 11주차

<!-- followup-20261116:start -->
## 11/16 후속 확인

사용자는2024 기말 예제를 지난주에 풀었다고 확인했다. Proj2는 현재 API 미제출이며 **11/20 23:59:59 마감**이다. 이번 주 [12주차](../12주차/study.md)는 Memory Hierarchy·Cache다.
<!-- followup-20261116:end -->

기간: **2026-11-09~11-15** · iCampus API 확인일: **2026-11-09**, 한국 시간.

이번 주 연결 주제: **Pipeline Hazards 2**

## 목차

1. 이번 주 자료·공부 연결
2. iCampus 11주차 항목
3. 과제·퀴즈와 제출 상태
4. 정리 범위

## 이번 주 자료·공부 연결

- [CA_Lecture_11 원본](./CA_Lecture_11.pdf): 52페이지, Chapter 4-6 Pipeline Hazards 2. 기존 파일을 보존했다.
- 탐색 목차: p.2~11 branch hazards·분기 지연 감소·분기 비교의 data hazard, p.12~19 not-taken 예측과 잘못 가져온 명령 flush, p.20~30 static/dynamic·1-bit/2-bit predictor, p.31 branch target, p.32~39 multiple issue·static dual issue·스케줄링, p.40~51 loop unrolling·레지스터 분리·single/dual issue 비교, p.52 dynamic multiple issue.
- 주요 공부 질문: 분기 결과가 언제 결정되는가? 어느 명령을 flush하는가? 1-bit와 2-bit 예측기의 상태가 어떻게 바뀌는가? 독립 명령을 배치해 bubble을 줄일 수 있는가?
- **Proj2: 11/20(금) 23:59:59**. [받아둔 원본 명세](../PA/PA_2/CA_Proj2.pdf)는 Single-cycle RISC-V CPU Simulator 32페이지다. 이번 작업에서 전체 명세 분석·구현은 하지 않았다.
- Project 1은 대화에서 구현 흐름을 리뷰했고, 사용자는 상세 코드 재검토를 당장 하지 않기로 했다. 이번 주 진도 보충과 Proj2 준비를 우선하고 Project 1 전체 코드 리뷰를 급한 미완료 과제로 되살리지 않는다.

- [과목 홈](../README.md) · [공지 원문](../notices.md) · [과제/퀴즈](../assignments.md)

## iCampus 11주차 항목

| 순서 | API에 표시된 제목 | 종류 | 접근·완료 표시 |
| --- | --- | --- | --- |
| 1 | [CA_Lecture_11](https://canvas.skku.edu/courses/76835/modules/items/5927438) | ExternalTool | API 완료 정보 없음 |

ExternalTool은 LearningX 등 별도 도구의 연결 항목이다. 표준 API의 제목만으로 PDF/영상 구분·재생시간·개별 수강 마감·출결 완료를 확정하지 않는다. 같은 제목의 항목도 서로 다른 ID이면 별도로 보관했다.

## 과제·퀴즈와 제출 상태

| 항목 | 마감(KST) | 현재 조회 상태 |
| --- | --- | --- |
| [Proj2](https://canvas.skku.edu/courses/76835/assignments/2065807) | 2026-11-20 23:59:59 | API 미제출 |

API 미제출은 확인 시점의 상태다. 아직 마감 전인 항목을 지각·누락으로 표시하지 않는다. 외부 UI 서버 코드 제출, 별도 보고서, 팀 과제의 개인 업로드는 같은 상태가 아니다.

## 정리 범위

이번 작업은 주차 세팅·API 목록·확정 일정·기존 원본 연결을 준비한 것이다. PDF 전체의 그림·수식·코드까지 읽어 작성한 전문 교안이 아니므로 1만자 미만이다. 새로운 상세 study는 실제 교안·영상 대본을 읽고 출처별로 확장한다. 파일 존재·API 모듈 상태·사용자 이해·제출 완료를 각각 구분한다.

조회 제한: 과목 파일 목록은 이번 API로 조회되지 않았다. 퀴즈 목록 조회가 되지 않은 과목도 있다. 제한을 자료가 없다는 뜻으로 바꾸지 않는다. 원본 PDF와 VMShare 제출물은 기존 경로에서 보존했다.
