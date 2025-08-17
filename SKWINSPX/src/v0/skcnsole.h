#ifndef _SKCONSOLE_H_

//------------------------------------------------------------------------------
// CONSOLE COLORS / ATTRIBUTES

#define RESET			0
#define BRIGHT 			1
#define DIM				2
#define UNDERLINE 		3
#define CONSOLE_BLINK	4
#define REVERSE			7
#define HIDDEN			8

#if (defined (__MINGW__))
#define BLACK 			0
#define RED				1
#define GREEN			2
#define GOLD			3
#define YELLOW			3
#define BLUE			4		
#define PURPLE			5
#define AQUA			6
#define LIGHT_GRAY		7

#define GRAY			8
#define LIGHT_RED		9
#define LIGHT_GREEN		10
#define LIGHT_YELLOW	11
#define LIGHT_BLUE		12
#define LIGHT_PURPLE	13
#define CYAN			14
#define WHITE			15

#elif (defined (__DJGPP__))

#define BLACK 			0
#define BLUE			4		
#define GREEN			2
#define AQUA			6
#define RED				1
#define PURPLE			5
#define GOLD			3
	#define YELLOW			3
#define LIGHT_GRAY		7
#define GRAY			0
#define LIGHT_BLUE		4
#define LIGHT_GREEN		2
#define CYAN			6
#define LIGHT_RED		1
#define LIGHT_PURPLE	5
#define LIGHT_YELLOW	3
#define WHITE			7

//#elif (defined (__LINUX__) || (__CYGWIN__))
//#elif (defined (__LINUX__) || (__MINGW__))
#elif (defined (__LINUX__))

#define BLACK 			0
#define BLUE			4		
#define GREEN			2
#define AQUA			6
#define RED				1
#define PURPLE			5
#define GOLD			3
	#define YELLOW			3
#define LIGHT_GRAY		7
#define GRAY			0
#define LIGHT_BLUE		4
#define LIGHT_GREEN		2
#define CYAN			6
#define LIGHT_RED		1
#define LIGHT_PURPLE	5
#define LIGHT_YELLOW	3
#define WHITE			7

#elif (defined (WIN32))
//#elif (defined (__WINDOWS__))

#define BLACK 			0
#define BLUE			1		
#define GREEN			2
#define AQUA			3
#define RED				4
#define PURPLE			5
#define GOLD			6
	#define YELLOW			6
#define LIGHT_GRAY		7
#define GRAY			8
#define LIGHT_BLUE		9
#define LIGHT_GREEN		10
#define CYAN			11
#define LIGHT_RED		12
#define LIGHT_PURPLE	13
#define LIGHT_YELLOW	14
#define WHITE			15

#endif

void CHANGE_CONSOLE_COLOR(int attr, int fg, int bg);

#endif // _SKCONSOLE_H_