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

	// set initial state
	GameOfLife.state = GAMEOFLIFE_STATE_PLAY;
	GameOfLife.bufferOffset = 0;

	// clear screen
	cls();
	send();
}

// ----------------------------------------------------------------------
// process game of life
void processGameOfLife( void )
{

	// states
	switch( GameOfLife.state )
	{
		
		// play mode
		case GAMEOFLIFE_STATE_PLAY :

			// loop through all cells
			for( unsigned char x = 0; x != 16; x++ )
			{
				for( unsigned char y = 0; y != 16; y++ )
				{

					// create read and write masks
					unsigned char readMask = (1 << GameOfLife.bufferOffset);
					unsigned char writeMask = (2 >> GameOfLife.bufferOffset);

					// count adjacent cells
					unsigned char count = 0, xm = x*16;
					if( (*(GameOfLife.frameBuffer + ((y+1)&0x0F) + ((xm+1)&0x0F) )) & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + ((y+1)&0x0F) + (xm&0x0F) ))     & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + ((y+1)&0x0F) + ((xm-1)&0x0F) )) & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + (y&0x0F)     + (((x-1)*16)&0x0F) )) & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + ((y-1)&0x0F) + (((x-1)*16)&0x0F) )) & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + ((y-1)&0x0F) + ((x*16)&0x0F) ))     & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + ((y-1)&0x0F) + (((x+1)*16)&0x0F) )) & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + (y&0x0F)     + (((x+1)*16)&0x0F) )) & readMask ) count++;

					// less than 3 cells, cell dies

				}
			}

			break;

		// edit mode
		case GAMEOFLIFE_STATE_EDIT :

			break;

		// error
		default : GameOfLife.state = GAMEOFLIFE_STATE_PLAY; break;
	}
}