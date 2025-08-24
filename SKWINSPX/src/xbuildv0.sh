#!/bin/bash
source ./xenvv0.sh
echo "Removing object files..."
rm -f ../build/SKWinV0_Linux_Release/*.o
## base v0
./xcomp.sh skmini
./xcomp.sh skparam
./xcomp.sh skwinapp
./xcomp.sh skrender
./xcomp.sh skdos
./xcomp.sh sksdl2
./xcomp.sh skmfc
./xcomp.sh skwinmfc
./xcomp.sh skvram
## file + utils
./xcomp.sh skdebug
./xcomp.sh skfile
./xcomp.sh skcnsole
## general game info / func / structures
./xcomp.sh skglobal
./xlink.sh