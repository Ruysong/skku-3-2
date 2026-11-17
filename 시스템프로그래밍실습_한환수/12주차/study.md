# 시스템프로그래밍실습 12주차

<!-- calendar-20261116:start -->
## 캘린더 교차 확인

Canvas 캘린더에도 **11/17(화) 18:00~19:00, week12, 화상 강의**가 등록되어 있다. 이 항목의 종료 시각으로 평소 전체 실습 수업 시간이1시간으로 줄었다고 단정하지 않는다. 주간 과제 마감은23:59:59다.
<!-- calendar-20261116:end -->

기간: **2026-11-16(월)~11-22(일)** · API 확인: **2026-11-16 10:31 KST 전후**.

이번 주 주제: **Concurrent Server·poll 채팅방**

## 바로가기·정리 범위

- [과목 홈](../README.md) · [공지](../notices.md) · [과제·퀴즈](../assignments.md) · [원본·조회 manifest](./source-manifest.json)

이번 작업은 **주차 세팅·원본 다운로드·목차·학습 연결·마감 및 제출 상태 정리**다. 공개 PDF의 텍스트와 핵심 그림·요구사항을 대조했으며, 모든 페이지를 전문 교안화하거나 영상 전체를 수강·대본 검수한 것은 아니다. 따라서 이 study는 1만자보다 짧은 학습지도이고 수강·학습 완료를 대신 표시하지 않는다.

## 받은 원본

| 파일 | 위치 | 상태 |
| --- | --- | --- |
| PA3.pdf | [원본](../PA/PA_3/PA3.pdf) | 다운로드·SHA-256 기록 |
| week11_code.zip | [원본](../11%EC%A3%BC%EC%B0%A8/week11_code.zip) | 동일 원본 기존 보관 확인 |
| week12_concurrent_programming.pdf | [원본](week12_concurrent_programming.pdf) | 동일 원본 기존 보관 확인 |
| pa3_example1.tar.zst | [원본](../PA/PA_3/pa3_example1.tar.zst) | 다운로드·SHA-256 기록 |
| pa3_skeleton_code.tar.zst | [원본](../PA/PA_3/pa3_skeleton_code.tar.zst) | 다운로드·SHA-256 기록 |

## 이번 주 목차: Concurrent Programming

```text
순차 서버: 한 연결을 처리하는 동안 다른 연결이 기다림
    |
    +-- fork 서버 ------ 프로세스별 처리 / child 회수·fd 닫기
    +-- pthread 서버 --- 연결별 스레드 / 인자 수명·join/detach
    +-- thread pool ---- worker 재사용 / 큐·mutex·condvar
    +-- I/O multiplex -- select·poll / 준비된 fd만 처리
                                      |
                               poll 채팅방 실습
```

### week12_concurrent_programming.pdf — 34쪽

| 페이지 | 목차 | 핵심 질문 |
| --- | --- | --- |
| 2~5 | Iterative server와 concurrent flow | 한 클라이언트가 멈추면 다른 연결은 어떻게 되는가? |
| 6~9 | Multi-process server | 부모·자식 중 누가 listenfd/connfd를 닫고 누가 wait하는가? |
| 10~14 | Multi-thread server | 반복문 지역 변수 connfd의 주소를 넘기면 왜 경합하는가? |
| 15~18 | Thread pool | 공유 큐를 언제 잠그고, condvar를 왜 while로 기다리는가? |
| 19~24 | Event-driven와 select | fd_set 복사, 최대 fd+1, 연결 수락·종료 처리 |
| 25~29 | poll | pollfd 배열, events/revents, POLLIN·POLLOUT·POLLERR·POLLHUP |
| 30~31 | 신호 마스크·설계 비교 | pselect/ppoll 계열과 process/thread/event 모델의 비용 |
| 32~34 | 채팅방 실습·제출 | 서버와 클라이언트 모두 poll 사용 |

### 실습 요구사항

- username을 입력하고 채팅방에 접속한다.
- 클라이언트는 `/quit` 조건으로 빠져나올 수 있는 반복 입력 구조를 갖춘다.
- 서버는 한 클라이언트 메시지를 **다른 클라이언트들**에 전달한다.
- 입장·퇴장을 다른 클라이언트에게 알린다.
- 클라이언트는 **stdin과 서버 소켓을 함께 감시**하고 서버는 listen socket과 연결된 client socket들을 감시한다.
- `revents & POLLIN`으로 준비 이벤트를 확인한다. 타임아웃은 필수가 아니며 예시처럼 `poll(...,-1)`로 대기할 수 있다.
- 실행 예시의 `*`는 실제 별표 문자다. 암호 입력·인증을 만드는 과제가 아니다.

```text
client                              server
 stdin ----+                   listenfd -> accept -> fd 목록에 추가
           +-> poll -> send            |
 socket ---+                   clients -> poll -> recv
   |                                  -> 다른 client에 send
   +-> 받은 메시지 출력
 /quit -> 종료                  연결 종료 -> fd 제거·퇴장 안내
```

이 구조도는 기능 이해용이며 아직 공개되지 않은 skeleton의 함수명·자료구조를 확정한 구현 설계는 아니다. TCP는 메시지 경계를 보존하지 않으므로 실제 코드에서는 제공 프로토콜의 경계와 read/write 반환 길이를 확인한다.

### 제출·공개 상태

- 과제 마감: **11/17(화) 23:59:59**, 제출창은 11/17 00:00 공개 예정이다.
- 소스와 Makefile을 `week12/`에 넣어 **2024315329_w12.tar.zst**로 제출한다.
- `make`가 **w12_server, w12_client**를 생성해야 한다. 컴파일 실패는 교안에서 0점으로 안내한다.
- week12 코드 원본 `week12.zip`은 **11/17(화) 18:00 공개 예정**, 현재 opened=false다. 아직 받은 것으로 표시하지 않는다.
- `week12` 항목은 파일이 아니라 11/17 18:00 Zoom 수업 연결이다. 별도 대면 운영 안내는 교수님 지시를 따른다.
- VMShare의 원본·제출본은 이동하거나 수정하지 않았다. week11 코드 원본은 이미 보관된 파일과 해시가 같음을 확인했다.

### PA3 연결

[PA3 원본·공개 파일](../PA/PA_3/study.md)은 **12/6(일) 마감**이며 이번 주 화요일 실습 제출과 별개다. 다중 클라이언트 예약 서버, worker별 poll, 좌석·사용자 동기화로 연결된다. 이번 정리에서는 원본·스켈레톤·예제를 보관하고 요구사항 목차를 연결했다. 구현·제출은 하지 않았다.

### 주의해서 이해할 표현

joinable 스레드는 종료 후 join 등으로 자원을 회수해야 하고 detached 스레드는 종료 때 자원을 회수한다. detached라는 것이 취소 불가능하다는 뜻은 아니다. Reactor/Proactor는 준비 통지/완료 통지의 차이이고 edge/level-trigger와 일대일 동의어는 아니므로 교안 p.19의 간략 분류를 일반 정의로 확대하지 않는다.


## iCampus 공개 항목

| 항목 | 종류 | 재생시간 | 공개/수강 마감(KST) |
| --- | --- | --- | --- |
| [week12](https://canvas.skku.edu/courses/75630/modules/items/5936399) | video_conference | - | 공개 / 시작 2026-11-17 18:00:00 |
| [week12_concurrent_programming](https://canvas.skku.edu/courses/75630/modules/items/5936407) | PDF | - | 공개 / 시작 2026-11-16 00:00:00 |
| [week12 weekly assignment](https://canvas.skku.edu/courses/75630/modules/items/5936408) | 과제 | - | LMS 일정 참조 / 시작 2026-11-17 00:00:00 / 마감 2026-11-17 23:59:59 |
| [week12_code](https://canvas.skku.edu/courses/75630/modules/items/5936409) | 파일 | - | 공개 전 / 시작 2026-11-17 18:00:00 |

## 과제·퀴즈 제출 확인

| 항목 | 정규 마감(KST) | 현재 조회 상태·제출 시각 |
| --- | --- | --- |
| [week12 weekly assignment](https://canvas.skku.edu/courses/75630/assignments/2071543) | 2026-11-17 23:59:59 | LMS 미제출 |
| [PA3 submit](https://canvas.skku.edu/courses/75630/assignments/2068885) | 2026-12-06 23:59:59 | LMS 미제출 |

LMS 미제출은 현재 사용자 계정의 제출 상태다. 서버 코드 제출·팀 내부 전달·학습 완료 여부와 구분하며 잠금 시각을 정규 마감으로 바꾸지 않는다. 퀴즈를 시작하거나 답안을 입력·제출하지 않았다.

## 공지·확인 한계

- 11/9 이후 게시된 새 공지 없음. 기존 운영 공지와 현재 과제 일정을 함께 확인했다.
- Canvas 전체 파일 목록은 HTTP 401이므로 학생에게 제공된 LearningX 강의콘텐츠·수업자료실·과제 첨부 경로로 원본을 확인했다. 파일 목록 오류를 자료 부재로 처리하지 않았다.
- 이 과목의 Canvas quizzes 목록은 HTTP 404다. 과제·모듈·공지를 같이 확인했고 모든 평가가 없다고 확대하지 않았다.
