param([string]$NedFile, [string]$XmlFile)

$bashRelativePath = "tools/win32.x86_64/usr/bin/bash.exe"
$cygpathRelativePath = "tools/win32.x86_64/usr/bin/cygpath.exe"

function Get-Bash-Path {
    Join-Path `
        -Path $Env:OMNETPP_HOME `
        -ChildPath $bashRelativePath
}

function Get-Posix-Path([string]$Path) {
    $cygpathAbsolutePath = Join-Path -Path $Env:OMNETPP_HOME -ChildPath $cygpathRelativePath

    Invoke-Expression $("{0} -u {1}" -f $($cygpathAbsolutePath), $Path)
}

function New-Temporary-Directory {
    $parent = [System.IO.Path]::GetTempPath()
    [string] $name = [System.Guid]::NewGuid()
    $path = Join-Path -Path $parent -ChildPath $name
    New-Item -ItemType Directory -Path $path | Out-Null
    return $path
}

$absoluteNedFile = [IO.Path]::Combine($PSScriptRoot, $NedFile)
$absoluteXmlFile = [IO.Path]::Combine($PSScriptRoot, $XmlFile)

$normalizedNedFile = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($absoluteNedFile)
$normalizedXmlFile = $ExecutionContext.SessionState.Path.GetUnresolvedProviderPathFromPSPath($absoluteXmlFile)

$tempDir = New-Temporary-Directory

$tempNedFile = Join-Path -Path $tempDir -ChildPath "file.ned"
$tempXmlFile = Join-Path -Path $tempDir -ChildPath "file.ned.xml"

Copy-Item -Path $normalizedNedFile -Destination $tempNedFile

$nedFilePosix = Get-Posix-Path -Path $tempNedFile
$nedToolPosix = Get-Posix-Path -Path $(
    Join-Path -Path $Env:OMNETPP_HOME -ChildPath "bin/opp_nedtool"
)

$bashPath = Get-Bash-Path

$convertCommand = "{0} -l -c `"{1} convert {2}`"" -f $bashPath, $nedToolPosix, $nedFilePosix

$Env:MSYSTEM = "MINGW64"
Invoke-Expression $convertCommand
Remove-Item Env:\MSYSTEM

Copy-Item -Path $tempXmlFile -Destination $normalizedXmlFile
Remove-Item -Path $tempDir -Force -Recurse
