# Project 2: Single-cycle RISC-V CPU Simulator

## 목표와 파일

Proj1의 명령어 비트 추출과 immediate 복원을 재사용하고, 레지스터·PC·메모리를 변경하는 실행 기능을 추가한다.

- 명세: [CA_Proj2.pdf](./CA_Proj2.pdf)
- 구현: [main.cpp](./main.cpp), [Makefile](./Makefile)
- 서버 작업 위치: `/home/s24315329/computer_architecture/project2`
- 기존 Proj1은 수정하지 않았으며 서버의 `computer_architecture/.backups/` 아래에 별도로 백업했다.

## 실행 흐름

```text
입력 인자 확인 → instruction binary 읽기
                     ↓
레지스터 32개를 0, 데이터 메모리 64KiB를 0xFF로 초기화
                     ↓
data binary가 있으면 0x10000000부터 적재
                     ↓
PC=0에서 fetch → decode → execute → PC 갱신
                     ↓
N회 실행 또는 다음 명령어 없음 → x0~x31 출력
```

## 주요 함수와 구현

| 함수 | 역할 |
|---|---|
| `bits`, `sign_extend` | 명령어 필드 추출과 immediate 부호 확장 |
| `store_offset`, `branch_offset`, `jump_offset` | 흩어진 S/B/J 형식 immediate 복원 |
| `signed_value` | 레지스터 비트를 signed 32-bit 값으로 해석하여 비교 |
| `arithmetic_right` | 부호를 유지하는 오른쪽 시프트 |
| `read_binary`, `read_word` | 바이너리 읽기와 little-endian 32-bit word 복원 |
| `data_address` | 일반 데이터 메모리 접근 범위 확인 |
| `execute` | opcode/funct3/funct7에 따라 한 명령어 실행 |
| `main` | 초기화, 실행 횟수 제한, 종료와 최종 레지스터 출력 |

지원 명령어는 명세의 27개다. 레지스터는 `uint32_t`로 저장하여 덧셈·뺄셈 결과를 32비트 범위에서 유지한다. signed 비교와 논리/산술 시프트는 구분한다. `rd == 0`이면 레지스터 기록만 생략하며, 메모리 접근이나 콘솔 입력 자체는 실행한다.

기본 다음 PC는 현재 PC + 4이다. 분기·점프일 때만 덮어쓴다. `jalr`는 rd에 기록하기 전에 rs1 값을 읽어 두므로 rd와 rs1이 같아도 기존 값을 사용한다. 분기·점프를 포함하여 실제 실행할 때마다 N을 한 번 소비한다.

데이터 메모리는 byte 배열이며 주소에서 0x10000000을 뺀 위치로 접근한다. `lw`/`sw`는 4바이트를 little-endian으로 읽고 쓴다. 0x20000000은 별도로 처리하여 `sw`는 하위 8비트 문자만 출력하고, `lw`는 십진수 정수를 입력받는다. 디버깅 문구는 stdout에 출력하지 않는다.

## 서버 검증: 2026-11-16

- 서버 g++ 13.1.0, C++17, `-O2 -Wall -Wextra -Wpedantic` 컴파일 성공. 경고 없음.
- 교수님 reference `~swe3005/2026f/proj2/riscv-sim`과 공개 테스트 11개 모두 출력 일치.
- 실행 횟수 경계, 중간 레지스터 상태, 음수·최대값, x0, 메모리 마지막 word, 미초기화 영역의 0xFF, AUIPC, 분기와 점프, 콘솔 입력 등을 추가 비교했다.
- 총 171개 비교 중 170개 일치, 1개 차이. 공개 테스트는 전부 일치한다.

차이 1개는 추가로 만든 `jalr` 홀수 주소 테스트다. 현재 구현은 `(rs1 + imm) & ~1`로 주소 13을 12로 바꾸어 점프한다. reference는 동일 입력에서 다음 명령어를 실행하지 않는다. 목적 주소 12/13과 rd=rs1 여부를 나누어 확인했으며, 정렬된 목적 주소에서는 둘 다 일치했다.

[RISC-V 공식 ISA](https://github.com/riscv/riscv-isa-manual/blob/main/src/unpriv/rv32.adoc)는 JALR 목적 주소의 최하위 비트를 0으로 만들도록 정의한다. 현재 코드는 이 동작을 유지했다. 따라서 reference와 모든 임의 입력에서 완전히 일치한다고 표현하지 않는다. 해당 차이의 채점 취급은 미확인이다.

서버의 테스트 입력과 비교 결과는 제출 폴더 밖의 `computer_architecture/project2-checks/`에 있다. 테스트 통과는 공개·자체 검증 결과이며 hidden test 채점 결과가 아니다.

## 제출

마감: **2026-11-20 금요일 23:59:59**, 지각 제출 불가. 명세에 별도 보고서 요구는 없다.

서버 project2 안에는 `main.cpp`, `Makefile`만 남겼다. `make`하면 실행파일 `riscv-sim`이 생성되고 `make clean`하면 빌드 산출물이 제거된다. **2026-11-16 사용자 서버 제출 완료 확인**. Canvas 항목의 미제출 표시는 외부 서버 제출 상태와 구분한다.

```bash
cd ~/computer_architecture/project2
make
# reference와 비교
make clean
cd ..
~swe3005/bin/submit proj2 project2
~swe3005/bin/check-submission proj2
```

AI 보조는 명세에서 허용하지만 본인 코드의 독창성과 설명 책임은 본인에게 있다. 협업 금지와 표절 규칙도 그대로 적용된다. 구체적 함수명이나 클래스 구조는 강제되어 있지 않다.
