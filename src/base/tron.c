// ----------------------------------------------------------------------
// Tron
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "gameenable.h"
#include "tron.h"
#include "framework.h"
#include "uart.h"

#ifdef GAME_ENABLE_TRON

static struct Tron_t Tron;

// ----------------------------------------------------------------------
// load tron
void loadTron( unsigned char* frameBuffer, unsigned char* playerCount )
{
}

// ----------------------------------------------------------------------
// process tron loop
void processTronLoop( void )
{
	endGame(); // remove this
}

// ----------------------------------------------------------------------
// process tron input
void processTronInput( void )
{
}

#endif // GAME_ENABLE_TRON
