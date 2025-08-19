#!/bin/bash
## Get param
if [[ "$1" = "clean" ]]; then OPT_CLEAN="CLEAN"; fi

## Create build folders
if [[ ! -d "../build/SKWinV0_Linux_Release" ]]; then mkdir ../build/SKWinV0_Linux_Release; fi
if [[ ! -d "../build/SKWinV4_Linux_Release" ]]; then mkdir ../build/SKWinV4_Linux_Release; fi
if [[ ! -d "../build/SKWinV5_Linux_Release" ]]; then mkdir ../build/SKWinV5_Linux_Release; fi
if [[ ! -d "../build/SKWinV6_Linux_Release" ]]; then mkdir ../build/SKWinV6_Linux_Release; fi

## Clean before building or not
if [[ -n "$OPT_CLEAN" ]] ; then
    make -f makefile_linux_skwinv0 clean
fi
make -f makefile_linux_skwinv0 skwinv0-linux

## Put builds into skgame folder 
mv ../build/SKWinV0_Linux_Release/skwinv0-linux ../skgame/

## List linux builds into skgame
echo "Linux build state:"
ls -al ../skgame/*linux*
