//------------------------------------------------------------------------------
#ifndef __LINUX__
#include <stdafx.h>	// Required for MVC6 compilation
#endif // __LINUX__

#include <skcnsole.h>
#include <stdio.h>
//#include <stdlib.h> // rand note: putting stdlib here and not right after stdafx prevents a bunch of conflicts with min/max macros
#include <string.h>	// mingw

//------------------------------------------------------------------------------

void CHANGE_CONSOLE_COLOR(int attr, int fg, int bg)
{
	/* Command is the control command to the terminal */
#if (defined __LINUX__  || __CYGWIN__)
	{
		char command[48];
		//--- This is C magic!
		fflush(stdout);
		sprintf(command, "%c[%d;%d;%dm", 0x1B, attr, fg + 30, bg + 40);
		if (attr == RESET)
			sprintf(command, "\x1B[0m");
		printf("%s", command);
	}
#elif (__MINGW__)
	{
		char command[48];
		char sBold[8];
		bool bUseBold = false;
		memset(command, 0, 48);
		memset(sBold, 0, 8);

		if (fg >= 8)
		{
			bUseBold = true;
			fg = (fg%8);
			sprintf(sBold, "\x1B[1m");	// bold -- or light colors
		}
		if (bg >= 8)
			bg = (bg%8);

		//-- Bit of color magic :/
		if (bg == 1)
			bg = 4;
		else if (bg == 4)
			bg = 1;

		sprintf(command, "%s\x1B[3%dm\x1B[4%dm", sBold, fg, bg);
		if (attr == RESET)
			sprintf(command, "\x1B[0m");
		printf("%s", command);
	}
#elif (defined __WINDOWS__ || defined WIN32)
	HANDLE hconsole = GetStdHandle (STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute (hconsole, fg | bg << 4); 
#endif
}