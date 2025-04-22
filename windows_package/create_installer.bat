@echo off
echo ================================================
echo Creating Installer for 3D Model Painter
echo ================================================

rem Check if the executable exists
if not exist "3DModelPainter.exe" (
    echo Error: 3DModelPainter.exe not found!
    echo Please run windows_build.bat first to compile the application.
    goto end
)

rem Check if NSIS is installed
echo Checking for NSIS installation...
reg query "HKLM\SOFTWARE\NSIS" >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    reg query "HKCU\SOFTWARE\NSIS" >nul 2>&1
)

if %ERRORLEVEL% NEQ 0 (
    echo NSIS (Nullsoft Scriptable Install System) is not installed.
    echo Please download and install NSIS from https://nsis.sourceforge.io/Download
    echo After installation, run this script again.
    goto end
)

rem Locate makensis.exe
set NSIS_FOUND=0
for %%p in (
    "%PROGRAMFILES%\NSIS\makensis.exe"
    "%PROGRAMFILES(x86)%\NSIS\makensis.exe"
) do (
    if exist %%p (
        set NSIS_EXE=%%p
        set NSIS_FOUND=1
    )
)

if %NSIS_FOUND% EQU 0 (
    echo Could not locate makensis.exe
    echo Please make sure NSIS is installed correctly.
    goto end
)

echo Found NSIS at: %NSIS_EXE%
echo Building installer...

rem Run NSIS to build the installer
"%NSIS_EXE%" 3DModelPainter.nsi

if %ERRORLEVEL% NEQ 0 (
    echo Failed to create installer. Please check the error messages above.
    goto end
)

echo ================================================
echo Installer created successfully!
echo The installer is available at: 3DModelPainter_Setup.exe
echo ================================================
echo.
echo You can now distribute this installer to your users.
echo When run, it will:
echo 1. Install 3D Model Painter to the user's Program Files
echo 2. Create Start Menu and Desktop shortcuts
echo 3. Add the application to the Windows taskbar
echo 4. Register the application for Add/Remove Programs
echo.

:end
pause