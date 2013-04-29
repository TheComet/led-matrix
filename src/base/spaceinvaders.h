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
	unsigned short* frameBuffer;
	unsigned char* playerCount;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadSpaceInvaders( unsigned short* frameBuffer, unsigned char* playerCount );
void processSpaceInvadersLoop( void );
void processSpaceInvadersInput( void );
void drawSpaceInvadersMenuIcon( void );

#endif // _SPACEINVADERS_H_