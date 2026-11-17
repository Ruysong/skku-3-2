# 시스템프로그래밍실습 — Linux 사용자 공간 시스템 프로그래밍

C로 파일 처리, 프로세스·시그널, IPC, TCP 소켓, POSIX thread를 사용하는 프로그램을 작성했다. 파일 검색기와 셸 과제를 수행하고 주차별 실습 소스·Makefile을 보관했다.

## 대표 구현과 읽을 순서

| 경험 | 핵심 기술 | 관련 파일 |
|---|---|---|
| PA1 파일 검색기 | Unix I/O, 동적 line buffer, 네 질의, 출력 버퍼 | [구현 설명](./PA/PA_1/study.md) → [제출 소스](./PA/PA_1/submitted/)·manifest |
| 2~7주 실습 | GCC/GDB·Makefile, fork/exec/wait, signal, pipe·redirection | [실습 구현 설명](./lab-study.md), 각 주차 `submitted/` |
| 9주 메시지 큐 | System V queue, 채팅·ACK | [작업 코드](./VMShare/week9/w9.c), [주차 정리](./9주차/study.md) |
| 10주 TCP 파일 전송 | client/server, 메시지·파일 I/O | [작업 소스](./10주차/SPLweek10Practice/), [교안·요구사항](./10주차/study.md) |
| 11주 pthreads | 행렬–벡터 곱, 작업 분할, create/join | [제출 소스](./11주차/submitted/)·[해시 근거](./11주차/submitted/source-manifest.json) |
| PA2 shell | 명령 실행·pipe·redirection·signal·파일 도구를 다루는 셸 과제 수행 | [과제 목차·제공 코드](./pa-study.md) |
| PA3 예약 서버 | worker·poll·mutex | [명세·스켈레톤](./PA/PA_3/study.md) · 진행할 과제 |

자소서·면접 연결은 [S1/S2 경험 카드](../경험-근거지도.md#s1s2--linux-파일-처리프로세스ipc네트워크스레드), 전체 버전 선택은 [pa-study.md](./pa-study.md)를 읽는다. 파일별 책임과 입출력 흐름은 상세 구현 문서에 정리했다.

## 수업 자료 찾기

[1주차](./1주차/study.md) · [2주차](./2주차/study.md) · [3주차](./3주차/study.md) · [4주차](./4주차/study.md) · [5주차](./5주차/study.md) · [6주차](./6주차/study.md) · [7주차](./7주차/study.md) · [9주차](./9주차/study.md) · [10주차](./10주차/study.md) · [11주차](./11주차/study.md) · [12주차](./12주차/study.md)

- [공지 원문](./notices.md) · [과제·퀴즈·제출 기록](./assignments.md) · [전체 일정](../tasks.md)
- [전체 경험·근거 지도](../경험-근거지도.md) · [저장소 홈](../README.md)

정리 기준: 2026-11-17. 각 프로젝트의 코드·보고서·학습 자료를 위 링크에서 볼 수 있다.
