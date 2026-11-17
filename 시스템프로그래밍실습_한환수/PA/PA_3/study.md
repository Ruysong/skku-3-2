# PA3 — 다중 클라이언트 좌석 예약 서버

확인: **2026-11-16 iCampus API·LearningX 학생 공개 자료**. 이번 작업은 원본 보관·목차·일정 정리이며 코드 구현이나 제출을 수행하지 않았다.

## 받은 파일

- [PA3.pdf](./PA3.pdf): 41쪽 명세.
- [pa3_skeleton_code.tar.zst](./pa3_skeleton_code.tar.zst): 과제 제출 페이지에 제공된 스켈레톤 원본.
- [pa3_example1.tar.zst](./pa3_example1.tar.zst): 같은 페이지의 공개 예제 원본.
- 원본 크기·SHA-256·다운로드 상태는 [12주차 manifest](../../12주차/source-manifest.json)에 기록했다. 압축본을 실행하거나 제출 코드에 덮어쓰지 않았다.

## 마감과 제출

- 정규 마감: **2026-12-06(일) 23:59:59 KST**.
- 제출창 잠금: 12/10 23:59:59. 정규 마감 연장이 아니다.
- 제출처: [PA3 submit](https://canvas.skku.edu/courses/75630/assignments/2068885).
- 소스·Makefile이 들어 있는 `pa3/` 디렉터리를 **2024315329_pa3.tar.zst**로 묶는다.
- 설계·구현 보고서는 **2024315329_pa3.pdf**, 국문·영문 가능.
- 압축 해제 → `cd pa3` → `make`가 **pa3_server, pa3_client**를 생성해야 한다.
- 최신 제출본을 채점하며 그 제출 시각이 기준이다. 명세는 총100점, 하루10점·최대4일 지각 감점을 안내한다.
- 명세 p.3은 개인 과제이며 논의는 가능하지만 코드는 본인이 작성해야 한다고 명시한다. AI 사용을 포괄적으로 허용한다고 바꾸어 해석하지 않는다.

## 명세 목차

| 페이지 | 내용 |
| --- | --- |
| 2~5 | 목표·환경·제약·제출물·빌드 이름 |
| 6~8 | 사용 가능한 CPU 코어 수, 사전 설치 패키지 |
| 9~12 | User·Server·Client: 사용자·접속 클라이언트 구분, 100좌석, 파일/대화형 모드 |
| 13~15 | 전송 구조·길이 정보가 있는 데이터 전송·동기화 |
| 17~19 | 7개 action ID와 response code·출력 원칙 |
| 20~21 | Termination |
| 22~27 | Login·등록·비밀번호 해시·오류 |
| 28~29 | Book |
| 30~31 | Confirm booking |
| 32~33 | Cancel booking |
| 34~35 | Logout |
| 36~37 | Query |
| 39~41 | 단일·다중 클라이언트 예제와 stdout/stderr 검증 |

## 전체 구조

```text
client: 사용자 명령 -> request -> TCP -> server
                                          |
                                    새 연결 수락
                                          |
                              client 수가 가장 적은 worker
                                          |
                           각 worker가 자기 client들을 poll
                                          |
                           User·Users·Seat 공유 상태 접근
                                          |
                            mutex 보호 -> response 전송
```

명세 p.10은 NUM_CORES개의 스레드를 사용하고 각 스레드가 여러 클라이언트를 poll하도록 한다. 새 연결은 클라이언트 수가 가장 적은 스레드에 배정한다. 단순히 연결당 새 스레드 하나를 만드는 구조로 바꾸면 이 요구와 다르다.

p.15는 좌석 읽기·수정을 mutex로 보호하고 User·Users도 동기화하도록 안내한다. user-array → user → seat처럼 락 획득 순서를 일관되게 두며, **poll set의 mutex를 잡은 채 socket read/write를 하지 않는다**. 이때의 poll·클라이언트 입출력은 [12주차 채팅방 실습](../../12주차/study.md), mutex·condvar는 운체 동시성 단원과 연결된다.

## 다음 분석에서 확인할 부분

지금은 핵심 제약·제출 페이지·목차를 확인했다. 각 action의 정확한 wire format, response code, 오류 문구, 사용자 로그인 상태 전이, 스켈레톤 함수별 책임과 테스트 전체를 아직 완전 분석한 상태는 아니다. 본격 구현 전에 명세와 스켈레톤을 함께 읽는다. 예시를 본 사실과 모든 채점 조건을 검증한 사실을 구분한다.
