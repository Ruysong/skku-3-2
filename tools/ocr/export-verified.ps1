param(
  [Parameter(Mandatory = $true)]
  [string]$Review
)

$ErrorActionPreference = "Stop"

$repoRoot = (& git rev-parse --show-toplevel).Trim()
if (-not $repoRoot) { throw "Run this inside the git repository." }

$reviewPath = (Resolve-Path -LiteralPath $Review).Path
$repoPath = (Resolve-Path -LiteralPath $repoRoot).Path
if (-not $reviewPath.StartsWith($repoPath)) {
  throw "Review file must be inside this repository: $reviewPath"
}

$content = Get-Content -LiteralPath $reviewPath -Raw

function Read-Meta($Name) {
  $pattern = "(?m)^- $([Regex]::Escape($Name)): (.+)$"
  $match = [Regex]::Match($content, $pattern)
  if (-not $match.Success) { throw "Missing metadata: $Name" }
  return $match.Groups[1].Value.Trim()
}

$course = Read-Meta "과목"
$week = Read-Meta "주차"
$source = Read-Meta "원본"
$targetRelative = Read-Meta "최종 저장 위치"
$today = Get-Date -Format "yyyy-MM-dd"

$matches = [Regex]::Matches(
  $content,
  '<!-- verified:start page=(\d+) -->\s*````text\s*(.*?)\s*````\s*<!-- verified:end page=\1 -->',
  [Text.RegularExpressions.RegexOptions]::Singleline
)

if ($matches.Count -eq 0) {
  throw "No verified blocks found."
}

$pages = @()
foreach ($match in $matches) {
  $pageNo = [int]$match.Groups[1].Value
  $text = $match.Groups[2].Value.Trim()
  if ([string]::IsNullOrWhiteSpace($text) -or $text -match 'TODO_VERIFY') {
    throw "Page $pageNo is not verified yet."
  }
  $pages += [PSCustomObject]@{ Page = $pageNo; Text = $text }
}

$targetPath = Join-Path $repoRoot $targetRelative
$targetResolvedParent = Split-Path -Parent $targetPath
New-Item -ItemType Directory -Force -Path $targetResolvedParent | Out-Null

$title = [IO.Path]::GetFileNameWithoutExtension($targetPath)
$out = New-Object System.Collections.Generic.List[string]
$out.Add("# $title")
$out.Add("")
$out.Add("## 자료 정보")
$out.Add("")
$out.Add("- 과목: $course")
$out.Add("- 원본: $source")
$out.Add("- 주차: $week")
$out.Add("- 정리일: $today")
$out.Add("- 방식: 원본 PDF/PPTX 페이지를 확인한 눈검수/OCR 검수본")
$out.Add("")

foreach ($page in ($pages | Sort-Object Page)) {
  $out.Add("## Page $($page.Page)")
  $out.Add("")
  $out.Add('```text')
  $out.Add($page.Text)
  $out.Add('```')
  $out.Add("")
}

Set-Content -LiteralPath $targetPath -Value $out -Encoding UTF8
Write-Host "Verified Markdown exported:"
Write-Host $targetPath


