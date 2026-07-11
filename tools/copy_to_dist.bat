@echo off
REM Optional script to copy Release build to dist/ and release/ folders
REM This script is for local use only and is not part of the Visual Studio build process

setlocal enabledelayedexpansion

cd /d "%~dp0\.."

if not exist "FlexiSoftMdReader\Release\FlexiSoftMdReader.exe" (
    echo Error: Release executable not found
    echo Run Release build first: Select Release ^| Win32 and build
    exit /b 1
)

echo Copying Release executable...

if not exist "dist" mkdir dist
copy /Y "FlexiSoftMdReader\Release\FlexiSoftMdReader.exe" "dist\" >nul
if errorlevel 1 goto error

if not exist "release\v0.1.0" mkdir release\v0.1.0
copy /Y "FlexiSoftMdReader\Release\FlexiSoftMdReader.exe" "release\v0.1.0\" >nul
if errorlevel 1 goto error

echo.
echo Success! Executable copied to:
echo   - dist\FlexiSoftMdReader.exe
echo   - release\v0.1.0\FlexiSoftMdReader.exe
echo.
exit /b 0

:error
echo Error: Failed to copy executable
exit /b 1
