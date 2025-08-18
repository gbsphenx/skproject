@ECHO OFF

ECHO SKWin MinGW Build script

::------------------------------------------------------------------------------
:: Setting MinGW path
SET MINGWPATH=..\MinGW\bin
SET SEARCHMINGW=MinGW\bin

:: Check if MinGW\bin is already in the system PATH
::ECHO %PATH% | FIND /I "%SEARCHMINGW%" >nul

::IF ERRORLEVEL 1 (
::    ECHO Adding "%MINGWPATH%" to PATH...
SET PATH=%PATH%;%MINGWPATH%
::) ELSE (
::    ECHO "%MINGWPATH%" is already in PATH.
::)

::------------------------------------------------------------------------------
:: Creating build folders
SET FOLDER_BUILD_V0=..\build\SKWinV0_MinGW_Release
SET FOLDER_BUILD_V4=..\build\SKWinV4_MinGW_Release
SET FOLDER_BUILD_V5=..\build\SKWinV5_MinGW_Release
SET FOLDER_BUILD_V6=..\build\SKWinV6_MinGW_Release

SET FOLDER_BUILD=%FOLDER_BUILD_V0%
IF NOT EXIST "%FOLDER_BUILD%" (
    MKDIR "%FOLDER_BUILD%"
    ECHO Folder "%FOLDER_BUILD%" created.
) ELSE (
    ECHO Folder "%FOLDER_BUILD%" already exists.
)

SET FOLDER_BUILD=%FOLDER_BUILD_V4%
IF NOT EXIST "%FOLDER_BUILD%" (
    MKDIR "%FOLDER_BUILD%"
    ECHO Folder "%FOLDER_BUILD%" created.
) ELSE (
    ECHO Folder "%FOLDER_BUILD%" already exists.
)

SET FOLDER_BUILD=%FOLDER_BUILD_V5%
IF NOT EXIST "%FOLDER_BUILD%" (
    MKDIR "%FOLDER_BUILD%"
    ECHO Folder "%FOLDER_BUILD%" created.
) ELSE (
    ECHO Folder "%FOLDER_BUILD%" already exists.
)

SET FOLDER_BUILD=%FOLDER_BUILD_V6%
IF NOT EXIST "%FOLDER_BUILD%" (
    MKDIR "%FOLDER_BUILD%"
    ECHO Folder "%FOLDER_BUILD%" created.
) ELSE (
    ECHO Folder "%FOLDER_BUILD%" already exists.
)

::------------------------------------------------------------------------------

ECHO.
ECHO MinGW version:
mingw32-c++.exe --version

::------------------------------------------------------------------------------
mingw32-make -f makefile_mingw_skwinv0 clean
mingw32-make -f makefile_mingw_skwinv0 skwinv0-mingw
::mingw32-make -f makefile_mingw_skwin9821 clean
::mingw32-make -f makefile_mingw_skwin9821 skwinv4_mingw
::mingw32-make -f makefile_mingw_skwindos clean
::mingw32-make -f makefile_mingw_skwindos skwinv5_mingw
::mingw32-make -f makefile_mingw_skwinspx clean
::mingw32-make -f makefile_mingw_skwinspx skwinv6_mingw
COPY %FOLDER_BUILD_V0%\skwinv0-mingw.exe ..\skgame\skwinv0-mingw.exe
::COPY ..\build\Sk9821V4_MinGW_Release\skwinv4_mingw.exe ..\skgame\skwinv4_mingw.exe
::COPY ..\build\SkDOSV5_MinGW_Release\skwinv5_mingw.exe ..\skgame\skwinv5_mingw.exe
::COPY ..\build\SkWinSPX_MinGW_Release\skwinv6_mingw.exe ..\skgame\skwinv6_mingw.exe