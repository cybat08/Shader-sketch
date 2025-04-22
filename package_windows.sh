#!/bin/bash
# Script to package the application for Windows distribution

# Text formatting
BOLD="\033[1m"
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
BLUE="\033[34m"
CYAN="\033[36m"
RESET="\033[0m"

echo -e "${BOLD}${CYAN}===== Creating Windows Package for 3D Model Painter =====${RESET}"

# Create package directory
mkdir -p windows_package

# Copy necessary files
cp EnhancedModelPainter.cpp windows_package/
cp CMakeLists_windows.txt windows_package/CMakeLists.txt
cp run_3d_model_painter.bat windows_package/
cp run_3d_model_painter_demo.bat windows_package/
cp windows_readme.txt windows_package/readme.txt

# Create a Windows build script to be included in the package
cat > windows_package/build.bat << 'EOF'
@echo off
echo Building 3D Model Painter...

rem Check if g++ is available
where g++ >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Error: g++ compiler not found.
    echo Please install MinGW-w64 and ensure g++ is in your PATH.
    pause
    exit /b 1
)

rem Compile the application
g++ -std=c++17 EnhancedModelPainter.cpp -o 3DModelPainter.exe -static

if %ERRORLEVEL% NEQ 0 (
    echo Failed to build 3D Model Painter.
    pause
    exit /b 1
)

echo Build successful! You can now run the application using run_3d_model_painter.bat
pause
EOF

# Create an NSIS installer script (for those who want to create an installer)
cat > windows_package/installer.nsi << 'EOF'
; 3D Model Painter Installer Script
; NSIS Modern User Interface

!include "MUI2.nsh"
!include "LogicLib.nsh"

; Application information
Name "3D Model Painter"
OutFile "3DModelPainter_Setup.exe"
InstallDir "$PROGRAMFILES\3D Model Painter"
InstallDirRegKey HKCU "Software\3D Model Painter" ""

; Interface settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "readme.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Languages
!insertmacro MUI_LANGUAGE "English"

; Installer sections
Section "3D Model Painter" SecMain
    SetOutPath "$INSTDIR"
    
    ; Application files
    File "3DModelPainter.exe"
    File "run_3d_model_painter.bat"
    File "run_3d_model_painter_demo.bat"
    File "readme.txt"
    
    ; Create uninstaller
    WriteUninstaller "$INSTDIR\Uninstall.exe"
    
    ; Create start menu shortcuts
    CreateDirectory "$SMPROGRAMS\3D Model Painter"
    CreateShortcut "$SMPROGRAMS\3D Model Painter\3D Model Painter.lnk" "$INSTDIR\run_3d_model_painter.bat"
    CreateShortcut "$SMPROGRAMS\3D Model Painter\3D Model Painter Demo.lnk" "$INSTDIR\run_3d_model_painter_demo.bat"
    CreateShortcut "$SMPROGRAMS\3D Model Painter\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
    
    ; Registry information for uninstall
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3D Model Painter" "DisplayName" "3D Model Painter"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3D Model Painter" "UninstallString" "$INSTDIR\Uninstall.exe"
SectionEnd

; Uninstaller section
Section "Uninstall"
    ; Remove files
    Delete "$INSTDIR\3DModelPainter.exe"
    Delete "$INSTDIR\run_3d_model_painter.bat"
    Delete "$INSTDIR\run_3d_model_painter_demo.bat"
    Delete "$INSTDIR\readme.txt"
    Delete "$INSTDIR\Uninstall.exe"
    
    ; Remove shortcuts
    Delete "$SMPROGRAMS\3D Model Painter\3D Model Painter.lnk"
    Delete "$SMPROGRAMS\3D Model Painter\3D Model Painter Demo.lnk"
    Delete "$SMPROGRAMS\3D Model Painter\Uninstall.lnk"
    RMDir "$SMPROGRAMS\3D Model Painter"
    
    ; Remove directory
    RMDir "$INSTDIR"
    
    ; Remove registry entries
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3D Model Painter"
SectionEnd
EOF

# Create a README for the package
cat > windows_package/README_PACKAGE.md << 'EOF'
# 3D Model Painter - Windows Package

This package contains everything needed to build and run 3D Model Painter on Windows.

## Contents

- `EnhancedModelPainter.cpp` - Source code file
- `CMakeLists.txt` - CMake build file (if using CMake)
- `build.bat` - Script to compile the application with MinGW
- `run_3d_model_painter.bat` - Script to run the application
- `run_3d_model_painter_demo.bat` - Script to run the demo
- `readme.txt` - User instructions
- `installer.nsi` - NSIS installer script (optional)

## Building Instructions

### Option 1: Using the build script

1. Install MinGW-w64 with g++ compiler
2. Add MinGW bin directory to your PATH
3. Run `build.bat`

### Option 2: Using CMake

1. Install CMake and MinGW-w64
2. Open Command Prompt and navigate to this directory
3. Run:
   ```
   mkdir build
   cd build
   cmake .. -G "MinGW Makefiles"
   cmake --build .
   ```

### Option 3: Manual compilation

1. Install MinGW-w64 with g++ compiler
2. Open Command Prompt and navigate to this directory
3. Run:
   ```
   g++ -std=c++17 EnhancedModelPainter.cpp -o 3DModelPainter.exe -static
   ```

## Creating an Installer (Optional)

1. Install NSIS (Nullsoft Scriptable Install System)
2. Right-click on `installer.nsi` and select "Compile NSIS Script"
3. This will create a setup executable

## Running the Application

After building the application:

1. Double-click `run_3d_model_painter.bat` to run the full application
2. Double-click `run_3d_model_painter_demo.bat` to run the demo mode
EOF

# Create a zip package
echo -e "${BLUE}Creating Windows package...${RESET}"
cd windows_package
zip -r ../3DModelPainter_Windows_Source.zip *
cd ..

echo -e "${BOLD}${GREEN}Windows package created successfully!${RESET}"
echo -e "${CYAN}Package available at: 3DModelPainter_Windows_Source.zip${RESET}"
echo -e "${YELLOW}Note: The user will need to compile the application on their Windows system.${RESET}"
echo -e "${YELLOW}      The package includes instructions and build scripts.${RESET}"