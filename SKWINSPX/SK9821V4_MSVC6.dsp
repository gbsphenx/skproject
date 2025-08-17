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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "./src/v0" /I "./src/v4" /I "./src/v6" /I "./al/include" /I "." /I "./lua/include" /I "./SDL2-2.0.8/include" /D "SK9821V4" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USE_MFC60" /D "_AFXDLL" /D "SDL_MAIN_HANDLED" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 allegro_primitives.lib allegro.lib allegro_audio.lib alleg.lib allegro_acodec.lib lua5.1.lib lua51.lib SDL2.lib /nologo /subsystem:console /machine:I386 /out:"build/Sk9821V4_MSVC6_Release/skwinv4.exe" /libpath:"./al/libs/Win32" /libpath:"./allegro/lib" /libpath:"./allegro-5.0.9/allegro5/lib" /libpath:"./lua/lib" /libpath:"./SDL2-2.0.8/lib/x86"
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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./src/v0" /I "./src/v4" /I "./src/v6" /I "./al/include" /I "." /I "./lua/include" /I "./SDL2-2.0.8/include" /D "SK9821V4" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USE_MFC60" /D "_AFXDLL" /D "SDL_MAIN_HANDLED" /D "__SDL__" /FR /FD /GZ /c
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
# ADD LINK32 allegro_primitives.lib allegro.lib allegro_audio.lib alleg.lib allegro_acodec.lib lua5.1.lib lua51.lib SDL2.lib /nologo /subsystem:console /debug /machine:I386 /out:"build/Sk9821V4_MSVC6_Debug/skwinv4debug.exe" /pdbtype:sept /libpath:"./al/libs/Win32" /libpath:"./allegro/lib" /libpath:"./allegro-5.0.9/allegro5/lib" /libpath:"./lua/lib" /libpath:"./SDL2-2.0.8/lib/x86"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "SK9821V4 - Win32 Release"
# Name "SK9821V4 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "BASE_V4"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\v4\dme.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skchamp.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skcore.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skcore.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\skcorev4.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\skeditdn.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skcrture.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skdungn.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skbkgrnd.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skevent.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skibmio.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skdevui.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skaudio.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skdoor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skgameui.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skgame.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skgdat.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skbltgfx.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skguidrw.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skitem.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\sktext.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skuievnt.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\skweathr.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\winatl.h
# End Source File
# End Group
# Begin Group "WINDOW"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\v4\SkWin.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\SKWIN4.rc
# End Source File
# End Group
# Begin Group "BASE_V0"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\v0\skcnsole.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\skdebug.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\skdos.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\skdos.h
# End Source File
# Begin Source File

SOURCE=.\src\v0\skfile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\skglobal.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\sklua.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\skmfc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\skmidi.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\skmidi.h
# End Source File
# Begin Source File

SOURCE=.\src\v0\skparam.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\skparam.h
# End Source File
# Begin Source File

SOURCE=.\src\skprobe\skprobe.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\skrender.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\sksdl2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\sktypes.h
# End Source File
# Begin Source File

SOURCE=.\src\v0\skver.h
# End Source File
# Begin Source File

SOURCE=.\src\v0\skvram.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\skvram.h
# End Source File
# Begin Source File

SOURCE=.\src\v0\skwinapp.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\skwinapp.h
# End Source File
# Begin Source File

SOURCE=.\src\v0\skwinmfc.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v0\skwinmfc.h
# End Source File
# End Group
# Begin Group "AUDIO"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\v4\sksound.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v4\SBlast.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v6\XSFXSDL.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v6\XSFXSDL.h
# End Source File
# End Group
# Begin Group "BASE_V6"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\v6\XCOREDM1.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v6\XCOREENH.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v6\XCORELOG.cpp
# End Source File
# Begin Source File

SOURCE=.\src\v6\xstartup.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\src\v4\StdAfx.cpp
# ADD CPP /Yc"StdAfx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\v4\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\DME.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\KAITABLE.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\KSK3672.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\KSK37FC.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\KSKVAL1.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\KSKVAL2.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\MemVuff.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\SBlast.h
# End Source File
# Begin Source File

SOURCE=.\src\v0\skdebug.h
# End Source File
# Begin Source File

SOURCE=.\src\v0\skfile.h
# End Source File
# Begin Source File

SOURCE=.\src\v0\skglobal.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\sklua.h
# End Source File
# Begin Source File

SOURCE=.\src\skprobe\skprobe.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\SkWin.h
# End Source File
# Begin Source File

SOURCE=.\src\v4\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\src\v4\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\src\v4\pointer.cur
# End Source File
# End Group
# End Target
# End Project
