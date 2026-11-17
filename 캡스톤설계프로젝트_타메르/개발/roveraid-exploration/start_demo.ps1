param([string]$Source = '', [switch]$SkipModel)
$ErrorActionPreference = 'Stop'
Set-Location -LiteralPath $PSScriptRoot
$python = Join-Path $PSScriptRoot '.venv\Scripts\python.exe'
if (-not (Test-Path -LiteralPath $python)) { throw 'Run setup_environment.ps1 first.' }
if (-not $Source) {
    $Source = Join-Path $PSScriptRoot 'samples\pedestrians.avi'
    if (-not (Test-Path -LiteralPath $Source)) { $Source = '0' }
}
$launchArgs = @('-X', 'utf8', '-u', (Join-Path $PSScriptRoot 'run_demo.py'), '--source', $Source, '--autostart')
if ($SkipModel) { $launchArgs += '--no-auto-model' }
& $python @launchArgs
exit $LASTEXITCODE
