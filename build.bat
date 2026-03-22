@echo off
setlocal enabledelayedexpansion

:: Find VS Build Tools - check common install locations
set "VCVARS="
for %%v in (2022 2019 2017) do (
    for %%e in (BuildTools Community Professional Enterprise) do (
        set "try=C:\Program Files (x86)\Microsoft Visual Studio\%%v\%%e\VC\Auxiliary\Build\vcvars32.bat"
        if exist "!try!" if "!VCVARS!"=="" set "VCVARS=!try!"
    )
)

if "!VCVARS!"=="" (
    echo ERROR: VS Build Tools not found.
    echo Install from: https://visualstudio.microsoft.com/visual-cpp-build-tools/
    echo Make sure to select "Desktop development with C++" workload.
    exit /b 1
)

echo Using: !VCVARS!
call "!VCVARS!" >nul 2>&1

if not exist build mkdir build

:: Build the proxy DLL targeting x86 (Civ 5 is 32-bit)
cl /nologo /LD /MD /O2 /W3 ^
    src\dllmain.cpp src\matchmaking_hook.cpp ^
    /link /DEF:steam_api.def /OUT:build\steam_api.dll ^
    /MACHINE:X86 kernel32.lib

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo BUILD FAILED
    exit /b 1
)

echo.
echo Build succeeded: build\steam_api.dll
echo.
echo To deploy:
echo   1. Go to: "C:\Program Files (x86)\Steam\steamapps\common\Sid Meier's Civilization V\"
echo   2. Rename steam_api.dll  -^>  steam_api_orig.dll
echo   3. Copy build\steam_api.dll into that folder
echo.
echo To uninstall: delete steam_api.dll, rename steam_api_orig.dll back.
