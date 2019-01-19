Add-Type -AssemblyName System.IO.Compression.FileSystem
function Unzip
{
    param([string]$zipfile, [string]$outpath)

    [System.IO.Compression.ZipFile]::ExtractToDirectory($zipfile, $outpath)
}

If(!(test-path build\SDL2))
{
    Write-Host Creating build\SDL2
    New-Item -ItemType Directory -Force -Path build\SDL2
    Write-Host Download ZIP
    Invoke-WebRequest -Uri 'https://www.libsdl.org/release/SDL2-devel-2.0.8-VC.zip' -OutFile 'build\SDL2.zip'
    Write-Host Extract zip
    Unzip "build\SDL2.zip" "build\SDL2"
}
else
{
    Write-Host Dir already exists, assuming we also have the extracted zip
}

$item = (Get-ChildItem "build\SDL2" -directory | Select FullName)
Write-Host Run cmake in the root as cmake .. -DSDL2_DIR=($item.FullName)

pause
