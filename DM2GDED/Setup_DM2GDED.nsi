; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

;--------------------------------

!define APP "DM2GDED"
!define VER "Rev.9"

; The name of the installer
Name "${APP} ${VER}"

; The file to write
OutFile "Setup_${APP}.exe"

; The default installation directory
InstallDir "$APPDATA\${APP}"

; Request application privileges for Windows Vista
RequestExecutionLevel user

;--------------------------------

; Pages

Page directory
Page instfiles

;--------------------------------

; The stuff to install
Section "" ;No components page, name is not important

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File    "Release\DM2GDED.exe"
  File /r "PubFiles\*.*"
  
  CreateShortCut "$DESKTOP\DM2GDED.lnk" "$INSTDIR\${APP}.exe"
  
  Exec '"$INSTDIR\${APP}.exe"'
  
SectionEnd ; end the section
