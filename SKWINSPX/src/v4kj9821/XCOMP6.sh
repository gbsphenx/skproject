#!/bin/bash
## Unit Compilation
SOURCE=$1
gcc -o ${SOURCE}.o -c ../V6SPXMEX/${SOURCE}.cpp -I. -I../v6spxmex -D__LINUX__ -DSK9821V4 -D__SK_NO_ALLEGRO__
