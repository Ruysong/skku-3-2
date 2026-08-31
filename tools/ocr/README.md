# OCR review engine

PDF/PPTX 강의자료를 최종 Markdown으로 바로 변환하지 않고, 먼저 검수 작업장을 만드는 도구입니다. 자동 OCR 결과는 `tmp/` 아래에만 두고, 사람이 페이지 이미지와 대조해 `Verified text`를 채운 뒤에만 해당 주차 폴더의 `transcript-자료명.md`로 export합니다.

## 흐름

1. 원본 PDF를 주차 폴더에 둡니다.
2. `new-review.ps1`로 페이지 이미지, OCR 초안, 검수용 Markdown/HTML을 생성합니다.
3. `tmp/ocr-review/.../review.md`에서 각 페이지의 `Verified text` 블록을 원본 이미지와 대조해 채웁니다.
4. `export-verified.ps1`로 검수 완료본만 원본과 같은 주차 폴더에 저장합니다.

## 사용 예시

```powershell
.\tools\ocr\new-review.ps1 `
  -Pdf ".\시스템프로그램_엄영익\1주차\SP00(CourseOutline)-(2026-2)-(SWE2001-41).pdf" `
  -Course "시스템프로그램_엄영익" `
  -Week "1주차"
```

검수 완료 후:

```powershell
.\tools\ocr\export-verified.ps1 `
  -Review ".\tmp\ocr-review\시스템프로그램_엄영익__1주차__SP00-CourseOutline-2026-2-SWE2001-41\review.md"
```

## 언어 데이터

기본값은 다운로드 없이 로컬 언어 데이터만 사용합니다. 필요한 파일을 `tools/ocr/tessdata/`에 둡니다.

- 영어: `eng.traineddata`
- 한국어: `kor.traineddata`

언어 데이터가 없으면 `new-review.ps1`는 렌더링 전에 바로 실패합니다. 네트워크 다운로드를 의도한 경우에만 `-AllowDownload`를 붙입니다.

```powershell
.\tools\ocr\new-review.ps1 `
  -Pdf ".\운영체제_신동군\1주차\OS01-OS intro.pdf" `
  -Course "운영체제_신동군" `
  -Week "1주차" `
  -Lang "eng+kor" `
  -AllowDownload
```

## 원칙

- `review.md`는 검수 작업장입니다. 최종본이 아닙니다.
- OCR 결과는 초안으로만 봅니다.
- 그림 속 텍스트, 도식 내부 글자, 표, 수식은 반드시 페이지 이미지와 대조합니다.
- `TODO_VERIFY`가 남아 있으면 export가 실패합니다.
- export된 `transcript-자료명.md`만 커밋합니다.
