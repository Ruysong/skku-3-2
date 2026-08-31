param(
  [Parameter(Mandatory = $true)]
  [string]$Pdf,

  [Parameter(Mandatory = $true)]
  [string]$Course,

  [Parameter(Mandatory = $true)]
  [string]$Week,

  [string]$Lang = "eng",
  [int]$Dpi = 300,
  [string]$OutputRoot = "tmp/ocr-review",
  [string]$TessdataPath = "",
  [switch]$AllowDownload
)

$ErrorActionPreference = "Stop"

function Resolve-ToolPath($Name, $Fallback) {
  $cmd = Get-Command $Name -ErrorAction SilentlyContinue
  if ($cmd) { return $cmd.Source }
  if ($Fallback -and (Test-Path -LiteralPath $Fallback)) { return $Fallback }
  throw "Cannot find $Name. Expected fallback: $Fallback"
}

function Convert-ToSlug($Text) {
  $slug = [Regex]::Replace($Text, '[^\p{L}\p{Nd}._-]+', '-')
  $slug = [Regex]::Replace($slug, '-+', '-').Trim('-')
  if ([string]::IsNullOrWhiteSpace($slug)) { return "ocr-review" }
  return $slug
}
function Test-LangData($Dir, $LangName) {
  if (-not (Test-Path -LiteralPath $Dir)) { return $false }
  $names = @("$LangName.traineddata", "$LangName.traineddata.gz", "$LangName.traineddata.cache")
  foreach ($name in $names) {
    if (Test-Path -LiteralPath (Join-Path $Dir $name)) { return $true }
  }
  return $false
}

function Assert-LangData($LangSpec, $Dir, [bool]$AllowDownloadValue) {
  if ($AllowDownloadValue) { return }
  $missing = @()
  foreach ($langName in ($LangSpec -split '\+')) {
    $trimmed = $langName.Trim()
    if ($trimmed -and -not (Test-LangData $Dir $trimmed)) { $missing += $trimmed }
  }
  if ($missing.Count -gt 0) {
    throw "Missing local Tesseract language data: $($missing -join ', '). Put *.traineddata or *.traineddata.gz in $Dir, or rerun with -AllowDownload."
  }
}

$repoRoot = (& git rev-parse --show-toplevel).Trim()
if (-not $repoRoot) { throw "Run this inside the git repository." }

$pdfPath = (Resolve-Path -LiteralPath $Pdf).Path
$repoPath = (Resolve-Path -LiteralPath $repoRoot).Path
if (-not $pdfPath.StartsWith($repoPath)) {
  throw "PDF must be inside this repository: $pdfPath"
}

$pdftoppm = Resolve-ToolPath "pdftoppm" "C:\Users\rruys\.cache\codex-runtimes\codex-primary-runtime\dependencies\native\poppler\Library\bin\pdftoppm.exe"
$node = Resolve-ToolPath "node" "C:\Users\rruys\.cache\codex-runtimes\codex-primary-runtime\dependencies\node\bin\node.exe"
$tesseractModule = "C:\Users\rruys\.cache\codex-runtimes\codex-primary-runtime\dependencies\node\node_modules\tesseract.js"

if ([string]::IsNullOrWhiteSpace($TessdataPath)) {
  $TessdataPath = Join-Path $repoRoot "tools\ocr\tessdata"
}

$baseName = [IO.Path]::GetFileNameWithoutExtension($pdfPath)
$reviewId = "$(Convert-ToSlug $Course)__$(Convert-ToSlug $Week)__$(Convert-ToSlug $baseName)"
$reviewDir = Join-Path $repoRoot (Join-Path $OutputRoot $reviewId)
$pagesDir = Join-Path $reviewDir "pages"
$ocrDir = Join-Path $reviewDir "ocr"

New-Item -ItemType Directory -Force -Path $pagesDir, $ocrDir, $TessdataPath | Out-Null
Assert-LangData $Lang $TessdataPath $AllowDownload.IsPresent

$pagePrefix = Join-Path $pagesDir "page"
& $pdftoppm -r $Dpi -png $pdfPath $pagePrefix
if ($LASTEXITCODE -ne 0) { throw "pdftoppm failed with exit code $LASTEXITCODE" }

$script = Join-Path $repoRoot "tools\ocr\ocr-pages.js"
& $node $script --imagesDir $pagesDir --outDir $ocrDir --lang $Lang --cachePath (Join-Path $reviewDir ".tess-cache") --langPath $TessdataPath --allowDownload $AllowDownload.IsPresent --tesseractModule $tesseractModule
if ($LASTEXITCODE -ne 0) { throw "OCR failed with exit code $LASTEXITCODE" }

$ocrJson = Get-Content -LiteralPath (Join-Path $ocrDir "ocr-pages.json") -Raw | ConvertFrom-Json
$targetDir = Join-Path $repoRoot (Join-Path $Course "텍스트")
$targetName = "$(Convert-ToSlug $Week)-$(Convert-ToSlug $baseName).md"
$targetPath = Join-Path $targetDir $targetName
$sourceRelative = [IO.Path]::GetRelativePath($repoRoot, $pdfPath).Replace('\', '/')
$today = Get-Date -Format "yyyy-MM-dd"

$review = New-Object System.Collections.Generic.List[string]
$review.Add("# OCR 검수 작업장")
$review.Add("")
$review.Add("- 과목: $Course")
$review.Add("- 주차: $Week")
$review.Add("- 원본: $sourceRelative")
$review.Add("- 생성일: $today")
$review.Add("- OCR 언어: $Lang")
$review.Add("- 최종 저장 위치: $([IO.Path]::GetRelativePath($repoRoot, $targetPath).Replace('\', '/'))")
$review.Add("- 상태: 검수 필요")
$review.Add("")
$review.Add("> 이 파일은 최종 강의 텍스트가 아닙니다. 각 페이지 이미지를 보면서 `Verified text` 블록을 채운 뒤 export하세요.")
$review.Add("")

$html = New-Object System.Collections.Generic.List[string]
$html.Add("<!doctype html>")
$html.Add("<html lang=`"ko`">")
$html.Add("<head><meta charset=`"utf-8`"><title>OCR Review - $baseName</title>")
$html.Add("<style>body{font-family:system-ui,sans-serif;margin:0;background:#f6f7f9;color:#15171a}.page{display:grid;grid-template-columns:minmax(320px,1fr) minmax(320px,1fr);gap:16px;padding:16px;border-bottom:1px solid #ddd}.page img{width:100%;background:white;border:1px solid #ddd}.text{white-space:pre-wrap;background:white;border:1px solid #ddd;padding:12px;font-family:Consolas,monospace;font-size:13px;line-height:1.45}.meta{padding:16px;background:white;border-bottom:1px solid #ddd}@media(max-width:900px){.page{grid-template-columns:1fr}}</style>")
$html.Add("</head><body>")
$html.Add("<div class=`"meta`"><h1>OCR Review</h1><p>$Course / $Week / $baseName</p><p>최종본은 review.md 검수 후 export합니다.</p></div>")

foreach ($page in $ocrJson.pages) {
  $pageNo = [int]$page.page
  $imageRel = "pages/$([IO.Path]::GetFileName([string]$page.image))"
  $ocrText = [string]$page.text
  $review.Add("## Page $pageNo")
  $review.Add("")
  $review.Add("![Page $pageNo]($imageRel)")
  $review.Add("")
  $review.Add("### OCR draft")
  $review.Add("")
  $review.Add('````text')
  $review.Add($ocrText)
  $review.Add('````')
  $review.Add("")
  $review.Add("### Verified text")
  $review.Add("")
  $review.Add("<!-- verified:start page=$pageNo -->")
  $review.Add('````text')
  $review.Add("TODO_VERIFY")
  $review.Add('````')
  $review.Add("<!-- verified:end page=$pageNo -->")
  $review.Add("")

  $escaped = [Net.WebUtility]::HtmlEncode($ocrText)
  $html.Add("<section class=`"page`"><div><h2>Page $pageNo</h2><img src=`"$imageRel`"></div><div><h2>OCR draft - confidence $($page.confidence)</h2><div class=`"text`">$escaped</div></div></section>")
}

$html.Add("</body></html>")

$reviewPath = Join-Path $reviewDir "review.md"
$htmlPath = Join-Path $reviewDir "review.html"
Set-Content -LiteralPath $reviewPath -Value $review -Encoding UTF8
Set-Content -LiteralPath $htmlPath -Value $html -Encoding UTF8

Write-Host "Review workspace created:"
Write-Host $reviewPath
Write-Host $htmlPath
Write-Host ""
Write-Host "After manual verification, run:"
Write-Host ".\tools\ocr\export-verified.ps1 -Review `"$reviewPath`""


