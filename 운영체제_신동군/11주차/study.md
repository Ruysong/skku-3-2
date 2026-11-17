# 운영체제 11주차

<!-- followup-20261116:start -->
## 11/16 API·원본 후속 확인

- OS11-CondVar.pdf와 OS11-CondVar-Quiz.pdf, OS12-Semaphore.pdf와 OS12-Semaphore-Quiz.pdf가 이 폴더에 모두 보관되어 있다. 아래 OS11 미보관 표시는11/9 당시 기록이다.
- Quiz8은 **11/12 13:42:38 제출·채점 상태**다.
- PA3는 **11/18 23:59:59 정규 마감**, 현재 LMS 보고서 미제출. UI 서버 코드 제출 상태는 별도다.
- [12주차](../12주차/study.md): Deadlock·I/O Devices & SSD. 영상 마감은11/19 12:59:59다.
<!-- followup-20261116:end -->

기간: **2026-11-09~11-15** · iCampus API 확인일: **2026-11-09**, 한국 시간.

이번 주 연결 주제: **Condition Variables·Semaphores**

## 목차

1. 이번 주 자료·공부 연결
2. iCampus 11주차 항목
3. 과제·퀴즈와 제출 상태
4. 정리 범위

## 이번 주 자료·공부 연결

- 11주차 모듈: OS11-CondVar, OS11-CondVar-강의, OS12-Semaphore 두 항목. 같은 제목의 항목을 임의로 중복 삭제하지 않았다.
- [현재 보관된 Semaphore PDF](./OS12-Semaphore.pdf): 14페이지.
- OS11-CondVar 이번 학기 원본은 로컬 미보관. [이전 수업 참고자료](../운영체제%20PE/OS11-CondVar.pdf)는 이번 학기 교안과 동일하다고 가정하지 않는다.
- Semaphore 탐색 목차: p.2~3 정의·wait/post, p.4 binary semaphore, p.5~6 실행 순서 동기화, p.7~9 bounded buffer·교착 상태·올바른 lock 범위, p.10 reader-writer lock, p.11~13 dining philosophers, p.14 mutex/condvar로 구현.
- **Quiz8: 11/12(목) 13:33~13:43, 10분**. API에서 시각을 확인했지만 퀴즈 설명은 비어 있어 구체적 출제 범위는 미확정이다.
- **PA3 Virtual Memory: 11/18(수) 23:59:59**, UI 서버 코드와 iCampus PDF 보고서(5페이지 이내) 별도. [원본 명세](../Assignment/pa3/Project%203_26f.pdf) 참조. API의 unsubmitted는 iCampus 제출 상태이며 서버 코드 제출 상태를 직접 증명하지 않는다.

- [과목 홈](../README.md) · [공지 원문](../notices.md) · [과제/퀴즈](../assignments.md)

## iCampus 11주차 항목

| 순서 | API에 표시된 제목 | 종류 | 접근·완료 표시 |
| --- | --- | --- | --- |
| 1 | [OS11-CondVar](https://canvas.skku.edu/courses/75500/modules/items/5928731) | ExternalTool | API 완료 정보 없음 |
| 2 | [OS11-CondVar-강의](https://canvas.skku.edu/courses/75500/modules/items/5928733) | ExternalTool | API 완료 정보 없음 |
| 3 | [OS12-Semaphore](https://canvas.skku.edu/courses/75500/modules/items/5928732) | ExternalTool | API 완료 정보 없음 |
| 4 | [OS12-Semaphore](https://canvas.skku.edu/courses/75500/modules/items/5928736) | ExternalTool | API 완료 정보 없음 |

ExternalTool은 LearningX 등 별도 도구의 연결 항목이다. 표준 API의 제목만으로 PDF/영상 구분·재생시간·개별 수강 마감·출결 완료를 확정하지 않는다. 같은 제목의 항목도 서로 다른 ID이면 별도로 보관했다.

## 과제·퀴즈와 제출 상태

| 항목 | 마감(KST) | 현재 조회 상태 |
| --- | --- | --- |
| [Quiz8](https://canvas.skku.edu/courses/75500/assignments/2069954) | 2026-11-12 13:43:00 | API 미제출 |
| [Project 3_26f](https://canvas.skku.edu/courses/75500/assignments/2059639) | 2026-11-18 23:59:59 | API 미제출 |

API 미제출은 확인 시점의 상태다. 아직 마감 전인 항목을 지각·누락으로 표시하지 않는다. 외부 UI 서버 코드 제출, 별도 보고서, 팀 과제의 개인 업로드는 같은 상태가 아니다.

## 정리 범위

이번 작업은 주차 세팅·API 목록·확정 일정·기존 원본 연결을 준비한 것이다. PDF 전체의 그림·수식·코드까지 읽어 작성한 전문 교안이 아니므로 1만자 미만이다. 새로운 상세 study는 실제 교안·영상 대본을 읽고 출처별로 확장한다. 파일 존재·API 모듈 상태·사용자 이해·제출 완료를 각각 구분한다.

조회 제한: 과목 파일 목록은 이번 API로 조회되지 않았다. 퀴즈 목록 조회가 되지 않은 과목도 있다. 제한을 자료가 없다는 뜻으로 바꾸지 않는다. 원본 PDF와 VMShare 제출물은 기존 경로에서 보존했다.
