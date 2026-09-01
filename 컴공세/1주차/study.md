# 컴공세 1주차 관리

## 상태

1주차 공지 기준으로 과목 운영 방식과 리포트 제출 규칙이 확인되었습니다. 로컬에는 1주차 세미나 자료 `End-to-End Speech Interface (Prof. Kyuhong Shim).pdf`가 들어와 있고, 제출처/마감은 강의콘텐츠 및 과제 및 평가에서 확인해야 합니다.

## 이번 주 할 일

- [ ] 강의콘텐츠에서 1주차 세미나 영상이 열렸는지 확인
- [ ] 영상이 있으면 기간 안에 시청
- [ ] 세미나 주제/발표자/핵심 내용을 이 파일에 간단히 정리
- [x] 교수님 템플릿 기준으로 피드백 리포트 작성
- [x] 파일명을 `학번_이름.docx` 또는 `학번_이름.pdf` 형식으로 맞춤
- [ ] iCampus 지정 제출처에 제출
- [ ] `assignments.md`의 1주차 상태를 제출 완료로 변경

## 리포트 초안 자리

### 세미나 정보

- 주차: 1주차
- 주제: End-to-End Speech Interface for Conversational AI
- 발표자: Kyuhong Shim
- 자료: `End-to-End Speech Interface (Prof. Kyuhong Shim).pdf` (22쪽)
- 제출 마감: 미확인

### 들은 내용

- 모든 입력 신호를 token sequence로 볼 수 있다는 관점 소개
- text/image/speech token을 MLLM이 처리하는 흐름
- pipeline 방식(STT -> LLM -> TTS)과 end-to-end speech generation 방식 비교
- end-to-end speech GenAI의 challenge: speech token 수, heavy encoder, latency, personalization, deepfake/ethics, hallucination, on-device/privacy

### 생성된 리포트 파일

- `2024315329_송준혁.docx`
- `2024315329_송준혁.pdf`

### 느낀 점/피드백

리포트는 템플릿의 기본 정보와 Comments 영역만 남겨 영어로 작성했습니다. 이번 주 강의에는 별도 Q&A 문항이 없어서 Q&A 영역은 삭제했고, PDF 기준 1페이지 제출본으로 맞췄습니다.

### 리포트에 넣은 핵심 문장

- Modern generative AI systems can treat many kinds of information as tokens.
- A pipelined speech system is easier to build and debug, but STT errors and loss of paralinguistic information can affect the entire system.
- End-to-end SpeechLM systems seem promising, but they still face latency, personalization, hallucination, privacy, and ethical challenges.

## 운영 메모

- 컴공세는 Pass/Fail 과목이므로 매주 누락 방지가 핵심입니다.
- 15주 중 최소 12주 이상 출석과 리포트 제출을 모두 충족해야 합니다.
- 세미나 자료는 매주 해당 주차 폴더에 보관하고, 리포트 초안은 이 study.md에 붙여 관리합니다.
