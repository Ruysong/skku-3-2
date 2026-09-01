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

## 페이지별 흐름 정리

이 아래 내용은 `week1-course_overview.pdf` 36쪽 전체를 페이지 흐름 기준으로 다시 읽어 정리한 것입니다. 전문 OCR 검수본은 아니고, 시험/실습 관리에 필요한 내용을 빠뜨리지 않기 위한 학습지도입니다.

| 페이지 | 내용 | 공부할 때 볼 포인트 |
| --- | --- | --- |
| 1 | SWE2024 System Programming Lab 표지 | 과목이 이론 강의보다 실습 중심이라는 점 |
| 2 | Course Information | 시간, 강의실, TA, iCampus 사용처 |
| 3 | Course Outline (1) 구조 그림 | user space, system call interface, kernel space, hardware의 층위 |
| 4 | Course Outline (2) | Linux를 쓰는 이유와 Linux 환경 적응의 중요성 |
| 5 | Course Outline (3) | Linux command, system call, network, concurrent programming으로 이어지는 전체 범위 |
| 6 | Reference | CS:APP 3판이 기준 교재/참고서 |
| 7 | Grade | 출석 10%, Lab 15%, PA 45%, Final 30%, 결석 기준 |
| 8 | Academic Misconduct | 표절/부정행위 기준과 허용되는 도움의 범위 |
| 9 | Course Schedule (1) | 1~8주차: Linux 기초, file I/O, process, signal, IPC |
| 10 | Course Schedule (2) | 9~16주차: System V IPC, socket, pthread, synchronization, daemon, final |
| 11 | Ubuntu VM 세팅 표지 | 1주차 실습 환경 준비의 시작 |
| 12 | Steps | VirtualBox 설치, VM 생성, VM 세팅 |
| 13 | Installing VirtualBox (1) | VirtualBox 웹사이트에서 설치 파일 다운로드 |
| 14 | Installing VirtualBox (2) | 설치 후 재시작이 필요할 수 있음 |
| 15 | Downloading Ubuntu | Ubuntu Noble desktop image 다운로드 |
| 16 | Downloading Ubuntu (M1~M4) | ARM 기반 Mac은 별도 이미지 필요, 설치 후 build-essential 필요 |
| 17~19 | Creating a VM | VM 생성 화면 안내 |
| 20 | Creating a VM (5) | Ubuntu 자동 설치, 설치 실패 시 재시도/graphics controller 변경 |
| 21~22 | Starting a VM | 설치 후 VM 실행 화면 |
| 23 | Starting a VM (3) | 터미널 실행 후 `apt update/upgrade/install build-essential` |
| 24 | Coding in Linux 표지 | Linux에서 코딩하는 단계로 전환 |
| 25 | Text editor - Vim (1) | Vi/Vim의 의미, Vim 설치 명령 |
| 26 | Text editor - Vim (2) | Vim의 mode 구조 |
| 27 | Text editor - Vim (3) | insert/visual/movement/command/Esc 기본키 |
| 28 | Text editor - Vim (4) | `h`, `j`, `k`, `l` 이동키 |
| 29 | Text editor - Vim (5) | `:w`, `:q`, `:q!`, `:wq` 저장/종료 |
| 30~31 | Text editor - Vim (6~7) | Vim 실습 화면/예시 이미지 성격 |
| 32 | Text editor - Vim (8) | Vim Adventures, Open Vim tutorial, 반복 연습 |
| 33 | Lab Exercise | `hello.c`, `Hello, Linux!`, 스크린샷 제출, `STUDENT ID_w1.png`, 2026-09-08 23:59 |
| 34 | Precautions | VM/Ubuntu/WSL2/인의예지 클러스터 관련 주의 |
| 35 | Final remarks | 전자출결, 실습 마감, Q&A board, PA 질문 시점, 수업 종료시간 |
| 36 | Any Questions? | 첫 수업 질의응답 |

## 운영 파트 자세히 보기

2쪽의 Course Information은 이 과목을 관리할 때 제일 먼저 봐야 하는 페이지입니다. 수업 시간은 화요일 18:00-21:45로 잡혀 있고, PDF에는 강의실이 `#85702`로 적혀 있습니다. 다만 이전 수강신청표에는 다른 강의실 정보가 있었으므로, 실제 출석할 장소는 iCampus 최신 정보와 첫 수업 안내를 우선해야 합니다. 이 부분은 나중에 헷갈리면 바로 결석/지각 리스크로 이어질 수 있어서 `tasks.md`에도 재확인 항목으로 남겨둡니다.

TA는 두 명입니다. Gwangbin Rhee와 Jinseok Park가 안내되어 있고, 이메일도 함께 들어 있습니다. 보통 이런 실습 과목은 교수님 설명보다 TA 안내가 제출 형식, 실습 서버/VM 문제, 컴파일 환경, 채점 스크립트에서 더 직접적으로 중요해질 수 있습니다. 따라서 PA가 열리면 교수님 공지뿐 아니라 TA가 Q&A board나 iCampus에 남기는 답변을 같이 추적해야 합니다.

이 과목에서 iCampus는 단순 강의자료 저장소가 아닙니다. 공지, 강의콘텐츠, 주간 Lab assignment 제출, Programming assignment 제출, Q&A board가 모두 iCampus로 연결됩니다. 즉 시프실은 매주 화요일 수업만 듣고 끝나는 과목이 아니라, 수업 후 iCampus 제출처를 확인하고 당일 또는 지정 마감 전까지 결과물을 올리는 루틴이 중요합니다.

7쪽의 Grade 페이지는 이번 학기 리스크를 정리하는 기준입니다. 출석은 10%, Lab exercise는 15%, Programming assignment는 총 45%, Final exam은 30%입니다. PA는 세 번이고 배점이 10%, 15%, 20%로 뒤로 갈수록 커집니다. 처음 PA 하나를 가볍게 넘겼다가 뒤 PA에 필요한 환경/코드 구조를 놓치면 손해가 커질 수 있습니다.

출석 정책도 꽤 빡빡합니다. 최대 2번의 결석은 허용되지만, 그 이후부터는 최종 성적에 영향을 줍니다. 4번 결석하면 자동 F입니다. 이 말은 단순히 “출석 10점 중 일부 감점”이 아니라 일정 횟수를 넘으면 과목 자체가 위험해진다는 뜻입니다. 화요일 저녁 긴 수업이라 피곤할 수 있지만, 출석 확인은 매주 확실히 해둬야 합니다.

8쪽의 Academic Misconduct는 앞으로 PA를 할 때 기준선이 됩니다. 다른 사람의 작업을 제출하거나, 인터넷에서 가져온 작업을 자기 과제로 제출하거나, 자기 작업을 다른 사람이 제출하게 허용하는 것은 cheating/plagiarism으로 들어갑니다. 반대로 시스템이나 도구 사용법을 도와주는 것, high-level design issue를 토론하는 것, 디버깅을 도와주는 것은 cheating이 아니라고 되어 있습니다. 그래서 이 과목에서는 “아이디어 수준의 설명/환경 도움/디버깅 방향”과 “제출할 코드 자체 공유”를 구분해야 합니다.

## 학기 흐름을 왜 미리 알아야 하나

시프실은 주차가 뒤로 갈수록 앞 주차 도구를 계속 재사용하는 형태입니다. 1주차에 Linux와 Vim을 잡고, 2주차에 컴파일/디버깅, 3주차에 Git/makefile/shell script를 잡습니다. 이 셋은 뒤 주차 실습의 기반입니다. 4주차부터는 system call로 들어가며, file I/O, process, signal, IPC, socket, pthread, synchronization, daemon이 순서대로 이어집니다.

이 흐름은 운영체제 수업과도 잘 맞물립니다. 운영체제에서 process, thread, synchronization, IPC를 이론적으로 배우면, 시프실에서는 Linux API와 C 코드로 실제 호출해보는 식이 됩니다. 그래서 네가 말한 “시프실이 운체 확장판 같다”는 감이 맞습니다. 운영체제 개념을 외우는 데서 끝내지 않고, Linux user program이 kernel 기능을 어떤 인터페이스로 요청하는지 직접 확인하는 수업입니다.

4주차 File I/O에서 `open`, `read`, `write`, `lseek`가 나오면 C 표준 라이브러리의 `fopen`, `fread` 같은 함수보다 낮은 수준의 파일 접근을 보게 됩니다. 5주차 Process에서는 `fork`나 process 생성/관리 개념이 연결될 가능성이 높고, 6주차 Signal에서는 비동기 이벤트 처리 감각이 중요해질 수 있습니다. 7주차와 9주차 IPC는 process끼리 정보를 주고받는 방식으로 넘어가고, 10주차 Socket은 client/server 프로그램으로 이어집니다. 11~13주차는 pthread, multi-process/multi-thread, synchronization이므로 기말과 PA 후반의 핵심이 될 가능성이 큽니다.

## Ubuntu/VM 세팅에서 놓치면 안 되는 점

11~23쪽은 Ubuntu VM 설치 흐름입니다. 이 부분은 시험 개념보다 실습 생존에 가깝습니다. 환경이 안 되면 `hello.c`도 못 내고, 뒤 PA도 계속 밀릴 수 있습니다. PDF는 VirtualBox 설치, VM 생성, Ubuntu 설치, VM 실행, 터미널 실행, 기본 개발도구 설치 순서로 안내합니다.

VirtualBox는 <https://www.virtualbox.org/>에서 설치 파일을 받고, 설치 과정은 특별한 커스터마이징보다 기본 설치 흐름을 따르는 식으로 안내되어 있습니다. 설치 후 재시작이 필요할 수 있다는 점도 적혀 있습니다. Ubuntu는 <https://releases.ubuntu.com/noble/>에서 desktop image를 받는 흐름입니다. 파일 크기가 6.2GB 정도로 표시되어 있으므로 다운로드 시간이 걸릴 수 있습니다.

ARM 기반 컴퓨터, 특히 Apple Silicon Mac을 쓰는 경우가 따로 언급되어 있습니다. 일반 Ubuntu desktop image를 그대로 쓰기 어렵기 때문에 ARM용 desktop daily-live image나 Ubuntu Server ARM image 중 선택해야 합니다. Desktop은 GUI가 있어 편하지만 불안정할 수 있고, Server는 GUI가 없지만 안정적일 수 있습니다. PDF는 ARM 환경에서 설치 후 `sudo apt update`, `sudo apt upgrade`, `sudo apt install build-essential`을 실행해야 한다고 안내합니다.

VM 설치 실패에 대한 안내도 있습니다. Ubuntu가 자동 설치되지만 몇 분 걸릴 수 있고, 가끔 실패할 수 있습니다. 이때는 다시 설치를 시도하거나, `vmwgfx seems to be running on an unsupported hypervisor` 문제가 나오면 graphics controller를 `VBoxVGA` 또는 `VBoxSVGA`로 바꿔보라고 되어 있습니다. 이건 나중에 설치가 안 될 때 바로 검색할 키워드로 남겨둘 만합니다.

23쪽에서는 VM 테스트 명령이 나옵니다. `Ctrl + Alt + T`로 terminal을 열고, `sudo apt update`, `sudo apt upgrade`, `sudo apt install build-essential`을 실행합니다. `build-essential`은 C/C++ 컴파일에 필요한 기본 도구 묶음이라고 보면 됩니다. 이게 없으면 `gcc` 같은 컴파일러가 없어서 C 파일을 만들어도 실행 파일을 만들 수 없습니다.

## Vim을 왜 첫 주부터 시키는가

24~32쪽은 Linux에서 코딩하기, 특히 Vim입니다. 처음 보면 “왜 굳이 불편한 editor를 쓰지?” 싶을 수 있지만, 시스템프로그래밍 수업에서는 terminal 기반 환경을 자주 만나게 됩니다. GUI editor가 없는 서버나 VM 콘솔에서 바로 파일을 열어 수정해야 할 수 있고, 그런 상황에서 Vim은 거의 기본 도구처럼 등장합니다.

25쪽은 Vi와 Vim의 관계를 설명합니다. Vi는 UNIX 운영체제의 기본 editor이고, Linux의 Vi는 보통 Vim, 즉 Vi Improved인 경우가 많습니다. PDF는 Vim이 배우기 어려울 수 있지만 유용하다고 말합니다. 설치 명령은 `sudo apt install vim`입니다. 여기서도 `$` 기호는 입력하는 문자가 아니라 command-line prompt라는 주의가 붙어 있습니다.

26쪽은 Vim의 모드 구조입니다. Normal mode, Insert mode, Visual/select mode, Command-line/Ex mode가 나옵니다. 일반적인 editor와 달리 Vim은 “지금 내가 글을 입력하는 모드인지, 명령을 내리는 모드인지”가 중요합니다. `i`를 누르지 않고 그냥 글자를 치면 입력되는 게 아니라 명령으로 해석될 수 있습니다.

27쪽은 기본 interface입니다. `i`, `a`, `o`, `s`는 Insert mode로 들어가는 키입니다. `v`는 Visual mode, `h`, `j`, `k`, `l` 또는 cursor key는 이동, `:`와 `/`는 command mode, `Esc`는 normal mode로 돌아가는 키입니다. 이번 `hello.c` 과제에서 최소한 필요한 흐름은 파일 열기, `i`로 입력 모드 진입, 내용 작성, `Esc`, `:wq`로 저장하고 종료입니다.

28쪽은 `h`, `j`, `k`, `l` 이동키를 그림으로 보여줍니다. 왼쪽이 `h`, 아래가 `j`, 위가 `k`, 오른쪽이 `l`입니다. 처음부터 외울 필요는 없지만, Vim을 계속 쓰다 보면 마우스나 방향키보다 이 이동이 자연스러워지는 구조입니다.

29쪽은 저장과 종료입니다. `:w`는 저장, `:q`는 종료, `:q!`는 변경사항을 버리고 강제 종료, `:wq`는 저장 후 종료입니다. 첫 주 실습에서 가장 중요한 것은 `:wq`입니다. Vim을 처음 쓰면 빠져나오는 방법에서 막히는 경우가 많아서, 이 페이지는 실제 실습 전에 한 번 손으로 따라 해보는 게 좋습니다.

32쪽은 Vim 연습 자료를 제시합니다. Vim Adventures와 Open Vim Tutorial이 있습니다. 이건 필수 제출물은 아니지만, Vim 조작이 너무 낯설면 10~20분만 해도 실습 스트레스가 줄어듭니다. PDF의 표현처럼 반복 연습이 핵심입니다.

## 1주차 Lab Exercise를 실제로 해석하면

33쪽 Lab Exercise는 이번 주 가장 직접적인 제출물입니다. 요구사항은 Vim으로 `hello.c`를 작성하고, `hello.c` 프로그램이 `Hello, Linux!`를 출력하게 하는 것입니다. 제출은 코드 파일이 아니라 스크린샷만 iCampus에 제출합니다. 파일명은 `STUDENT ID_w1.png`이고, 마감은 다음 주 2026-09-08 23:59입니다.

이 과제의 의도는 “C 문법 문제를 풀어라”보다는 “수업 기준 환경이 작동하는지 증명하라”에 가깝습니다. VM/Ubuntu가 준비되어 있고, terminal을 열 수 있고, Vim으로 파일을 만들 수 있고, C 컴파일러가 있고, 프로그램을 실행할 수 있다는 것을 한 번 확인하는 과제입니다. 그래서 스크린샷에는 작성한 코드 또는 실행 명령과 출력 결과가 분명히 보이게 찍는 것이 안전합니다.

제출 파일명이 중요합니다. PDF에는 `STUDENT ID_w1.png`라고 되어 있으므로, 실제로는 본인 학번을 넣어 `학번_w1.png` 형태로 저장하면 됩니다. 공백이나 한글 이름을 임의로 붙이지 않는 것이 좋습니다. iCampus 제출처에서 별도 지시가 있으면 그 지시가 우선입니다.

34쪽 Precautions는 환경 선택 리스크입니다. VirtualBox가 기본 안내지만 다른 virtual machine monitor, 예를 들어 VMware, UTM 등을 쓰거나 native Linux server/computer를 사용하는 것은 허용됩니다. 하지만 In-Ui-Ye-Ji cluster는 strongly discouraged라고 되어 있습니다. Ubuntu는 22.04 이상 사용 가능하고, 오래된 버전은 권장되지 않습니다.

WSL2와 다른 distro는 자기 책임입니다. PDF는 이런 환경에서 컴파일된 프로그램이 다르게 동작할 수 있고, 그로 인해 감점이 생길 수 있다고 안내합니다. 즉 “내 컴퓨터에서는 됐는데 채점 환경에서는 다르게 동작함” 문제가 생길 수 있습니다. 수업에서 권장하는 Ubuntu 환경에 맞추는 것이 가장 안전합니다.

35쪽 Final remarks는 실무적인 운영 메모입니다. 전자출결을 사용하고, SAM 앱 또는 리더기 QR/학생증으로 출석을 처리합니다. 출결을 깜빡하면 출결변경신청을 할 수 있습니다. 실습 마감은 일반적으로 같은 날 자정, 즉 화요일 23:59입니다. 질문은 Q&A Board를 쓰라고 안내합니다. 특히 PA 질문은 일찍 하라고 되어 있으니, PA가 열리면 마감 직전보다 초반에 막히는 지점을 빨리 올리는 게 좋습니다.

## 1주차 체크 질문

- 이 과목에서 iCampus는 어떤 용도로 쓰이는가?
- 출석, Lab exercise, PA, Final exam의 비율은 각각 얼마인가?
- 결석은 몇 회까지 허용되고, 몇 회부터 자동 F인가?
- cheating으로 보는 행동과 cheating이 아닌 도움의 경계는 무엇인가?
- user space와 kernel space 사이에는 어떤 인터페이스가 있는가?
- Linux를 이 과목의 실습 환경으로 쓰는 이유는 무엇인가?
- 1주차 Lab Exercise에서 제출해야 하는 것은 코드 파일인가, 스크린샷인가?
- `STUDENT ID_w1.png`에서 실제로 바꿔야 하는 부분은 무엇인가?
- Vim에서 입력을 시작하려면 어떤 mode로 들어가야 하는가?
- Vim에서 저장하고 종료하려면 어떤 명령을 쓰는가?
- `$ sudo apt update` 예시에서 `$`를 같이 입력하면 안 되는 이유는 무엇인가?
- WSL2나 다른 Linux distro를 사용할 때 어떤 리스크가 있는가?

## 오늘 수업에서 추가로 확인할 것

- PDF 표기 강의실 `#85702`가 실제 수업 장소와 맞는지
- 1주차 Lab Exercise 제출처가 iCampus 어디에 열리는지
- 스크린샷에 어떤 화면이 보여야 하는지
- `hello.c` 코드 파일 자체 제출이 정말 필요 없는지
- WSL2 사용을 실제로 허용하는 분위기인지, 감점 리스크가 어느 정도인지
- PA에서 사용할 기준 Ubuntu 버전이나 채점 환경이 따로 있는지
- TA가 권장하는 질문 방식이 Q&A board인지 이메일인지
- 매주 Lab Exercise 마감이 정말 화요일 23:59로 고정인지
## 다음에 업데이트할 것

- 실제 iCampus 제출처 링크/마감이 보이면 assignments.md에 반영
- 1주차 수업 중 교수님/TA가 말한 추가 주의사항 반영
- 2주차 compile/debugging 자료가 올라오면 2주차 폴더 생성
