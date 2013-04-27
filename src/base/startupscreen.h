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
void drawStartUpScreenIconDummy( void );
void loadStartUpScreen( unsigned short* frameBuffer, unsigned char* playerCount );
void processStartUpScreenLoop( void );
void processStartUpScreenInput( void );

#endif // _STARTUPSCREEN_H_
