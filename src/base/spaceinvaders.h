// ----------------------------------------------------------------------
// Space Invaders
// ----------------------------------------------------------------------

#ifndef _SPACEINVADERS_H_
	#define _SPACEINVADERS_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct SpaceInvaders_t
{
	unsigned char* frameBuffer;
	unsigned char* playerCount;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadSpaceInvaders( unsigned char* frameBuffer, unsigned char* playerCount );
void processSpaceInvadersLoop( void );
void processSpaceInvadersInput( void );

#endif // _SPACEINVADERS_H_