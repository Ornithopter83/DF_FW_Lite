param()
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$target = Join-Path $root "testModule\Assets"
$original = Join-Path (Split-Path -Parent $root) "DF_FW_Code\testModule\Assets"
$revision = "926c010b2f65263cfb96b207cf4ad57ddb2510ea"
New-Item -ItemType Directory -Force -Path $target | Out-Null
foreach ($name in @("reel-top-v2.png", "reel-bottom.png")) {
    $destination = Join-Path $target $name
    $localOriginal = Join-Path $original $name
    if (Test-Path $destination) {
        Write-Host "Already present: $destination"
    } elseif (Test-Path $localOriginal) {
        Copy-Item -LiteralPath $localOriginal -Destination $destination
        Write-Host "Copied: $destination"
    } else {
        $url = "https://raw.githubusercontent.com/Ornithopter83/DF_FW_Code/$revision/testModule/Assets/$name"
        Invoke-WebRequest -UseBasicParsing -Uri $url -OutFile $destination
        Write-Host "Downloaded: $destination"
    }
}
Write-Host "Asset restoration complete."
