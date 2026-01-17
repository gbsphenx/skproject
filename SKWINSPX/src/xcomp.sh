#!/bin/bash
## Unit Compilation
SOURCE=$1
FLAGS=$2
echo "Compiling ${SOURCE} ..."
gcc -o ${BUILD_FOLDER}/${SOURCE}${VC}.o -c ${SOURCE_FOLDER}/${SOURCE}.cpp -I${INCLUDE_FOLDER}/v0 -I${INCLUDE_FOLDER}/v4 -I${INCLUDE_FOLDER}/v5 -I${INCLUDE_FOLDER}/v6 -D__LINUX__ -D__SDL__ -D__SK_NO_ALLEGRO__ ${GCC_FLAGS} ${FLAGS}
