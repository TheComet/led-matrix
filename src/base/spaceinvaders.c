// ----------------------------------------------------------------------
// Space Invaders
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "spaceinvaders.h"
#include "framework.h"
#include "uart.h"
#include "gameenable.h"

#ifdef GAME_ENABLE_SPACE_INVADERS

static struct SpaceInvaders_t SpaceInvaders;

// ----------------------------------------------------------------------
// load tron
void loadSpaceInvaders( unsigned short* frameBuffer, unsigned char* playerCount )
{
}

// ----------------------------------------------------------------------
// process tron loop
void processSpaceInvadersLoop( void )
{
	endGame(); // remove this
}

// ----------------------------------------------------------------------
// process tron input
void processSpaceInvadersInput( void )
{
}

// ----------------------------------------------------------------------
// draws the space invaders icon for the menu
void drawSpaceInvadersMenuIcon( void )
{
}
#endif // GAME_ENABLE_SPACE_INVADERS