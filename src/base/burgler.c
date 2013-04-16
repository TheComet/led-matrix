// ----------------------------------------------------------------------
// Burgler
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include Files
// ----------------------------------------------------------------------

#include "burgler.h"
#include "framework.h"
#include "uart.h"
#include "gameenable.h"

#ifdef GAME_ENABLE_BURGLER

static struct Burgler_t Burgler;

// ----------------------------------------------------------------------
// load burgler
void loadBurgler( unsigned short* frameBuffer, unsigned char* playerCount )
{

	// get frame buffer
	Burgler.frameBuffer = frameBuffer;

	// get players
	Burgler.playerCount = playerCount;
}

// ----------------------------------------------------------------------
// process burgler main loop
void processBurglerLoop( void )
{
	endGame(); // remove this
}

// ----------------------------------------------------------------------
// process burgler input
void processBurglerInput( void )
{
}
#endif // GAME_ENABLE_BURGLER
