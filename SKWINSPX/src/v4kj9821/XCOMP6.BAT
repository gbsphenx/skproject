REM Unit Compilation
SET SOURCE=%1
gpp -o %SOURCE%.o -c ..\V6SPXMEX\%SOURCE%.cpp -I. -I..\v6spxmex -DSK9821V4 -D__SK_NO_ALLEGRO__
