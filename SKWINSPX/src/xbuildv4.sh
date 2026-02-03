#!/bin/bash
source ./xenvv4.sh
echo "Removing object files..."
rm -f ../build/SKWinV4_Linux_Release/*.o
## base v0
export SOURCE_FOLDER=./v0
./xcomp.sh skparam
./xcomp.sh skwinapp
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
./xcomp.sh skglobal

## SYS CORE
export SOURCE_FOLDER=./v4
./xcomp.sh dme
./xcomp.sh skbltgfx
./xcomp.sh skaudio
./xcomp.sh sksound
## MAIN GAME
./xcomp.sh skmemcch
./xcomp.sh skgdat
./xcomp.sh skuievnt
./xcomp.sh skevent
./xcomp.sh skchamp
./xcomp.sh skitem
./xcomp.sh skcrture
./xcomp.sh skbkgrnd
./xcomp.sh sktext
./xcomp.sh skweathr
./xcomp.sh skdungn
./xcomp.sh skgame
./xcomp.sh skguidrw
./xcomp.sh skeditdn -w
./xcomp.sh skcore -w
./xcomp.sh skdoor
./xcomp.sh skibmio
./xcomp.sh skdevui
./xcomp.sh skfileop
./xcomp.sh skgameui
./xcomp.sh skspell
./xcomp.sh sklua -w

export SOURCE_FOLDER=./v6
## EXTRA
./xcomp.sh xcoreenh
./xcomp.sh xcoredm1
./xcomp.sh xcorelog -w
./xcomp.sh xstartup
./xcomp.sh xcoretq
./xcomp.sh xsfxsdl -w

./xlink.sh