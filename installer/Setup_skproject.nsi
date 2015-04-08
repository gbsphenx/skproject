; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory,
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

!define APP "skproject"
!define VER "20150409"

!define TTL "skproject" ; uninst-title

; The name of the installer
Name "${APP} ${VER}"

; The file to write
OutFile "Setup_${APP}.exe"

; The default installation directory
InstallDir "$APPDATA\${APP}"

; Registry key to check for directory (so if you install again, it will
; overwrite the old one automatically)
InstallDirRegKey HKCU "Software\${APP}" "Install_Dir"

; Request application privileges for Windows Vista
RequestExecutionLevel user

XPStyle on

AddBrandingImage left 138

;--------------------------------

; Pages

Page components "" showComponents
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

Function showComponents
  SetBrandingImage "brand.bmp"
FunctionEnd

; http://nsis.sourceforge.net/Cool_CheckBox_Bitmaps
CheckBitmap "Checks_Sajal_Woody.bmp"

;--------------------------------

; The stuff to install
Section ""

  SectionIn RO

  ; Set output path to the installation directory.
  SetOutPath $INSTDIR

  ; Put file there

  ; Write the installation path into the registry
  WriteRegStr HKCU "Software\${APP}" "Install_Dir" "$INSTDIR"

  ; Write the uninstall keys for Windows
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}" "DisplayName" "${TTL}"
  WriteRegStr HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}" "NoModify" 1
  WriteRegDWORD HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}" "NoRepair" 1
  WriteUninstaller "uninstall.exe"

SectionEnd

; Optional section (can be disabled by the user)
Section /o "SKWin.Mod.Release"
  SetOutPath "$INSTDIR"
  File           "..\skwin\SKWin.Mod.Release.exe"
  
  CreateShortcut "$DESKTOP\SKWin.Mod.Release.lnk" \
                  "$OUTDIR\SKWin.Mod.Release.exe"
SectionEnd

Section    "SKWin.Release_EN_JP"
  SetOutPath "$INSTDIR"
  File           "..\skwin\SKWin.Release_EN_JP.exe"

  CreateShortcut "$DESKTOP\SKWin.Release_EN_JP.lnk" \
                  "$OUTDIR\SKWin.Release_EN_JP.exe"
SectionEnd

Section /o "SKWin.Release_JP_JP"
  SetOutPath "$INSTDIR"
  File           "..\skwin\SKWin.Release_JP_JP.exe"

  CreateShortcut "$DESKTOP\SKWin.Release_JP_JP.lnk" \
                  "$OUTDIR\SKWin.Release_JP_JP.exe"
SectionEnd

Section /o "SKWin.SuperMode"
  SetOutPath "$INSTDIR"
  File           "..\skwin\SKWin.SuperMode.exe"

  CreateShortcut "$DESKTOP\SKWin.SuperMode.lnk" \
                  "$OUTDIR\SKWin.SuperMode.exe"
SectionEnd

Section               "data"
  SetOutPath "$INSTDIR\data"
  File       "..\skwin\data\DUNGEON.DAT"
  File       "..\skwin\data\GRAPHICS.dat"
  File       "..\skwin\data\GRAPHICS_NJ.dat"
SectionEnd

Section /o            "data_csb"
  SetOutPath "$INSTDIR\data_csb"
  File       "..\skwin\data_csb\dungeon.dat"
  File       "..\skwin\data_csb\GRAPHICS.dat"
SectionEnd

Section /o            "data_dm1"
  SetOutPath "$INSTDIR\data_dm1"
  File       "..\skwin\data_dm1\dungeon.dat"
  File       "..\skwin\data_dm1\graphics.dat"
SectionEnd

Section               "DM2GDED (Graphics editor)"
  SetOutPath "$INSTDIR\DM2GDED"
  File /r          "..\DM2GDED\PubFiles\*.*"
  File /r          "..\DM2GDED\res\App.ico"

  SetOutPath     "$INSTDIR\DM2GDED"
  CreateShortCut "$DESKTOP\DM2GDED.lnk" \
                  "$OUTDIR\DM2GDED.exe" \
                  "" \
                  "$OUTDIR\App.ico"
SectionEnd

Section               "DMDC2 (Map editor)"
  SetOutPath "$INSTDIR\DMDC2"
  File /r          "..\DMDC2\PubFiles\*.*"
  File             "..\DMDC2\res\DMDC2.ico"

  SetOutPath "$INSTDIR\DMDC2\DM2"
  File /r          "..\DMDC2\DM2\*.*"

  SetOutPath "$INSTDIR\DMDC2\DM2Beta"
  File /r          "..\DMDC2\DM2Beta\*.*"

  SetOutPath     "$INSTDIR\DMDC2"
  CreateShortCut "$DESKTOP\DMDC2.lnk" \
                  "$OUTDIR\DMDC2.exe" \
                  "" \
                  "$OUTDIR\DMDC2.ico"
SectionEnd

Section ""
  IfErrors +2
    SetAutoClose true ; close if no error.
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"

  ; Remove registry keys
  DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP}"
  DeleteRegKey HKCU "Software\${APP}"

  ; Remove files and uninstaller
  Delete "$INSTDIR\skwin.*.exe"

  Delete "$INSTDIR\uninstall.exe"

  ; Remove shortcuts, if any

  ; Remove directories used
  RMDir "$INSTDIR"

SectionEnd
