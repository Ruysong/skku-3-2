# 운영체제 7주차 (2026-10-12~10-18)

이번 주에 보관된 원본은 Page Replacement다. 지난 주 Paging의 주소 변환과 연결하되, 온라인 강의·목요일 퀴즈 범위는 따로 확인한다.

## 목차

1. [자료와 확인 수준](#자료와-확인-수준)
2. [원본의 학습 목차](#원본의-학습-목차)
3. [학습 구조](#학습-구조)
4. [수업·퀴즈·PA·시험 구분](#수업퀴즈pa시험-구분)
5. [정리 상태](#정리-상태)

## 자료와 확인 수준

| 자료 | 역할 | 확인 수준 |
| --- | --- | --- |
| [OS08-PageReplacement.pdf](./OS08-PageReplacement.pdf) | Page Replacement, 61페이지, 교안 표기 Chap.21~23 | 파일·텍스트의 제목과 페이지 구성을 확인. 전체 도표·예제의 눈검수는 아직 안 함 |
| [OS07-Paging.pdf](../6주차/OS07-Paging.pdf) | 이전 주 Paging 원본 | 7주차 원본과 분리해서 보관 |
| [6주차 study](../6주차/study.md) | 이전 주 정리 | 복습 연결 |
| [과제 기록](../assignments.md) | PA 및 퀴즈 운영 | 이전 상태 표기는 최신 사용자 완료 보고와 구분 |

## 원본의 학습 목차

아래는 원본 텍스트의 제목·페이지 구성을 바탕으로 만든 탐색용 목차다. 그림의 화살표, 참조열 예제의 정답, 알고리즘별 fault 수를 검수한 해설은 아니다.

| PDF 페이지 | 단원 |
| --- | --- |
| 2~5 | Demand Paging, Page Fault 처리, AMAT |
| 6~11 | Page Replacement, dirty page, paging out, watermark와 memory reclamation |
| 12~15 | FIFO와 Belady's anomaly |
| 16~18 | MIN/OPT |
| 19~23 | LRU, counter와 stack 구현 |
| 24~32 | LRU 근사: NUR, reference-bit history, Clock, Enhanced Clock |
| 33~35 | LFU/MFU, workload, application buffering |
| 36~38 | Thrashing, locality, admission control |
| 39~42 | Demand Zeroing, Copy-on-Write |
| 43~45 | Memory-Mapped Files, MMAP 도식 |
| 46~54 | Prepaging, Page Size, TLB Reach, Huge Pages, Program Structure, I/O Interlock |
| 55 | Homework 안내: vmstat, paging-policy.py |
| 56~58 | MMAP 예제와 pmap |
| 59~61 | Linux Virtual Memory와 Page Cache |

## 학습 구조

```text
Paging: VA를 page table로 PA에 연결
  -> Demand Paging: 필요한 page만 RAM에 둠
  -> Page Fault: 필요한 page가 아직 RAM에 없음
  -> free frame이 없으면 victim 선택
  -> FIFO / OPT / LRU / Clock 등의 기준 비교
  -> fault 비용과 AMAT
  -> locality와 thrashing
  -> Demand Zeroing / Copy-on-Write / MMAP
  -> 실제 메모리 관리와 page cache로 연결
```

정책 이름만 외우기보다 같은 reference string·frame 수에서 어떤 page를 선택하는지 표로 추적하는 부분을 원본 확인 후 추가한다. OPT의 미래 참조 정보와 실제 정책의 과거 참조 정보를 구분해서 복습한다.

## 수업·퀴즈·PA·시험 구분

- 화요일 온라인 수강과 목요일 퀴즈를 연결하는 운영이라는 사용자 설명을 기준으로 공부한다. **10/15(목) 실제 퀴즈 시행 여부·정확한 범위는 공지 확인 전까지 확정하지 않는다.**
- PA1 코드와 보고서는 사용자가 제출 완료했다고 확인했다. 이미 낸 PA1을 이번 주 제출물로 다시 등록하지 않는다.
- 다음 PA 명세는 현재 로컬에 없다. 새 과제의 마감을 이번 주나 다음 주로 추정하지 않는다.
- PDF p.55의 Homework는 교재 연습 안내다. iCampus 제출 지시·배점·마감이 확인되지 않아 신규 PA와 같은 제출 과제로 세지 않는다.
- 중간고사는 사용자 확정 기준 **11/5(목)**. 시간·장소·최종 범위는 별도 확인한다.

## 정리 상태

7주차 주차 세팅과 페이지별 목차를 먼저 마련한 문서다. 61페이지의 도표·시뮬레이션·예제를 모두 검수한 1만자 상세 교안은 아직 아니므로 현재는 짧게 둔다. 자동 추출 텍스트를 전문 검수본으로 저장하지 않았으며, 공부 정리 요청 시 원본 화면과 대조하여 내용을 확장한다.
