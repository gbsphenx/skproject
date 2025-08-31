#!/bin/bash
echo "SKWin MinGW Build script"
## Get param
if [[ "$1" = "clean" ]]; then OPT_CLEAN="CLEAN"; fi

## Create build folders
if [[ ! -d "../build/SKWinV0_MinGW_Release" ]]; then mkdir ../build/SKWinV0_MinGW_Release; fi
if [[ ! -d "../build/SKWinV4_MinGW_Release" ]]; then mkdir ../build/SKWinV4_MinGW_Release; fi
if [[ ! -d "../build/SKWinV5_MinGW_Release" ]]; then mkdir ../build/SKWinV5_MinGW_Release; fi
if [[ ! -d "../build/SKWinV6_MinGW_Release" ]]; then mkdir ../build/SKWinV6_MinGW_Release; fi

echo "MinGW version:"
mingw32-c++.exe --version

SKGAME_FOLDER=..\skgame
TARGET_V0=skwinv0-mingw
TARGET_V4=skwinv4-mingw
TARGET_V5=skwinv5-mingw
TARGET_V6=skwinv6-mingw

## Clean before building or not
if [[ -n "$OPT_CLEAN" ]] ; then
    make -f makefile_mingw_skwinv0 clean
    make -f makefile_mingw_skwinv4 clean
fi

#make -f makefile_mingw_skwinv0 ${TARGET_V0}
make -f makefile_mingw_skwinv4 ${TARGET_V4}

## Put builds into skgame folder 
cp -p ../build/SKWinV0_MinGW_Release/${TARGET_V0} ../skgame/
cp -p ../build/SKWinV4_MinGW_Release/${TARGET_V4} ../skgame/
##mv ../build/SKWinV5_MinGW_Release/${TARGET_V5} ../skgame/
##mv ../build/SKWinV6_MinGW_Release/${TARGET_V6} ../skgame/

## List MinGW builds into skgame
echo "MinGW build state:"
ls -al ../skgame/*mingw*

::------------------------------------------------------------------------------
