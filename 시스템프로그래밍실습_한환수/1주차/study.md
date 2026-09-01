# 시스템프로그래밍실습 - 한환수 1주차 학습지도

## 한줄 결론

1주차는 수업 전체 운영 방식을 확인하고, 앞으로 모든 실습의 기본 환경이 될 Ubuntu/Linux와 Vim 사용을 준비하는 주차입니다. 이번 주 결과물은 거창한 프로그램이 아니라, Linux 환경에서 Vim으로 `hello.c`를 작성하고 `Hello, Linux!` 실행 화면을 스크린샷으로 제출하는 것입니다.

## 원본 자료

- `./week1-course_overview.pdf`: 과목 안내, 평가 방식, 학기 주제 흐름, Ubuntu VM 설치, Vim 기초, 1주차 Lab Exercise

## 이번 주에 바로 해야 할 것

- [ ] VirtualBox/Ubuntu 또는 허용 가능한 Linux 환경 준비
- [ ] 터미널에서 `sudo apt update`, `sudo apt upgrade`, `sudo apt install build-essential` 실행
- [ ] 필요하면 `sudo apt install vim`으로 Vim 설치
- [ ] Vim 기본 모드와 저장/종료 명령 익히기
- [ ] Vim으로 `hello.c` 직접 작성
- [ ] 컴파일/실행 결과 확인
- [ ] 실행 화면 스크린샷을 `STUDENT ID_w1.png` 형식으로 저장
- [ ] 2026-09-08 23:59까지 iCampus에 스크린샷 제출

## 과목 운영 핵심

이 과목은 운영체제/시스템프로그램 개념을 실제 Linux 환경에서 직접 써보는 실습 과목에 가깝습니다. 단순히 C 문법을 배우는 과목이라기보다, Linux가 제공하는 개발 환경, system call, process, IPC, socket, thread, synchronization 같은 기능을 직접 프로그램으로 다루는 쪽입니다.

PDF의 구조 그림은 아래 층위를 보여줍니다.

- User Space: Pthread library, standard library, `ssh`, `vi`, `gcc`, `apt` 같은 사용자 도구와 라이브러리
- System Call Interface: 사용자 프로그램이 커널 기능을 요청하는 경계
- Kernel Space: Operating System Kernel
- Hardware: CPU, memory, I/O devices

앞으로의 실습은 대체로 user space에서 코드를 작성하지만, 실제로는 system call interface를 통해 kernel space의 기능을 호출하는 흐름을 이해하는 것이 중요합니다.

## 왜 Linux를 쓰는가

PDF는 Linux를 쓰는 이유를 세 가지로 설명합니다.

첫째, Linux는 과학/산업 현장에서 많이 쓰입니다. 서버, 연구 환경, 개발 환경에서 Linux를 기본으로 보는 경우가 많습니다.

둘째, 인터넷 서버와 서비스가 Linux 위에서 많이 동작합니다. 시스템프로그래밍을 배우려면 실제 서버 환경에 가까운 운영체제에 익숙해지는 것이 유리합니다.

셋째, Linux는 system call과 개발 도구를 직접 다루기 좋습니다. 앞으로 파일 입출력, 프로세스, 신호, IPC, socket, pthread 같은 주제를 배울 때 Linux 환경이 실습의 기준이 됩니다.

## 학기 전체 흐름

| 주차 | 주제 | 내용 |
| --- | --- | --- |
| 1주차 | Introduction | Linux install with VirtualBox, text editor Vim |
| 2주차 | Programming in Linux (1) | Linux 환경에서 compile/debugging |
| 3주차 | Programming in Linux (2) | Git, makefile, shell scripts |
| 4주차 | File I/O | `open`, `read`, `write`, `lseek` |
| 5주차 | Process | process 생성/관리 |
| 6주차 | Signal | signal 보내기/처리하기 |
| 7주차 | IPC | pipe, redirection 등 IPC mechanisms |
| 8주차 | No class | 수업 없음 |
| 9주차 | System V IPC | shared memory, named pipe, message queue 등 |
| 10주차 | Socket | socket을 이용한 basic client/server program |
| 11주차 | Pthread | pthread API 사용 |
| 12주차 | Concurrent Programming | multi-process, multi-thread programming, multiplexing I/O |
| 13주차 | Synchronization | spinlock, mutex, conditional variable |
| 14주차 | Daemon | Linux daemon programming |
| 15주차 | No class | 수업 없음 |
| 16주차 | Final Exam | 기말고사 |

## 1주차 환경 세팅

### 기본 흐름

1. VirtualBox 설치
2. Virtual machine 생성
3. Ubuntu 설치/설정
4. VM 실행
5. Terminal 열기
6. 개발 도구 설치

PDF 기준 추천 Ubuntu는 Noble 계열 다운로드 페이지입니다.

- VirtualBox: <https://www.virtualbox.org/>
- Ubuntu Noble desktop image: <https://releases.ubuntu.com/noble/>

Apple Silicon Mac처럼 ARM 기반 컴퓨터를 쓰는 경우 일반 x86 desktop image를 바로 쓸 수 없습니다. PDF는 두 가지 선택지를 제시합니다.

- Ubuntu Desktop ARM daily-live image: GUI는 있지만 불안정할 수 있음
- Ubuntu Server ARM image: GUI는 없지만 안정적일 수 있음

설치 후 터미널에서 기본 개발 도구를 설치합니다.

```bash
sudo apt update
sudo apt upgrade
sudo apt install build-essential
```

## Linux에서 코딩하기

이번 주 코딩 도구는 Vim입니다. Vi는 UNIX 계열 운영체제의 기본 editor이고, Linux에서는 보통 개선판인 Vim을 씁니다. 처음에는 불편하지만 서버/터미널 환경에서 매우 자주 쓰이므로 익숙해질 가치가 있습니다.

Vim이 없다면 설치합니다.

```bash
sudo apt install vim
```

명령어 예시에서 앞의 `$`는 shell prompt를 뜻하므로 실제로 복사해서 입력하지 않습니다.

## Vim 핵심

Vim은 모드가 있는 editor입니다. 처음 열면 보통 Normal mode입니다. 글자를 바로 치는 일반 메모장식 editor가 아니므로, 입력 모드로 들어가야 텍스트를 작성할 수 있습니다.

| 모드 | 역할 |
| --- | --- |
| Normal mode | 이동, 삭제, 복사, 명령 진입의 기본 모드 |
| Insert mode | 새 텍스트 입력 |
| Visual/Select mode | 텍스트 선택과 조작 |
| Command-line/Ex mode | 저장, 종료, 검색, editor 명령 입력 |

기본 키:

- `i`, `a`, `o`, `s`: Insert mode 진입
- `v`: Visual mode 진입
- `h`, `j`, `k`, `l`: 왼쪽, 아래, 위, 오른쪽 이동
- `:`, `/`: command-line mode 진입
- `Esc`: Normal mode로 복귀

저장/종료:

- `:w`: 저장
- `:w filename`: 이름을 지정해 저장
- `:q`: 종료
- `:q!`: 저장하지 않고 강제 종료
- `:wq`: 저장하고 종료, 이번 실습에서는 이걸 제일 자주 쓰면 됨

Vim 연습 자료:

- Vim Adventures: <http://vim-adventures.com/>
- Open Vim Tutorial: <http://www.openvim.com/tutorial.html>

## 1주차 Lab Exercise 의미

이번 실습의 목적은 C 프로그램 실력 자체보다 아래 흐름을 한 번 연결하는 것입니다.

1. Linux terminal을 연다.
2. Vim으로 파일을 만든다.
3. C source file을 저장한다.
4. Compiler로 빌드한다.
5. 실행 결과를 확인한다.
6. iCampus에 요구 형식대로 증거를 제출한다.

즉 `hello.c`는 앞으로 더 복잡한 system call/process/socket/thread 실습을 하기 위한 최소 환경 점검입니다. 여기서 VM, terminal, Vim, compiler, screenshot 제출 흐름을 확실히 만들어두면 뒤 주차가 훨씬 편해집니다.

## 환경 선택 주의

PDF는 VirtualBox + Ubuntu VM을 기본 흐름으로 설명하지만, 다른 환경도 일부 허용합니다.

- VMware, UTM, native Linux server/computer 사용 가능
- Ubuntu 22.04 이상 권장
- older Ubuntu version은 권장되지 않음
- In-Ui-Ye-Ji cluster 사용은 strongly discouraged
- WSL2와 다른 Linux distribution은 자기 책임으로 사용

특히 WSL2나 다른 distro는 프로그램 동작이 수업 기준 환경과 다를 수 있고, 그 차이로 감점이 생길 수 있다고 안내되어 있습니다. 가능하면 수업에서 권장하는 Ubuntu 환경을 우선으로 맞추는 것이 안전합니다.

## 출석/질문 운영

전자출결을 사용합니다. 성균관대학교 전자출결(SAM) 앱을 쓰거나, 리더기에 Kingo-M QR code 또는 학생증을 사용할 수 있습니다. 출결을 놓치면 출결변경신청을 확인합니다.

질문은 iCampus Q&A Board(문의게시판)를 사용하는 것이 좋습니다. 수업 중 실습 파트에서도 Q&A board를 사용할 수 있고, 같은 질문을 다른 학생들도 볼 수 있으며, 답변을 한국어 등으로 번역해서 보기에도 좋다고 안내되어 있습니다.

Programming Assignment 관련 질문은 늦게 하지 말고 가능한 한 빨리 해야 합니다. 수업은 21:45에 끝나지만, 마감 전 추가 질문은 iCampus나 이메일로 할 수 있습니다.

## 이번 주 공부 순서

1. `week1-course_overview.pdf`에서 평가/출석/과제 마감부터 확인합니다.
2. 본인 노트북에서 사용할 Linux 환경을 정합니다.
3. Ubuntu VM 또는 대체 환경을 설치하고 terminal을 엽니다.
4. `build-essential`과 `vim` 설치를 확인합니다.
5. Vim mode 전환, 저장, 종료를 연습합니다.
6. `hello.c`를 직접 작성하고 컴파일/실행합니다.
7. 실행 결과 화면을 캡처해 `STUDENT ID_w1.png` 형식으로 저장합니다.
8. iCampus 제출처에 제출하고 `assignments.md` 상태를 갱신합니다.

## 헷갈리기 쉬운 점

- `$ sudo apt update`에서 `$`는 입력하는 문자가 아니라 terminal prompt 표시입니다.
- Vim은 처음부터 글자가 입력되는 editor가 아닙니다. `i`로 Insert mode에 들어가고, `Esc`로 Normal mode에 돌아옵니다.
- 실습 제출물은 코드 파일이 아니라 스크린샷입니다. 다만 실제 코드는 반드시 본인 환경에서 직접 작성/실행해야 합니다.
- 일반 실습 마감은 화요일 23:59이지만, 1주차 과제는 `Next week (9/8 23:59)`로 별도 안내되어 있습니다.
- PDF 강의실 표기와 수강신청표 강의실이 다를 수 있으므로 실제 강의실은 당일 iCampus/첫 수업 안내를 우선합니다.

## 다음에 업데이트할 것

- 실제 iCampus 제출처 링크/마감이 보이면 assignments.md에 반영
- 1주차 수업 중 교수님/TA가 말한 추가 주의사항 반영
- 2주차 compile/debugging 자료가 올라오면 2주차 폴더 생성
