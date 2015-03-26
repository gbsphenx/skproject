; example1.nsi
;
; This script is perhaps one of the simplest NSIs you can make. All of the
; optional settings are left to their default settings. The installer simply 
; prompts the user asking them where to install, and drops a copy of example1.nsi
; there. 

;--------------------------------

!define APP "DMDC2"
!define VER "Rev.10+"

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
  File    "Release\DMDC2.exe"
  
  File /r "PubFiles\*.*"
  
  SetOutPath "$INSTDIR\Hint_Actu"
  File /r             "Hint_Actu\*.*"

  SetOutPath "$INSTDIR\DM2"
  File /r             "DM2\*.*"

  SetOutPath "$INSTDIR\DM2Beta"
  File /r             "DM2Beta\*.*"

  SetOutPath "$INSTDIR"

  CreateShortCut "$DESKTOP\DMDC2.lnk" "$INSTDIR\${APP}.exe"
  
SectionEnd ; end the section

Section ""
  Exec '"$INSTDIR\${APP}.exe"'
SectionEnd

Section ""
  IfErrors +2
    SetAutoClose true
SectionEnd
