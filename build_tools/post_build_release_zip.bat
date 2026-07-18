@echo off
setlocal EnableExtensions

rem Creates the public FlexiSoftMdReader release package from the current build.
rem
rem Expected layout:
rem   parent\
rem     FlexiSoftMdReader\
rem       README.md
rem       examples\
rem       Release\FlexiSoftMdReader.exe
rem       build_tools\post_build_release_zip.bat
rem     FlexiSoftMdReader_Release\
rem
rem Output:
rem   parent\FlexiSoftMdReader_Release\FlexiSoftMdReader_v1.0.1_win32_xp\
rem   parent\FlexiSoftMdReader_Release\FlexiSoftMdReader_v1.0.1_win32_xp.zip
rem   parent\FlexiSoftMdReader_Release\SHA256SUMS.txt

set "ROOT=%~1"
set "OUTDIR=%~2"
set "TARGET=%~3"
set "CONFIG=%~4"

if "%ROOT%"=="" (
  rem Script is in root\build_tools, so root is one level above this script directory.
  set "ROOT=%~dp0.."
)

for %%I in ("%ROOT%") do set "ROOT=%%~fI"
if not "%ROOT:~-1%"=="\" set "ROOT=%ROOT%\"

set "PARENT=%ROOT%.."
for %%I in ("%PARENT%") do set "PARENT=%%~fI"
if not "%PARENT:~-1%"=="\" set "PARENT=%PARENT%\"

set "SRC_README=%ROOT%README.md"
set "SRC_EXAMPLES=%ROOT%examples"
set "SRC_EXE=%ROOT%Release\FlexiSoftMdReader.exe"

set "RELEASE_ROOT=%PARENT%FlexiSoftMdReader_Release"
set "RELEASE_VERSION=1.0.1"
set "PACKAGE_NAME=FlexiSoftMdReader_v%RELEASE_VERSION%_win32_xp"
set "PACKAGE_DIR=%RELEASE_ROOT%\%PACKAGE_NAME%"
set "ZIP_FILE=%RELEASE_ROOT%\%PACKAGE_NAME%.zip"
set "SHA_FILE=%RELEASE_ROOT%\SHA256SUMS.txt"

echo.
echo === FlexiSoftMdReader release ZIP build ===
echo ROOT:         "%ROOT%"
echo PARENT:       "%PARENT%"
echo RELEASE_ROOT: "%RELEASE_ROOT%"
echo PACKAGE_DIR:  "%PACKAGE_DIR%"
echo ZIP_FILE:     "%ZIP_FILE%"
echo SHA_FILE:     "%SHA_FILE%"
echo.

set "FAILED=0"

call :RequireFile "%SRC_README%" "README.md"
call :RequireDir "%SRC_EXAMPLES%" "examples"
call :RequireFile "%SRC_EXE%" "Release\FlexiSoftMdReader.exe"

if not "%FAILED%"=="0" (
  echo ERROR: Required source items are missing. Release ZIP was not created.
  exit /b 1
)

if not exist "%RELEASE_ROOT%" mkdir "%RELEASE_ROOT%" >nul 2>nul
if errorlevel 1 (
  echo ERROR: Cannot create release root:
  echo        "%RELEASE_ROOT%"
  exit /b 1
)

if exist "%PACKAGE_DIR%" (
  echo Cleaning old package directory...
  rmdir /s /q "%PACKAGE_DIR%" >nul 2>nul
)

mkdir "%PACKAGE_DIR%" >nul 2>nul
if errorlevel 1 (
  echo ERROR: Cannot create package directory:
  echo        "%PACKAGE_DIR%"
  exit /b 1
)

echo Copying README.md...
copy /y "%SRC_README%" "%PACKAGE_DIR%\README.md" >nul
if errorlevel 1 (
  echo ERROR: Copy failed: README.md
  exit /b 1
)

echo Copying FlexiSoftMdReader.exe...
copy /y "%SRC_EXE%" "%PACKAGE_DIR%\FlexiSoftMdReader.exe" >nul
if errorlevel 1 (
  echo ERROR: Copy failed: FlexiSoftMdReader.exe
  exit /b 1
)

echo Syncing examples...
robocopy "%SRC_EXAMPLES%" "%PACKAGE_DIR%\examples" /MIR /R:1 /W:1 /NFL /NDL /NJH /NJS /NP >nul
set "RC=%ERRORLEVEL%"
if %RC% GEQ 8 (
  echo ERROR: Robocopy failed for examples. Exit code: %RC%
  exit /b 1
)

if exist "%ZIP_FILE%" (
  echo Removing old ZIP...
  del /f /q "%ZIP_FILE%" >nul 2>nul
)

echo Creating ZIP...
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$ErrorActionPreference='Stop';" ^
  "$package='%PACKAGE_DIR%';" ^
  "$zip='%ZIP_FILE%';" ^
  "Compress-Archive -Force -LiteralPath $package -DestinationPath $zip"

if errorlevel 1 (
  echo ERROR: ZIP creation failed.
  exit /b 1
)

if not exist "%ZIP_FILE%" (
  echo ERROR: ZIP file was not created:
  echo        "%ZIP_FILE%"
  exit /b 1
)

echo Creating SHA256SUMS.txt...
powershell -NoProfile -ExecutionPolicy Bypass -Command ^
  "$ErrorActionPreference='Stop';" ^
  "$exe='%PACKAGE_DIR%\FlexiSoftMdReader.exe';" ^
  "$zip='%ZIP_FILE%';" ^
  "$sha='%SHA_FILE%';" ^
  "$exeHash=(Get-FileHash -Algorithm SHA256 -LiteralPath $exe).Hash.ToUpperInvariant();" ^
  "$zipHash=(Get-FileHash -Algorithm SHA256 -LiteralPath $zip).Hash.ToUpperInvariant();" ^
  "$content=($exeHash + '  FlexiSoftMdReader.exe' + [Environment]::NewLine + $zipHash + '  %PACKAGE_NAME%.zip' + [Environment]::NewLine);" ^
  "[IO.File]::WriteAllText($sha, $content, [Text.Encoding]::ASCII);"

if errorlevel 1 (
  echo ERROR: SHA256SUMS.txt creation failed.
  exit /b 1
)

echo.
echo Release package created:
echo   "%PACKAGE_DIR%"
echo   "%ZIP_FILE%"
echo   "%SHA_FILE%"
echo.
type "%SHA_FILE%"
echo.
echo Done.
exit /b 0

:RequireFile
if not exist "%~1" (
  echo MISSING file: %~2
  echo               "%~1"
  set "FAILED=1"
)
goto :eof

:RequireDir
if not exist "%~1\" (
  echo MISSING directory: %~2
  echo                    "%~1"
  set "FAILED=1"
)
goto :eof
