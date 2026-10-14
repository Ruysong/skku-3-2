# 시프실 실습·PA 구현목차

주차별 수업 내용은 기존 `study.md`, **실제로 작성해서 제출용 압축본에 넣은 코드**는 아래 링크로 구분한다. 압축파일이 있다는 사실과 LMS 업로드 성공은 별개다. 제출용 원본 VMShare는 그대로 두었다.

## PA

| 과제 | 확인 상태 | 학습 자료 |
| --- | --- | --- |
| PA1 파일 검색기 | 제출 압축본 소스 확인, 사용자 제출 완료 확인 | [상세 구현 교안](./PA/PA_1/study.md), [사용자 구조도](./PA/PA_1/architecture.md) |
| PA2 shell | 과제 PDF는 있으나 별도 완성 코드 없음 | [PA2 명세](./PA/PA_2/PA2.pdf); 5·7주 실습은 기반 연습이지 PA2 완성본이 아님 |

## 주차별 구현

| 주차 | 실제 작성한 것 | 코드·근거 | 설명 |
| --- | --- | --- | --- |
| 1 | 환경 구축·제출 화면 | 코드 압축본은 이번 확인 대상에서 발견하지 못함 | [1주차 학습](./1주차/study.md) |
| 2 | 명령행 정수 두 개의 plus/minus, GDB 기록 | [소스와 로그](./2주차/submitted/) | [2주차 구현](./lab-study.md#2주차-다중-소스와-디버깅) |
| 3 | Makefile과 결과 비교 script | [완성 소스·Makefile·script](./3주차/submitted/) | [3주차 구현](./lab-study.md#3주차-makefile과-shell-script) |
| 4 | Unix I/O 줄 번호 붙이기 | [완성 소스](./4주차/submitted/) | [4주차 구현](./lab-study.md#4주차-file-io와-줄-번호) |
| 5 | fork/exec/wait 기반 foreground shell | [완성 week5.c](./5주차/submitted/week5.c) | [5주차 구현](./lab-study.md#5주차-process와-간단한-shell) |
| 6 | 부모·자식 signal 요청/응답 | [완성 w6.c](./6주차/submitted/w6.c) | [6주차 구현](./lab-study.md#6주차-signal로-부모자식-통신) |
| 7 | 단일 pipe 또는 redirection shell | [소스·helper·Makefile](./7주차/submitted/) | [7주차 구현](./lab-study.md#7주차-pipe와-redirection) |

## 버전 선택 규칙

1. 각 `submitted/source-manifest.json`에 원본 압축파일 경로, 압축파일 SHA-256, 멤버명, 복사한 소스 SHA-256을 남겼다.
2. 문서는 그 소스 사본을 설명한다. VMShare의 이름이 비슷한 skeleton이나 예전 debug 소스를 대신 설명하지 않는다.
3. PA1은 압축본과 `pa1_fixed_final/pa1`이 일치한다. 오래된 `pa1_debug`의 일부 소스는 다른 내용이므로 복습 기준에서 제외했다.
4. 5·6주차는 압축본에 완성 코드가 있고, 다른 경로에는 TODO skeleton이 남아 있었다. 파일을 지우지 않고 둘을 구분했다.
5. 컴파일된 실행파일·object·IDE 임시 파일은 이 구현 아카이브에 복사하지 않았다. 제출 전용 압축파일을 새로 만든 것도 아니다.

**요구사항 차이:** PA1 압축본은 단어 경계를 `' '`만으로 검사한다. TA가 추가 안내한 tab·carriage return 경계까지 지원한다고 보장할 수 없다. [PA1의 실제 한계](./PA/PA_1/study.md#요구사항과-실제-구현의-차이)에서 따로 확인한다.
