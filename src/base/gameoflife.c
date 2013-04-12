// ----------------------------------------------------------------------
// Game of Life
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// include files
// ----------------------------------------------------------------------

#include "gameoflife.h"
#include "framework.h"
#include "uart.h"
#include "gameenable.h"

#ifdef GAME_ENABLE_GAME_OF_LIFE

static struct GameOfLife_t GameOfLife;

// ----------------------------------------------------------------------
// load game of life
void loadGameOfLife( unsigned char* frameBuffer, unsigned char* playerCount )
{

	// get frame buffer and player count
	GameOfLife.frameBuffer = frameBuffer;
	GameOfLife.bufferOffset = 0;
	GameOfLife.playerCount = playerCount;
	clearFrameBuffer( frameBuffer );

	// set player cursor positions
	unsigned char i;
	for( i = 0; i != 4; i++ )
	{
		GameOfLife.player[i].cursor.x = 7;
		GameOfLife.player[i].cursor.y = 7;
		GameOfLife.player[i].oldCursor = GameOfLife.player[0].cursor;
	}

	// multi-player specific settings
	if( *playerCount )
	{

		// set initial game state
		GameOfLife.state = GAMEOFLIFE_STATE_PLAY_MULTI;

		// draw starting positions for each player
		*(GameOfLife.frameBuffer+12+(7*16)) = 0x01;
		*(GameOfLife.frameBuffer+12+(8*16)) = 0x01;
		*(GameOfLife.frameBuffer+13+(7*16)) = 0x01;
		*(GameOfLife.frameBuffer+13+(8*16)) = 0x01;

		// player 2
		if( (*playerCount) & 0x01 )
		{
			*(GameOfLife.frameBuffer+2+(7*16)) = 0x04;
			*(GameOfLife.frameBuffer+2+(8*16)) = 0x04;
			*(GameOfLife.frameBuffer+3+(7*16)) = 0x04;
			*(GameOfLife.frameBuffer+3+(8*16)) = 0x04;
		}

		// player 3
		if( (*playerCount) & 0x02 )
		{
			*(GameOfLife.frameBuffer+7+(2*16)) = 0x40;
			*(GameOfLife.frameBuffer+7+(3*16)) = 0x40;
			*(GameOfLife.frameBuffer+8+(2*16)) = 0x40;
			*(GameOfLife.frameBuffer+8+(3*16)) = 0x40;
		}

		// player 4
		if( (*playerCount) & 0x04 )
		{
			*(GameOfLife.frameBuffer+7+(12*16)) = 0x10;
			*(GameOfLife.frameBuffer+7+(13*16)) = 0x10;
			*(GameOfLife.frameBuffer+8+(12*16)) = 0x10;
			*(GameOfLife.frameBuffer+8+(13*16)) = 0x10;
		}

	// single player specific settinsg
	}else{

		// set initial game state
		GameOfLife.state = GAMEOFLIFE_STATE_PLAY_SINGLE;

		// randomize frame buffer
		randomizeFrameBuffer();
	}

	// set refresh rate
	setRefreshRate( 64 );

	// initialise screen
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
		
		// single player mode
		case GAMEOFLIFE_STATE_PLAY_SINGLE :
computeNextCycle();
/*
			// loop through all cells
			for( unsigned char x = 0; x != 16; x++ )
			{
				for( unsigned char y = 0; y != 16; y++ )
				{

					// create read and write masks
					unsigned char readMask = (0x01 << GameOfLife.bufferOffset);
					unsigned char writeMask = (0x02 >> GameOfLife.bufferOffset);

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

			// flip buffers
			GameOfLife.bufferOffset = 1 - GameOfLife.bufferOffset;

			// draw buffer
			drawFrameBuffer();
			send();

			break;*/

		// multi player mode
		case GAMEOFLIFE_STATE_PLAY_MULTI :



			break;

		default : break;
	}
}

// ----------------------------------------------------------------------
// process game of life input
void processGameOfLifeInput( void )
{

	// state dependant input
	switch( GameOfLife.state )
	{

		// edit mode
		case GAMEOFLIFE_STATE_EDIT_SINGLE :

			// move cursor with up, down, left, right
			if( player1ButtonUp() ) GameOfLife.player[0].cursor.y--;
			if( player1ButtonDown() ) GameOfLife.player[0].cursor.y++;
			if( player1ButtonLeft() ) GameOfLife.player[0].cursor.x--;
			if( player1ButtonRight() ) GameOfLife.player[0].cursor.x++;

			// cursor limits
			GameOfLife.player[0].cursor.x &= 0x0F;
			GameOfLife.player[0].cursor.y &= 0x0F;

			// edit cells
			if( player1ButtonFire() )
			{

				// update frame buffer and draw new cursor
				if( (*(GameOfLife.frameBuffer + GameOfLife.player[0].cursor.y + (GameOfLife.player[0].cursor.x*16) )) & (0x01 << GameOfLife.bufferOffset) )
					(*(GameOfLife.frameBuffer + GameOfLife.player[0].cursor.y + (GameOfLife.player[0].cursor.x*16) )) = 0x00;
				else
					(*(GameOfLife.frameBuffer + GameOfLife.player[0].cursor.y + (GameOfLife.player[0].cursor.x*16) )) = (0x01 << GameOfLife.bufferOffset);
			}

			// remove old cursor
			if( (*(GameOfLife.frameBuffer + GameOfLife.player[0].oldCursor.y + (GameOfLife.player[0].oldCursor.x*16) )) & (0x01 << GameOfLife.bufferOffset) )
				dot( &GameOfLife.player[0].oldCursor.x, &GameOfLife.player[0].oldCursor.y, &BLUE );
			else
				dot( &GameOfLife.player[0].oldCursor.x, &GameOfLife.player[0].oldCursor.y, &BLACK );
			GameOfLife.player[0].oldCursor = GameOfLife.player[0].cursor;

			// draw new cursor
			if( (*(GameOfLife.frameBuffer + GameOfLife.player[0].cursor.y + (GameOfLife.player[0].cursor.x*16) )) & (0x01 << GameOfLife.bufferOffset) )
				dot( &GameOfLife.player[0].cursor.x, &GameOfLife.player[0].cursor.y, &YELLOW );
			else
				dot( &GameOfLife.player[0].cursor.x, &GameOfLife.player[0].cursor.y, &WHITE );

			// clear frame buffer with clear button
			if( player1ButtonClear() )
			{
				clearFrameBuffer( GameOfLife.frameBuffer );
				drawFrameBufferNoCheck();
				dot( &GameOfLife.player[0].cursor.x, &GameOfLife.player[0].cursor.y, &WHITE );
				send();
			}

			// resume simulation
			if( player1ButtonMenu() )
			{
				drawFrameBufferNoCheck(); // force re-drawing of all pixels
				GameOfLife.state = GAMEOFLIFE_STATE_PLAY_SINGLE;
			}

			// send graphic changes to display
			send();

			break;

		// during play
		case GAMEOFLIFE_STATE_PLAY_SINGLE :

			// switch to editing mode
			if( player1ButtonFire() )
			{
				drawFrameBufferCustom( &BLUE, &RED, &GREEN, &YELLOW ); // force re-drawing of all pixels in a different colour
				dot( &GameOfLife.player[0].cursor.x, &GameOfLife.player[0].cursor.y, &WHITE );
				send();
				GameOfLife.state = GAMEOFLIFE_STATE_EDIT_SINGLE;
			}

			// end game with menu button
			if( player1ButtonMenu() ) endGame();

			break;

		// during multi play
		case GAMEOFLIFE_STATE_PLAY_MULTI :

			// loop through each player
			for( unsigned char i = 0; i != 4; i++ )
			{

				// move cursor
				if( playerButtonLeft(i) ) GameOfLife.player[i].cursor.x--;
				if( playerButtonRight(i) ) GameOfLife.player[i].cursor.x++;
				if( playerButtonUp(i) ) GameOfLife.player[i].cursor.y--;
				if( playerButtonDown(i) ) GameOfLife.player[i].cursor.y++;

				// cursor limits
				GameOfLife.player[i].cursor.x &= 0x0F;
				GameOfLife.player[i].cursor.y &= 0x0F;

				// edit cells
				if( playerButtonFire(i) )
				{

					// update frame buffer and draw new cursor
					if( (*(GameOfLife.frameBuffer + GameOfLife.player[i].cursor.y + (GameOfLife.player[i].cursor.x*16) )) & (0x01 << GameOfLife.bufferOffset) )
						(*(GameOfLife.frameBuffer + GameOfLife.player[i].cursor.y + (GameOfLife.player[i].cursor.x*16) )) = 0x00;
					else
						(*(GameOfLife.frameBuffer + GameOfLife.player[i].cursor.y + (GameOfLife.player[i].cursor.x*16) )) = (0x01 << GameOfLife.bufferOffset);
				}

				// remove old cursor
				if( (*(GameOfLife.frameBuffer + GameOfLife.player[i].oldCursor.y + (GameOfLife.player[i].oldCursor.x*16) )) & (0x01 << GameOfLife.bufferOffset) )
					dot( &GameOfLife.player[i].oldCursor.x, &GameOfLife.player[i].oldCursor.y, &BLUE );
				else
					dot( &GameOfLife.player[i].oldCursor.x, &GameOfLife.player[i].oldCursor.y, &BLACK );
				GameOfLife.player[i].oldCursor = GameOfLife.player[i].cursor;

				// draw new cursor
				if( (*(GameOfLife.frameBuffer + GameOfLife.player[i].cursor.y + (GameOfLife.player[i].cursor.x*16) )) & (0x01 << GameOfLife.bufferOffset) )
					dot( &GameOfLife.player[i].cursor.x, &GameOfLife.player[i].cursor.y, &YELLOW );
				else
					dot( &GameOfLife.player[i].cursor.x, &GameOfLife.player[i].cursor.y, &WHITE );

			}

			// end game with menu button
			if( player1ButtonMenu() ) endGame();

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
		*(GameOfLife.frameBuffer+x) = ((rnd() >> 4)&0x01);
		x++;
	}while( x != 0 );
}

// ----------------------------------------------------------------------
// draws entire buffer without checking for differences
void drawFrameBufferNoCheck()
{

	// render pixels
	unsigned char x, y;
	for( x = 0; x != 16; x++ )
	{
		for( y = 0; y != 16; y++ )
		{
			unsigned char buffer = (*(GameOfLife.frameBuffer+y+(x*16)));
			if( buffer & (0x01 << GameOfLife.bufferOffset ) ) dot( &x, &y, &GREEN );
			if( buffer & (0x04 << GameOfLife.bufferOffset ) )  dot( &x, &y, &RED );
			if( buffer & (0x10 << GameOfLife.bufferOffset ) ) dot( &x, &y, &BLUE );
			if( buffer & (0x40 << GameOfLife.bufferOffset ) ) dot( &x, &y, &YELLOW );
			if( (buffer & (0x55 << GameOfLife.bufferOffset)) == 0 ) dot( &x, &y, &BLACK );
		}
	}
}

// ----------------------------------------------------------------------
// renders entire frame buffer with out checks and custom colours
void drawFrameBufferCustom( const unsigned short* colour1, const unsigned short* colour2, const unsigned short* colour3, const unsigned short* colour4 )
{

	// render pixels
	unsigned char x, y;
	for( x = 0; x != 16; x++ )
	{
		for( y = 0; y != 16; y++ )
		{
			unsigned char buffer = (*(GameOfLife.frameBuffer+y+(x*16)));
			if( buffer & (0x01 << GameOfLife.bufferOffset ) ) dot( &x, &y, colour1 );
			if( buffer & (0x04 << GameOfLife.bufferOffset ) ) dot( &x, &y, colour2 );
			if( buffer & (0x10 << GameOfLife.bufferOffset ) ) dot( &x, &y, colour3 );
			if( buffer & (0x40 << GameOfLife.bufferOffset ) ) dot( &x, &y, colour4 );
			if( (buffer & (0x55 << GameOfLife.bufferOffset)) == 0 ) dot( &x, &y, &BLACK );
		}
	}
}

// ----------------------------------------------------------------------
// draws the buffer for all players
// optimized to only update the pixels that have changed
void drawFrameBuffer( void )
{

	// render pixels
	register unsigned char x, y;
	unsigned char buffer;
	for( x = 0; x != 16; x++ )
	{
		for( y = 0; y != 16; y++ )
		{

			// get buffer content
			buffer = (*(GameOfLife.frameBuffer+y+(x<<4)));

			// player 1 - draw new pixels
			if( (buffer & (0x01 << GameOfLife.bufferOffset)) )
			{
				if( (buffer & (0x02 >> GameOfLife.bufferOffset)) == 0 ) dot( &x, &y, &GREEN );
			}else{

				// player 2 - draw new pixels
				if( (buffer & (0x04 << GameOfLife.bufferOffset)) )
				{
					if( (buffer & (0x08 >> GameOfLife.bufferOffset)) == 0 ) dot( &x, &y, &RED );
				}else{

					// player 3 - draw new pixels
					if( (buffer & (0x10 << GameOfLife.bufferOffset)) )
					{
						if( (buffer & (0x20 >> GameOfLife.bufferOffset)) == 0 ) dot( &x, &y, &BLUE );
					}else{

						// player 4 - draw new pixels
						if( (buffer & (0x40 << GameOfLife.bufferOffset)) )
						{
							if( (buffer & (0x80 >> GameOfLife.bufferOffset)) == 0 ) dot( &x, &y, &YELLOW );
						}else{

							// clear old pixels
							if( buffer & (0xAA >> GameOfLife.bufferOffset) ) dot( &x, &y, &BLACK );
						}
					}
				}
			}

		}
	}
}

// ----------------------------------------------------------------------
// computes next cycle of evolution
void computeNextCycle( void )
{

	// local variables
	register unsigned char count;
	unsigned char readMask;
	register unsigned char i, x, y;
	unsigned char* bufferPtr;

	// loop through all cells
	for( x = 0; x != 16; x++ )
	{
		for( y = 0; y != 16; y++ )
		{

			// create read mask
			readMask = (0x55 << GameOfLife.bufferOffset);

			// count adjacent cells (general for all players)
			count = 0;
			if( (*(GameOfLife.frameBuffer + ((y+1)&0x0F) + (((x+1)&0x0F)<<4) )) & readMask ) count++;
			if( (*(GameOfLife.frameBuffer + ((y+1)&0x0F) + (x<<4)            )) & readMask ) count++;
			if( (*(GameOfLife.frameBuffer + ((y+1)&0x0F) + (((x-1)&0x0F)<<4) )) & readMask ) count++;
			if( (*(GameOfLife.frameBuffer + (y&0x0F)     + (((x-1)&0x0F)<<4) )) & readMask ) count++;
			if( (*(GameOfLife.frameBuffer + ((y-1)&0x0F) + (((x-1)&0x0F)<<4) )) & readMask ) count++;
			if( (*(GameOfLife.frameBuffer + ((y-1)&0x0F) + (x<<4)            )) & readMask ) count++;
			if( (*(GameOfLife.frameBuffer + ((y-1)&0x0F) + (((x+1)&0x0F)<<4) )) & readMask ) count++;
			if( (*(GameOfLife.frameBuffer + (y&0x0F)     + (((x+1)&0x0F)<<4) )) & readMask ) count++;

			// get buffer pointer
			bufferPtr = (GameOfLife.frameBuffer + y + (x<<4));

			// current cell is alive
			if( (*bufferPtr) & readMask )
			{

				// less than 2 neighbours or more than 3 neighbours kills it
				if( count < 2 || count > 3 )
				{
					(*bufferPtr) &= readMask;

				// cell remains alive
				}else{
					if( GameOfLife.bufferOffset ) (*bufferPtr) |= (((*bufferPtr) & readMask)>>1); else (*bufferPtr) |= (((*bufferPtr) & readMask)<<1);
				}

			// current cell is dead
			}else{

				// has 3 neighbours, new cell is born
				if( count == 3 )
				{

					// determine who it belongs to by counting who has the most adjacent cells
					for( i = 0; i != 4; i++ )
					{

						// counts surrounding cells of current player
						count = 0;
						readMask = (1<<(i<<1))<<GameOfLife.bufferOffset;
						if( (*(GameOfLife.frameBuffer + ((y+1)&0x0F) + (((x+1)&0x0F)<<4) )) & readMask ) count++;
						if( (*(GameOfLife.frameBuffer + ((y+1)&0x0F) + (x<<4)            )) & readMask ) count++;
						if( (*(GameOfLife.frameBuffer + ((y+1)&0x0F) + (((x-1)&0x0F)<<4) )) & readMask ) count++;
						if( (*(GameOfLife.frameBuffer + (y&0x0F)     + (((x-1)&0x0F)<<4) )) & readMask ) count++;
						if( (*(GameOfLife.frameBuffer + ((y-1)&0x0F) + (((x-1)&0x0F)<<4) )) & readMask ) count++;
						if( (*(GameOfLife.frameBuffer + ((y-1)&0x0F) + (x<<4)            )) & readMask ) count++;
						if( (*(GameOfLife.frameBuffer + ((y-1)&0x0F) + (((x+1)&0x0F)<<4) )) & readMask ) count++;
						if( (*(GameOfLife.frameBuffer + (y&0x0F)     + (((x+1)&0x0F)<<4) )) & readMask ) count++;

						// more than one cell and we have found our winner
						if( count > 1 ) break;
					}

					// rare case where 3 players surround the same cell - cell remains dead
					if( i == 4 )
					{
						(*bufferPtr) &= (0x55<<GameOfLife.bufferOffset);
					
					// spawn cell
					}else{
						(*bufferPtr) |= ((1<<(i<<1)) << (1>>GameOfLife.bufferOffset));;
					}

				// cell remains dead
				}else{
					(*bufferPtr) &= readMask;
				}
			}

		}
	}

	// flip buffers
	GameOfLife.bufferOffset = 1 - GameOfLife.bufferOffset;

	// draw buffer
	drawFrameBuffer();
	send();

}
#endif // GAME_ENABLE_GAME_OF_LIFE
