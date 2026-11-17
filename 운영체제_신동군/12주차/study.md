# 운영체제 12주차

<!-- quiz9-20261117:start -->
## 11/17 갱신: Quiz9

**11/19(목) 13:32~13:40, 8분·12문항**으로 공개됐다. 과제와 퀴즈 API를 확인했으며 설명란에는 범위가 없다. 아래 11/16의 공개 대기 안내보다 이 확인을 우선한다. 사용자는 수요일 운체 중심으로 공부한다.
<!-- quiz9-20261117:end -->

<!-- pa3-complete-20261116:start -->
## PA3 완료 확인 (11/16 16:28 KST)

PA3는 코드·PDF 모두 제출 완료다. PDF는 **11/16 14:52:03 KST**, Canvas API `submitted`와 첨부 파일명으로 확인했다. 이번 주 남은 공부는 Deadlock·I/O Devices & SSD와 목요일 퀴즈 준비이며, 아래 오전 조회의 PA3 미제출 표시는 과거 상태다.
<!-- pa3-complete-20261116:end -->

기간: **2026-11-16(월)~11-22(일)** · API 확인: **2026-11-16 10:31 KST 전후**.

이번 주 주제: **Deadlock·I/O Devices & SSD**

## 바로가기·정리 범위

- [과목 홈](../README.md) · [공지](../notices.md) · [과제·퀴즈](../assignments.md) · [원본·조회 manifest](./source-manifest.json)

이번 작업은 **주차 세팅·원본 다운로드·목차·학습 연결·마감 및 제출 상태 정리**다. 공개 PDF의 텍스트와 핵심 그림·요구사항을 대조했으며, 모든 페이지를 전문 교안화하거나 영상 전체를 수강·대본 검수한 것은 아니다. 따라서 이 study는 1만자보다 짧은 학습지도이고 수강·학습 완료를 대신 표시하지 않는다.

## 받은 원본

| 파일 | 위치 | 상태 |
| --- | --- | --- |
| OS13-Deadlock.pdf | [원본](OS13-Deadlock.pdf) | 다운로드·SHA-256 기록 |
| OS14-IO Devices and SSD.pdf | [원본](OS14-IO%20Devices%20and%20SSD.pdf) | 다운로드·SHA-256 기록 |

## 이번 주 목차: 동시성 오류 → 장치와 저장장치

```text
스레드 동기화
  atomicity/order violation -> deadlock 4조건
          -> prevention / avoidance / detection & recovery
                                      |
입출력 장치
  장치 레지스터 -> polling / interrupt -> DMA -> driver
                                      |
저장장치
  HDD 접근시간 -> NAND page/block -> FTL -> GC·wear leveling
```

### OS13-Deadlock.pdf — 22쪽

| 페이지 | 목차 | 설명·연습 |
| --- | --- | --- |
| 2~5 | Non-deadlock bugs | atomicity violation과 order violation을 구분하고 lock/condvar의 역할 연결 |
| 6~8 | Deadlock와 네 필요조건 | mutual exclusion, hold-and-wait, no preemption, circular wait |
| 9~14 | Prevention | 락 순서 고정, 일괄 확보, trylock 재시도, 상호배제 회피·원자 연산 |
| 15~16 | Avoidance | safe/unsafe 상태, 미리 알고 있는 자원 요구와 스케줄링 |
| 17~20 | Detect and recover | 자원 그래프 축약, 종료·회수·rollback의 선택 |
| 21~22 | 요약·Homework | Chap.32(Deadlock) 연습 안내. LMS 제출형 과제와 같은 것으로 단정하지 않음 |

unsafe가 곧 현재 deadlock이라는 뜻은 아니다. 락 순서를 모두 같게 하는 방법은 circular wait를 끊는 예방이다. trylock 실패 뒤 보유 락을 풀고 재시도하면 deadlock을 피할 수 있지만 공정성·기아까지 자동 보장되는 것은 아니다. 두 스레드가 L1/L2를 반대 순서로 잡는 상황을 실행 순서로 직접 그려본다.

### OS14-IO Devices and SSD.pdf — 26쪽

| 페이지 | 목차 | 설명·연습 |
| --- | --- | --- |
| 2~4 | 시스템·장치 구조와 protocol | status/command/data 레지스터와 OS의 장치 제어 |
| 5~8 | Polling·interrupt·DMA·MMIO | 완료 확인과 실제 데이터 이동을 구분 |
| 9~10 | Device driver | OS의 공통 인터페이스와 장치별 구현 분리 |
| 11~13 | HDD | seek, 평균 회전 지연, transfer의 합 계산 |
| 14~17 | NAND·SSD | page read/program, block erase, 수명과 오류 |
| 18~20 | SSD 사례·FTL | logical block/page를 physical 위치로 변환, write amplification·wear leveling |
| 21~25 | Mapping·GC | direct mapping vs log-structured FTL, valid page 이동 후 erase, mapping table과 DFTL |
| 26 | 성능 비교 | 순차·랜덤 I/O를 분리해서 읽고, 옛 장치 측정치를 최신 SSD의 보편 수치로 외우지 않음 |

DMA는 CPU가 전송을 설정한 뒤 장치/메모리 사이 이동을 맡긴다. CPU가 바이트마다 복사하지 않는다는 뜻이지 설정·완료 처리까지 CPU가 전혀 필요 없다는 뜻은 아니다. FTL은 프로세스 VA→PA 변환과 다른 계층의 논리 저장 주소 변환이다.

### 영상과 퀴즈 주의

두 영상은 약 105분 48초이며 **11/19(목) 12:59:59 수강 마감**이다. PDF 목록의 11/22 마감과 혼동하지 않는다. 매주 목요일 퀴즈를 본다는 기존 수업 운영에 맞춰 준비하되, **현재 Quiz9는 API 목록에 아직 없으므로 정확한 응시 시각과 범위는 미확정**이다. 지난 Quiz8을 이번 주 퀴즈로 다시 표시하지 않는다.

### PA3

**11/18(수) 23:59:59 마감**. [현재 명세](../Assignment/pa3/Project%203_26f.pdf)와 코드·보고서 제출 절차를 확인한다. LMS에서는 현재 보고서 미제출이다. UI 서버 코드 제출 여부는 이 API로 판정할 수 없다. 11/22 23:59:59의 제출 잠금은 정규 마감 연장이 아니다.

PA4는 API상 11/19 18:00 공개 예정, 12/2 23:59:59 마감이다. 아직 공개되지 않은 명세를 내려받거나 구현 범위를 추정하지 않았다.


## iCampus 공개 항목

| 항목 | 종류 | 재생시간 | 공개/수강 마감(KST) |
| --- | --- | --- | --- |
| [OS13-Deadlock](https://canvas.skku.edu/courses/75500/modules/items/5928742) | PDF | - | 공개 / 시작 2026-11-14 00:00:00 / 마감 2026-11-22 23:59:59 |
| [OS13-Deadlock](https://canvas.skku.edu/courses/75500/modules/items/5928745) | 영상 | 39:44 | 공개 / 시작 2026-11-14 00:00:00 / 마감 2026-11-19 12:59:59 |
| [OS14-IO Devices and SSD](https://canvas.skku.edu/courses/75500/modules/items/5928744) | PDF | - | 공개 / 시작 2026-11-14 00:00:00 / 마감 2026-11-22 23:59:59 |
| [OS14-IO Devices and SSD](https://canvas.skku.edu/courses/75500/modules/items/5928746) | 영상 | 66:04 | 공개 / 시작 2026-11-14 00:00:00 / 마감 2026-11-19 12:59:59 |

## 과제·퀴즈 제출 확인

| 항목 | 정규 마감(KST) | 현재 조회 상태·제출 시각 |
| --- | --- | --- |
| [Project 3_26f](https://canvas.skku.edu/courses/75500/assignments/2059639) | 2026-11-18 23:59:59 | LMS 미제출 |
| [Project 4_26f](https://canvas.skku.edu/courses/75500/assignments/2059640) | 2026-12-02 23:59:59 | LMS 미제출 |

LMS 미제출은 현재 사용자 계정의 제출 상태다. 서버 코드 제출·팀 내부 전달·학습 완료 여부와 구분하며 잠금 시각을 정규 마감으로 바꾸지 않는다. 퀴즈를 시작하거나 답안을 입력·제출하지 않았다.

## 공지·확인 한계

- [[Notice] Mid-Term Exam Answer Sheet Review Location](https://canvas.skku.edu/courses/75500/discussion_topics/661140): 게시 2026-11-12 18:58:39. 공지 원문은 과목 notices에 연결.
- [Mid-Term Exam Score](https://canvas.skku.edu/courses/75500/discussion_topics/661026): 게시 2026-11-12 14:37:06. 공지 원문은 과목 notices에 연결.
- Canvas 전체 파일 목록은 HTTP 401이므로 학생에게 제공된 LearningX 강의콘텐츠·수업자료실·과제 첨부 경로로 원본을 확인했다. 파일 목록 오류를 자료 부재로 처리하지 않았다.
