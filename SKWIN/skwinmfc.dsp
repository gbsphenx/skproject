# Microsoft Developer Studio Project File - Name="skwinmfc" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=skwinmfc - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "skwinmfc.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "skwinmfc.mak" CFG="skwinmfc - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "skwinmfc - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "skwinmfc - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "skwinmfc - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./al/include" /I "." /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USE_MFC60" /FR /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:console /machine:I386 /libpath:"./al/libs/Win32"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "skwinmfc - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "./al/include" /I "." /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USE_MFC60" /FR /Yu"StdAfx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 allegro_primitives.lib allegro.lib allegro_audio.lib alleg.lib allegro_acodec.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"./al/libs/Win32" /libpath:"./allegro/lib" /libpath:"./allegro5/lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "skwinmfc - Win32 Release"
# Name "skwinmfc - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\A.rc
# End Source File
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DME.cpp
# End Source File
# Begin Source File

SOURCE=.\fileset.cpp
# End Source File
# Begin Source File

SOURCE=.\fileset.h
# End Source File
# Begin Source File

SOURCE=.\SBlast.cpp
# End Source File
# Begin Source File

SOURCE=.\SkCodeParam.cpp
# End Source File
# Begin Source File

SOURCE=.\SkCodeParam.h
# End Source File
# Begin Source File

SOURCE=.\SkGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\SkGlobal.h
# End Source File
# Begin Source File

SOURCE=.\SkVersionControl.h
# End Source File
# Begin Source File

SOURCE=.\SkWin.cpp
# End Source File
# Begin Source File

SOURCE=.\SkWinCore.cpp
# End Source File
# Begin Source File

SOURCE=.\SkWinCore2.cpp
# End Source File
# Begin Source File

SOURCE=.\SkWinCore.h
# End Source File
# Begin Source File

SOURCE=.\skwindebug.cpp
# End Source File
# Begin Source File

SOURCE=.\skwindebug.h
# End Source File
# Begin Source File

SOURCE=.\SkWinMFC.cpp
# End Source File
# Begin Source File

SOURCE=.\SkWinMIDI.cpp
# End Source File
# Begin Source File

SOURCE=.\SkWinMIDI.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\_4976_03a2.h
# End Source File
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\defines.h
# End Source File
# Begin Source File

SOURCE=.\DME.h
# End Source File
# Begin Source File

SOURCE=.\MemVuff.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SBlast.h
# End Source File
# Begin Source File

SOURCE=.\sk3672.h
# End Source File
# Begin Source File

SOURCE=.\sk37fc.h
# End Source File
# Begin Source File

SOURCE=.\skval1.h
# End Source File
# Begin Source File

SOURCE=.\skval2.h
# End Source File
# Begin Source File

SOURCE=.\SkWin.h
# End Source File
# Begin Source File

SOURCE=.\SkWinMFC.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\pointer.cur
# End Source File
# End Group
# End Target
# End Project
