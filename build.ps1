$ErrorActionPreference = 'Stop'

$toolRoot = Join-Path $env:LOCALAPPDATA 'Programs\STM32Toolchain'
$armRoot = Join-Path $toolRoot 'arm-gnu-toolchain-15.2.rel1-mingw-w64-x86_64-arm-none-eabi'
$cmakeBin = Join-Path $toolRoot 'cmake\bin'
$ninjaBin = Join-Path $toolRoot 'ninja'

if (-not (Test-Path (Join-Path $armRoot 'bin\arm-none-eabi-gcc.exe'))) {
    throw "ARM GCC was not found at $armRoot. See README.MD for setup details."
}
if (-not (Test-Path (Join-Path $cmakeBin 'cmake.exe'))) {
    throw "CMake was not found at $cmakeBin. See README.MD for setup details."
}
if (-not (Test-Path (Join-Path $ninjaBin 'ninja.exe'))) {
    throw "Ninja was not found at $ninjaBin. See README.MD for setup details."
}

$env:STM32_ARM_GCC_ROOT = $armRoot
$env:PATH = "$cmakeBin;$ninjaBin;$(Join-Path $armRoot 'bin');$env:PATH"

cmake --preset stm32-debug
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

cmake --build --preset stm32-debug
exit $LASTEXITCODE
