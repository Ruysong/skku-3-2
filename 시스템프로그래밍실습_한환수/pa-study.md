# 시프실 실습·PA 구현목차

<!-- pa3-assets-20261116:start -->
## PA3 원본 확보 (11/16)

[PA3 학습·명세 목차](./PA/PA_3/study.md)에 41쪽 명세·공개 스켈레톤·예제를 모았다. 다중 클라이언트 좌석 예약 서버, NUM_CORES worker와 각 worker의 poll, 사용자·좌석 mutex가 연결된다. **12/6 23:59:59 마감**, 소스·Makefile 압축본과 PDF 보고서 별도 제출. 현재는 명세와 제공 코드를 읽고 예약 서버의 구현 구조를 준비하는 단계다.
<!-- pa3-assets-20261116:end -->

주차별 수업 개념과 구현 코드를 함께 정리했다. 제출 코드 사본은 `submitted/`, 작업 파일은 VMShare와 실습 폴더에서 읽을 수 있다.

## PA

| 과제 | 확인 상태 | 학습 자료 |
| --- | --- | --- |
| PA1 파일 검색기 | 제출 압축본 소스 확인, 사용자 제출 완료 확인 | [상세 구현 교안](./PA/PA_1/study.md), [사용자 구조도](./PA/PA_1/architecture.md) |
| PA2 shell | Command-line Shell 과제 수행·제출. 과제 PDF·제공 스켈레톤·Makefile·Q&A 보관 | [PA2 명세](./PA/PA_2/PA2.pdf), [스켈레톤 원본](./PA/PA_2/pa2_skeleton_code.zip), [소스](./PA/PA_2/pa2_skeleton_code/), [Q&A](./PA/PA_2/qa.md) |

### PA2 스켈레톤 확인 (2026-10-28)

- ZIP과 풀어둔 소스·헤더·Makefile 19개가 바이트 단위로 일치한다.
- C·헤더 전체 1,897줄이며, 주석·빈 줄을 제외하면 1,388줄이다. Makefile 41줄은 별도다.
- 셸 C·헤더 1,578줄, 명령어 C 파일 319줄. `pa2_echo.c`는 제공된 참고 예제이며 채점 대상이 아니라고 주석에 명시되어 있다.
- lexer/parser, 작업 관리 기반, `jobs/fg/bg` 함수가 제공된다. `shell_src/pa2.c`에는 TODO 표시 18곳이 남아 있다.
- 직접 구현할 핵심은 명령 실행, `cd/pwd/exit`, pipe·redirection, 시그널 처리 연결과 `head/tail/cat/cp/mv/rm` 6개 명령이다.
- 제공 코드의 규모와 파일별 역할을 정리해 셸 실행·시그널·파일 도구 구현을 공부할 때 활용한다.

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
| 9 | System V 메시지 큐 기반 채팅·ACK | [VMShare 작업 소스](./VMShare/week9/w9.c), [Makefile](./VMShare/week9/Makefile), [제출용 압축본](./VMShare/week9/2024315329_w9.tar.zst) | [9주차 자료](./9주차/study.md). Makefile과 w9.c를 함께 보관 |

| 10 | TCP 파일 전송 client/server | [작업 코드](./10주차/SPLweek10Practice/) | [10주차 설명](./10주차/study.md). client/server 작업 소스와 Makefile |
| 11 | pthreads 행렬–벡터 곱 | [제출 소스](./11주차/submitted/), [해시·멤버 근거](./11주차/submitted/source-manifest.json) | 11/10 제출. pthread_create/join과 행렬–벡터 곱 구현 |

## 버전 선택 규칙

1. 각 `submitted/source-manifest.json`에 원본 압축파일 경로, 압축파일 SHA-256, 멤버명, 복사한 소스 SHA-256을 남겼다.
2. 문서는 그 소스 사본을 설명한다. VMShare의 이름이 비슷한 skeleton이나 예전 debug 소스를 대신 설명하지 않는다.
3. PA1은 압축본과 `pa1_fixed_final/pa1`이 일치한다. 오래된 `pa1_debug`의 일부 소스는 다른 내용이므로 복습 기준에서 제외했다.
4. 5·6주차는 압축본에 완성 코드가 있고, 다른 경로에는 TODO skeleton이 남아 있었다. 파일을 지우지 않고 둘을 구분했다.
5. 컴파일된 실행파일·object·IDE 임시 파일은 이 구현 아카이브에 복사하지 않았다. 제출 전용 압축파일을 새로 만든 것도 아니다.

파일 검색기의 입력 처리·검색 방식·단어 경계는 [PA1 상세 설명](./PA/PA_1/study.md)에 정리했다.
