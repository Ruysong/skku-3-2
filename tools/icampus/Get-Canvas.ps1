param(
    [Parameter(Mandatory = $true)]
    [string]$ApiPath,
    [switch]$AllPages
)

# Read-only helper. The encrypted token is tied to the current Windows user.
$ErrorActionPreference = 'Stop'
if ($ApiPath -notmatch '^/api/v1/' -or $ApiPath -match '(?i)access_token|[\r\n]') {
    throw 'Use an /api/v1/ path without credentials.'
}
$workspace = Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$tokenFile = Join-Path $workspace '.local/icampus/token.dpapi'
if (-not (Test-Path -LiteralPath $tokenFile)) {
    throw 'Local Canvas credential is not configured.'
}
$secureToken = (Get-Content -LiteralPath $tokenFile -Raw).Trim() | ConvertTo-SecureString
$pointer = [Runtime.InteropServices.Marshal]::SecureStringToBSTR($secureToken)
try {
    $plainToken = [Runtime.InteropServices.Marshal]::PtrToStringBSTR($pointer)
    $headers = @{ Authorization = 'Bearer ' + $plainToken; Accept = 'application/json' }
    $nextUri = 'https://canvas.skku.edu' + $ApiPath
    $visited = [Collections.Generic.HashSet[string]]::new()
    do {
        $parsedUri = [uri]$nextUri
        if ($parsedUri.Scheme -ne 'https' -or $parsedUri.Host -ne 'canvas.skku.edu' -or -not $parsedUri.IsDefaultPort -or $parsedUri.AbsolutePath -notlike '/api/v1/*' -or $parsedUri.Query -match '(?i)access_token' -or -not $visited.Add($nextUri)) {
            throw 'Invalid or repeated Canvas pagination URL.'
        }
        try {
            $response = Invoke-WebRequest -Uri $nextUri -Method Get -Headers $headers -MaximumRedirection 0 -TimeoutSec 30 -UseBasicParsing
        }
        catch {
            $status = if ($_.Exception.Response) { [int]$_.Exception.Response.StatusCode } else { 'unavailable' }
            throw "Canvas GET failed (HTTP $status). Credentials are not included in this error."
        }
        if ($response.Headers['Content-Type'] -notmatch 'json') {
            throw 'Canvas returned a non-JSON response.'
        }
        $response.Content | ConvertFrom-Json
        $nextUri = $null
        if ($AllPages) {
            $linkHeader = $response.Headers['Link'] -join ','
            if ($linkHeader -match '<([^>]+)>;\s*rel="next"') { $nextUri = $Matches[1] }
        }
    } while ($nextUri)
}
finally {
    [Runtime.InteropServices.Marshal]::ZeroFreeBSTR($pointer)
    if ($headers) { $headers.Clear() }
    $plainToken = $null
    $secureToken.Dispose()
}
