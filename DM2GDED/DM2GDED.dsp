# Microsoft Developer Studio Project File - Name="DM2GDED" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=DM2GDED - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "DM2GDED.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "DM2GDED.mak" CFG="DM2GDED - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "DM2GDED - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "DM2GDED - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/Proj6t/DM2GDED", IDBAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DM2GDED - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "./hyperlink" /I "./scb" /I "./gridctrl" /I "./tabcontrol" /I "./flatbar_src" /I "./XHyperLink_demo" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib msacm32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "DM2GDED - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I "./hyperlink" /I "./scb" /I "./gridctrl" /I "./tabcontrol" /I "./flatbar_src" /I "./XHyperLink_demo" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib msacm32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "DM2GDED - Win32 Release"
# Name "DM2GDED - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddImageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\AddImageDlg2.cpp
# End Source File
# Begin Source File

SOURCE=.\AnchorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\CompatDC.cpp
# End Source File
# Begin Source File

SOURCE=.\CSVrw.cpp
# End Source File
# Begin Source File

SOURCE=.\CustRichEditCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DiffExportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DiffExportExec.cpp
# End Source File
# Begin Source File

SOURCE=.\DiffImportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DiffImportExec.cpp
# End Source File
# Begin Source File

SOURCE=.\DM1ImageComp.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2BSwDomain.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2ColorSelStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2CompAdv.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2DescriptCtx.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2EdPicFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2EdPicView.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2GDED.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2GDED.rc
# End Source File
# Begin Source File

SOURCE=.\DM2GDEDDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2GDEDOPInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2GDEDSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2GDEDTeView.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2GDEDView.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2GDEDView2.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DM2ImageComp.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2ImageExch.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2NI1Page.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2NI2Page.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2NI3Page.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2NI4Page.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2NIBPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2NICtx.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2NIMan.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2NIPS.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2NIPvFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2NIPvView.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2Palo.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2PIPDescList.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2RawContConv.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2RPtr.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2RTextToken.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2SelColorKeyStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2SIIVCtx.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2SkCmdList.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2TextEncImpl.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2TextEncoder.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2TTbl.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2VwPicVwPal.cpp
# End Source File
# Begin Source File

SOURCE=.\DM2VwSceneFrame.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DM2VwSceneVw.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DMGHLci.cpp
# End Source File
# Begin Source File

SOURCE=.\DMLocPalWnd.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\DTreeDock.cpp
# End Source File
# Begin Source File

SOURCE=.\DTreeUsing.cpp
# End Source File
# Begin Source File

SOURCE=.\DTreeView.cpp
# ADD CPP /GR
# End Source File
# Begin Source File

SOURCE=.\EditHeroStatDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditImageFragsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditImgDlg.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\EditImgFlgsDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditPCMCB.cpp
# End Source File
# Begin Source File

SOURCE=.\EditPCMDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditStandbyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVCreaturePg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVCtx.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVFlagsPg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVIconAnimPg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVIOff.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVMSItemsPg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVPg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVRuneSel.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVSelectPg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVSharpResistPg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVSKText.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVSSItemPg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVTextPg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVWordValPg.cpp
# End Source File
# Begin Source File

SOURCE=.\EditVxxx.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportDATComment.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ExportDATDoc.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ExportDATFrame.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ExportDATHintDlg.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ExportDATListVw.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ExportDATTreeVw.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ExportEditCommentDlg.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\ExtBM.cpp
# End Source File
# Begin Source File

SOURCE=.\FlatSplitterWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\FM.cpp
# End Source File
# Begin Source File

SOURCE=.\FocusReminder.cpp
# End Source File
# Begin Source File

SOURCE=.\GDAT.cpp
# End Source File
# Begin Source File

SOURCE=.\GDEdCtx.cpp
# End Source File
# Begin Source File

SOURCE=.\GDEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellCheck.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellCombo.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\HexEditCB.cpp
# End Source File
# Begin Source File

SOURCE=.\HexEditFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\HexEditView.cpp
# End Source File
# Begin Source File

SOURCE=.\hyperlink\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\ILSp.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportDATFx.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportQueryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ImportResDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\InprogressDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenFileTypeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OpenFTQueryFileDlg.cpp
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

SOURCE=.\ReplC1Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RuneSelStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\scb\scbarcf.cpp
# End Source File
# Begin Source File

SOURCE=.\scb\scbarg.cpp
# End Source File
# Begin Source File

SOURCE=.\SelCMDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\scb\sizecbar.cpp
# End Source File
# Begin Source File

SOURCE=.\SizeVuff.cpp
# End Source File
# Begin Source File

SOURCE=.\SKTextGridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitStr.cpp
# End Source File
# Begin Source File

SOURCE=.\StaticImagePv.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Flatbar_src\ToolBarEx.cpp
# End Source File
# Begin Source File

SOURCE=.\WVCT.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddImageDlg.h
# End Source File
# Begin Source File

SOURCE=.\AddImageDlg2.h
# End Source File
# Begin Source File

SOURCE=.\AnchorDlg.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrame.h
# End Source File
# Begin Source File

SOURCE=.\CompatDC.h
# End Source File
# Begin Source File

SOURCE=.\CSVrw.h
# End Source File
# Begin Source File

SOURCE=.\CustRichEditCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DiffExportDlg.h
# End Source File
# Begin Source File

SOURCE=.\DiffExportExec.h
# End Source File
# Begin Source File

SOURCE=.\DiffImportDlg.h
# End Source File
# Begin Source File

SOURCE=.\DiffImportExec.h
# End Source File
# Begin Source File

SOURCE=.\DM1ImageComp.h
# End Source File
# Begin Source File

SOURCE=.\DM2BSwDomain.h
# End Source File
# Begin Source File

SOURCE=.\DM2ColorSelStatic.h
# End Source File
# Begin Source File

SOURCE=.\DM2CompAdv.h
# End Source File
# Begin Source File

SOURCE=.\DM2DescriptCtx.h
# End Source File
# Begin Source File

SOURCE=.\DM2EdPicFrame.h
# End Source File
# Begin Source File

SOURCE=.\DM2GDED.h
# End Source File
# Begin Source File

SOURCE=.\DM2GDEDDoc.h
# End Source File
# Begin Source File

SOURCE=.\DM2GDEDOPInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\DM2GDEDSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\DM2GDEDTeView.h
# End Source File
# Begin Source File

SOURCE=.\DM2GDEDView.h
# End Source File
# Begin Source File

SOURCE=.\DM2GDEDView2.h
# End Source File
# Begin Source File

SOURCE=.\DM2ImageComp.h
# End Source File
# Begin Source File

SOURCE=.\DM2ImageExch.h
# End Source File
# Begin Source File

SOURCE=.\DM2NI1Page.h
# End Source File
# Begin Source File

SOURCE=.\DM2NI2Page.h
# End Source File
# Begin Source File

SOURCE=.\DM2NI3Page.h
# End Source File
# Begin Source File

SOURCE=.\DM2NI4Page.h
# End Source File
# Begin Source File

SOURCE=.\DM2NIBPage.h
# End Source File
# Begin Source File

SOURCE=.\DM2NICtx.h
# End Source File
# Begin Source File

SOURCE=.\DM2NIMan.h
# End Source File
# Begin Source File

SOURCE=.\DM2NIPS.h
# End Source File
# Begin Source File

SOURCE=.\DM2NIPvFrame.h
# End Source File
# Begin Source File

SOURCE=.\DM2NIPvView.h
# End Source File
# Begin Source File

SOURCE=.\DM2Palo.h
# End Source File
# Begin Source File

SOURCE=.\DM2PIPDescList.h
# End Source File
# Begin Source File

SOURCE=.\DM2RawContConv.h
# End Source File
# Begin Source File

SOURCE=.\DM2RPtr.h
# End Source File
# Begin Source File

SOURCE=.\DM2RTextToken.h
# End Source File
# Begin Source File

SOURCE=.\DM2SelColorKeyStatic.h
# End Source File
# Begin Source File

SOURCE=.\DM2SIIVCtx.h
# End Source File
# Begin Source File

SOURCE=.\DM2SkCmdList.h
# End Source File
# Begin Source File

SOURCE=.\DM2TextEncImpl.h
# End Source File
# Begin Source File

SOURCE=.\DM2TextEncoder.h
# End Source File
# Begin Source File

SOURCE=.\DM2TTbl.h
# End Source File
# Begin Source File

SOURCE=.\DM2VwPicVwPal.h
# End Source File
# Begin Source File

SOURCE=.\DM2VwSceneFrame.h
# End Source File
# Begin Source File

SOURCE=.\DM2VwSceneVw.h
# End Source File
# Begin Source File

SOURCE=.\DMGHLc.h
# End Source File
# Begin Source File

SOURCE=.\DMGHLci.h
# End Source File
# Begin Source File

SOURCE=.\DMGHLt.h
# End Source File
# Begin Source File

SOURCE=.\DMLocPalWnd.h
# End Source File
# Begin Source File

SOURCE=.\DTreeDock.h
# End Source File
# Begin Source File

SOURCE=.\DTreeUsing.h
# End Source File
# Begin Source File

SOURCE=.\DTreeView.h
# End Source File
# Begin Source File

SOURCE=.\EditHeroStatDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditImageFragsDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditImgDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditImgFlgsDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditPCMCB.h
# End Source File
# Begin Source File

SOURCE=.\EditPCMDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditStandbyDlg.h
# End Source File
# Begin Source File

SOURCE=.\EditVCreaturePg.h
# End Source File
# Begin Source File

SOURCE=.\EditVCtx.h
# End Source File
# Begin Source File

SOURCE=.\EditVFlagsPg.h
# End Source File
# Begin Source File

SOURCE=.\EditVIconAnimPg.h
# End Source File
# Begin Source File

SOURCE=.\EditVIOff.h
# End Source File
# Begin Source File

SOURCE=.\EditVMSItemsPg.h
# End Source File
# Begin Source File

SOURCE=.\EditVPg.h
# End Source File
# Begin Source File

SOURCE=.\EditVPgTemp.h
# End Source File
# Begin Source File

SOURCE=.\EditVRuneSel.h
# End Source File
# Begin Source File

SOURCE=.\EditVSelectPg.h
# End Source File
# Begin Source File

SOURCE=.\EditVSharpResistPg.h
# End Source File
# Begin Source File

SOURCE=.\EditVSKText.h
# End Source File
# Begin Source File

SOURCE=.\EditVSSItemPg.h
# End Source File
# Begin Source File

SOURCE=.\EditVTextPg.h
# End Source File
# Begin Source File

SOURCE=.\EditVWordValPg.h
# End Source File
# Begin Source File

SOURCE=.\EditVxxx.h
# End Source File
# Begin Source File

SOURCE=.\EndianUtil.h
# End Source File
# Begin Source File

SOURCE=.\ExportDATComment.h
# End Source File
# Begin Source File

SOURCE=.\ExportDATDoc.h
# End Source File
# Begin Source File

SOURCE=.\ExportDATFrame.h
# End Source File
# Begin Source File

SOURCE=.\ExportDATHintDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExportDATListVw.h
# End Source File
# Begin Source File

SOURCE=.\ExportDATTreeVw.h
# End Source File
# Begin Source File

SOURCE=.\ExportEditCommentDlg.h
# End Source File
# Begin Source File

SOURCE=.\ExtBM.h
# End Source File
# Begin Source File

SOURCE=.\FlatSplitterWnd.h
# End Source File
# Begin Source File

SOURCE=.\FM.h
# End Source File
# Begin Source File

SOURCE=.\FocusReminder.h
# End Source File
# Begin Source File

SOURCE=.\GDAT.h
# End Source File
# Begin Source File

SOURCE=.\GDATDef.h
# End Source File
# Begin Source File

SOURCE=.\GDEdCtx.h
# End Source File
# Begin Source File

SOURCE=.\GDEdDef.h
# End Source File
# Begin Source File

SOURCE=.\GDEdOpt.h
# End Source File
# Begin Source File

SOURCE=.\GDEntry.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCell.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellCheck.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCellCombo.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\HexEditCB.h
# End Source File
# Begin Source File

SOURCE=.\HexEditFrame.h
# End Source File
# Begin Source File

SOURCE=.\HexEditView.h
# End Source File
# Begin Source File

SOURCE=.\hyperlink\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\ILSp.h
# End Source File
# Begin Source File

SOURCE=.\ImportDATFx.h
# End Source File
# Begin Source File

SOURCE=.\ImportQueryDlg.h
# End Source File
# Begin Source File

SOURCE=.\ImportResDlg.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\InprogressDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MemVuff.h
# End Source File
# Begin Source File

SOURCE=.\OleByteBin.h
# End Source File
# Begin Source File

SOURCE=.\OpenFileTypeDlg.h
# End Source File
# Begin Source File

SOURCE=.\OpenFTQueryFileDlg.h
# End Source File
# Begin Source File

SOURCE=.\OSP.h
# End Source File
# Begin Source File

SOURCE=.\PM.h
# End Source File
# Begin Source File

SOURCE=.\RCI.h
# End Source File
# Begin Source File

SOURCE=.\RCT2.h
# End Source File
# Begin Source File

SOURCE=.\ReplC1Dlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RuneSelStatic.h
# End Source File
# Begin Source File

SOURCE=.\scb\scbarcf.h
# End Source File
# Begin Source File

SOURCE=.\scb\scbarg.h
# End Source File
# Begin Source File

SOURCE=.\SelCMDlg.h
# End Source File
# Begin Source File

SOURCE=.\scb\sizecbar.h
# End Source File
# Begin Source File

SOURCE=.\SizeVuff.h
# End Source File
# Begin Source File

SOURCE=.\SKTextGridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SplitStr.h
# End Source File
# Begin Source File

SOURCE=.\StaticImagePv.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl\TitleTip.h
# End Source File
# Begin Source File

SOURCE=.\Flatbar_src\ToolBarEx.h
# End Source File
# Begin Source File

SOURCE=.\WVCT.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp320by.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CmdATT.ico
# End Source File
# Begin Source File

SOURCE=.\res\CmdDef.ico
# End Source File
# Begin Source File

SOURCE=.\res\CmdEat.ico
# End Source File
# Begin Source File

SOURCE=.\res\cmdef1.ico
# End Source File
# Begin Source File

SOURCE=.\res\CmdWiz.ico
# End Source File
# Begin Source File

SOURCE=.\res\CmdXXX.ico
# End Source File
# Begin Source File

SOURCE=.\res\cmmap.ico
# End Source File
# Begin Source File

SOURCE=.\res\cmway.ico
# End Source File
# Begin Source File

SOURCE=.\res\cmwiz1.ico
# End Source File
# Begin Source File

SOURCE=.\res\cmxxx.ico
# End Source File
# Begin Source File

SOURCE=.\res\cmxxx1.ico
# End Source File
# Begin Source File

SOURCE=.\res\DM2GDED.ico
# End Source File
# Begin Source File

SOURCE=.\res\DM2GDED.manifest
# End Source File
# Begin Source File

SOURCE=.\res\DM2GDED.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DM2GDEDDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\GrpA.ico
# End Source File
# Begin Source File

SOURCE=.\res\GrpC.ico
# End Source File
# Begin Source File

SOURCE=.\res\GrpJ.ico
# End Source File
# Begin Source File

SOURCE=.\res\GrpM.ico
# End Source File
# Begin Source File

SOURCE=.\res\GrpP.ico
# End Source File
# Begin Source File

SOURCE=.\res\GrpS.ico
# End Source File
# Begin Source File

SOURCE=.\res\GrpW.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_dm2g.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_dm2s.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_gded.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_vwsc.ico
# End Source File
# Begin Source File

SOURCE=.\res\ldef.ico
# End Source File
# Begin Source File

SOURCE=.\res\paball.ico
# End Source File
# Begin Source File

SOURCE=.\res\paew.ico
# End Source File
# Begin Source File

SOURCE=.\res\paoh.ico
# End Source File
# Begin Source File

SOURCE=.\res\paoh1.ico
# End Source File
# Begin Source File

SOURCE=.\res\papull.ico
# End Source File
# Begin Source File

SOURCE=.\res\papush.ico
# End Source File
# Begin Source File

SOURCE=.\res\paven.ico
# End Source File
# Begin Source File

SOURCE=.\res\pazo.ico
# End Source File
# Begin Source File

SOURCE=.\res\runes.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\About.rtf
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
