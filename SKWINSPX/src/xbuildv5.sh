#!/bin/bash
source ./xenvv5.sh
echo "Removing object files..."
rm -f ../build/SKWinV5_Linux_Release/*.o
## base v0
export SOURCE_FOLDER=./v0
./xcomp.sh skparam
./xcomp.sh skwinapp -w
./xcomp.sh skrender
./xcomp.sh skdos
./xcomp.sh sksdl2
./xcomp.sh skmfc
./xcomp.sh skwinmfc
./xcomp.sh skvram
## file
./xcomp.sh skdebug
./xcomp.sh skfile
./xcomp.sh skcnsole -w
## common
./xcomp.sh skmidi

## SYS CORE
export SOURCE_FOLDER=./v5
./xcomp.sh bcreatur -w
./xcomp.sh bgdat -w
./xcomp.sh bitem
./xcomp.sh dm2data -w
./xcomp.sh dm2debug
./xcomp.sh dm2globl
./xcomp.sh dosvideo

./xcomp.sh fileio
./xcomp.sh gfxblit -w
./xcomp.sh gfxbmp
./xcomp.sh gfxdec
./xcomp.sh gfxmain
./xcomp.sh gfxpal
./xcomp.sh gfxpixel
./xcomp.sh gfxstr

./xcomp.sh emu -w
./xcomp.sh ints
./xcomp.sh main
##./xcomp.sh mdata (mdatac)
SOURCE=mdata
gcc ${GCC_OPT} -o ${BUILD_FOLDER}/${SOURCE}.o -c ./v5/${SOURCE}.c -I./v0 -I./v4 -I./v5 -I./v6 -D__LINUX__ -D__SDL__ -DSKDOSV5 -D__SK_NO_ALLEGRO__

./xcomp.sh sfx -w
./xcomp.sh sfxmidi
./xcomp.sh sfxmuwav
./xcomp.sh sfxsnd

./xcomp.sh SK0AAF
./xcomp.sh SK1C9A -w
./xcomp.sh SK0350
./xcomp.sh SK1031

./xcomp.sh skaddon
./xcomp.sh skai
./xcomp.sh skalegro
./xcomp.sh skalloc
./xcomp.sh skalocdb -w
./xcomp.sh skcloud
./xcomp.sh skengage
./xcomp.sh skeventq
./xcomp.sh skevents

./xcomp.sh skgdtqdb
./xcomp.sh skguidrw
./xcomp.sh skguivwp
./xcomp.sh skhero
./xcomp.sh skimage
./xcomp.sh sklight
./xcomp.sh sklodlvl
./xcomp.sh skmap
./xcomp.sh skmcursr -w
./xcomp.sh skmove -w
./xcomp.sh skmovrec -w
./xcomp.sh skrandom
./xcomp.sh skrecord
./xcomp.sh skrect

./xcomp.sh skstr
./xcomp.sh sksvgame
./xcomp.sh sktimer
./xcomp.sh sktimprc
./xcomp.sh skweathr
./xcomp.sh skxrect

./xcomp.sh startend
./xcomp.sh types
./xcomp.sh unref
./xcomp.sh util
./xcomp.sh xtypes

./xcomp.sh uibutton
./xcomp.sh uiclrect
./xcomp.sh uidialog
./xcomp.sh uiinput -w
./xcomp.sh uikeybd
./xcomp.sh uitmouse

export SOURCE_FOLDER=./v6
./xcomp.sh xstartup

./xlink.sh