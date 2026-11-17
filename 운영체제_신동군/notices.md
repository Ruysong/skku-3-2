# 운영체제 - 신동군 공지

<!-- week12-notices-20261116:start -->
## 12주차 공지 확인 — 2026-11-16

### 2026-11-12 18:58:39 [Notice] Mid-Term Exam Answer Sheet Review Location

출처: [[Notice] Mid-Term Exam Answer Sheet Review Location](https://canvas.skku.edu/courses/75500/discussion_topics/661140)

핵심: 중간고사 성적·시험지 확인 안내다. 시험지 확인은 **11/13 13:00~17:00, 반도체관 400309**로 이미 지난 일정이며 이번 주 할 일에 넣지 않는다.

원문:

중간고사 시험지 확인 장소는 반도체관 400309입니다.
일시: 11월 13일(금) 오후 1시~5시


The review will be held in Room 400309, Semiconductor Building.
Date & Time: Friday, November 13, 1:00 PM – 5:00 PM

### 2026-11-12 14:37:06 Mid-Term Exam Score

출처: [Mid-Term Exam Score](https://canvas.skku.edu/courses/75500/discussion_topics/661026)

핵심: 중간고사 성적·시험지 확인 안내다. 시험지 확인은 **11/13 13:00~17:00, 반도체관 400309**로 이미 지난 일정이며 이번 주 할 일에 넣지 않는다.

원문:

The score file is attached.

You can review your answer sheet on Friday, November 13, from 1:00 PM to 5:00 PM.
<!-- week12-notices-20261116:end -->

## 2026-09-03 수업 구두 안내 메모

출처: 2026-09-03 목요일 운영체제 수업 중 구두 안내

### 핵심

- 질문 답변이나 수업 관련 답변은 한국어로 해도 된다고 안내됨.
- xv6 과제는 학기 중 약 6개 정도, 6단계 흐름으로 진행될 예정이라고 이해됨.
- xv6 과제는 팀프로젝트가 아니라 개인 과제로 안내됨.
- 퀴즈는 매주 운영된다고 수업 중 안내받은 것으로 관리함.

### 확인 필요

- 주차별 퀴즈의 정확한 날짜, 시간, 범위, 응시 방식은 iCampus 공식 공지를 다시 확인해야 함.
- xv6 과제의 세부 스펙, 마감일, 제출처, 채점 방식은 과제 공지가 올라오면 `assignments.md`에 확정 반영.

## 2026-09-17 Project 0 Makefile 변경

- UI 서버 컴파일러 업데이트로 make qemu-nox가 정상 실행되지 않을 수 있음.
- xv6-public clone 뒤 실습 전, Makefile **74행과 160행**의 gcc를 **gcc-11**로 변경.
- 적용 순서: clone -> Makefile 두 곳 수정 -> make qemu-nox로 boot 확인.

## 2026-09-17 Project 제출 절차 변경

- 기존 ~swe3004/bin/submit 대신 **~swe3004_41/bin/submit** 사용.
- PA0 제출: ~swe3004_41/bin/submit pa0 xv6-public
- PA0 PDF의 이전 제출 명령보다 이 9/17 공지가 우선.
- 제출은 여러 번 가능하지만 마지막 제출본만 채점.
- 추가 VPN 계정 등록은 완료. 설치 보안 이슈는 교내 IT 팀에 문의.
- VPN 안내: https://vpninfo.skku.edu/