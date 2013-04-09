// ----------------------------------------------------------------------
// Game of Life
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// include files
// ----------------------------------------------------------------------

#include "gameoflife.h"
#include "framework.h"
#include "uart.h"

static struct GameOfLife_t GameOfLife;

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
	setRefreshRate( 128 );

	// initialise screen
	cls();
	drawFrameBuffer( &GREEN, 0 );
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
			drawFrameBuffer( &GREEN, 0 );
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
	
	// switch modes
	if( player1ButtonFire() )
	{
		switch( GameOfLife.state )
		{

			// switching to edit mode
			case GAMEOFLIFE_STATE_PLAY :
				GameOfLife.bufferOffset = 0; // edit mode always in buffer 0
				drawFrameBuffer( &BLUE, 1 ); // force re-drawing of all pixels in a different colour
				GameOfLife.state = GAMEOFLIFE_STATE_EDIT;
				break;

			// switching to play mode
			case GAMEOFLIFE_STATE_EDIT :
				drawFrameBuffer( &GREEN, 1 ); // force re-drawing of all pixels in a different colour
				GameOfLife.state = GAMEOFLIFE_STATE_PLAY;
				break;

			default: break;
		}
	}

	// end game with menu button
	if( player1ButtonMenu() ) endGame();

	// state dependant input
	switch( GameOfLife.state )
	{

		// edit mode
		case GAMEOFLIFE_STATE_EDIT :

			// move cursor with up, down, left, right
			if( player1ButtonUp() ) GameOfLife.cursor.y--;
			if( player1ButtonDown() ) GameOfLife.cursor.y++;
			if( player1ButtonLeft() ) GameOfLife.cursor.x--;
			if( player1ButtonRight() ) GameOfLife.cursor.x++;

			// update cursor graphic
			if( (*(GameOfLife.frameBuffer + GameOfLife.oldCursor.y + (GameOfLife.oldCursor.x*16) )) & 0x02 )
				dot( &GameOfLife.oldCursor.x, &GameOfLife.oldCursor.x, &ZERO );
			

			break;

		default: break;
	}
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
void drawFrameBuffer( const unsigned short* colour, unsigned char forceDraw )
{

	// render pixels
	unsigned char x, y;
	for( x = 0; x != 16; x++ )
	{
		for( y = 0; y != 16; y++ )
		{
			unsigned char buffer = (*(GameOfLife.frameBuffer+y+(x*16)));
			if( buffer & (0x01 << GameOfLife.bufferOffset) )
			{
				if( forceDraw || (buffer & (0x02 >> GameOfLife.bufferOffset)) == 0 )
					dot( &x, &y, colour );
			}else{
				if( forceDraw || (buffer & (0x02 >> GameOfLife.bufferOffset)) )
					dot( &x, &y, &BLACK );
			}
		}
	}
}