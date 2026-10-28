# 운영체제 PA 학습목차

| 과제 | 한 일 | 실제 확인 자료 | 구현 정리 |
| --- | --- | --- | --- |
| PA0 | xv6 빌드·부팅, 본인 boot message, 서버 제출 | 과제 PDF와 9/19 사용자 터미널 제출 로그 | [PA0 기록](./Assignment/PA0-booting-xv6.md) |
| PA1 | nice 기본값·조회·변경, ps 시스템콜 | 최종 보고서 2페이지, 사용자 코드·보고서 제출 완료 확인 | [PA1 보고서 기반 교안](./Assignment/pa1/study.md) |
| PA2 | xv6 CPU Scheduling·CFS, runtime·vruntime, ps 확장 | 명세 17페이지, 보고서 2페이지. 실제 수정 소스와 서버 제출 기록은 별도 | [PA2 보고서 기반 목차](./Assignment/pa2/study.md) |

**중요:** 실제 수정된 `xv6-public` 소스는 로컬 과목 폴더에서 발견하지 못했다. 따라서 PA1은 보고서 기반 설명이며, `proc.c` 등의 최종 diff나 빌드 결과를 이번에 확인한 것처럼 표시하지 않는다. 서버 원본을 가져오면 그 소스와 대조할 수 있도록 파일별 역할을 남겼다.

PA2는 보고서까지 보관되어 있다. 보고서에 적힌 구현·검증 계획과 실제 소스·실행 로그·제출 성공은 구분한다. nice 인터페이스를 추가한 PA1과 CPU scheduling 정책을 바꾸는 PA2도 별개다.
