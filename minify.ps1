param(
    [Parameter(Mandatory=$true)][string]$InputFile
)

if ( -Not [System.IO.Path]::IsPathRooted($InputFile)) {
    $InputFile=Join-Path $(Get-Location) $InputFile | Resolve-Path
}

if ( -Not [System.IO.File]::Exists($InputFile)) {
    Write-Error "No input file is found"
}

If ( -Not (Get-Command llvm-strip -ErrorAction SilentlyContinue)) {
    Write-Error "llvm-strip is not found!"
}

llvm-strip -d -U -s $InputFile