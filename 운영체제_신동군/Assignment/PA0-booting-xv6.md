# PA0 - Booting xv6 Operating System

## 과제 정보

- 기간: 2026-09-17 ~ 2026-09-23
- 마감: **2026-09-23 (수) 23:59:59**
- 배점: 10점
- 형태: 개인 과제
- 원본: [Project 0_26f.pdf](./Project%200_26f.pdf)

## 해야 하는 일

UI 서버에서 MIT의 Unix-like teaching operating system인 xv6-public을 clone하고 boot한다. xv6 boot message에 본인 학번, 이름, 선택 메시지를 출력하도록 수정한다.

핵심은 큰 기능을 새로 구현하는 일이 아니다. xv6 소스에서 boot message가 출력되는 위치를 찾고, UI 서버에서 빌드·실행·제출 흐름을 익히는 입문 과제다.

## 환경과 실행

개발은 UI 서버 기준이다. 다른 환경에서 생기는 문제는 본인 책임으로 안내되어 있다.

    ssh s<학번 뒤 8자리>@swui.skku.edu -p 1398
    git clone https://github.com/mit-pdos/xv6-public.git
    cd xv6-public

xv6 종료: Ctrl-a 뒤 x.

## 실행 전 필수 수정

UI 서버 컴파일러 업데이트로 기본 make qemu-nox가 실행되지 않을 수 있다. clone 직후, 실습·수정 전 Makefile의 두 곳을 수정한다.

    74행: gcc -> gcc-11
    160행: gcc -> gcc-11

수정 후 다음으로 boot한다.

    make qemu-nox

## 소스 탐색

boot message 위치를 바로 추측하지 말고 문자열을 먼저 검색한다.

    grep -nR "문자열" .

교안은 grep, ctags, cscope를 소스 분석 도구로 소개한다. PA0는 grep -nR로 시작 위치를 찾으면 충분하다. 수정 뒤에는 boot 화면에서 학번·이름·메시지가 원하는 순서와 형식으로 출력되는지 확인한다.

## 제출 절차

PDF에 적힌 예전 명령은 사용하지 않는다. 9/17 공지의 최종 명령은 아래다.

    make clean
    ~swe3004_41/bin/submit pa0 xv6-public

- UI 서버에서 실행.
- xv6-public의 올바른 디렉터리에서 제출.
- 제출은 여러 번 가능.
- **마지막 제출본만 채점**.
- 빈 파일이나 잘못된 디렉터리 제출이 가능하므로 성공 여부를 반드시 확인.

## 채점과 기한

- 마감: 9/23 23:59:59
- 지각: 하루당 25% 감점
- 복사 금지: 전체 제출물 검사 프로그램을 실행하며, 양쪽 모두 0점·음수 점수·F 등의 제재가 가능하다고 명시됨.

## 지금 할 일

1. UI 서버 접속과 VPN 필요 여부 확인.
2. xv6-public clone.
3. Makefile gcc 두 곳을 gcc-11로 수정.
4. make qemu-nox로 boot 성공 확인.
5. 문자열 검색으로 boot message 위치 파악.
6. 학번·이름 출력 구현 및 boot 화면 확인.
7. make clean 후 새 제출 명령으로 제출.