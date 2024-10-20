# Microsoft Developer Studio Project File - Name="SK9821V4" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SK9821V4 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SK9821V4_MSVC6.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SK9821V4_MSVC6.mak" CFG="SK9821V4 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SK9821V4 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SK9821V4 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SK9821V4 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "./src/v4kj9821" /I "./src/v6spxmex" /I "./al/include" /I "." /I "./lua/include" /D "SK9821V4" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USE_MFC60" /D "_AFXDLL" /FR /Yu"StdAfx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 allegro_primitives.lib allegro.lib allegro_audio.lib alleg.lib allegro_acodec.lib lua5.1.lib lua51.lib /nologo /subsystem:console /machine:I386 /out:"build/Sk9821V4_MSVC6_Release/skwin9821.exe" /libpath:"./al/libs/Win32" /libpath:"./allegro/lib" /libpath:"./allegro-5.0.9/allegro5/lib" /libpath:"./lua/lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "SK9821V4 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./src/v4kj9821" /I "./src/v6spxmex" /I "./al/include" /I "." /I "./lua/include" /D "SK9821V4" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USE_MFC60" /D "_AFXDLL" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 allegro_primitives.lib allegro.lib allegro_audio.lib alleg.lib allegro_acodec.lib lua5.1.lib lua51.lib /nologo /subsystem:console /debug /machine:I386 /out:"build/Sk9821V4_MSVC6_Debug/skwin9821debug.exe" /pdbtype:sept /libpath:"./al/libs/Win32" /libpath:"./allegro/lib" /libpath:"./allegro-5.0.9/allegro5/lib" /libpath:"./lua/lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SK9821V4 - Win32 Release"
# Name "SK9821V4 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "SK9821V4_BASE"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\v4kj9821\KCHMPION.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KCORE.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KCORE.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KCOREDUN.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KCREATUR.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KDUNG.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KENVIRON.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KEVENT.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KFILESET.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KGAME.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KGDAT.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KGFX.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KGUIDRW.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KITEM.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KMOUSE.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KSFX.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KTEXT.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KUI.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KWEATHER.cpp
# End Source File
# End Group
# Begin Group "WINDOW"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\v4kj9821\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SKDOS.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SkMFC.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SkWin.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SKWIN4.rc
# End Source File
# End Group
# Begin Group "AUDIO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\v4kj9821\SBlast.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SkMIDI.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SkMIDI.h
# End Source File
# End Group
# Begin Group "SKSPXV6_COMMON"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\v6spxmex\XCOREDM1.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v6spxmex\XCOREENH.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v6spxmex\XCORELOG.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v6spxmex\XDEFINES.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\v4kj9821\DME.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\skdebug.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SkGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SkLUA.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SKPARAM.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\StdAfx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\v4kj9821\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\DME.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KAITABLE.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KFILESET.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KSK3672.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KSK37FC.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KSKVAL1.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\KSKVAL2.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\MemVuff.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SBlast.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\skdebug.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SKDOS.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SkGlobal.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SkLUA.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SkMFC.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SkParam.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SKVER.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\SkWin.h
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\src\v4kj9821\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\src\v4kj9821\pointer.cur
# End Source File
# End Group
# End Target
# End Project
