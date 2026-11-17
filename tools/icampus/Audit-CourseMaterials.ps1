param(
    [Parameter(Mandatory=$true)][int]$CourseId,
    [Parameter(Mandatory=$true)][string]$CourseFolder,
    [int]$ThroughWeek=12,
    [switch]$DownloadMissing
)
# Inspect the student's own published resources; never updates attendance or submissions.
$ErrorActionPreference='Stop'
$root=Split-Path (Split-Path $PSScriptRoot -Parent) -Parent
$homePath=Join-Path $root $CourseFolder
$cache=Join-Path $root '.local/icampus'
$rows=[Collections.Generic.List[object]]::new()
$issues=[Collections.Generic.List[string]]::new()
$localFiles=@(Get-ChildItem -LiteralPath $homePath -File -Recurse)
function School-Get([string]$Url,$Session,$Headers=@{},[string]$Output){
    $u=[uri]$Url
    if($u.Scheme -ne 'https' -or $u.Host -notin @('canvas.skku.edu','lcms.skku.edu')){throw 'Unexpected download host'}
    try{
        $args=@{Uri=$Url;UseBasicParsing=$true;TimeoutSec=35;Headers=$Headers}
        if($Session){$args.WebSession=$Session}
        if($Output){$args.OutFile=$Output;$args.PassThru=$true}
        Invoke-WebRequest @args
    }catch{throw 'School GET failed; authentication details suppressed.'}
}
function Match-Local([string]$Name){
    @($script:localFiles|Where-Object {$_.Name -eq $Name})
}
function Check-Asset($Detail,[string]$Title,[int]$Week,[string]$Referer,[string]$Source,[string]$SuggestedName=''){
    $a=$Detail.result
    if(-not $a){$a=$Detail}
    $name=[IO.Path]::GetFileName([string]$a.file_name)
    if(-not $name){$name=[IO.Path]::GetFileName($SuggestedName)}
    if(-not $name){$name=[IO.Path]::GetFileName($Title)}
    if($a.content_type -eq 'pdf' -and $name -notmatch '\.pdf$'){$name+='.pdf'}
    $row=[ordered]@{source=$Source;week=$Week;title=$Title;name=$name;status='external-link';paths=@();sha256=$null}
    if(-not $a.download_url){$script:rows.Add([pscustomobject]$row);return}
    if($name -notmatch '\.(pdf|zip|zst|tar|gz|pptx?|docx?|xlsx?|hwp[x]?|txt|c|cpp|h|png|jpe?g)$'){
        $row.status='unsupported-file-name';$script:rows.Add([pscustomobject]$row);return
    }
    $matches=@(Match-Local $name)
    if($matches.Count){
        $row.status='present-by-name'
        $row.paths=@($matches|ForEach-Object {[IO.Path]::GetRelativePath($root,$_.FullName)})
    }elseif(-not $DownloadMissing){$row.status='missing'}
    else{
        $part=Join-Path $cache ('audit-'+[guid]::NewGuid().ToString('N')+'.part')
        try{
            $null=School-Get $a.download_url $null @{Referer=$Referer} $part
            if((Get-Item -LiteralPath $part).Length -eq 0){throw 'Empty download'}
            if($name -match '\.pdf$'){
                $stream=[IO.File]::OpenRead($part)
                try{$bytes=New-Object byte[] 5;$null=$stream.Read($bytes,0,5)}finally{$stream.Dispose()}
                if([Text.Encoding]::ASCII.GetString($bytes) -ne '%PDF-'){throw 'Invalid PDF header'}
            }
            $row.sha256=(Get-FileHash -LiteralPath $part -Algorithm SHA256).Hash.ToLower()
            # Match content too, because LMS display names sometimes differ from local names.
            $same=@($script:localFiles|Where-Object {$_.Length -eq (Get-Item -LiteralPath $part).Length}|Where-Object {(Get-FileHash -LiteralPath $_.FullName).Hash.ToLower() -eq $row.sha256})
            if($same.Count){
                $row.status='present-identical-renamed'
                $row.paths=@($same|ForEach-Object {[IO.Path]::GetRelativePath($root,$_.FullName)})
                Remove-Item -LiteralPath $part
            }else{
                $dest=if($Week -gt 0){Join-Path $homePath ($Week.ToString()+'주차')}else{Join-Path $homePath '자료실'}
                $null=New-Item -ItemType Directory -Force -Path $dest
                $target=Join-Path $dest $name
                if(Test-Path -LiteralPath $target){throw 'Refusing to overwrite existing original'}
                Move-Item -LiteralPath $part -Destination $target
                $script:localFiles+=Get-Item -LiteralPath $target
                $row.status='downloaded'
                $row.paths=@([IO.Path]::GetRelativePath($root,$target))
            }
        }catch{$row.status='download-error';$script:issues.Add($Title+': '+$_.Exception.Message)}
    }
    $script:rows.Add([pscustomobject]$row)
}
$helper=Join-Path $PSScriptRoot 'Get-Canvas.ps1'
$launch=& $helper -ApiPath "/api/v1/courses/$CourseId/external_tools/sessionless_launch?id=297"
$initial=Invoke-WebRequest -Uri $launch.url -UseBasicParsing -SessionVariable studentSession -TimeoutSec 30
$form=[regex]::Match($initial.Content,'(?is)<form\b[^>]*action="([^"]+)"[^>]*>(.*?)</form>')
$action=[Net.WebUtility]::HtmlDecode($form.Groups[1].Value)
if(([uri]$action).Host -ne 'canvas.skku.edu' -or ([uri]$action).AbsolutePath -notlike '/learningx/lti/*'){throw 'Unexpected student login destination'}
$fields=@{}
foreach($tag in [regex]::Matches($form.Groups[2].Value,'(?is)<input\b[^>]*>')){
    $attrs=@{}
    foreach($attr in [regex]::Matches($tag.Value,'([\w:-]+)\s*=\s*"([^"]*)"')){$attrs[$attr.Groups[1].Value]=[Net.WebUtility]::HtmlDecode($attr.Groups[2].Value)}
    if($attrs.ContainsKey('name')){$fields[$attrs['name']]=$attrs['value']}
}
# LTI POST exchanges the signed login form only. It is not a course submission.
$null=Invoke-WebRequest -Uri $action -Method Post -Body $fields -ContentType 'application/x-www-form-urlencoded; charset=UTF-8' -WebSession $studentSession -UseBasicParsing -TimeoutSec 30
$cookie=$studentSession.Cookies.GetCookies([uri]'https://canvas.skku.edu/learningx/api/v1')|Where-Object Name -eq 'xn_api_token'|Select-Object -First 1
if(-not $cookie){throw 'Student LearningX session unavailable'}
$headers=@{Accept='application/json';Authorization=('Bearer '+$cookie.Value)}
$response=School-Get "https://canvas.skku.edu/learningx/api/v1/courses/$CourseId/modules?include_detail=true" $studentSession $headers
[IO.File]::WriteAllText((Join-Path $cache "audit-modules-$CourseId.json"),$response.Content,[Text.UTF8Encoding]::new($false))
$modules=$response.Content|ConvertFrom-Json
foreach($m in $modules){
    $week=[int]$m.week_position
    if($m.title -match '^(\d+)주차'){$week=[int]$Matches[1]}
    foreach($item in $m.module_items){
        $c=$item.content_data;$a=$c.item_content_data
        if($c.item_content_type -ne 'commons' -or $a.content_type -in @('movie','video','youtube','audio','zoom','screenlecture','everlec')){continue}
        if($week -gt $ThroughWeek -or -not $c.opened -or $a.content_id -eq 'not_open'){
            $rows.Add([pscustomobject]@{source='LearningX';week=$week;title=$item.title;name=$a.file_name;status='not-open-or-future';paths=@()});continue
        }
        try{
            $d=School-Get ('https://canvas.skku.edu/learningx/api/v1/commons/contents?content_id='+[uri]::EscapeDataString($a.content_id)) $studentSession $headers
            Check-Asset ($d.Content|ConvertFrom-Json) $item.title $week "https://canvas.skku.edu/courses/$CourseId" 'LearningX' $a.file_name
        }catch{$issues.Add($item.title+': '+$_.Exception.Message)}
    }
}
if($CourseId -eq 74762){
    $login=$fields['custom_canvas_user_login_id'];if(-not $login){$login=$fields['custom_user_login']}
    if(-not $login){throw 'Authorized student login not supplied'}
    $response=School-Get ('https://canvas.skku.edu/learningx/api/v1/courses/74762/resources_db?user_login='+[uri]::EscapeDataString($login)) $studentSession $headers
    [IO.File]::WriteAllText((Join-Path $cache 'audit-sp-resources.json'),$response.Content,[Text.UTF8Encoding]::new($false))
    foreach($resource in ($response.Content|ConvertFrom-Json)){
        try{
            $responseDetail=School-Get "https://canvas.skku.edu/learningx/api/v1/courses/74762/resources/$($resource.resource_id)" $studentSession $headers
            $obj=$responseDetail.Content|ConvertFrom-Json;$cc=$obj.commons_content
            if(-not $cc){$cc=$resource.commons_content}
            if(-not $cc.content_id){throw 'No downloadable student resource'}
            $d=School-Get ('https://canvas.skku.edu/learningx/api/v1/commons/contents?content_id='+[uri]::EscapeDataString($cc.content_id)) $studentSession $headers
            Check-Asset ($d.Content|ConvertFrom-Json) $resource.title 0 $cc.view_url 'CourseResource' $cc.file_name
        }catch{$issues.Add($resource.title+': '+$_.Exception.Message)}
    }
}
$snapshot=Join-Path $cache "week12-$CourseId.json"
if(Test-Path -LiteralPath $snapshot){
    $canvas=Get-Content -LiteralPath $snapshot -Raw -Encoding UTF8|ConvertFrom-Json
    $ids=[Collections.Generic.HashSet[string]]::new()
    foreach($text in @($canvas.assignments.description)+@($canvas.announcements.message)){
        foreach($match in [regex]::Matches([string]$text,'/courses/'+$CourseId+'/files/(\d+)')){$null=$ids.Add($match.Groups[1].Value)}
    }
    foreach($id in $ids){
        try{
            $meta=& $helper -ApiPath "/api/v1/courses/$CourseId/files/$id"
            if($meta.locked_for_user){$rows.Add([pscustomobject]@{source='Canvas attachment';week=0;title=$meta.display_name;name=$meta.display_name;status='locked';paths=@()});continue}
            Check-Asset ([pscustomobject]@{file_name=$meta.display_name;download_url=$meta.url}) $meta.display_name 0 "https://canvas.skku.edu/courses/$CourseId" 'Canvas attachment'
        }catch{$issues.Add('Canvas attachment '+$id+': '+$_.Exception.Message)}
    }
}
$result=[ordered]@{course=$CourseId;folder=$CourseFolder;checked_at=[DateTimeOffset]::Now.ToString('o');through_week=$ThroughWeek;items=@($rows);issues=@($issues)}
[IO.File]::WriteAllText((Join-Path $cache "audit-materials-$CourseId.json"),($result|ConvertTo-Json -Depth 20),[Text.UTF8Encoding]::new($false))
[pscustomobject]@{course=$CourseId;counts=@($rows|Group-Object status|ForEach-Object {[pscustomobject]@{status=$_.Name;count=$_.Count}});downloads=@($rows|Where-Object status -eq 'downloaded'|Select-Object title,paths);issues=@($issues)}|ConvertTo-Json -Depth 8
