$ErrorActionPreference = "Stop"

$root = Split-Path -Parent $PSScriptRoot
$destination = Join-Path $root "testModule\esptool_V4.5.1.exe"
$original = Join-Path (Split-Path -Parent $root) "DF_FW_Code\testModule\esptool_V4.5.1.exe"
$expectedSize = 6638312
$expectedBlob = "dc686a0d53221ee77e3f4de3967be18c23a786b2"
$url = "https://raw.githubusercontent.com/Ornithopter83/DF_FW_Code/926c010b2f65263cfb96b207cf4ad57ddb2510ea/testModule/esptool_V4.5.1.exe"

function Test-OriginalEsptool([string]$path) {
    if (!(Test-Path -LiteralPath $path -PathType Leaf)) { return $false }
    $data = [System.IO.File]::ReadAllBytes($path)
    if ($data.Length -ne $expectedSize) { return $false }
    $prefix = [System.Text.Encoding]::ASCII.GetBytes("blob $($data.Length)`0")
    $buffer = New-Object byte[] ($prefix.Length + $data.Length)
    [System.Buffer]::BlockCopy($prefix, 0, $buffer, 0, $prefix.Length)
    [System.Buffer]::BlockCopy($data, 0, $buffer, $prefix.Length, $data.Length)
    $sha1 = [System.Security.Cryptography.SHA1]::Create()
    try {
        $digest = $sha1.ComputeHash($buffer)
    } finally {
        $sha1.Dispose()
    }
    $id = -join ($digest | ForEach-Object { $_.ToString("x2") })
    return ($id -eq $expectedBlob)
}

if (Test-OriginalEsptool $destination) {
    Write-Host "Original embedded esptool ready."
    exit 0
}

$temp = "$destination.download"
try {
    if (Test-OriginalEsptool $original) {
        Copy-Item -LiteralPath $original -Destination $temp -Force
    } else {
        Write-Host "Restoring original esptool from DF_FW_Code..."
        Invoke-WebRequest -UseBasicParsing -Uri $url -OutFile $temp
    }
    if (!(Test-OriginalEsptool $temp)) {
        throw "Original esptool signature/size mismatch. The updater executable was not accepted."
    }
    Move-Item -LiteralPath $temp -Destination $destination -Force
    Write-Host "Original esptool successfully restored and verified."
} catch {
    throw "Failed to restore embedded esptool. Place the original DF_FW_Code/testModule/esptool_V4.5.1.exe next to the Lite checkout, or allow GitHub access. $($_.Exception.Message)"
} finally {
    if (Test-Path -LiteralPath $temp) { Remove-Item -LiteralPath $temp -Force }
}
