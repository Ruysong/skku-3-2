# PA2 CFS 명세와 보고서 목차

2026-10-28 현재 보관된 명세·보고서 기준. 실제 xv6 수정 소스는 이 폴더에서 확인되지 않아 아래 설명은 보고서 기반이다.

| 자료 | 페이지 | 역할 |
| --- | ---: | --- |
| [Project 2 명세](./Project%202_26f.pdf) | 17 | xv6 CFS·ps 확장 요구사항, 제출 절차 |
| [본인 PA2 보고서](./PA2_CFS_Report_Junhyuk_Song.pdf) | 2 | 수정 구성·스케줄링·회계 처리·검증 계획 |

## 보고서의 구현 구성

| 구성 | 설명 |
| --- | --- |
| proc.h | weight, runtime, vruntime, time slice 등 프로세스별 필드 |
| proc.c | RUNNABLE 중 최소 vruntime 선택, nice별 weight, fork 상속, wakeup 조정, ps 출력 |
| trap.c | timer tick마다 runtime·vruntime 계산, slice 소진 시 yield |
| defs.h | timer handler에서 사용하는 helper 선언 |

```text
nice → weight → time slice
                   ↓
실행·timer tick → runtime / vruntime 갱신
                   ↓
최소 vruntime의 RUNNABLE 프로세스 선택
```

## 제출과 상태

- 명세에 적힌 마감은 2026-10-21(수) 23:59:59이며 현재 주의 새 마감으로 재등록하지 않는다.
- 코드는 UI 서버에서 `make clean` 후 `~swe3004_41/bin/submit pa2 xv6-public`로 제출하는 안내다.
- 보고서는 iCampus에 PDF, 5페이지 이내로 별도 제출한다.
- 최종 소스, 서버 제출 내역, iCampus 보고서 제출 내역은 별도 확인이 필요하다. 보고서 파일 존재만으로 제출 완료를 표시하지 않는다.
- 보고서의 Verification Procedure는 최종 빌드에서 확인할 계획이며, 성공한 실행 로그로 표현하지 않는다.

## 공부할 때 확인할 것

1. 실제 CPU 사용 시간과 vruntime을 구분한다.
2. nice·weight가 slice와 vruntime 증가량에 미치는 영향을 설명한다.
3. fork·sleep/wakeup과 잠금 범위를 기존 코드에서 추적한다.
4. 원본 소스를 보관한 뒤 CPU-bound 테스트·ps 출력·기본 shell 동작을 대조한다.

이 문서는 자료·구현 구조 탐색용이며 과제의 정답 코드나 검증 완료 증명서가 아니다.
