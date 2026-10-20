@echo off
setlocal

for %%I in ("%~dp0..") do set "DF_REPO_ROOT=%%~fI"
set "DF_BUILD_SCRIPT=%DF_REPO_ROOT%\tools\build-rod.cmd"

echo [DF Rod] Release x64 build starting...
call "%DF_BUILD_SCRIPT%" Release x64
set "DF_BUILD_RESULT=%ERRORLEVEL%"

if not "%DF_BUILD_RESULT%"=="0" (
    echo.
    echo [DF Rod] Build failed. Exit code: %DF_BUILD_RESULT%
    pause
    exit /b %DF_BUILD_RESULT%
)

call "%DF_REPO_ROOT%\tools\firmware-versions.cmd"
if errorlevel 1 (
    pause
    exit /b 12
)

echo.
echo [DF Rod] Build and 4-file publish completed successfully.
echo [OUTPUT] "%DF_REPO_ROOT%\bin\Rod\%DF_ROD_VERSION%"
pause
exit /b 0
