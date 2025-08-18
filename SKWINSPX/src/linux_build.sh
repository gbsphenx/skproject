mkdir ../build/SKWinV0_Linux_Release
mkdir ../build/SKWinV4_Linux_Release
mkdir ../build/SKWinV5_Linux_Release
mkdir ../build/SKWinV6_Linux_Release
##make -f makefile_linux_skwinv0 clean
make -f makefile_linux_skwinv0 skwinv0-linux
mv ../build/SKWinV0_Linux_Release/skwinv0-linux ../skgame/