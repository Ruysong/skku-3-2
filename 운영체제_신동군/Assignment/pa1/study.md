# PA1 학습 교안: xv6 System Calls

**한 줄 결론:** 프로세스에 nice 정보를 저장하고 사용자 프로그램이 `getnice`, `setnice`, `ps`로 커널의 프로세스 정보를 다룰 수 있도록 시스템콜 경로를 확장한 과제다.

근거: [과제 PDF](./Project%201_26f.pdf), [최종 보고서](./PA1_System_Call_Report_Junhyuk_Song.pdf). 아래 구현 설명은 보고서 1~2페이지를 기준으로 한다. 실제 xv6 소스는 미보관이며 새로 실행하지 않았다. 사용자 확인상 코드와 재작성 보고서는 제출 완료다.

## 목차

1. [시스템콜 전체 경로](#시스템콜-전체-경로)
2. [세 함수의 동작](#세-함수의-동작)
3. [파일별 변경 역할](#파일별-변경-역할)
4. [동시성과 초기화](#동시성과-초기화)
5. [검증 기록과 한계](#검증-기록과-한계)
6. [복습 질문](#복습-질문)

## 시스템콜 전체 경로

```text
User program: getnice(pid)
       |
user.h: declaration
       |
usys.S: syscall stub / SYS_getnice
       |
x86 trap -> kernel dispatch
       |
syscall.c: number -> sys_getnice()
       |
sysproc.c: argint() retrieves pid
       |
proc.c: lock process table / search PID / read nice
       |
return value -> user program
```

이 과제의 `xv6-public`은 x86 계열이다. RISC-V xv6의 `ecall` 설명을 그대로 붙이지 않는다. 사용자 함수가 커널 `proc.c` 함수를 보통의 함수처럼 직접 호출하는 것이 아니다. 기존 trap과 syscall dispatcher를 거쳐 권한 경계를 넘는다.

보고서에서 정한 syscall 번호는 22, 23, 24다. 이 번호는 dispatcher에서 함수를 고르는 식별자다. x86 시스템콜 trap vector와 같은 번호를 의미하지 않으며, 과제에서 모든 학생에게 반드시 이 세 숫자를 요구했다고 해석하지 않는다.

## 세 함수의 동작

| 사용자 인터페이스 | 보고서에 적힌 동작 | 결과 |
| --- | --- | --- |
| `getnice(pid)` | non-UNUSED 프로세스 중 PID 일치 항목 검색 | 발견 시 nice, 미발견 시 -1 |
| `setnice(pid, value)` | 0~39 검사 후 대상 nice 변경 | 성공 0, 범위 오류·PID 없음 -1 |
| `ps(pid)` | PID 0이면 전체, 그 외에는 해당 PID만 출력 | name·PID·state·nice; 미발견이면 출력 없음 |

nice의 기본값은 20이다. 이 과제는 값의 저장과 관리 인터페이스를 추가한다. nice가 있다고 해서 scheduler가 이미 그 값을 기준으로 CPU 시간을 배분하도록 변경되었다는 뜻은 아니다. **정보 인터페이스 구현**과 **정책 구현**을 구분한다.

`ps`의 사용자 선언은 void지만, 커널 syscall wrapper는 기존 dispatch 구조에 맞추어 정수를 반환하는 것으로 보고서가 설명한다. 사용자 관점의 함수 prototype과 커널 dispatcher handler의 형식은 서로 다른 층이다.

## 파일별 변경 역할

| 파일 | 보고서에 적힌 변경 | 빠뜨리면 생기는 문제 |
| --- | --- | --- |
| `proc.h` | `struct proc`에 `int nice` | 프로세스별 상태 저장 위치가 없음 |
| `proc.c` | `allocproc` 기본값 20, 실제 세 커널 함수 | 호출은 등록되어도 실제 동작이 없음 |
| `defs.h` | 커널 함수 선언 | 커널 파일 간 인터페이스 불일치 |
| `sysproc.c` | `sys_*` wrapper, `argint`로 인수 읽기 | 사용자 인수가 커널 구현으로 전달되지 않음 |
| `syscall.h` | 새 `SYS_*` 번호 | stub와 dispatcher 식별자 누락 |
| `syscall.c` | extern handler 선언과 dispatch table | 번호를 보내도 올바른 handler에 연결되지 않음 |
| `usys.S` | 사용자 syscall stub | user 함수에서 trap으로 넘어가는 진입점 없음 |
| `user.h` | user 함수 prototype | 사용자 프로그램이 인터페이스를 알 수 없음 |

등록 작업은 같은 의미의 이름과 번호를 여러 위치에 일관되게 연결하는 작업이다. `proc.c` 함수 하나만 작성해도 사용자 프로그램에서 사용할 수 있을 것이라고 생각하면 안 된다.

## 동시성과 초기화

프로세스 테이블은 커널의 공유 상태다. 보고서는 get/set에서 `ptable.lock`을 잡고 탐색·읽기·수정을 수행한다고 설명한다. PID를 찾는 동안 다른 실행 흐름이 프로세스 슬롯을 변경할 수 있으므로, 단순 배열 탐색 이상의 동시성 의미가 있다.

`allocproc()`에서 매번 기본값을 넣는 것도 중요하다. 종료된 프로세스의 슬롯이 재사용될 때 이전 nice가 새 프로세스에 우연히 남으면 초기값 요구를 만족하지 못한다. 전역 배열이 부팅 때 0으로 초기화된다는 사실만으로 default 20을 대신할 수 없다.

`setnice`는 먼저 범위를 검사하고, 그다음 존재하는 PID를 찾아 변경한다. 실패 시 기존 상태를 변경하지 않는다는 의미로 읽는다. `ps(0)`은 프로세스 ID 0의 상세정보를 찾는 것이 아니라 전체 출력이라는 특별한 입력 계약이다.

## 검증 기록과 한계

보고서 2페이지의 Validation에는 아래 결과가 기록되어 있다.

| 사례 | 보고서의 관찰값 |
| --- | --- |
| 현재 PID 초기 `getnice` | 20 |
| 현재 PID에 `setnice(..., 7)` | 0 |
| 변경 후 `getnice` | 7 |
| 범위 밖 40 설정 | -1 |
| 없는 PID 조회 | -1 |
| 현재 PID `ps` | 한 프로세스 출력 |
| `ps(0)` | init, sh, testnice 출력 |

보고서는 make 빌드, `make qemu-nox`, 임시 user-space `testnice`로 확인했다고 서술한다. 이것은 제출 보고서에 남은 결과이며 이번에 서버에서 재현한 것은 아니다. testnice 소스, 실행 로그 파일, 최종 xv6 수정 소스가 이 아카이브에 없으므로 구현과 표의 일치 여부를 독립적으로 재검증하지 못했다.

최종 보고서는 2페이지다. 공지의 PDF·5페이지 이내 조건과 별개로, 코드 서버 제출과 보고서 제출은 각각 완료 여부를 확인해야 하는 산출물이다. 한쪽만 존재한다고 다른 쪽까지 제출되었다고 추정하지 않는다.

## 복습 질문

1. 사용자 `getnice`와 커널 `getnice` 사이에는 어떤 단계들이 있는가?
2. syscall 번호와 trap vector를 혼동하면 왜 안 되는가?
3. `argint`는 무엇을 가져오며, 왜 wrapper에서 필요한가?
4. 기본값을 `allocproc`에 넣는 이유는?
5. 프로세스 테이블 lock 없이 조회·변경하면 어떤 공유 상태 문제가 생기는가?
6. `ps(0)`과 없는 PID의 `ps`는 어떻게 다른가?
7. nice 값 변경이 곧 스케줄링 정책 구현은 아닌 이유는?

[운체 PA 목차](../../pa-study.md)
