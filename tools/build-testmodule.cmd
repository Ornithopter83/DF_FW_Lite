@echo off
setlocal
set "DF_TM_CONFIGURATION=%~1"
if not defined DF_TM_CONFIGURATION set "DF_TM_CONFIGURATION=Debug"

if /I "%DF_TM_CONFIGURATION%"=="Debug" (
    dotnet build "%~dp0..\testModule\DFTestModule.csproj" -c Debug -p:Platform=x64
    exit /b %ERRORLEVEL%
)
if /I "%DF_TM_CONFIGURATION%"=="Release" (
    dotnet publish "%~dp0..\testModule\DFTestModule.csproj" -c Release -r win-x64 --self-contained true -p:PublishSingleFile=true -p:Platform=x64 -o "%~dp0..\bin\testmodule\Release"
    exit /b %ERRORLEVEL%
)
echo [ERROR] Usage: build-testmodule.cmd [Debug^|Release]
exit /b 2
