// ----------------------------------------------------------------------
// Start-Up Screen
// ----------------------------------------------------------------------

#ifndef _STARTUPSCREEN_H_
	#define _STARTUPSCREEN_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct StartUpScreen_t
{
	unsigned char toggle;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void drawStartArrow( void );
void drawButtonHelp( void );
void loadStartUpScreen( void );
void processStartUpScreenLoop( void );
void processStartUpScreenInput( void );

#endif // _STARTUPSCREEN_H_