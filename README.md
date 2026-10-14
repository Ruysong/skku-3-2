# SKKU 3-2

2026년 2학기 과목별 공지, 과제, 퀴즈, 시험범위, 강의자료를 정리한 저장소입니다. 과목 폴더 안에는 `1주차`, `2주차`처럼 주차별 자료를 바로 보이게 두고, 긴 공부 정리는 각 주차의 `study.md`에 둡니다.

## 과목

| 과목 | 과목 홈 | 공지 | 과제/퀴즈 | 현재 자료 |
| --- | --- | --- | --- | --- |
| 시스템프로그래밍실습 - 한환수 | [README](./시스템프로그래밍실습_한환수/README.md) | [notices](./시스템프로그래밍실습_한환수/notices.md) | [assignments](./시스템프로그래밍실습_한환수/assignments.md) | 1~6주차 |
| 시스템프로그램 - 엄영익 | [README](./시스템프로그램_엄영익/README.md) | [notices](./시스템프로그램_엄영익/notices.md) | [assignments](./시스템프로그램_엄영익/assignments.md) | 1~6주차 |
| 알고리즘개론 - 김형식 | [README](./알고리즘개론_김형식/README.md) | [notices](./알고리즘개론_김형식/notices.md) | [assignments](./알고리즘개론_김형식/assignments.md) | 1~6주차 |
| 운영체제 - 신동군 | [README](./운영체제_신동군/README.md) | [notices](./운영체제_신동군/notices.md) | [assignments](./운영체제_신동군/assignments.md) | 1~6주차 |
| 캡스톤설계프로젝트 - 타메르 | [README](./캡스톤설계프로젝트_타메르/README.md) | [notices](./캡스톤설계프로젝트_타메르/notices.md) | [assignments](./캡스톤설계프로젝트_타메르/assignments.md) | 1~6주차, financial support |
| 컴퓨터구조개론 - 조형민 | [README](./컴퓨터구조개론_조형민/README.md) | [notices](./컴퓨터구조개론_조형민/notices.md) | [assignments](./컴퓨터구조개론_조형민/assignments.md) | 1~6주차 |

## 전체 관리

- [전 과목 PA·구현 학습목차](./PA-학습목차.md)
- [전체 할 일](./tasks.md)
- [정리 프로토콜](./protocol.md)
- [OCR 검수 엔진](./tools/ocr/README.md)

## 저장소 구조

각 과목의 주차별 `study.md`에서 수업 범위와 마감을 관리합니다. 새 자료가 들어오면 해당 주차 폴더에 원본과 정리를 누적합니다.


```text
.
├── README.md
├── PA-학습목차.md
├── protocol.md
├── tasks.md
├── tools/
│   └── ocr/
├── 시스템프로그래밍실습_한환수/
│   ├── README.md
│   ├── notices.md
│   ├── assignments.md
│   └── 1주차/
│       ├── study.md
│       └── week1-course_overview.pdf
├── 시스템프로그램_엄영익/
│   ├── README.md
│   ├── notices.md
│   ├── assignments.md
│   └── 1주차/
│       ├── study.md
│       └── SP00(CourseOutline)-(2026-2)-(SWE2001-41).pdf
├── 알고리즘개론_김형식/
│   ├── README.md
│   ├── notices.md
│   ├── assignments.md
│   ├── 1주차/
│   │   ├── study.md
│   │   ├── induction_교수님추천 참고예제.pdf
│   │   ├── lec00_overview.pdf
│   │   └── lec01_preliminaries.pdf
│   └── 2주차/
│       ├── study.md
│       ├── Performance Analysis.pdf
│       ├── Merge Sort.pdf
│       └── big_o.pdf
├── 운영체제_신동군/
│   ├── README.md
│   ├── notices.md
│   ├── assignments.md
│   └── 1주차/
│       ├── study.md
│       └── OS01-OS intro.pdf
├── 캡스톤설계프로젝트_타메르/
│   ├── README.md
│   ├── notices.md
│   ├── assignments.md
│   └── 1주차/
│       ├── study.md
│       └── financial-support/
│           ├── README.md
│           ├── Capstone Design Financial Support.zip
│           ├── 붙임1. 2026학년도 2학기 캡스톤디자인 지원사업(실습비).hwp
│           ├── 붙임3. [서식8] 실습비 청구서 (1).xlsx
│           ├── 붙임4. 참여학과 총괄표 (1).hwp
│           └── 붙임5. 캡스톤디자인 청구서 작성법 (1).pdf
└── 컴퓨터구조개론_조형민/
    ├── README.md
    ├── notices.md
    ├── assignments.md
    ├── 1주차/
    │   ├── study.md
    │   └── CA_Lecture_01.pdf
    └── 2주차/
        ├── study.md
        └── CA_Lecture_02.pdf
```

PA 구현 설명은 각 과목의 `pa-study.md`에서 연결합니다. 개별 과제 `study.md`와 시프실 주차별 `submitted/` 소스 보관 구조는 [PA 학습목차](./PA-학습목차.md#보관-방식)에 정리되어 있습니다.

## 정리 원칙

- 과목 `README.md`는 대문처럼 짧게 유지합니다.
- 공지 원문과 핵심 체크는 `notices.md`에 둡니다.
- 과제, 퀴즈, 마감, 평가 비율, 제출처는 `assignments.md`에 둡니다.
- 주차별 공부 흐름과 파일별 의미는 `주차/study.md`에 둡니다.
- 전문 텍스트화가 필요할 때만 `주차/transcript-자료명.md`를 추가합니다.
- 시험범위, 과제 조건, 퀴즈 일정은 공지나 자료에 명시된 내용을 기준으로 정리합니다.
- 파일이나 폴더 구조가 바뀌면 이 루트 README.md의 저장소 구조도 함께 갱신합니다.
- 자세한 운영 방식은 [정리 프로토콜](./protocol.md)을 따릅니다.
