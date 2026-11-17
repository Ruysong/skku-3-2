# 컴퓨터구조개론 12주차

## 11/17 사용자 학습 상태

월요일 집중 공부 후 **기말 기출 누적 2세트 완료**. 이번 주 새 교안은 가볍게 정리하고, 주말에 기출 추가 풀이/복습을 계획한다. 이번 발언에서는 두 시험지의 연도를 특정하지 않았으므로 임의로 세트별 완료 표시를 하지 않는다.

<!-- proj2-complete-20261116:start -->
## Proj2 제출 완료 (11/16 16:28 KST)

11/16 사용자 서버 제출 완료 확인. [구현·검증 기록](../PA/PA_2/study.md)을 보관했다. 이번 주는 캐시 개념·기출 공부를 진행한다. 아래 오전 API 미제출 표시는 외부 서버 제출과 구분한다.
<!-- proj2-complete-20261116:end -->

기간: **2026-11-16(월)~11-22(일)** · API 확인: **2026-11-16 10:31 KST 전후**.

이번 주 주제: **Memory Hierarchy·Cache**

## 바로가기·정리 범위

- [과목 홈](../README.md) · [공지](../notices.md) · [과제·퀴즈](../assignments.md) · [원본·조회 manifest](./source-manifest.json)

이번 작업은 **주차 세팅·원본 다운로드·목차·학습 연결·마감 및 제출 상태 정리**다. 공개 PDF의 텍스트와 핵심 그림·요구사항을 대조했으며, 모든 페이지를 전문 교안화하거나 영상 전체를 수강·대본 검수한 것은 아니다. 따라서 이 study는 1만자보다 짧은 학습지도이고 수강·학습 완료를 대신 표시하지 않는다.

## 받은 원본

| 파일 | 위치 | 상태 |
| --- | --- | --- |
| CA_Lecture_12.pdf | [원본](CA_Lecture_12.pdf) | 다운로드·SHA-256 기록 |

## 이번 주 목차: Memory Hierarchy 1

```text
SRAM / DRAM 구조·시간
          |
       지역성
          |
        Cache
  address -> tag / index / offset
          |
       hit / miss -> 교체·쓰기 정책
          |
   miss rate / penalty / CPI / AMAT
```

### CA_Lecture_12.pdf — 54쪽

| 페이지 | 목차 | 풀어볼 문제 |
| --- | --- | --- |
| 2~12 | SRAM·DRAM | 셀 구조, 크기와 접근 지연, open row, row hit/conflict, refresh, bank/rank/DIMM |
| 13~20 | 계층과 지역성 | line 단위 이동, temporal/spatial locality, L1/L2/L3 |
| 21~27 | Direct-mapped cache | 한 주소의 유일한 index, tag·valid, 블록 크기별 offset |
| 28~34 | Access trace | 주소별 hit/miss와 교체 후 tag/data 상태 |
| 35~38 | 주소 분할·용량 | tag/index/offset 비트, data 용량과 metadata 용량 구분 |
| 39~42 | Block size | 공간 지역성 이득 vs line 수 감소·충돌·전송량 |
| 43~50 | Write policy | write-through/back, dirty bit, write/no-write allocate, store trace |
| 51~54 | 성능 | memory stall, CPI, cache 설계와 clock period, AMAT |

### 주소 계산 기본식

2의 거듭제곱 크기, byte address, direct-mapped cache에서 data capacity C, block size B이면

```text
number_of_lines = C / B
offset_bits = log2(B)
index_bits  = log2(C/B)
tag_bits    = address_bits - index_bits - offset_bits
```

4 KiB data cache·16 B block·32-bit 주소라면 line 256개, offset 4bit, index 8bit, tag 20bit다. tag+valid만 세면 256×21=5,376bit가 추가된다. dirty나 replacement 정보를 문제에서 포함하라고 하면 별도로 더한다. C를 metadata까지 포함한 총 bit 수로 잘못 대입하지 않는다.

### 쓰기 정책

- write-through: cache write hit 때 하위 메모리에도 쓴다. write buffer와 연결된다.
- write-back: 우선 cache만 수정하고 dirty를 표시한 뒤 축출 시 하위로 쓴다.
- write-allocate: write miss 때 block을 가져와 cache에 채운 뒤 쓴다.
- no-write-allocate: write miss에서 cache를 채우지 않고 하위로 쓴다.

write-through/back은 hit 때 하위 반영 방식, allocate 여부는 miss 때 동작이다. 서로 다른 축이므로 조합별 trace를 구분한다. p.50 예제는 write-back+write-allocate이고 0x00과 0x20이 같은 line에 매핑되어 dirty eviction이 생긴다.

### 성능

`AMAT = hit time + miss rate × miss penalty`다. penalty가 추가 지연인지 miss 전체 시간인지 문제 정의를 확인한다. 2 ns hit, 추가 40 ns penalty, miss 6%이면 4.4 ns다. load/store 비율과 instruction/data miss rate가 주어지면 명령어당 stall을 각각 계산하고 base CPI에 더한다. miss rate가 낮아져도 clock period가 길어지면 전체 실행 시간이 개선되지 않을 수 있다.

### 이번 주 과제

**Proj2: 11/20(금) 23:59:59**. [원본 명세](../PA/PA_2/CA_Proj2.pdf)와 기존 PA 폴더를 연결하며 새 주차로 제출 소스를 옮기지 않았다. API는 현재 미제출이다. 이번 작업은 교안·마감 정리이고 Proj2 구현·제출을 수행한 것은 아니다.

2024 기말 예제를 이미 풀었다는 사용자 기록을 유지한다. 새 캐시 개념을 배운 뒤 기존 문제에서 주소 분할·hit/miss·쓰기 정책·AMAT 유형을 연결해 복습한다. 이번 교안의 범위와 교수님 기말 공식 범위는 같은 표현으로 단정하지 않는다.


## iCampus 공개 항목

| 항목 | 종류 | 재생시간 | 공개/수강 마감(KST) |
| --- | --- | --- | --- |
| [CA_Lecture_12](https://canvas.skku.edu/courses/76835/modules/items/5936358) | PDF | - | 공개 / 시작 2026-11-16 00:00:00 / 마감 2026-11-22 23:59:59 |

## 과제·퀴즈 제출 확인

| 항목 | 정규 마감(KST) | 현재 조회 상태·제출 시각 |
| --- | --- | --- |
| [Proj2](https://canvas.skku.edu/courses/76835/assignments/2065807) | 2026-11-20 23:59:59 | LMS 미제출 |

LMS 미제출은 현재 사용자 계정의 제출 상태다. 서버 코드 제출·팀 내부 전달·학습 완료 여부와 구분하며 잠금 시각을 정규 마감으로 바꾸지 않는다. 퀴즈를 시작하거나 답안을 입력·제출하지 않았다.

## 공지·확인 한계

- 11/9 이후 게시된 새 공지 없음. 기존 운영 공지와 현재 과제 일정을 함께 확인했다.
- Canvas 전체 파일 목록은 HTTP 401이므로 학생에게 제공된 LearningX 강의콘텐츠·수업자료실·과제 첨부 경로로 원본을 확인했다. 파일 목록 오류를 자료 부재로 처리하지 않았다.
- 이 과목의 Canvas quizzes 목록은 HTTP 404다. 과제·모듈·공지를 같이 확인했고 모든 평가가 없다고 확대하지 않았다.
