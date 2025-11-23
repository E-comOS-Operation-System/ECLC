# Simple ECLC Installation Script
Write-Host "Installing ECLC..." -ForegroundColor Green

# Check GCC
if (!(Get-Command gcc -ErrorAction SilentlyContinue)) {
    Write-Host "Error: GCC not found" -ForegroundColor Red
    exit 1
}

# Build
Write-Host "Building..." -ForegroundColor Cyan
make clean
make

if ($LASTEXITCODE -ne 0) {
    Write-Host "Build failed" -ForegroundColor Red
    exit 1
}

# Install
$InstallDir = "$HOME/.local/bin"
New-Item -ItemType Directory -Path $InstallDir -Force | Out-Null
Copy-Item "bin/eclc" "$InstallDir/" -Force
& chmod +x "$InstallDir/eclc"

Write-Host "Installed to $InstallDir" -ForegroundColor Green
Write-Host "Add to PATH: export PATH=\"${InstallDir}:\`$PATH\"" -ForegroundColor Yellow