# Windows Packaging for 3D Model Painter

This document explains the Windows packaging process for the 3D Model Painter application, including how to create Windows-specific builds and installers.

## Windows Build Process

The Windows build is handled through several specialized scripts that ensure proper integration with the Windows environment.

### Prerequisites

Before building the Windows package, ensure you have:

1. A Windows development environment or a cross-compiler for Windows
2. NSIS (Nullsoft Scriptable Install System) for creating the installer
3. Resource Compiler (rc.exe) for Windows resources

### Step 1: Windows-Specific Build

The application is built for Windows using the `windows_build.bat` script:

```batch
@echo off
echo ===================================
echo    Building 3D Model Painter for Windows
echo ===================================

:: Ensure build directories exist
if not exist build mkdir build
cd build

:: Build using CMake for Windows
cmake -G "MinGW Makefiles" ..
mingw32-make

:: Copy resources
copy ..\resources\*.* .

:: Return to root directory
cd ..

echo Build completed successfully!
echo Run the application with: build\3DModelPainter.exe
```

### Step 2: Creating Windows Resources

Windows-specific resources (icons, version info) are defined in `resources.rc`:

```c
// Microsoft Visual C++ generated resource script.
#include "resource.h"

#define APSTUDIO_READONLY_SYMBOLS
/////////////////////////////////////////////////////////////////////////////
//
// Generated from the TEXTINCLUDE 2 resource.
//
#include "winres.h"

/////////////////////////////////////////////////////////////////////////////
#undef APSTUDIO_READONLY_SYMBOLS

/////////////////////////////////////////////////////////////////////////////
// English (United States) resources

#if !defined(AFX_RESOURCE_DLL) || defined(AFX_TARG_ENU)
LANGUAGE LANG_ENGLISH, SUBLANG_ENGLISH_US

// Icon resource
IDI_APPICON            ICON                    "3DModelPainter.ico"

// Version information
VS_VERSION_INFO VERSIONINFO
 FILEVERSION 0,2,0,0
 PRODUCTVERSION 0,2,0,0
 FILEFLAGSMASK 0x3fL
#ifdef _DEBUG
 FILEFLAGS 0x1L
#else
 FILEFLAGS 0x0L
#endif
 FILEOS 0x40004L
 FILETYPE 0x1L
 FILESUBTYPE 0x0L
BEGIN
    BLOCK "StringFileInfo"
    BEGIN
        BLOCK "040904b0"
        BEGIN
            VALUE "CompanyName", "3D Model Painter"
            VALUE "FileDescription", "3D Model Painter Application"
            VALUE "FileVersion", "0.2.0.0"
            VALUE "InternalName", "3DModelPainter.exe"
            VALUE "LegalCopyright", "Copyright (C) 2023"
            VALUE "OriginalFilename", "3DModelPainter.exe"
            VALUE "ProductName", "3D Model Painter"
            VALUE "ProductVersion", "0.2.0.0"
        END
    END
    BLOCK "VarFileInfo"
    BEGIN
        VALUE "Translation", 0x409, 1200
    END
END

#endif    // English (United States) resources
```

### Step 3: Creating the Windows Installer

The NSIS installer script (`3DModelPainter.nsi`) defines how the application is packaged and installed on Windows systems:

```nsi
; 3D Model Painter Installer Script
; NSIS (Nullsoft Scriptable Install System) Script

; Settings
!define APPNAME "3D Model Painter"
!define COMPANYNAME "3D Model Painter"
!define DESCRIPTION "3D Model Painting Application"
!define VERSIONMAJOR 0
!define VERSIONMINOR 2
!define VERSIONBUILD 0
!define HELPURL "https://github.com/3D-Model-Painter/3D-Model-Painter/wiki"
!define UPDATEURL "https://github.com/3D-Model-Painter/3D-Model-Painter/releases"
!define ABOUTURL "https://github.com/3D-Model-Painter/3D-Model-Painter"

; Main Install Settings
Name "${APPNAME}"
InstallDir "$PROGRAMFILES64\${APPNAME}"
InstallDirRegKey HKLM "Software\${COMPANYNAME}\${APPNAME}" "Install_Dir"
OutFile "3DModelPainter-Setup-v${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}.exe"
RequestExecutionLevel admin

; Modern UI
!include "MUI2.nsh"

!define MUI_ABORTWARNING
!define MUI_ICON "3DModelPainter.ico"
!define MUI_UNICON "3DModelPainter.ico"

; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "LICENSE"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Languages
!insertmacro MUI_LANGUAGE "English"

; Installation sections
Section "3D Model Painter (required)" SecMain
    SectionIn RO
    SetOutPath $INSTDIR
    
    ; Main executable and necessary files
    File "3DModelPainter.exe"
    File "README.txt"
    File "cube.obj"
    File "cube.mtl"
    File "run_3d_model_painter.bat"
    File "run_3d_model_painter_demo.bat"
    File "update_from_github.sh"
    File ".version_info"
    
    ; Create uninstaller
    WriteUninstaller "$INSTDIR\uninstall.exe"
    
    ; Create start menu shortcut
    CreateDirectory "$SMPROGRAMS\${APPNAME}"
    CreateShortcut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\3DModelPainter.exe"
    CreateShortcut "$SMPROGRAMS\${APPNAME}\${APPNAME} Demo.lnk" "$INSTDIR\run_3d_model_painter_demo.bat"
    CreateShortcut "$SMPROGRAMS\${APPNAME}\Uninstall.lnk" "$INSTDIR\uninstall.exe"
    
    ; Create desktop shortcut
    CreateShortcut "$DESKTOP\${APPNAME}.lnk" "$INSTDIR\3DModelPainter.exe"
    
    ; Registry information for add/remove programs
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayName" "${APPNAME}"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "InstallLocation" "$\"$INSTDIR$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayIcon" "$\"$INSTDIR\3DModelPainter.ico$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "Publisher" "$\"${COMPANYNAME}$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "HelpLink" "$\"${HELPURL}$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "URLUpdateInfo" "$\"${UPDATEURL}$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "URLInfoAbout" "$\"${ABOUTURL}$\""
    WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "DisplayVersion" "$\"${VERSIONMAJOR}.${VERSIONMINOR}.${VERSIONBUILD}$\""
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "VersionMajor" ${VERSIONMAJOR}
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "VersionMinor" ${VERSIONMINOR}
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoModify" 1
    WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}" "NoRepair" 1
SectionEnd

; Optional section for documentation
Section "Documentation" SecDocs
    SetOutPath $INSTDIR\docs
    File /r "docs\*.*"
    
    CreateShortcut "$SMPROGRAMS\${APPNAME}\Documentation.lnk" "$INSTDIR\docs\index.html"
SectionEnd

; Optional section for example models
Section "Example Models" SecExamples
    SetOutPath $INSTDIR\examples
    File /r "examples\*.*"
SectionEnd

; Descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} "The main program files required to run ${APPNAME}."
    !insertmacro MUI_DESCRIPTION_TEXT ${SecDocs} "Documentation and help files."
    !insertmacro MUI_DESCRIPTION_TEXT ${SecExamples} "Example 3D models for testing and learning."
!insertmacro MUI_FUNCTION_DESCRIPTION_END

; Uninstaller Section
Section "Uninstall"
    ; Remove desktop and start menu shortcuts
    Delete "$DESKTOP\${APPNAME}.lnk"
    Delete "$SMPROGRAMS\${APPNAME}\*.lnk"
    RMDir "$SMPROGRAMS\${APPNAME}"
    
    ; Remove files and uninstaller
    Delete $INSTDIR\3DModelPainter.exe
    Delete $INSTDIR\README.txt
    Delete $INSTDIR\cube.obj
    Delete $INSTDIR\cube.mtl
    Delete $INSTDIR\run_3d_model_painter.bat
    Delete $INSTDIR\run_3d_model_painter_demo.bat
    Delete $INSTDIR\update_from_github.sh
    Delete $INSTDIR\.version_info
    Delete $INSTDIR\uninstall.exe
    
    ; Remove docs and examples if installed
    RMDir /r "$INSTDIR\docs"
    RMDir /r "$INSTDIR\examples"
    
    ; Remove install directory if empty
    RMDir "$INSTDIR"
    
    ; Remove registry keys
    DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APPNAME}"
    DeleteRegKey HKLM "Software\${COMPANYNAME}\${APPNAME}"
SectionEnd
```

### Step 4: Creating the Installer

The installer is created using the `create_installer.bat` script:

```batch
@echo off
echo ===================================
echo    Creating 3D Model Painter Installer
echo ===================================

:: Check if NSIS is installed
if not exist "%PROGRAMFILES(X86)%\NSIS\makensis.exe" (
    echo Error: NSIS is not installed or not found at the expected location.
    echo Please install NSIS from https://nsis.sourceforge.io/Download
    exit /b 1
)

:: Compile the installer
"%PROGRAMFILES(X86)%\NSIS\makensis.exe" 3DModelPainter.nsi

echo ===================================
echo Installer created successfully!
echo ===================================
```

## Windows Package Structure

The final Windows package includes:

1. **Main Executable**: `3DModelPainter.exe`
2. **Documentation**: User guides and reference
3. **Example Models**: Sample 3D models for testing
4. **Convenience Scripts**: Batch files for running demos
5. **Update Mechanism**: Scripts to check for and download updates
6. **Integration**: Start menu shortcuts, desktop icons, and proper registry entries

## Windows-Specific Features

The Windows version includes several special features:

1. **Taskbar Integration**: Proper application icon and taskbar presence
2. **File Associations**: `.obj` and `.mtl` files associated with the application
3. **Auto-Update**: Checks GitHub for updates automatically
4. **Console Handling**: Special handling for Windows console ANSI color support
5. **Resource Usage**: Efficient memory and graphics resource management for Windows

## Creating a Windows Release

To create a full Windows release package:

1. Run `create_windows_package.sh` (from Linux/macOS) or `windows_build.bat` (from Windows)
2. Follow the prompts to build the application
3. Run `create_installer.bat` to generate the installer
4. The final installer will be named `3DModelPainter-Setup-vX.Y.Z.exe`

## Troubleshooting Windows Builds

If you encounter issues with Windows packaging:

1. **Missing Dependencies**: Ensure MinGW and all required libraries are installed
2. **Icon Issues**: Verify the .ico file is a valid Windows icon format
3. **NSIS Errors**: Check NSIS installation and script syntax
4. **Path Issues**: Windows paths use backslashes, ensure all paths are correct
5. **Permission Issues**: Run batch files as Administrator if needed