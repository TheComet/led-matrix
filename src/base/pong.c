// ----------------------------------------------------------------------
// Pong
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include Files
// ----------------------------------------------------------------------

#include "framework.h"
#include "uart.h"
#include "pong.h"
#include "gameenable.h"

#ifdef GAME_ENABLE_PONG

struct Pong_t Pong;

// ----------------------------------------------------------------------
// load pong
void loadPong( unsigned short* frameBuffer, unsigned char* playerCount )
{
}

// ----------------------------------------------------------------------
// process pong main loop
void processPongLoop( void )
{
	endGame(); // remove this
}

// ----------------------------------------------------------------------
// process pong input
void processPongInput( void )
{
}
#endif // GAME_ENABLE_PONG