# 운영체제 — xv6 시스템콜·스케줄링·가상메모리

프로세스·스케줄링·가상메모리·동기화·I/O를 배우고, 교육용 xv6 커널을 확장하는 개인 과제를 수행했다. 현재 공개 저장소는 **보고서·명세·제출 기록 중심**이다.

## 대표 과제와 근거

| 과제 | 수행·학습 주제 | 확인 자료 |
|---|---|---|
| PA0 | xv6 빌드·부팅·메시지·서버 제출 | [부팅·제출 기록](./Assignment/PA0-booting-xv6.md) |
| PA1 | nice 조회·설정, ps, 사용자→커널 시스템콜 연결 | [보고서 기반 설명](./Assignment/pa1/study.md) |
| PA2 | nice/weight, runtime/vruntime, time slice·timer tick, CFS | [보고서 기반 설명](./Assignment/pa2/study.md) |
| PA3 | mmap/munmap/freemem, eager/lazy 할당, page fault·권한, fork 공유·참조 수·해제 | [범위·검증 근거](./Assignment/pa3/study.md), [제출 보고서](./Assignment/pa3/PA3_Report_2024315329_Junhyuk_Song.pdf) |

PA3 코드·PDF 제출 완료를 확인했고 보고서에는 자체 통합 시험 30 PASS/0 FAIL이 기록돼 있다. **최종 xv6 수정 소스는 현재 공개 저장소에 보관되지 않았다.** 보고서 결과를 새 코드 검수나 공식 채점으로 바꾸지 않는다.

자소서·면접은 [O1/O2/O3 경험 카드](../경험-근거지도.md#o1o2o3--xv6-시스템콜스케줄링가상메모리), 상세 과제 선택은 [pa-study.md](./pa-study.md)에서 시작한다. 실제 Linux BSP·드라이버 개발 경력과 xv6 교육 과제는 구분한다.

## 학습 흐름

프로세스 → CPU scheduling → 주소 공간·paging·replacement → thread·lock·condition variable·semaphore → deadlock → I/O·SSD → filesystem.

- [중간 대비 교안·퀴즈·과거 수업자료](./midterm-prep/목차.md). 파일에 퀴즈가 섞여 있어도 독립 시험지로 가정하지 않는다.

## 수업 자료 찾기

[1주차](./1주차/study.md) · [2주차](./2주차/study.md) · [3주차](./3주차/study.md) · [4주차](./4주차/study.md) · [5주차](./5주차/study.md) · [6주차](./6주차/study.md) · [7주차](./7주차/study.md) · [8주차](./8주차/study.md) · [9주차](./9주차/study.md) · [10주차](./10주차/study.md) · [11주차](./11주차/study.md) · [12주차](./12주차/study.md)

- [공지 원문](./notices.md) · [과제·퀴즈·제출 기록](./assignments.md) · [전체 일정](../tasks.md)
- [전체 경험·근거 지도](../경험-근거지도.md) · [저장소 홈](../README.md)

최신 정리: 2026-11-17. 소스·보고서의 보관 상태와 수강·개인 학습 완료는 별도로 관리한다.
