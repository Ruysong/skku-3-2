# 컴퓨터구조개론 7주차 (2026-10-12~10-18)

이번 주는 10/16 Project 1 제출과 10/21 중간고사 대비를 구분해 진행한다.

## 목차

1. [자료와 역할](#자료와-역할)
2. [공부 구조](#공부-구조)
3. [Project 1 제출 조건](#project-1-제출-조건)
4. [시험과 수업](#시험과-수업)
5. [정리 상태](#정리-상태)

## 자료와 역할

| 자료 | 역할 |
| --- | --- |
| [sample_midterm_withAnswers.pdf](./sample_midterm_withAnswers.pdf) | 이미 넣어둔 4페이지 중간고사 예제·답안. 새 강의 교안과 별도로 문풀에 사용 |
| [중간고사 준비 자료](../midterm-prep/목차.md) | RISC-V 기출 3세트와 교수님 예제를 모은 폴더. 문항 중복, MIPS 제외 이유, 답안 차이 확인 |
| [CA_Proj1.pdf](../PA/PA_1/CA_Proj1.pdf) | RISC-V Disassembly 과제의 입력·출력·실행·제출 조건 |
| [과제 기록](../assignments.md) | Project 1 마감, 서버 테스트와 패키징 기준 |
| [6주차 study](../6주차/study.md) | 이전 주차 정리 연결 |
| [CA_Lecture_06.pdf](../6주차/CA_Lecture_06.pdf) | 기존 강의 원본. 새 7주차 강의 PDF로 이름을 바꾸거나 이동하지 않음 |

새 강의 자료는 이 폴더에 그대로 추가하고, 예제 문제와 수업 내용은 독립된 섹션으로 정리한다.

## 공부 구조

```text
시험 대비: sample_midterm_withAnswers.pdf
  -> 문제를 먼저 풂
  -> 답안과 대조
  -> 오답의 개념을 원래 주차 교안으로 연결

과제: Project 1 명세
  -> binary 입력
  -> RV32I instruction decoding
  -> 정확한 assembly 출력 형식
  -> 학과 Linux 서버 reference와 diff
  -> 제출 디렉터리·build 파일 확인
```

예제 문제는 연습 자료이며 최종 시험 범위의 전부라고 단정하지 않는다. 원본 문제·답안은 수정하지 않는다.

## Project 1 제출 조건

- 마감: **10/16(금) 23:59:59**, 저장된 명세 기준 지각 제출 불가.
- 과제: RV32I binary를 읽어 instruction별 assembly를 출력하는 RISC-V Disassembly.
- C/C++은 서버에서 `riscv-sim`을 생성하는 Makefile, Python은 지정된 executable launcher가 필요하다.
- 명세의 mnemonic, 레지스터, signed immediate, instruction별 출력 형식을 정확히 맞춘다.
- 학과 Linux 서버에서 reference output과 비교한다. 로컬 실행 성공만으로 서버 검증을 완료했다고 기록하지 않는다.
- 부모 디렉터리에서 `~swe3005/bin/submit proj1 project1`로 제출하고, `~swe3005/bin/check-submission proj1`로 확인한다. 실제 제출 디렉터리 이름에 맞춘다.
- source와 build 파일을 제출 디렉터리 바로 아래에 두고 불필요한 build artifact를 제거한다.
- 현재 구현·테스트·제출 완료 여부는 미확인이다.

## 시험과 수업

중간고사는 사용자 제공 교수 공지 기준 **10/21(수)**다. 10/5 휴강을 이번 주 월요일에도 적용하지 않는다. 이번 주 수업 변경과 최종 시험 범위는 새 공지가 우선한다. 강의 일정표에 있던 잠정 범위와 예제 문제를 최종 확정 범위로 바꾸어 쓰지 않는다.

## 정리 상태

주차 세팅과 자료 연결을 위한 짧은 문서다. 기존 예제의 풀이 검수나 7주차 강의의 1만자 상세 교안은 아직 수행하지 않았으므로, 이 문서를 완성된 문제 해설·강의 전문으로 표시하지 않는다.
