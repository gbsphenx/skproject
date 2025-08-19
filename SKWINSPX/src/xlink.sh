#!/bin/bash
TARGET=$SKULL_TARGET
TARGET_BUILD_PATH=$BUILD_FOLDER

## Link
echo "Linking ..."
## Requirement for SDL2
## sudo apt install libsdl2-dev
g++ -g -W -Wall -O3 -D__LINUX__ -D__NOSDL__ -Iv0 -Iv4 -Iv5 -Iv6 -Ilua/include -L. -L../lua/lib $TARGET_BUILD_PATH/*.o -lSDL2 -o $TARGET_BUILD_PATH/$TARGET
GCC_RC=$?
if [[ $GCC_RC -eq 0 ]]; then
    echo "$TARGET done."
    chmod a+x $TARGET_BUILD_PATH/$TARGET
    ## Copy to game folder
    cp -p $TARGET_BUILD_PATH/$TARGET ../skgame/
else
    echo "$TARGET failed!"
fi
