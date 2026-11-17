# 시스템프로그래밍실습 — Linux 사용자 공간 시스템 프로그래밍

C로 파일 descriptor, 프로세스·시그널, IPC, TCP 소켓, POSIX thread를 다루는 실습과 PA를 보관한다. **제출 압축본에서 확인한 코드**, VMShare 작업본, 교수 제공 예제·스켈레톤을 구분해서 읽는다.

## 대표 구현과 읽을 순서

| 경험 | 핵심 기술 | 근거·확인 수준 |
|---|---|---|
| PA1 파일 검색기 | Unix I/O, 동적 line buffer, 네 질의, 출력 버퍼 | [구현 설명](./PA/PA_1/study.md) → [제출 소스](./PA/PA_1/submitted/)·manifest |
| 2~7주 실습 | GCC/GDB·Makefile, fork/exec/wait, signal, pipe·redirection | [실습 구현 설명](./lab-study.md), 각 주차 `submitted/` |
| 9주 메시지 큐 | System V queue, 채팅·ACK | [작업 코드](./VMShare/week9/w9.c), [주차 정리](./9주차/study.md) |
| 10주 TCP 파일 전송 | client/server, 메시지·파일 I/O | [작업 소스](./10주차/SPLweek10Practice/), [교안·요구사항](./10주차/study.md). 제출본과의 동일성은 별도 |
| 11주 pthreads | 행렬–벡터 곱, 작업 분할, create/join | [제출 소스](./11주차/submitted/)·[해시 근거](./11주차/submitted/source-manifest.json) |
| PA2 shell | 제공 lexer/parser·작업 관리와 구현 과제 | [PA2 구분](./pa-study.md#pa2-스켈레톤-확인-2026-10-28). 현재 보관본은 TODO가 있는 스켈레톤 |
| PA3 예약 서버 | worker·poll·mutex | [명세·스켈레톤](./PA/PA_3/study.md). 구현 완료로 분류하지 않음 |

자소서·면접 연결은 [S1/S2 경험 카드](../경험-근거지도.md#s1s2--linux-파일-처리프로세스ipc네트워크스레드), 전체 버전 선택은 [pa-study.md](./pa-study.md)를 읽는다. PA1의 단어 경계 처리 제한도 구현 설명에 남겼다.

## 수업 자료 찾기

[1주차](./1주차/study.md) · [2주차](./2주차/study.md) · [3주차](./3주차/study.md) · [4주차](./4주차/study.md) · [5주차](./5주차/study.md) · [6주차](./6주차/study.md) · [7주차](./7주차/study.md) · [9주차](./9주차/study.md) · [10주차](./10주차/study.md) · [11주차](./11주차/study.md) · [12주차](./12주차/study.md)

- [공지 원문](./notices.md) · [과제·퀴즈·제출 기록](./assignments.md) · [전체 일정](../tasks.md)
- [전체 경험·근거 지도](../경험-근거지도.md) · [저장소 홈](../README.md)

최신 정리: 2026-11-17. 소스·보고서의 보관 상태와 수강·개인 학습 완료는 별도로 관리한다.
