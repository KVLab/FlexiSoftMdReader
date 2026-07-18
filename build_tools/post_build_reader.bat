@echo off
setlocal EnableExtensions

set "ROOT=%~1"
set "OUTDIR=%~2"
set "TARGET=%~3"
set "CONFIG=%~4"

if "%ROOT%"=="" (
  echo [post_build_reader] ERROR: missing ROOT argument
  exit /b 0
)

if "%OUTDIR%"=="" (
  echo [post_build_reader] ERROR: missing OUTDIR argument
  exit /b 0
)

if not "%ROOT:~-1%"=="\" set "ROOT=%ROOT%\"
if not "%OUTDIR:~-1%"=="\" set "OUTDIR=%OUTDIR%\"

echo [post_build_reader] ROOT=%ROOT%
echo [post_build_reader] OUTDIR=%OUTDIR%
echo [post_build_reader] TARGET=%TARGET%
echo [post_build_reader] CONFIG=%CONFIG%

if /I not "%CONFIG%"=="Release" (
  echo [post_build_reader] INFO: release packaging and installer-stage sync skipped for %CONFIG%
  exit /b 0
)

if not exist "%OUTDIR%" mkdir "%OUTDIR%" >nul 2>nul

rem ---------------------------------------------------------------------------
rem Build public MdReader release ZIP.
rem This is part of the public repo and uses only files from this repo/build.
rem ---------------------------------------------------------------------------

set "RELEASE_ZIP_SCRIPT=%ROOT%build_tools\post_build_release_zip.bat"

if exist "%RELEASE_ZIP_SCRIPT%" (
  echo [post_build_reader] running release ZIP script
  call "%RELEASE_ZIP_SCRIPT%" "%ROOT%" "%OUTDIR%" "%TARGET%" "%CONFIG%"
  if errorlevel 1 (
    echo [post_build_reader] WARNING: release ZIP script returned errorlevel %ERRORLEVEL%
    echo [post_build_reader] Build continues, but release ZIP may be incomplete.
  )
) else (
  echo [post_build_reader] WARNING: release ZIP script not found:
  echo [post_build_reader]          %RELEASE_ZIP_SCRIPT%
)

rem ---------------------------------------------------------------------------
rem Optional local-only external stage sync.
rem This script is not part of the public repo.
rem It must never be required for successful clean-clone build.
rem ---------------------------------------------------------------------------

set "PARENT=%ROOT%.."
set "LOCAL_SYNC=%PARENT%\sync_installer_stage.bat"

if exist "%LOCAL_SYNC%" (
  echo [post_build_reader] running optional local sync:
  echo [post_build_reader]   %LOCAL_SYNC% /reader
  call "%LOCAL_SYNC%" /reader
  if errorlevel 1 (
    echo [post_build_reader] WARNING: optional sync_installer_stage.bat returned errorlevel %ERRORLEVEL%
    echo [post_build_reader] Build continues.
  )
) else (
  echo [post_build_reader] INFO: optional local sync script not found, skipping:
  echo [post_build_reader]       %LOCAL_SYNC%
)

echo [post_build_reader] done
exit /b 0
