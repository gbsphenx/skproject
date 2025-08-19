#!/bin/bash
echo "Setting SKWin compilation env. for V5 ..."
export SKULL_TARGET=skwinv5-linux
export BUILD_FOLDER=../build/SKWinV5_Linux_Release
export SOURCE_FOLDER=./v5
export INCLUDE_FOLDER=.
export GCC_FLAGS="-D__SDL__ -DSKDOSV5 -D__SK_NO_ALLEGRO__"
if [[ ! -d ../build ]]; then mkdir ../build; fi
if [[ ! -d "${BUILD_FOLDER}" ]]; then mkdir ${BUILD_FOLDER}; fi 
