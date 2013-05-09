// ----------------------------------------------------------------------
// Tetris
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "tetris.h"
#include "framework.h"
#include "uart.h"
#include "moduleenable.h"

#ifdef MODULE_ENABLE_TETRIS

static struct Tetris_t Tetris;

// ----------------------------------------------------------------------
// load tetris
void loadTetris( unsigned short* frameBuffer, unsigned char* playerCount )
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

// ----------------------------------------------------------------------
// draws menu icon for tetris
void drawTetrisMenuIcon( void )
{
	unsigned char x1=5, y1=4, x2=7, y2=9;
	fillBox( &x1, &y1, &x2, &y2, &WHITE );
	x1=8; y1=7; x2=10; y2=12;
	fillBox( &x1, &y1, &x2, &y2, &WHITE );
}
#endif // GAME_ENABLE_TETRIS
