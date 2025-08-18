REM DOS/FREEDOS SKWIN/SKULL -- Run this command from src folder
REM COPY V:\SRC\XCOPYV4.BAT \SRC\
REM Copy all source code from host server (V:\) to FreeDOS VM environment
DEL \SRC\V0\*.*
DEL \SRC\V4\*.*
DEL \SRC\V5\*.*
DEL \SRC\V6\*.*
COPY V:\SRC\*.BAT \SRC\
COPY V:\SRC\V0\*.* \SRC\V0\
COPY \SRC\V0\DM2SK.RAW \DM2SK.RAW
COPY V:\SRC\V4\*.* \SRC\V4\
COPY V:\SRC\V5\*.* \SRC\V5\
COPY V:\SRC\V6\*.* \SRC\V6\





