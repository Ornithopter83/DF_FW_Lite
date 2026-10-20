@echo off
setlocal

for %%I in ("%~dp0..") do set "DF_REPO_ROOT=%%~fI"
set "DF_BUILD_SCRIPT=%DF_REPO_ROOT%\tools\build-testmodule.cmd"
set "DF_ARTIFACT=%DF_REPO_ROOT%\bin\testmodule\Release\DFTestModuleLite.exe"
set "DF_PUBLISH_DIR=%DF_REPO_ROOT%\bin\TM"

echo [DF TestModule] Release x64 publish starting...
call "%DF_BUILD_SCRIPT%" Release
set "DF_BUILD_RESULT=%ERRORLEVEL%"

if not "%DF_BUILD_RESULT%"=="0" (
    echo.
    echo [DF TestModule] Publish failed. Exit code: %DF_BUILD_RESULT%
    pause
    exit /b %DF_BUILD_RESULT%
)

if not exist "%DF_ARTIFACT%" (
    echo [ERROR] TestModule single-file executable not found: "%DF_ARTIFACT%"
    pause
    exit /b 20
)

powershell.exe -NoProfile -NonInteractive -ExecutionPolicy Bypass -File "%DF_REPO_ROOT%\tools\remove-output-directory.ps1" -Target "%DF_PUBLISH_DIR%"
if errorlevel 1 (
    echo [ERROR] Unable to prepare TestModule publish directory.
    pause
    exit /b 8
)
mkdir "%DF_PUBLISH_DIR%"
if errorlevel 1 (
    echo [ERROR] Unable to create TestModule publish directory.
    pause
    exit /b 1
)
copy /y "%DF_ARTIFACT%" "%DF_PUBLISH_DIR%\DFTestModuleLite.exe" >nul
if errorlevel 1 (
    echo [ERROR] Unable to publish TestModule executable.
    pause
    exit /b 1
)

echo.
echo [DF TestModule] Build and single-file publish completed successfully.
echo [OUTPUT] "%DF_PUBLISH_DIR%\DFTestModuleLite.exe"
pause
exit /b 0
