// ----------------------------------------------------------------------
// Tetris
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "gameenable.h"
#include "tetris.h"
#include "framework.h"
#include "uart.h"

#ifdef GAME_ENABLE_TETRIS

static struct Tetris_t Tetris;

// ----------------------------------------------------------------------
// load tetris
void loadTetris( unsigned char* frameBuffer, unsigned char* playerCount )
{
}

// ----------------------------------------------------------------------
// process tetris loop
void processTetrisLoop( void )
{
	endGame(); // remove this
}

// ----------------------------------------------------------------------
// process tetris input
void processTetrisInput( void )
{
}

#endif // GAME_ENABLE_TETRIS
