#!/bin/bash
echo "Setting SKWin compilation env. for V0 ..."
export SKULL_TARGET=skwinv0-linux
export BUILD_FOLDER=../build/SKWinV0_Linux_Release
export SOURCE_FOLDER=./v0
export INCLUDE_FOLDER=.
export GCC_FLAGS="-D__SDL__ -D__SK_NO_ALLEGRO__"
if [[ ! -d ../build ]]; then mkdir ../build; fi
if [[ ! -d "${BUILD_FOLDER}" ]]; then mkdir ${BUILD_FOLDER}; fi 
