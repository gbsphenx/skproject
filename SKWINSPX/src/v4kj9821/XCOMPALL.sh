#!/bin/bash
## Compile all V4KJ9821 code

## SYS CORE
./XCOMP.sh DME
./XCOMP.sh KMOUSE
./XCOMP.sh KFILESET
./XCOMP.sh SKDOS
./XCOMP.sh SKSDL2

./XCOMP.sh SKMIDI
./XCOMP.sh SKDEBUG
./XCOMPW.sh SKGLOBAL
./XCOMP.sh SKMEM

./XCOMP.sh KGFX
./XCOMP.sh KSFX


## MAIN GAME
./XCOMP.sh SKPARAM
./XCOMP.sh KGDAT
./XCOMP.sh KUI
./XCOMP.sh KEVENT
./XCOMP.sh KCHMPION
./XCOMP.sh KITEM
./XCOMP.sh KCREATUR
./XCOMP.sh KENVIRON
./XCOMP.sh KTEXT
./XCOMP.sh KWEATHER
./XCOMP.sh KDUNG
./XCOMP.sh KGAME
./XCOMP.sh KGUIDRW

## EXTRA
./XCOMP6.sh XCOREENH
./XCOMP6.sh XCOREDM1
./XCOMP6.sh XCOREEXT
./XCOMPW6.sh XCORELOG
./XCOMP.sh KCOREDUN
./XCOMPW.sh KCORE
./XCOMP6.sh XSTARTUP

gcc -w -o SKLUA.o -c SKLUA.cpp -I. -I../../lua/include -I../v6spxmex -D__LINUX__ -D__SDL__ -DSK9821V4 -D__SK_NO_ALLEGRO__ 

cd ..
g++ -g -W -Wall -O3 -D__LINUX__ -D__NOSDL__ -I.. -Iv4kj9821 -Iv6spxmex -I../lua/include -L.. -L../lua/lib v4kj9821/*.o -lSDL2 -o ../build/Sk9821V4_Linux_Release/skwin9821_linux
g++ -g -W -Wall -O3 -D__LINUX__ -D__NOSDL__ -I.. -Iv4kj9821 -Iv6spxmex -I../lua/include -L.. -L../lua/lib v4kj9821/*.o -lSDL2 -o skwin9821_linux

#./skwin9821_linux -data DATA2SK