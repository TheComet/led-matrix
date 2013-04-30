// ----------------------------------------------------------------------
// Pong
// ----------------------------------------------------------------------

#ifndef _PONG_H_
	#define _PONG_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct Pong_t
{
	unsigned short* frameBuffer;
	unsigned char* playerCount;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadPong( unsigned short* frameBuffer, unsigned char* playerCount );
void processPongLoop( void );
void processPongInput( void );
void drawPongMenuIcon( void );

#endif // _PONG_H_