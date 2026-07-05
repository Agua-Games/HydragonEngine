<#
    setup.ps1 - Hydragon Next dependency bootstrap.

    Does two things:
      1. Bootstraps vcpkg (for imgui/glfw3/glm/spdlog, and later usd) and wires
         it into Visual Studio via `vcpkg integrate install`.
      2. Downloads a prebuilt Google Filament SDK into Engine/ThirdParty/Filament.

    Filament is NOT available as a vcpkg port, so it is handled separately here.

    Usage (from a Developer PowerShell):
        ./scripts/setup.ps1                 # vcpkg + Filament
        ./scripts/setup.ps1 -SkipFilament   # only vcpkg
        ./scripts/setup.ps1 -FilamentVersion v1.72.1
#>
[CmdletBinding()]
param(
    [string]$VcpkgRoot = $env:VCPKG_ROOT,
    [string]$FilamentVersion = "v1.72.1",
    [switch]$SkipFilament
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot   # repo root

# --- 1. vcpkg -------------------------------------------------------------
if (-not $VcpkgRoot) {
    $VcpkgRoot = Join-Path $root "Engine\ThirdParty\vcpkg"
    if (-not (Test-Path $VcpkgRoot)) {
        Write-Host "Cloning vcpkg into $VcpkgRoot ..." -ForegroundColor Cyan
        git clone https://github.com/microsoft/vcpkg.git $VcpkgRoot
    }
}

$bootstrap = Join-Path $VcpkgRoot "bootstrap-vcpkg.bat"
if (Test-Path $bootstrap) {
    Write-Host "Bootstrapping vcpkg ..." -ForegroundColor Cyan
    & $bootstrap -disableMetrics
    & (Join-Path $VcpkgRoot "vcpkg.exe") integrate install
    [Environment]::SetEnvironmentVariable("VCPKG_ROOT", $VcpkgRoot, "User")
    Write-Host "vcpkg ready. VCPKG_ROOT set to $VcpkgRoot (new shells)." -ForegroundColor Green
} else {
    Write-Warning "bootstrap-vcpkg.bat not found under $VcpkgRoot"
}

# --- 2. Filament prebuilt -------------------------------------------------
if (-not $SkipFilament) {
    $filamentDir = Join-Path $root "Engine\ThirdParty\Filament"
    if (Test-Path (Join-Path $filamentDir "include")) {
        Write-Host "Filament already present at $filamentDir" -ForegroundColor Green
    } else {
        $asset = "filament-$FilamentVersion-windows.tgz"
        $url = "https://github.com/google/filament/releases/download/$FilamentVersion/$asset"
        $tmp = Join-Path $env:TEMP $asset
        Write-Host "Downloading Filament $FilamentVersion ..." -ForegroundColor Cyan
        Invoke-WebRequest -Uri $url -OutFile $tmp
        New-Item -ItemType Directory -Force -Path $filamentDir | Out-Null
        Write-Host "Extracting to $filamentDir ..." -ForegroundColor Cyan
        tar -xzf $tmp -C $filamentDir --strip-components=1
        Remove-Item $tmp -Force
        Write-Host "Filament installed. Set <UseFilament>true</UseFilament> to enable." -ForegroundColor Green
    }
}

Write-Host "`nSetup complete. Open Hydragon.sln in Visual Studio 2022." -ForegroundColor Green
