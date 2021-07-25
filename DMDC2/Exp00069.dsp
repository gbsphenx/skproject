# Microsoft Developer Studio Project File - Name="Exp00069" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Exp00069 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Exp00069.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Exp00069.mak" CFG="Exp00069 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Exp00069 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Exp00069 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Exp00069 - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /I "./DO2" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D DWORD_PTR=DWORD /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /fo"Release/DMDC2.res" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Release/DMDC2.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Release/DMDC2.exe"

!ELSEIF  "$(CFG)" == "Exp00069 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "./DO2" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D DWORD_PTR=DWORD /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Exp00069 - Win32 Release"
# Name "Exp00069 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActuHintWebFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ActuHintWebVw.cpp
# End Source File
# Begin Source File

SOURCE=.\AfxDeclStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\AHRichEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BMO.cpp
# End Source File
# Begin Source File

SOURCE=.\BuildLabuDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CompatDC.cpp
# End Source File
# Begin Source File

SOURCE=.\CSVrw.cpp
# End Source File
# Begin Source File

SOURCE=.\CurDir.cpp
# End Source File
# Begin Source File

SOURCE=.\CustRichEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DDAT.cpp
# End Source File
# Begin Source File

SOURCE=.\DDAT2.cpp
# End Source File
# Begin Source File

SOURCE=.\DDATUndo.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBits.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2EditTilePropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2HierMapView.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2ItemListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2NameDic.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2NameResolver.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2Ornate1ComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2OrnateIdxMig.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2OrnatePvw.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2OrnateTreeDic.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2RPtr.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2ScrollRec.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2SelOrnateImage.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2TeVol.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DM2TextRec.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2WorldShiftDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DMCIBar.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DMCITree.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DMDBBT.cpp
# End Source File
# Begin Source File

SOURCE=.\DMDBRMask.cpp
# End Source File
# Begin Source File

SOURCE=.\DMEdCtx.cpp
# End Source File
# Begin Source File

SOURCE=.\DMLvMetrBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DMLvSelWnd2.cpp
# End Source File
# Begin Source File

SOURCE=.\DMObBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DMObTreeData.cpp
# End Source File
# Begin Source File

SOURCE=.\DMObTreeView.cpp
# End Source File
# Begin Source File

SOURCE=.\DMRecFmt.cpp
# End Source File
# Begin Source File

SOURCE=.\DMTarTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\DMTeleLocFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\DMTeleLocVw.cpp
# End Source File
# Begin Source File

SOURCE=.\DMTextEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\DO2\DO2PathStr.cpp
# End Source File
# Begin Source File

SOURCE=.\EditDBItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditDBItemExc.cpp
# End Source File
# Begin Source File

SOURCE=.\EditDM2FileDescDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditDMFileDescDlg.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\EditDMItemSubDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditDMRootSel.cpp
# End Source File
# Begin Source File

SOURCE=.\EditLvDesc3Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditLvDescDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIActu1.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIActu2Page.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIActu3Page.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIActuHintPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIChestPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIClothPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPICreaturePage.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIDoorPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIMiscPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPINoPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIPotionPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIRTSink.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIScrollPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPITele.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIText2Page.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPITextPage.cpp
# End Source File
# Begin Source File

SOURCE=.\EdPIWeapon.cpp
# End Source File
# Begin Source File

SOURCE=.\EdTileDefaultDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EdTileDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EdTileDoorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EdTileFalseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EdTilePitDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EdTileStairsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EdTileTeleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Exp00069.cpp
# End Source File
# Begin Source File

SOURCE=.\Exp00069.rc
# End Source File
# Begin Source File

SOURCE=.\Exp00069Doc.cpp
# End Source File
# Begin Source File

SOURCE=.\Exp00069View.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ExtBM.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\FinderComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\FinderDock.cpp
# End Source File
# Begin Source File

SOURCE=.\FinderFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\FinderInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FinderVw.cpp
# End Source File
# Begin Source File

SOURCE=.\FM.cpp
# End Source File
# Begin Source File

SOURCE=.\GetBackButton.cpp
# End Source File
# Begin Source File

SOURCE=.\HexEditCB.cpp
# End Source File
# Begin Source File

SOURCE=.\HexEditDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\HexEditFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\HexEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\HintOfStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\HugHug.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MkStatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\NamedWndBox.cpp
# End Source File
# Begin Source File

SOURCE=.\OffSc.cpp
# End Source File
# Begin Source File

SOURCE=.\OSP.cpp
# End Source File
# Begin Source File

SOURCE=.\RCI.cpp
# End Source File
# Begin Source File

SOURCE=.\RCT2.cpp
# End Source File
# Begin Source File

SOURCE=.\scbarcf.cpp
# End Source File
# Begin Source File

SOURCE=.\scbarg.cpp
# End Source File
# Begin Source File

SOURCE=.\Serialized.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowcaseDock.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowcaseFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowcaseLv.cpp
# End Source File
# Begin Source File

SOURCE=.\sizecbar.cpp
# End Source File
# Begin Source File

SOURCE=.\SizeVuff.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\SplashVw.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitStr.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Str2Clipbrd.cpp
# End Source File
# Begin Source File

SOURCE=.\Ternary.cpp
# End Source File
# Begin Source File

SOURCE=.\TextInputDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TriViewNe.cpp
# End Source File
# Begin Source File

SOURCE=.\TsDataObject.cpp
# End Source File
# Begin Source File

SOURCE=.\TsDataSource.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActuF.h
# End Source File
# Begin Source File

SOURCE=.\ActuHintWebFrm.h
# End Source File
# Begin Source File

SOURCE=.\ActuHintWebVw.h
# End Source File
# Begin Source File

SOURCE=.\ActuW.h
# End Source File
# Begin Source File

SOURCE=.\AfxDeclStatic.h
# End Source File
# Begin Source File

SOURCE=.\AHRichEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BMO.h
# End Source File
# Begin Source File

SOURCE=.\BMODID.h
# End Source File
# Begin Source File

SOURCE=.\CompatDC.h
# End Source File
# Begin Source File

SOURCE=.\CSVrw.h
# End Source File
# Begin Source File

SOURCE=.\CurDir.h
# End Source File
# Begin Source File

SOURCE=.\CustRichEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DDAT.h
# End Source File
# Begin Source File

SOURCE=.\DDAT2.h
# End Source File
# Begin Source File

SOURCE=.\DDATDef.h
# End Source File
# Begin Source File

SOURCE=.\DDATLD.h
# End Source File
# Begin Source File

SOURCE=.\DDATUndo.h
# End Source File
# Begin Source File

SOURCE=.\DDATx.h
# End Source File
# Begin Source File

SOURCE=.\DIBits.h
# End Source File
# Begin Source File

SOURCE=.\DM2EditTilePropDlg.h
# End Source File
# Begin Source File

SOURCE=.\DM2HierMapView.h
# End Source File
# Begin Source File

SOURCE=.\DM2ItemListBox.h
# End Source File
# Begin Source File

SOURCE=.\DM2NameDic.h
# End Source File
# Begin Source File

SOURCE=.\DM2NameResolver.h
# End Source File
# Begin Source File

SOURCE=.\DM2Ornate1ComboBox.h
# End Source File
# Begin Source File

SOURCE=.\DM2OrnateIdxMig.h
# End Source File
# Begin Source File

SOURCE=.\DM2OrnatePvw.h
# End Source File
# Begin Source File

SOURCE=.\DM2OrnateTreeDic.h
# End Source File
# Begin Source File

SOURCE=.\DM2RPtr.h
# End Source File
# Begin Source File

SOURCE=.\DM2ScrollRec.h
# End Source File
# Begin Source File

SOURCE=.\DM2SelOrnateImage.h
# End Source File
# Begin Source File

SOURCE=.\DM2TeVol.h
# End Source File
# Begin Source File

SOURCE=.\DM2TextRec.h
# End Source File
# Begin Source File

SOURCE=.\DM2WorldShiftDlg.h
# End Source File
# Begin Source File

SOURCE=.\DMCIBar.h
# End Source File
# Begin Source File

SOURCE=.\DMCITree.h
# End Source File
# Begin Source File

SOURCE=.\DMDBBT.h
# End Source File
# Begin Source File

SOURCE=.\DMDBRMask.h
# End Source File
# Begin Source File

SOURCE=.\DMEdCtx.h
# End Source File
# Begin Source File

SOURCE=.\DMLvMetrBar.h
# End Source File
# Begin Source File

SOURCE=.\DMLvSelWnd2.h
# End Source File
# Begin Source File

SOURCE=.\DMn.h
# End Source File
# Begin Source File

SOURCE=.\DMObBar.h
# End Source File
# Begin Source File

SOURCE=.\DMObTreeData.h
# End Source File
# Begin Source File

SOURCE=.\DMObTreeView.h
# End Source File
# Begin Source File

SOURCE=.\DMRecFmt.h
# End Source File
# Begin Source File

SOURCE=.\DMTarTrack.h
# End Source File
# Begin Source File

SOURCE=.\DMTeleLocFrm.h
# End Source File
# Begin Source File

SOURCE=.\DMTeleLocVw.h
# End Source File
# Begin Source File

SOURCE=.\DMTextEdit.h
# End Source File
# Begin Source File

SOURCE=.\DO2\DO2PathStr.h
# End Source File
# Begin Source File

SOURCE=.\EditDBItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditDBItemExc.h
# End Source File
# Begin Source File

SOURCE=.\EditDM2FileDescDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditDMFileDescDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditDMItemSubDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditDMRootSel.h
# End Source File
# Begin Source File

SOURCE=.\EditLvDesc3Dlg.h
# End Source File
# Begin Source File

SOURCE=.\EditLvDescDlg.h
# End Source File
# Begin Source File

SOURCE=.\EdPIActu1.h
# End Source File
# Begin Source File

SOURCE=.\EdPIActu2Page.h
# End Source File
# Begin Source File

SOURCE=.\EdPIActu3Page.h
# End Source File
# Begin Source File

SOURCE=.\EdPIActuHintPage.h
# End Source File
# Begin Source File

SOURCE=.\EdPIChestPage.h
# End Source File
# Begin Source File

SOURCE=.\EdPIClothPage.h
# End Source File
# Begin Source File

SOURCE=.\EdPICreaturePage.h
# End Source File
# Begin Source File

SOURCE=.\EdPIDlg.h
# End Source File
# Begin Source File

SOURCE=.\EdPIDoorPage.h
# End Source File
# Begin Source File

SOURCE=.\EdPIMiscPage.h
# End Source File
# Begin Source File

SOURCE=.\EdPINoPage.h
# End Source File
# Begin Source File

SOURCE=.\EdPIPotionPage.h
# End Source File
# Begin Source File

SOURCE=.\EdPIRTSink.h
# End Source File
# Begin Source File

SOURCE=.\EdPIScrollPage.h
# End Source File
# Begin Source File

SOURCE=.\EdPITele.h
# End Source File
# Begin Source File

SOURCE=.\EdPIText2Page.h
# End Source File
# Begin Source File

SOURCE=.\EdPITextPage.h
# End Source File
# Begin Source File

SOURCE=.\EdPIWeapon.h
# End Source File
# Begin Source File

SOURCE=.\EdTileDefaultDlg.h
# End Source File
# Begin Source File

SOURCE=.\EdTileDlg.h
# End Source File
# Begin Source File

SOURCE=.\EdTileDoorDlg.h
# End Source File
# Begin Source File

SOURCE=.\EdTileFalseDlg.h
# End Source File
# Begin Source File

SOURCE=.\EdTilePitDlg.h
# End Source File
# Begin Source File

SOURCE=.\EdTileStairsDlg.h
# End Source File
# Begin Source File

SOURCE=.\EdTileTeleDlg.h
# End Source File
# Begin Source File

SOURCE=.\Exp00069.h
# End Source File
# Begin Source File

SOURCE=.\Exp00069Doc.h
# End Source File
# Begin Source File

SOURCE=.\Exp00069View.h
# End Source File
# Begin Source File

SOURCE=.\ExtBM.h
# End Source File
# Begin Source File

SOURCE=.\ExtComboBox.h
# End Source File
# Begin Source File

SOURCE=.\FinderComboBox.h
# End Source File
# Begin Source File

SOURCE=.\FinderDock.h
# End Source File
# Begin Source File

SOURCE=.\FinderFrm.h
# End Source File
# Begin Source File

SOURCE=.\FinderInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\FinderVw.h
# End Source File
# Begin Source File

SOURCE=.\FM.h
# End Source File
# Begin Source File

SOURCE=.\GetBackButton.h
# End Source File
# Begin Source File

SOURCE=.\HexEditCB.h
# End Source File
# Begin Source File

SOURCE=.\HexEditDoc.h
# End Source File
# Begin Source File

SOURCE=.\HexEditFrame.h
# End Source File
# Begin Source File

SOURCE=.\HexEditView.h
# End Source File
# Begin Source File

SOURCE=.\HintOfStatic.h
# End Source File
# Begin Source File

SOURCE=.\HugHug.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MainFrmIndir.h
# End Source File
# Begin Source File

SOURCE=.\MemVuff.h
# End Source File
# Begin Source File

SOURCE=.\MkStatusBar.h
# End Source File
# Begin Source File

SOURCE=.\NamedWndBox.h
# End Source File
# Begin Source File

SOURCE=.\OffSc.h
# End Source File
# Begin Source File

SOURCE=.\OSP.h
# End Source File
# Begin Source File

SOURCE=.\PM.h
# End Source File
# Begin Source File

SOURCE=.\PWM.h
# End Source File
# Begin Source File

SOURCE=.\RCI.h
# End Source File
# Begin Source File

SOURCE=.\RCT2.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RN.h
# End Source File
# Begin Source File

SOURCE=.\scbarcf.h
# End Source File
# Begin Source File

SOURCE=.\scbarg.h
# End Source File
# Begin Source File

SOURCE=.\Serialized.h
# End Source File
# Begin Source File

SOURCE=.\ShowcaseData.h
# End Source File
# Begin Source File

SOURCE=.\ShowcaseDock.h
# End Source File
# Begin Source File

SOURCE=.\ShowcaseFrame.h
# End Source File
# Begin Source File

SOURCE=.\ShowcaseLv.h
# End Source File
# Begin Source File

SOURCE=.\sizecbar.h
# End Source File
# Begin Source File

SOURCE=.\SizeVuff.h
# End Source File
# Begin Source File

SOURCE=.\SplashFrame.h
# End Source File
# Begin Source File

SOURCE=.\SplashVw.h
# End Source File
# Begin Source File

SOURCE=.\SplitStr.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Str2Clipbrd.h
# End Source File
# Begin Source File

SOURCE=.\Ternary.h
# End Source File
# Begin Source File

SOURCE=.\TextInputDlg.h
# End Source File
# Begin Source File

SOURCE=.\TriViewNe.h
# End Source File
# Begin Source File

SOURCE=.\TsDataObject.h
# End Source File
# Begin Source File

SOURCE=.\TsDataSource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ARW05DN.ICO
# End Source File
# Begin Source File

SOURCE=.\res\ARW05LT.ICO
# End Source File
# Begin Source File

SOURCE=.\res\ARW05RT.ICO
# End Source File
# Begin Source File

SOURCE=.\res\ARW05UP.ICO
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\black.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exp00069.ico
# End Source File
# Begin Source File

SOURCE=.\res\Exp00069.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Exp00069Doc.ico
# End Source File
# Begin Source File

SOURCE=.\res\finder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\getback.bmp
# End Source File
# Begin Source File

SOURCE=.\res\H_MOVE.CUR
# End Source File
# Begin Source File

SOURCE=.\res\H_POINT.CUR
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\MOVE4WAY.CUR
# End Source File
# Begin Source File

SOURCE=.\res\NOTE06.ICO
# End Source File
# Begin Source File

SOURCE=.\res\PENCIL14.ICO
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\TRCK4WAY.CUR
# End Source File
# Begin Source File

SOURCE=.\res\TRCKNESW.CUR
# End Source File
# Begin Source File

SOURCE=.\res\TRCKNS.CUR
# End Source File
# Begin Source File

SOURCE=.\res\trcknwse.cur
# End Source File
# Begin Source File

SOURCE=.\res\TRCKWE.CUR
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\About.rtf
# End Source File
# Begin Source File

SOURCE=.\res\CHANGES.rtf
# End Source File
# Begin Source File

SOURCE=.\res\DnD.rtf
# End Source File
# Begin Source File

SOURCE=.\res\Findervw.rtf
# End Source File
# Begin Source File

SOURCE=.\res\Shortcuts.rtf
# End Source File
# Begin Source File

SOURCE=.\res\splash.rle
# End Source File
# Begin Source File

SOURCE=.\res\Tileprop.rtf
# End Source File
# Begin Source File

SOURCE=.\TODO.txt
# End Source File
# End Target
# End Project
