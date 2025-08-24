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
SET SKGAME_FOLDER=..\skgame
SET TARGET_V0=skwinv0-mingw
SET TARGET_V4=skwinv4-mingw
SET TARGET_V5=skwinv5-mingw
SET TARGET_V6=skwinv6-mingw

::mingw32-make -f makefile_mingw_skwinv0 clean
mingw32-make -f makefile_mingw_skwinv0 %TARGET_V0%
::mingw32-make -f makefile_mingw_skwinv4 clean
mingw32-make -f makefile_mingw_skwinv4 %TARGET_V4%
::mingw32-make -f makefile_mingw_skwinv5 clean
::mingw32-make -f makefile_mingw_skwinv5 %TARGET_V5%
::mingw32-make -f makefile_mingw_skwinv6 clean
::mingw32-make -f makefile_mingw_skwinv6 %TARGET_V6%
COPY %FOLDER_BUILD_V0%\%TARGET_V0%.exe %SKGAME_FOLDER%\%TARGET_V0%.exe
COPY %FOLDER_BUILD_V4%\%TARGET_V4%.exe %SKGAME_FOLDER%\%TARGET_V4%.exe
COPY %FOLDER_BUILD_V5%\%TARGET_V5%.exe %SKGAME_FOLDER%\%TARGET_V5%.exe
COPY %FOLDER_BUILD_V6%\%TARGET_V6%.exe %SKGAME_FOLDER%\%TARGET_V6%.exe