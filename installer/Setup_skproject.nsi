; example2.nsi
;
; This script is based on example1.nsi, but it remember the directory,
; has uninstall support and (optionally) installs start menu shortcuts.
;
; It will install example2.nsi into a directory that the user selects,

;--------------------------------

!define APP "skproject"
!define VER "20210626"

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

AddBrandingImage right 320 13

;--------------------------------

; Pages

Page components "" showComponents
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

Function showComponents
  InitPluginsDir
  File      "/oname=$PLUGINSDIR\brand.bmp" "brand.bmp"
  SetBrandingImage "$PLUGINSDIR\brand.bmp"
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

  SetOutPath "$INSTDIR"
  File           "readme_SkProject.txt"
  File           "readme_SkWin.txt"

SectionEnd

; Optional section (can be disabled by the user)
Section    "SKWin (DM2 V4 PC-9821 exe)"
  SetOutPath "$INSTDIR\SKWIN"
  File           "..\skwin\SKWin.exe"
  SetOutPath "$INSTDIR\SKWIN\DATA"
  File           /oname=$OUTDIR\DUNGEON.DAT "..\skwin\data\DUNGEON_PC9821.dat"
  File           /oname=$OUTDIR\GRAPHICS.DAT "..\skwin\data\GRAPHICS_PC9821_v4.0.1_9821J13.dat"
  File           "..\skwin\data\GRAPHICS_PC9821_V4_CLASSIC.DAT"
  SetOutPath "$INSTDIR\SKWIN"
  File           "..\skwin\PubFiles\SkWin V4 Japanese*.bat"
  File           "..\skwin\PubFiles\SkWin V4 English*.bat"
  
  CreateShortcut "$DESKTOP\SKWin.lnk" \
                  "$OUTDIR\SKWin.exe"
SectionEnd

Section /o "SKWin Multilang (JP/EN/DE/FR/ES/IT)"
  SetOutPath "$INSTDIR\SKWIN"
  File           "..\skwin\PubFiles\SkWin*.bat"
SectionEnd

Section /o "SKWin Musics (HMP/midi)"
  SetOutPath "$INSTDIR\SKWIN\DATA"
  File           "..\skwin\data\*hmp*"
SectionEnd

;Section /o "SKWin Musics (WAV/ogg)"
;  SetOutPath "$INSTDIR\SKWIN\DATA"
;  File           "..\skwin\data\*ogg*"
;SectionEnd

Section /o "SKWin Beta Dungeon"
  SetOutPath "$INSTDIR\SKWIN"
  File           "..\skwin\SKWin.exe"
  File           "..\skwin\PubFiles\DM2 Beta SkWin V4*.bat"
  SetOutPath "$INSTDIR\SKWIN\data_beta"
  File           /oname=$OUTDIR\DUNGEON.DAT "..\skwin\data_beta\pcsk_beta.dat"
  File           "..\skwin\data_beta\GRAPHICS_PC9821_V4_CLASSIC.DAT"
SectionEnd


Section /o "Skull (DM2 V5 PC-DOS exe)"
  SetOutPath "$INSTDIR\SKULLWIN"
  File           "..\skullwin\Skull.exe"
  File           "..\skullwin\PubFiles\Skull V5 English (PCDOS)*.bat"
  File           "..\skullwin\PubFiles\Skull (2013) V5 English (PCDOS)*.bat"
  File           "..\skullwin\PubFiles\skullasm.exe"
  File           "..\skullwin\alleg42.dll"
  File           "..\skullwin\allegro_acodec*.dll"
  File           "..\skullwin\allegro_audio*.dll"
  File           "..\skullwin\allegro-5*.dll"
  File           "..\skullwin\img1.dat"
  File           "..\skullwin\mouse1.dat"
  File           "..\skullwin\mouse2.dat"
  File           "..\skullwin\music.bin"
  File           "..\skullwin\v1d39bc.dat"
  File           "..\skullwin\v1d296c.dat"
  File           "..\skullwin\v1d338c.dat"
  File           "..\skullwin\v1d653c.dat"
  File           "..\skullwin\v1d6802.dat"
  File           "..\skullwin\v1d7108.dat"
  File           "..\skullwin\xblitb.dat"
; Blitb + pmdata for skullasm
  File           "..\skullwin\PubFiles\blitb.dat"
  File           "..\skullwin\PubFiles\pmdata.bin"
  SetOutPath "$INSTDIR\SKULLWIN\DATA"
  File           "..\skullwin\data\*hmp*"
  File           /oname=$OUTDIR\DUNGEON.DAT "..\skullwin\PubFiles\pcsk_skullkeep.dat"
  File           /oname=$OUTDIR\GRAPHICS.DAT "..\skullwin\PubFiles\pcsk_graphics_v5.2c.dat"
  File           /oname=$OUTDIR\SONGLIST.DAT "..\skullwin\PubFiles\pcsk_songlist.dat"

  CreateShortcut "$DESKTOP\Skull.lnk" \
                  "$OUTDIR\Skull.exe"
SectionEnd

Section /o "Skull + Amiga Musics"
  SetOutPath "$INSTDIR\SKULLWIN"
  File           "..\skullwin\Skull.exe"
  File           "..\skullwin\PubFiles\Skull V5 English (Amiga Music)*.bat"
  File           "..\skullwin\PubFiles\skullasm.exe"
  File           "..\skullwin\alleg42.dll"
  File           "..\skullwin\allegro_acodec*.dll"
  File           "..\skullwin\allegro_audio*.dll"
  File           "..\skullwin\allegro-5*.dll"
  File           "..\skullwin\img1.dat"
  File           "..\skullwin\mouse1.dat"
  File           "..\skullwin\mouse2.dat"
  File           "..\skullwin\music.bin"
  File           "..\skullwin\v1d39bc.dat"
  File           "..\skullwin\v1d296c.dat"
  File           "..\skullwin\v1d338c.dat"
  File           "..\skullwin\v1d653c.dat"
  File           "..\skullwin\v1d6802.dat"
  File           "..\skullwin\v1d7108.dat"
  File           "..\skullwin\xblitb.dat"  
; Blitb + pmdata for skullasm
  File           "..\skullwin\PubFiles\blitb.dat"
  File           "..\skullwin\PubFiles\pmdata.bin"
  SetOutPath "$INSTDIR\SKULLWIN\DATA"
  File           "..\skullwin\data\sk*ogg*"
  File           "..\skullwin\data\MODLIST.DAT"
  File           /oname=$OUTDIR\DUNGEON.DAT "..\skullwin\PubFiles\pcsk_skullkeep.dat"
  File           /oname=$OUTDIR\GRAPHICS.DAT "..\skullwin\PubFiles\pcsk_graphics_v5.2c.dat"
  File           /oname=$OUTDIR\SONGLIST.DAT "..\skullwin\PubFiles\pcsk_songlist.dat"

  CreateShortcut "$DESKTOP\Skull.lnk" \
                  "$OUTDIR\Skull.exe"
SectionEnd



;Section /o            "data_csb"
;  SetOutPath "$INSTDIR\data_csb"
;  File       "..\skwin\data_csb\dungeon.dat"
;  File       "..\skwin\data_csb\GRAPHICS.dat"
;SectionEnd

;Section /o            "data_dm1"
;  SetOutPath "$INSTDIR\data_dm1"
;  File       "..\skwin\data_dm1\dungeon.dat"
;  File       "..\skwin\data_dm1\graphics.dat"
;SectionEnd

Section               "DM2GDED (Graphics/Resources editor)"
  SetOutPath "$INSTDIR\DM2GDED"
  File /r          "..\DM2GDED\PubFiles\*.*"
  File /r          "..\DM2GDED\res\App.ico"

  SetOutPath     "$INSTDIR\DM2GDED"
  CreateShortCut "$DESKTOP\DM2GDED.lnk" \
                  "$OUTDIR\DM2GDED.exe" \
                  "" \
                  "$OUTDIR\App.ico"
SectionEnd

Section               "DMDC2 (Dungeon/Map editor)"
  SetOutPath "$INSTDIR\DMDC2"
  File /r          "..\DMDC2\PubFiles\*.*"
  File             "..\DMDC2\res\DMDC2.ico"
  File             "..\DMDC2\DMDC2.exe"

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
  ; Remove SKWIN
  Delete "$INSTDIR\SKWIN\skwin.exe"
  RMDir /r "$INSTDIR\SKWIN"
  
  ; Remove SKULL
  Delete "$INSTDIR\SKULLWIN\*.exe"
  RMDir /r "$INSTDIR\SKULLWIN"

  ; Remove DMDC2
  Delete "$INSTDIR\DMDC2\skwin.exe"
  RMDir /r "$INSTDIR\DMDC2"
  RMDir "$INSTDIR\DMDC2"

  ; Remove DM2GDED
  Delete "$INSTDIR\DM2GDED\*"
  RMDir /r "$INSTDIR\DM2GDED"

  Delete "$INSTDIR\uninstall.exe"

  ; Remove shortcuts, if any

  ; Remove directories used
  RMDir "$INSTDIR"

SectionEnd
