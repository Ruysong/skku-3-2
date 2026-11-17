# 운영체제 PA 구현목차

xv6의 시스템콜·CPU 스케줄링·가상메모리를 확장하는 과제를 수행했다.

| 과제 | 수행 내용 | 관련 자료 | 설명 |
|---|---|---|---|
| PA0 | 빌드·부팅·본인 메시지·서버 제출 | 명세·터미널 기록 | [부팅 기록](./Assignment/PA0-booting-xv6.md) |
| PA1 | nice 기본값·조회·변경, ps 시스템콜 | 구현 보고서 | [PA1](./Assignment/pa1/study.md) |
| PA2 | CFS·runtime/vruntime·time slice·ps 확장 | 명세·구현 보고서 | [PA2](./Assignment/pa2/study.md) |
| PA3 | mmap/munmap/freemem, lazy fault·권한, fork 공유·참조 수·해제 | 보고서·테스트·제출 기록 | [PA3](./Assignment/pa3/study.md) |

PA3는 QEMU 통합 시험에서 30 PASS/0 FAIL을 기록하고 코드·PDF 보고서를 제출했다. 사용자→커널 호출부터 프로세스 상태·페이지·파일의 수명까지 연결해서 설명할 수 있도록 정리했다.

[O1/O2/O3 경험 카드](../경험-근거지도.md#o1o2o3--xv6-시스템콜스케줄링가상메모리) → 과제별 구현 설명 → 제출 보고서 순서로 읽는다.
