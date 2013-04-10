// ----------------------------------------------------------------------
// Colour Demo
// ----------------------------------------------------------------------

#ifndef _COLOURDEMO_H_
	#define _COLOURDEMO_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct ColourDemo_t
{
	unsigned short angle;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadColourDemo( unsigned char* frameBuffer, unsigned char* playerCount );
void processColourDemoLoop( void );
void processColourDemoInput( void );

#endif // _COLOURDEMO_H_