; 3D Model Painter NSIS Installer Script

!include "MUI2.nsh"
!include "FileFunc.nsh"

; Basic configuration
Name "3D Model Painter"
OutFile "3DModelPainter_Setup.exe"
Unicode True

; Default installation directory
InstallDir "$PROGRAMFILES\3D Model Painter"
InstallDirRegKey HKLM "Software\3D Model Painter" "Install_Dir"

; Request administrator privileges
RequestExecutionLevel admin

; Interface settings
!define MUI_ABORTWARNING
!define MUI_ICON "3DModelPainter.ico"
!define MUI_UNICON "3DModelPainter.ico"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\win.bmp"
!define MUI_UNWELCOMEFINISHPAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Wizard\win.bmp"
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${NSISDIR}\Contrib\Graphics\Header\win.bmp"
!define MUI_HEADERIMAGE_UNBITMAP "${NSISDIR}\Contrib\Graphics\Header\win.bmp"

; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE "README.txt"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN "$INSTDIR\3DModelPainter.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Launch 3D Model Painter"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\README.txt"
!define MUI_FINISHPAGE_SHOWREADME_TEXT "Show README"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_UNPAGE_FINISH

; Languages (must be after pages)
!insertmacro MUI_LANGUAGE "English"

; Installation section
Section "Install"
  SetOutPath "$INSTDIR"
  
  ; Application files
  File "3DModelPainter.exe"
  File "README.txt"
  File "cube.obj"
  File "cube.mtl"
  File "3DModelPainter.ico"
  
  ; Create uninstaller
  WriteUninstaller "$INSTDIR\uninstall.exe"
  
  ; Create Start Menu shortcuts
  CreateDirectory "$SMPROGRAMS\3D Model Painter"
  CreateShortcut "$SMPROGRAMS\3D Model Painter\3D Model Painter.lnk" "$INSTDIR\3DModelPainter.exe" "" "$INSTDIR\3DModelPainter.ico"
  CreateShortcut "$SMPROGRAMS\3D Model Painter\Uninstall.lnk" "$INSTDIR\uninstall.exe"
  
  ; Create Desktop shortcut
  CreateShortcut "$DESKTOP\3D Model Painter.lnk" "$INSTDIR\3DModelPainter.exe" "" "$INSTDIR\3DModelPainter.ico"
  
  ; Create Quick Launch shortcut (Pin to Taskbar)
  CreateShortcut "$QUICKLAUNCH\3D Model Painter.lnk" "$INSTDIR\3DModelPainter.exe" "" "$INSTDIR\3DModelPainter.ico"
  
  ; Add or remove programs registry entries
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DModelPainter" "DisplayName" "3D Model Painter"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DModelPainter" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DModelPainter" "DisplayIcon" "$INSTDIR\3DModelPainter.ico"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DModelPainter" "Publisher" "3D Model Painter"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DModelPainter" "DisplayVersion" "1.0.0"
  
  ; Get installation size
  ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
  IntFmt $0 "0x%08X" $0
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DModelPainter" "EstimatedSize" "$0"
  
  ; File associations for .obj files
  WriteRegStr HKCR ".obj\OpenWithProgids" "3DModelPainter.objfile" ""
  WriteRegStr HKCR "3DModelPainter.objfile" "" "3D Model OBJ File"
  WriteRegStr HKCR "3DModelPainter.objfile\DefaultIcon" "" "$INSTDIR\3DModelPainter.ico"
  WriteRegStr HKCR "3DModelPainter.objfile\shell\open\command" "" '"$INSTDIR\3DModelPainter.exe" "%1"'
  WriteRegStr HKCR "3DModelPainter.objfile\shell\edit\command" "" '"$INSTDIR\3DModelPainter.exe" "%1"'
  
  ; Register application capabilities for Windows search and Start Menu
  WriteRegStr HKLM "Software\3D Model Painter" "Install_Dir" "$INSTDIR"
  WriteRegStr HKLM "Software\RegisteredApplications" "3DModelPainter" "Software\3D Model Painter\Capabilities"
  WriteRegStr HKLM "Software\3D Model Painter\Capabilities" "ApplicationDescription" "A powerful 3D model painting application"
  WriteRegStr HKLM "Software\3D Model Painter\Capabilities" "ApplicationName" "3D Model Painter"
  WriteRegStr HKLM "Software\3D Model Painter\Capabilities\FileAssociations" ".obj" "3DModelPainter.objfile"
  
SectionEnd

; Uninstaller section
Section "Uninstall"
  ; Remove installed files
  Delete "$INSTDIR\3DModelPainter.exe"
  Delete "$INSTDIR\README.txt"
  Delete "$INSTDIR\cube.obj"
  Delete "$INSTDIR\cube.mtl"
  Delete "$INSTDIR\3DModelPainter.ico"
  Delete "$INSTDIR\uninstall.exe"
  
  ; Remove shortcuts
  Delete "$SMPROGRAMS\3D Model Painter\3D Model Painter.lnk"
  Delete "$SMPROGRAMS\3D Model Painter\Uninstall.lnk"
  Delete "$DESKTOP\3D Model Painter.lnk"
  Delete "$QUICKLAUNCH\3D Model Painter.lnk"
  
  ; Remove directories
  RMDir "$SMPROGRAMS\3D Model Painter"
  RMDir "$INSTDIR"
  
  ; Remove registry entries
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\3DModelPainter"
  DeleteRegKey HKLM "Software\3D Model Painter"
  DeleteRegKey HKCR "3DModelPainter.objfile"
  DeleteRegValue HKCR ".obj\OpenWithProgids" "3DModelPainter.objfile"
  DeleteRegValue HKLM "Software\RegisteredApplications" "3DModelPainter"
  
SectionEnd