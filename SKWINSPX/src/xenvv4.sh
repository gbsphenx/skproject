#!/bin/bash
echo "Setting SKWin compilation env. for V4 ..."
export SKULL_TARGET=skwinv4-linux
export BUILD_FOLDER=../build/SKWinV4_Linux_Release
export SOURCE_FOLDER=./v4
export INCLUDE_FOLDER=.
export GCC_FLAGS=-DSK9821V4
if [[ ! -d ../build ]]; then mkdir ../build; fi
if [[ ! -d "${BUILD_FOLDER}" ]]; then mkdir ${BUILD_FOLDER}; fi 
