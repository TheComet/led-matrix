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
	unsigned short* frameBuffer;
	unsigned short angle;
	unsigned char startR;
	unsigned char startG;
	unsigned char startB;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadColourDemo( unsigned short* frameBuffer, unsigned char* playerCount );
void processColourDemoLoop( void );
void processColourDemoInput( void );
void colourDemoDrawFrameBuffer( void );

#endif // _COLOURDEMO_H_
