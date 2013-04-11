// ----------------------------------------------------------------------
// Space Invaders
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "gameenable.h"
#include "spaceinvaders.h"
#include "framework.h"
#include "uart.h"

#ifdef GAME_ENABLE_SPACE_INVADERS

static struct SpaceInvaders_t SpaceInvaders;

// ----------------------------------------------------------------------
// load tron
void loadSpaceInvaders( unsigned char* frameBuffer, unsigned char* playerCount )
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

#endif // GAME_ENABLE_SPACE_INVADERS
