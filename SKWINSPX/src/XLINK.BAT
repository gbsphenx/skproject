REM Build SKULL EXE
gpp -g -o %SKULL_TARGET% %BUILD_FOLDER%\*.o
REM Copy to root folder
COPY %SKULL_TARGET% \%SKULL_TARGET%
REM Put it back on shared host folder
COPY %SKULL_TARGET% V:\SKGAME\%SKULL_TARGET%
