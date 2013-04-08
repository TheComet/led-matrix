// ----------------------------------------------------------------------
// Game of Life
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// include files
// ----------------------------------------------------------------------

#include "gameoflife.h"
#include "framework.h"
#include "uart.h"

struct GameOfLife_t GameOfLife;

// ----------------------------------------------------------------------
// load game of life
void loadGameOfLife( unsigned char* frameBuffer )
{

	// get frame buffer and reset it
	GameOfLife.frameBuffer = frameBuffer;
	clearFrameBuffer( GameOfLife.frameBuffer );

	// clear screen
	cls();
	send();
}

// ----------------------------------------------------------------------
// process game of life
void processGameOfLife( void )
{
}