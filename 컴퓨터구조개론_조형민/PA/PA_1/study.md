# PA1 구현 교안: RV32I Disassembler

**한 줄 결론:** 바이너리 파일에서 4바이트씩 읽고, RISC-V 명령어 필드를 분해해 과제에서 정한 어셈블리 한 줄로 출력했다. 명령어를 실행하는 CPU simulator는 아니다.

근거: [과제 PDF](./CA_Proj1.pdf), [실제 main.cpp](./main.cpp), [Makefile](./Makefile). 제출 완료는 10/14 사용자 확인 기준이다. 소스는 이번 정리 과정에서 수정하지 않았다.

## 목차

1. [전체 구조](#전체-구조)
2. [파일과 함수 역할](#파일과-함수-역할)
3. [바이트에서 명령어까지](#바이트에서-명령어까지)
4. [즉시값 복원](#즉시값-복원)
5. [37개 명령어 판별](#37개-명령어-판별)
6. [출력·빌드·검증](#출력빌드검증)
7. [혼자 설명할 것](#혼자-설명할-것)

## 전체 구조

```text
argv[1]: binary file
          |
          v
disassemble_file()
  read 4 bytes -> little-endian uint32_t
          |
          v
decode()
  opcode -> instruction family
  funct3 / funct7 -> exact mnemonic
  rd / rs1 / rs2 / immediate -> operands
          |
          v
format helpers -> one assembly string
          |
          v
stdout: inst index: 8-digit hex assembly
```

핵심 경계는 **읽기**, **해독**, **출력 형식**이다. `add`를 읽어도 실제 덧셈을 하지 않는다. `jal`을 읽어도 파일 읽기 위치를 점프시키지 않는다. 파일의 명령어들을 순서대로 해독한다.

## 파일과 함수 역할

| 함수/파일 | 실제 역할 | 중요한 이유 |
| --- | --- | --- |
| `main` | 인자가 정확히 한 개의 파일 경로인지 확인하고 해독 호출 | 입력 계약을 한곳에서 결정 |
| `disassemble_file` | 바이너리 열기, 4바이트 읽기, word 조립, 줄 출력 | ISA 형식과 파일 형식을 분리 |
| `bits` | `[high:low]`의 연속 비트 추출 | 필드 위치를 직접 반복 계산하지 않음 |
| `sign_extend` | 지정 폭의 2의 보수 값을 signed 정수로 해석 | 음수 immediate를 올바르게 출력 |
| `store_offset` | S형의 두 immediate 조각 결합 | store에는 연속 12비트 필드가 없음 |
| `branch_offset` | B형의 흩어진 비트 결합, 13비트 부호 해석 | branch offset의 배치와 정렬 처리 |
| `jump_offset` | J형 비트 결합, 21비트 부호 해석 | jal의 긴 상대 offset 처리 |
| `upper_immediate` | U형 상위 20비트를 12비트 이동한 signed 값으로 출력 | 과제 출력은 원래 20비트 숫자와 다름 |
| `reg` | 레지스터 번호를 `x번호`로 변환 | ABI 별명 대신 과제 형식 유지 |
| `three_regs` | R형의 `rd, rs1, rs2` 출력 | 피연산자 순서 고정 |
| `reg_reg_imm` | `rd, rs1, immediate` 출력 | 산술 immediate와 jalr 등 |
| `reg_offset` | `register, offset(base)` 출력 | load/store의 메모리 표기 |
| `Makefile` | C++17로 `main.o`, `riscv-sim` 생성 | 채점 환경의 빌드 진입점 |

`bits()`의 mask 계산은 64비트 값을 이용한다. 폭이 커질 때 32비트 정수의 잘못된 shift가 생기지 않도록 한 구현이다. `sign_extend()`는 최상위 부호 비트가 켜지면 64비트 계산에서 `2^width`를 빼고 결과를 반환한다.

## 바이트에서 명령어까지

```text
파일: b0 b1 b2 b3
word = b0 | (b1 << 8) | (b2 << 16) | (b3 << 24)

31           25 24      20 19      15 14   12 11       7 6       0
+--------------+----------+----------+-------+-----------+---------+
|    funct7    |   rs2    |   rs1    |funct3 |    rd     | opcode  |
+--------------+----------+----------+-------+-----------+---------+
```

위 공통 추출 위치는 R형을 기준으로 이해하면 쉽다. 다른 형식에서는 같은 위치가 immediate로 쓰일 수 있으므로, `decode()`가 opcode로 형식을 정한 뒤 필요한 필드만 사용한다.

리틀엔디언은 **낮은 주소의 바이트가 정수의 하위 비트**라는 뜻이다. 파일 word 조립과 immediate 재배치는 서로 다른 단계다. 먼저 32비트 명령어 자체를 만들고, 그다음 ISA가 정한 필드를 읽는다.

## 즉시값 복원

| 형식 | 복원식/폭 | 실제 처리 |
| --- | --- | --- |
| I | `instruction[31:20]`, 12비트 | `sign_extend(..., 12)` |
| S | `{instruction[31:25], instruction[11:7]}`, 12비트 | `store_offset` |
| B | `{31, 7, 30:25, 11:8, 0}`, 13비트 | `branch_offset` |
| U | `instruction & 0xfffff000` | 이미 12비트 이동한 signed 32비트 값 |
| J | `{31, 19:12, 20, 30:21, 0}`, 21비트 | `jump_offset` |

B·J형의 마지막 0은 파일에서 읽어오는 추가 비트가 아니다. 상대 offset이 2바이트 단위 정렬을 표현하므로 복원할 때 붙이는 0이다. B형 `instruction[7]`은 offset의 11번째 비트이지 최하위 비트가 아니다.

부호 확장 예시: 12비트 `0xfff`는 unsigned로 4095지만, 부호 비트가 켜져 있으므로 `4095 - 4096 = -1`이다. 반대로 shift immediate 명령의 `shamt`는 5비트 shift 양이다. 일반 I형의 12비트 signed immediate 전체를 shift 양으로 출력하지 않는다.

분기와 `jal`의 출력은 **상대 offset**이다. 현재 출력 인덱스나 절대 주소를 더해 target address를 만드는 기능은 이 과제 코드에 없다. `lui`·`auipc`는 상위 비트를 이동한 값을 signed decimal로 출력한다.

## 37개 명령어 판별

| 분류 | 구현된 mnemonic |
| --- | --- |
| 상위 immediate | `lui`, `auipc` |
| jump | `jal`, `jalr` |
| branch | `beq`, `bne`, `blt`, `bge`, `bltu`, `bgeu` |
| load | `lb`, `lh`, `lw`, `lbu`, `lhu` |
| store | `sb`, `sh`, `sw` |
| I형 산술·논리 | `addi`, `slti`, `sltiu`, `xori`, `ori`, `andi` |
| immediate shift | `slli`, `srli`, `srai` |
| R형 산술·논리·shift | `add`, `sub`, `sll`, `slt`, `sltu`, `xor`, `srl`, `sra`, `or`, `and` |

예를 들어 R형 opcode `0x33`이고 funct3가 0이면 아직 `add`로 확정할 수 없다. funct7가 `0x00`이면 `add`, `0x20`이면 `sub`다. funct3가 5인 경우도 funct7로 `srl`과 `sra`를 구분한다. 지원하지 않는 조합은 임의의 mnemonic으로 해석하지 않고 `unknown instruction`으로 출력한다.

이 코드는 RV32I 전체 모든 확장 명령을 지원하는 범용 도구가 아니다. 곱셈 확장, 압축 명령어, 시스템 명령 등을 포함했다고 설명하지 않는다.

## 출력·빌드·검증

정상 출력 예시는 다음 구조다.

```text
inst 0: 00208033 add x0, x1, x2
```

인덱스는 0부터, 명령어 word는 소문자 16진수 8자리, 레지스터는 `x번호`, immediate는 10진수다. 16진수 stream 설정 후 인덱스 등의 다음 출력이 영향을 받지 않도록 10진수 설정을 되돌린다. 오류는 stderr로 보내 정상 출력과 구분한다.

파일이 없거나 마지막 명령어가 4바이트 미만이면 실패한다. 빈 파일은 명령어가 0개인 입력으로 처리된다. 처리 시간은 명령어 수를 m이라 할 때 `O(m)`, 매번 한 word와 작은 문자열만 처리하므로 입력 파일 크기에 비례하는 저장 배열은 없다.

```sh
make
./riscv-sim input.bin
make clean
```

Makefile은 `g++ -std=c++17 -O2 -Wall -Wextra -Wpedantic`으로 컴파일한다. `clean`은 실행파일과 object만 지우는 타깃이다. 제출 산출물은 이 폴더의 `main.cpp`, `Makefile`이며 별도 보고서가 필요한 과제로 기록하지 않는다.

**확인 근거 구분:** 앞선 제출 검수에서 Windows MSVC C++17 빌드와 99,924개 생성 명령어 사례의 출력 비교를 수행했다. 37개 mnemonic, I/S signed immediate, 분기 offset, shift 양, U/J 경계값, 지원하지 않는 필드 조합과 파일 오류를 포함했다. 이 테스트 산출물은 임시 검수 자료였고 저장소에 남긴 공식 회귀 테스트는 아니다. 이번 아카이브 작업에서는 소스를 다시 읽었으며 해당 테스트를 재실행했다고 쓰지 않는다. Linux `g++`·`make` 실행과 교수님 서버의 공식 7개 파일 비교는 별도의 검증이며, 로컬 검수 통과와 같다고 취급하지 않는다.

## 혼자 설명할 것

1. 4바이트의 순서와 32비트 명령어의 필드 위치는 어떻게 다른가?
2. opcode만 보고 `add`와 `sub`를 구분할 수 없는 이유는?
3. 음수 immediate에서 sign extension을 빼면 어떤 출력이 나오는가?
4. B형의 bit 7과 마지막 0은 각각 어디서 오는가?
5. `jal` 해독과 `jal` 실행의 차이는 무엇인가?
6. load는 `rd`, store는 `rs2`를 데이터 레지스터로 출력하는 이유는?
7. U형 출력에서 12비트 이동을 놓치면 왜 과제 결과와 달라지는가?

[과목 PA 목차로 돌아가기](../../pa-study.md)
