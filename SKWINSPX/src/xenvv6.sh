#!/bin/bash
echo "Setting SKWin compilation env. for V6 ..."
export SKULL_TARGET=skwinv6-linux
export BUILD_FOLDER=../build/SKWinV6_Linux_Release
export SOURCE_FOLDER=./v6
export INCLUDE_FOLDER=.
##export GCC_FLAGS=-DSKWINSPX
export GCC_FLAGS="-D__SDL__ -DSKWINSPX -D__SK_NO_ALLEGRO__"
if [[ ! -d ../build ]]; then mkdir ../build; fi
if [[ ! -d "${BUILD_FOLDER}" ]]; then mkdir ${BUILD_FOLDER}; fi 
