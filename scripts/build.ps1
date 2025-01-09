# UltraNote Infinity Core Build Script
# This script handles building the project on Windows, Linux (via WSL), and macOS (via remote build)

param(
    [string]$BuildType = "Release",
    [string]$Platform = "windows",
    [switch]$RunTests = $false,
    [switch]$Clean = $false
)

$ErrorActionPreference = "Stop"

# Configuration
$TimeStamp = Get-Date -Format "yyyyMMddHHmmss"
$BuildDir = "build_$TimeStamp"
$BoostRoot = "C:/local/boost_1_72_0"
$OpenSSLRoot = "C:/Program Files/OpenSSL-Win64"

function Write-Header {
    param([string]$Message)
    Write-Host "`n=== $Message ===`n" -ForegroundColor Cyan
}

function Initialize-Build {
    Write-Header "Initializing Build Environment"
    
    if ($Clean -and (Test-Path $BuildDir)) {
        Write-Host "Cleaning previous build..." -ForegroundColor Yellow
        Remove-Item -Recurse -Force $BuildDir
    }
    
    if (-not (Test-Path $BuildDir)) {
        New-Item -ItemType Directory -Path $BuildDir | Out-Null
    }
}

function Build-Windows {
    Write-Header "Building for Windows"
    
    Push-Location $BuildDir
    try {
        # Configure
        Write-Host "Configuring CMake..." -ForegroundColor Yellow
        cmake -G "Visual Studio 16 2019" -A x64 `
            -DBOOST_ROOT="$BoostRoot" `
            -DBoost_USE_STATIC_RUNTIME=ON `
            -DBoost_USE_STATIC_LIBS=ON `
            -DCMAKE_BUILD_TYPE=$BuildType ..
        
        if ($LASTEXITCODE -ne 0) { throw "CMake configuration failed" }
        
        # Build
        Write-Host "Building project..." -ForegroundColor Yellow
        cmake --build . --config $BuildType --parallel $env:NUMBER_OF_PROCESSORS
        
        if ($LASTEXITCODE -ne 0) { throw "Build failed" }
        
        # Run tests if requested
        if ($RunTests) {
            Write-Host "Running tests..." -ForegroundColor Yellow
            ctest -C $BuildType --output-on-failure
            if ($LASTEXITCODE -ne 0) { throw "Tests failed" }
        }
    }
    finally {
        Pop-Location
    }
}

function Build-Linux {
    Write-Header "Building for Linux (via WSL)"
    
    # Check if WSL is available
    if (-not (Get-Command wsl -ErrorAction SilentlyContinue)) {
        throw "WSL is not installed. Please install WSL to build for Linux."
    }
    
    # Create build script for WSL
    $wslScript = @"
#!/bin/bash
set -e
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=$BuildType ..
make -j\$(nproc)
if [ "$RunTests" = "True" ]; then
    ctest --output-on-failure
fi
"@
    
    $wslScript | wsl bash
}

function Build-MacOS {
    Write-Header "Building for macOS"
    Write-Host "MacOS builds must be performed on a macOS machine. Please use the GitHub Actions workflow for macOS builds." -ForegroundColor Yellow
}

# Main execution
try {
    Initialize-Build
    
    switch ($Platform.ToLower()) {
        "windows" { Build-Windows }
        "linux" { Build-Linux }
        "macos" { Build-MacOS }
        default { throw "Unsupported platform: $Platform" }
    }
    
    Write-Header "Build Completed Successfully!"
}
catch {
    Write-Host "Build failed: $_" -ForegroundColor Red
    exit 1
}
