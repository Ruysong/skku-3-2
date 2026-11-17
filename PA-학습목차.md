# 전과목 구현·제출물 목차

기준일: **2026-11-17**. [경험·근거 지도](./경험-근거지도.md)에서 자소서·면접 소재를 고르고, 이 목차에서 원본 코드·보고서로 이동한다. 강의 개념은 각 주차 `study.md`, 일정·제출 확인은 `assignments.md`와 [tasks.md](./tasks.md)에 있다.

| 과목 | 보관된 수행 내용 | 구현 설명 | 근거 수준 |
|---|---|---|---|
| 컴퓨터구조 | RV32I 해독기, single-cycle 실행 시뮬레이터 | [PA 목차](./컴퓨터구조개론_조형민/pa-study.md) | 두 C++·Makefile·구현 설명. Proj2 reference 비교와 차이 사례 기록 |
| 알고리즘 | stable linked-list merge sort, Min-Max-Median PQ, k-DNA DP, 증명 과제 | [PA·증명 목차](./알고리즘개론_김형식/pa-study.md) | 실제 C·보고서·공개 예제. 제출과 채점 상태 구분 |
| 운영체제 | xv6 부팅, nice/ps, CFS, mmap·page fault·fork 참조 수 | [PA 목차](./운영체제_신동군/pa-study.md) | 보고서·명세·제출 기록. 최종 커널 소스는 공개 저장소 미보관 |
| 시프실 | 파일 검색기, 프로세스·signal·pipe·MQ·TCP·pthreads | [실습·PA 목차](./시스템프로그래밍실습_한환수/pa-study.md) | 제출본 사본·SHA-256, VMShare 작업본, 제공 스켈레톤을 분리 |
| 시스템프로그램 | Chap.2 표현·변환, Chap.3 C/어셈블리 풀이 | [과제 범위](./시스템프로그램_엄영익/pa-study.md) | 풀이 과제·교안. 별도 소프트웨어 제품 구현으로 해석하지 않음 |
| 캡스톤 | RoverAid 통신 보고·인식·로컬 AI 데모·발표 | [프로젝트 목차](./캡스톤설계프로젝트_타메르/pa-study.md) | 본인 역할이 명시된 보고서, 로컬 데모 소스·tests, 팀 제안·계획 구분 |

## 제출 코드 선택 규칙

1. `submitted/source-manifest.json`이 있으면 압축 경로·멤버명·SHA-256을 확인하고 그 사본을 읽는다. VMShare 원본은 유지한다.
2. `week*_code`, `skeleton`, `Template`은 제공 자료일 수 있다. TODO·함수 본문·제출본을 대조한다.
3. 11주차 pthreads 제출 소스는 [여기](./시스템프로그래밍실습_한환수/11주차/submitted/)에 사본·출처를 보관했다. 교수 제공 `week11_code`와 구분한다.
4. 시프실 PA2는 제공 스켈레톤을 보관 중이며 완성 제출 소스로 보장하지 않는다. PA3는 [예약 서버 요구사항·원본](./시스템프로그래밍실습_한환수/PA/PA_3/study.md) 분석 단계다.
5. 캡스톤의 통신 MockMotor 검증과 실제 구동·navigation 통합, xv6 보고서와 최종 소스, 자체 시험과 공식 채점은 각각 구분한다.

## 원본 보관 구조

```text
과목/README.md          분야·대표 경험·근거 입구
과목/pa-study.md        과제·실습 전체 선택
과제/study.md           구조·자료구조·입출력·검증·제한
주차/submitted/        실제 제출 압축본 소스 사본
  source-manifest.json 압축·멤버 경로와 SHA-256
주차/study.md          수업 학습지도와 원본 자료
```

시프실 PA1의 공백 경계 제한 등 실제 구현 차이는 해당 과제 설명에 남긴다. 개선 아이디어를 이미 구현한 기능으로 바꾸거나 보고서만 있는 항목을 코드 검수 완료로 표시하지 않는다.
