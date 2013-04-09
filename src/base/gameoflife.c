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

	// get frame buffer and fill it with random data
	GameOfLife.frameBuffer = frameBuffer;
	randomizeFrameBuffer();

	// set initial state
	GameOfLife.state = GAMEOFLIFE_STATE_PLAY;
	GameOfLife.bufferOffset = 0;

	// set refresh rate
	setRefreshRate( 64 );

	// draw screen
	cls();
	drawFrameBuffer();
	send();
}

// ----------------------------------------------------------------------
// process game of life
void processGameOfLifeLoop( void )
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
					unsigned char readMask = (0x02 >> GameOfLife.bufferOffset);
					unsigned char writeMask = (0x01 << GameOfLife.bufferOffset);

					// count adjacent cells
					unsigned char count = 0;
					if( (*(GameOfLife.frameBuffer + ((y+1)&0x0F) + (((x+1)&0x0F)*16) )) & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + ((y+1)&0x0F) + (x*16)            )) & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + ((y+1)&0x0F) + (((x-1)&0x0F)*16) )) & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + (y&0x0F)     + (((x-1)&0x0F)*16) )) & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + ((y-1)&0x0F) + (((x-1)&0x0F)*16) )) & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + ((y-1)&0x0F) + (x*16)            )) & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + ((y-1)&0x0F) + (((x+1)&0x0F)*16) )) & readMask ) count++;
					if( (*(GameOfLife.frameBuffer + (y&0x0F)     + (((x+1)&0x0F)*16) )) & readMask ) count++;

					// current cell is alive
					if( (*(GameOfLife.frameBuffer + y + (x*16))) & readMask )
					{

						// less than 2 neighbours or more than 3 neighbours kills it
						if( count < 2 || count > 3 ) (*(GameOfLife.frameBuffer + y + (x*16))) &= ~writeMask; else *(GameOfLife.frameBuffer + y + (x*16)) |= writeMask;

					// current cell is dead
					}else{

						// has 3 neighbours, new cell is born
						if( count == 3 ) (*(GameOfLife.frameBuffer + y + (x*16))) |= writeMask; else *(GameOfLife.frameBuffer + y + (x*16)) &= ~writeMask;
					}
				}
			}

			// draw buffer
			drawFrameBuffer();
			send();

			// flip buffers
			GameOfLife.bufferOffset = 1 - GameOfLife.bufferOffset;

			break;

		// edit mode
		case GAMEOFLIFE_STATE_EDIT :

			// 

			break;

		// error
		default : break;
	}
}

// ----------------------------------------------------------------------
// process game of life input
void processGameOfLifeInput( void )
{
	if( player1ButtonFire() ) endGame();
}

// ----------------------------------------------------------------------
// randomizes buffer
void randomizeFrameBuffer( void )
{
	unsigned char x = 0;
	do{
		*(GameOfLife.frameBuffer+x) = ((rnd() >> 4)&0x02);
		x++;
	}while( x != 0 );
}

// ----------------------------------------------------------------------
// draws the buffer
void drawFrameBuffer( void )
{

	// colours
	unsigned short cD = 0x0E0, cC = 0;

	// render pixels
	unsigned char x, y;
	for( x = 0; x != 16; x++ )
	{
		for( y = 0; y != 16; y++ )
		{
			unsigned char buffer = (*(GameOfLife.frameBuffer+y+(x*16)));
			if( buffer & (0x01 << GameOfLife.bufferOffset) )
			{
				if( (buffer & (0x02 >> GameOfLife.bufferOffset)) == 0 )
					dot( &x, &y, &cD );
			}else{
				if( buffer & (0x02 >> GameOfLife.bufferOffset) )
					dot( &x, &y, &cC );
			}
		}
	}
}