# iCampus Canvas API 연결

2026-11-09에 사용자 제공 개인 토큰으로 계정과 수강 과목 목록의 GET 조회를 확인했다.

- 서버: `https://canvas.skku.edu`
- 호출 도구: [Get-Canvas.ps1](./Get-Canvas.ps1)
- 인증: HTTP Authorization Bearer 헤더. 토큰을 URL에 넣지 않는다.
- 토큰: 저장소 루트 `.local/icampus/token.dpapi`에 Windows DPAPI로 암호화해 보관한다. 생성한 Windows 사용자 환경에 연결된 파일이므로 다른 사용자·컴퓨터에서 재사용할 수 있는 평문 설정 파일이 아니다.
- 연결 확인 정보: `.local/icampus/connection.json`. 해당 `.local/` 전체는 Git에서 제외한다.
- 이 도구는 GET만 수행한다. 과제 제출·퀴즈 응시·메시지 발송을 구현한 도구가 아니다.

```powershell
.\tools\icampus\Get-Canvas.ps1 -ApiPath '/api/v1/users/self/profile' |
    Select-Object id, name

.\tools\icampus\Get-Canvas.ps1 -ApiPath '/api/v1/courses?per_page=100&page=1&include[]=term' -AllPages
```

목록 API는 페이지가 나뉠 수 있다. `-AllPages`를 지정하면 Link 응답 헤더의 next를 따라 같은 Canvas HTTPS 호스트의 API 페이지만 읽는다. 기본값은 한 요청이다. 수강 목록에는 과거 학기 등록이 남을 수 있어 이번 학기 과목은 학기와 과목 ID로 구분한다. 외부 도구 안의 강의 영상·대본·출결이 표준 Canvas API에서 모두 제공되는지는 별도 검증이 필요하다.

기본 실행 환경의 DNS 제한 때문에 연결 확인은 네트워크 사용이 가능한 승인된 실행 환경에서 수행했다. GET 실패 시 자격증명이나 응답 본문을 오류에 출력하지 않는다. 자동 주기 조회·일괄 동기화는 아직 설정하지 않았다.

## 별도 LearningX 수업자료실 (11/9 확인)

시프의 자료는 일반 파일 목록이 아니라 `external_tools/2`의 CourseResource 도구에 보관된다. 파일 목록 API 실패를 자료 부재로 해석하지 않는다. 이번에는 다음 정상 경로로 SP05-3-FC.pdf를 확보했다.

1. Canvas tabs API에서 수업자료실 메뉴의 sessionless_launch 경로를 확인한다.
2. 공식 sessionless_launch API 응답의 서명된 LTI 양식을 동일 학교 도메인의 `/learningx/lti/courseresource`에 전달해 정상 학생 세션을 연다. 이 POST는 자료실 로그인 교환이며 과제·퀴즈 제출이 아니다.
3. 화면이 사용하는 LearningX API의 `/courses/{course_id}/resources_db?user_login={현재 학생 로그인}` 목록과 resource 상세를 확인한다. 자신의 세션·역할을 유지하며 관리자 역할이나 다른 학생 ID로 바꾸지 않는다.
4. 해당 content_id의 `/commons/contents`에서 제공한 download_url을 이용한다. Canvas API 토큰을 다른 도메인에 전달하지 않는다.
5. 다운로드 파일의 PDF 헤더·크기를 확인하고, 교안 SP 번호·영상 S 범위에 맞춰 연결한다. 같은 원본이 여러 주차에 걸치면 파일을 중복 저장하지 않는다.

학습 완료·출결을 강제로 변경하는 progress/force 계열 API는 사용하지 않는다. 서명 양식·세션 쿠키를 재사용 가능한 공개 문서에 넣지 않으며, 이번 조회용 임시 인증 파일은 작업 후 정리한다.
