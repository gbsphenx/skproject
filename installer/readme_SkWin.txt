SkWin & SkWinDOS
================
Current Release : june 2021, Sphenx
SkWin conversion code (DM2 PC9821) by Kentaro
SkWinDOS conversion code (DM2 PC-DOS) by Bit
Enhancements, packaging and custom graphics.dat by Sphenx


Executables
-----------

SkWin refers to converted PC-9821 version.
It natively runs DM2 V4 (Japanese + English)
	It was enhanced to support V5 256 colored images (like in DM2 PC-DOS & Mac)

SkullWin, or Skull.exe refers to converted PC-DOS version
It natively runs DM2 V5 (English + German + French)


SKWin
-----

Note: If you get a "SYSTEM ERROR 41" when starting SKWin.exe, this is because the program does not find the game data file.
 That means you entered a combination of choices that do not match any game data file ("graphics.dat") in your directories.

In order to restore a correct startup you may :

- Delete the file SkWin.ini from your directory : it will reset all parameters.
- Edit file SkWin.ini
	Set gdat_vers=0 and dung=5 to get the original Skullkeep data file.



Game Combination
----------------

DM2: The Legend of Skullkeep was released on different systems over time (from end of 1993 to 1995)
and also has different version assets improved along the different releases.
Mainly, there are the "classic" look vs "cartoony" look of the PC/Mac version
(Weirdly, the Amiga version is a mix between classic and cartoon).
But what if you would want to play the PC-DOS version (the best colored version) with the classic look ?

Well, the graphics.dat design allows different combinations to be made.
Hence I worked on both PC-9821 and PC-DOS datasets to provide compatible assets
 so you can choose the engine and the look of the game.

Here is how this works:

"Skull" runs the PC-DOS engine.
"SkWin" runs the PC-9821 engine.

V3 is the first classic looking, mainly for Japanese systems PC-9801, FM-Towns, SegaCD (end of 1993)
V4 is the second classic looking, mainly for Japanese systems IBM/PSV, PC-9821 (summer 1994)
V5 is the enhanced 256 colors looking, mainly for PC-DOS and Mac (summer 1995)

You can combine run options in batch file to launch DM2 in different modes.
Here are the possible switches* (check the existing .bat)

-V3   Opens a V3 graphics.dat
-V4   Opens a V4 graphics.dat
-V5   Opens a V5 graphics.dat

-jp   Select Japanese language
-en   Select English language
-ge   Select German language
-fr   Select French language
-es   Select Spanish language
-it   Select Italian language

-hmp  Use converted PC HMP musics (actually converted to midi files) 
-mod  Use converted Amiga "modules" music (actually converted to ogg files)
-ogg  Use ogg musics (allows some custom musics)


* Switches work if the combination assets exist for the target engine.

Please note that all of this is still in experimental state and not everything may yet work properly.

Contact me:
-----------
gb_sphenx@hotmail.com
