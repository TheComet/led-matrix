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
void loadGameOfLife( unsigned short* frameBuffer, unsigned char* playerCount )
{

	// get frame buffer and player count
	GameOfLife.frameBuffer = frameBuffer;
	GameOfLife.bufferOffset = 0;
	GameOfLife.playerCount = playerCount;
	clearFrameBuffer( frameBuffer );

	// set player data
	unsigned char i;
	for( i = 0; i != 4; i++ )
	{
		GameOfLife.player[i].cursor.x = 7;
		GameOfLife.player[i].cursor.y = 7;
		GameOfLife.player[i].oldCursor = GameOfLife.player[i].cursor;
		if( ((*playerCount) & (1<<(i-1))) || i == 0 )
		{
			GameOfLife.player[i].cellsPlaced = 2;
		}else{
			GameOfLife.player[i].cellsPlaced = 0;
		}
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
			*(GameOfLife.frameBuffer+7+(2*16)) = 0x10;
			*(GameOfLife.frameBuffer+7+(3*16)) = 0x10;
			*(GameOfLife.frameBuffer+8+(2*16)) = 0x10;
			*(GameOfLife.frameBuffer+8+(3*16)) = 0x10;
		}

		// player 4
		if( (*playerCount) & 0x04 )
		{
			*(GameOfLife.frameBuffer+7+(12*16)) = 0x40;
			*(GameOfLife.frameBuffer+7+(13*16)) = 0x40;
			*(GameOfLife.frameBuffer+8+(12*16)) = 0x40;
			*(GameOfLife.frameBuffer+8+(13*16)) = 0x40;
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
	gameOfLifeDrawFrameBuffer();
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

			// simulate continuously
			computeNextCycle();

			break;

		// multi player mode
		case GAMEOFLIFE_STATE_PLAY_MULTI :

			// everything happens in the input state

			break;

		// winner
		case GAMEOFLIFE_STATE_WINNER :

			break;

		default : break;
	}
}

// ----------------------------------------------------------------------
// process game of life input
void processGameOfLifeInput( void )
{

	// local variables
	unsigned short* bufferPtr;
	unsigned char readMask;
	unsigned char* cursorX;
	unsigned char* cursorY;
	unsigned char* oldCursorX;
	unsigned char* oldCursorY;
	unsigned short cursorColour;
	unsigned short cursorSelectColour;
	unsigned short cellColour;
	unsigned char i;

	// state dependant input
	switch( GameOfLife.state )
	{

		// edit mode
		case GAMEOFLIFE_STATE_EDIT_SINGLE :

			// for speed reasons, get pointers to player data
			cursorX = &GameOfLife.player[0].cursor.x;
			cursorY = &GameOfLife.player[0].cursor.y;
			oldCursorX = &GameOfLife.player[0].oldCursor.x;
			oldCursorY = &GameOfLife.player[0].oldCursor.y;

			// move cursor with up, down, left, right
			if( player1ButtonUp() ) (*cursorY) = (((*cursorY)-1)&0x0F);
			if( player1ButtonDown() ) (*cursorY) = (((*cursorY)+1)&0x0F);
			if( player1ButtonLeft() ) (*cursorX) = (((*cursorX)-1)&0x0F);
			if( player1ButtonRight() ) (*cursorX) = (((*cursorX)+1)&0x0F);

			// get buffer
			bufferPtr = (GameOfLife.frameBuffer + (*cursorY) + ((*cursorX)<<4));

			// get read mask
			readMask = (0x01<<GameOfLife.bufferOffset);

			// edit cells
			if( player1ButtonFire() )
			{

				// update frame buffer and draw new cursor
				if( (*bufferPtr) & readMask )
					(*bufferPtr) = 0x00;
				else
					(*bufferPtr) = readMask;
			}

			// remove old cursor
			if( (*(GameOfLife.frameBuffer + (*oldCursorY) + ((*oldCursorX)<<4))) & readMask )
				dot( oldCursorX, oldCursorY, &BLUE );
			else
				dot( oldCursorX, oldCursorY, &BLACK );
			GameOfLife.player[0].oldCursor = GameOfLife.player[0].cursor;

			// draw new cursor
			if( (*bufferPtr) & readMask )
				dot( cursorX, cursorY, &YELLOW );
			else
				dot( cursorX, cursorY, &WHITE );

			// clear frame buffer with clear button
			if( player1ButtonClear() )
			{
				clearFrameBuffer( GameOfLife.frameBuffer );
				gameOfLifeDrawFrameBufferNoCheck();
				dot( cursorX, cursorY, &WHITE );
				send();
			}

			// resume simulation
			if( player1ButtonMenu() )
			{
				gameOfLifeDrawFrameBufferNoCheck(); // force re-drawing of all pixels
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
				gameOfLifeDrawFrameBufferCustom( &BLUE, &RED, &GREEN, &YELLOW ); // force re-drawing of all pixels in a different colour
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
			for( i = 0; i != 4; i++ )
			{

				// only active players and players that are able to place
				if( (((*GameOfLife.playerCount) & (1<<(i-1))) || i == 0) && GameOfLife.player[i].cellsPlaced )
				{

					// for speed reasons, get pointers to player data
					cursorX = &GameOfLife.player[i].cursor.x;
					cursorY = &GameOfLife.player[i].cursor.y;
					oldCursorX = &GameOfLife.player[i].oldCursor.x;
					oldCursorY = &GameOfLife.player[i].oldCursor.y;

					// player specific actions
					switch( i )
					{
						case 0 :

							// move cursor with up, down, left, right
							if( playerButtonUp(i) ) (*cursorY) = (((*cursorY)-1)&0x0F);
							if( playerButtonDown(i) ) (*cursorY) = (((*cursorY)+1)&0x0F);
							if( playerButtonLeft(i) ) (*cursorX) = (((*cursorX)-1)&0x0F);
							if( playerButtonRight(i) ) (*cursorX) = (((*cursorX)+1)&0x0F);

							// set colours
							cursorColour = BLUEGREEN;
							cursorSelectColour = LIGHTGREEN;
							break;
						case 1 :

							// move cursor with up, down, left, right
							if( playerButtonUp(i) ) (*cursorY) = (((*cursorY)+1)&0x0F);
							if( playerButtonDown(i) ) (*cursorY) = (((*cursorY)-1)&0x0F);
							if( playerButtonLeft(i) ) (*cursorX) = (((*cursorX)+1)&0x0F);
							if( playerButtonRight(i) ) (*cursorX) = (((*cursorX)-1)&0x0F);

							// set colours
							cursorColour = PURPLE;
							cursorSelectColour = ORANGE;
							break;
						case 2 :

							// move cursor with up, down, left, right
							if( playerButtonUp(i) ) (*cursorX) = (((*cursorX)+1)&0x0F);
							if( playerButtonDown(i) ) (*cursorX) = (((*cursorX)-1)&0x0F);
							if( playerButtonLeft(i) ) (*cursorY) = (((*cursorY)-1)&0x0F);
							if( playerButtonRight(i) ) (*cursorY) = (((*cursorY)+1)&0x0F);

							// set colours
							cursorColour = PINK;
							cursorSelectColour = LIGHTBLUE;
							break;
						case 3 :

							// move cursor with up, down, left, right
							if( playerButtonUp(i) ) (*cursorX) = (((*cursorX)-1)&0x0F);
							if( playerButtonDown(i) ) (*cursorX) = (((*cursorX)+1)&0x0F);
							if( playerButtonLeft(i) ) (*cursorY) = (((*cursorY)+1)&0x0F);
							if( playerButtonRight(i) ) (*cursorY) = (((*cursorY)-1)&0x0F);

							// set colours
							cursorColour = LIGHTYELLOW;
							cursorSelectColour = WHITE;
							break;
						default: break;
					}

					// find cell colour under old cursor
					cellColour = BLACK;
					bufferPtr = GameOfLife.frameBuffer + (*oldCursorY) + ((*oldCursorX)<<4);
					if( (*bufferPtr) & (0x55<<GameOfLife.bufferOffset) )
					{
						if( (*bufferPtr) & (0x01<<GameOfLife.bufferOffset) ) cellColour = GREEN;
						if( (*bufferPtr) & (0x04<<GameOfLife.bufferOffset) ) cellColour = RED;
						if( (*bufferPtr) & (0x10<<GameOfLife.bufferOffset) ) cellColour = BLUE;
						if( (*bufferPtr) & (0x40<<GameOfLife.bufferOffset) ) cellColour = YELLOW;
					}
					
					// get buffer
					bufferPtr = (GameOfLife.frameBuffer + (*cursorY) + ((*cursorX)<<4));

					// get read mask
					readMask = ((1<<(i<<1))<<GameOfLife.bufferOffset);

					// edit cells
					if( playerButtonFire(i) )
					{

						// decrement cells placed
						GameOfLife.player[i].cellsPlaced--;

						// update frame buffer
						if( (*bufferPtr) & readMask )
							(*bufferPtr) &= ~readMask;
						else
							(*bufferPtr) |= readMask;
					}

					// remove old cursor
					dot( oldCursorX, oldCursorY, &cellColour );
					GameOfLife.player[i].oldCursor = GameOfLife.player[i].cursor;

					// draw new cursors
					if( GameOfLife.player[i].cellsPlaced )
					{
						if( (*bufferPtr) & readMask )
							dot( cursorX, cursorY, &cursorSelectColour );
						else
							dot( cursorX, cursorY, &cursorColour );

					// player has placed all cells for this round, remove cursor
					}else{
						cellColour = BLACK;
						if( (*bufferPtr) & (0x55<<GameOfLife.bufferOffset) )
						{
							if( (*bufferPtr) & (0x01<<GameOfLife.bufferOffset) ) cellColour = GREEN;
							if( (*bufferPtr) & (0x04<<GameOfLife.bufferOffset) ) cellColour = RED;
							if( (*bufferPtr) & (0x10<<GameOfLife.bufferOffset) ) cellColour = BLUE;
							if( (*bufferPtr) & (0x40<<GameOfLife.bufferOffset) ) cellColour = YELLOW;
						}
						dot( cursorX, cursorY, &cellColour );
					}

				}
			}

			// check if any players have any cells left
			unsigned char x;
			for( i = 0; i != 4; i++ )
			{
				if( GameOfLife.player[i].cellsPlaced ) break;

			// time to update
			} if( i == 4 )
			{

				// compute next frame
				computeNextCycle();

				// reset player cell counters
				for( i = 0; i != 4; i++ )
				{
					if( ((*GameOfLife.playerCount) & (1<<(i-1)) ) || i == 0 )
					{

						// check if player has any more cells left on the field
						x = 0;
						do{
							if( (*(GameOfLife.frameBuffer+x)) & ((1<<(i<<1))<<GameOfLife.bufferOffset) ) break;
							x++;
						}while( x != 0 );
						if( x != 0 ) GameOfLife.player[i].cellsPlaced = 2;
					}
				}

				// only one more player active
				x = 0;
				for( i = 0; i != 4; i++ )
				{
					if( GameOfLife.player[i].cellsPlaced ) x++;
				} if( x == 1 ) GameOfLife.state = GAMEOFLIFE_STATE_WINNER;
			}

			// update display
			send();

			// end game with menu button
			if( player1ButtonMenu() ) endGame();

			break;

		// winner state
		case GAMEOFLIFE_STATE_WINNER :

			// end game with menu button
			if( player1ButtonMenu() ) endGame();

			break;

		// winner state
		case GAMEOFLIFE_STATE_WINNER2 :

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
void gameOfLifeDrawFrameBufferNoCheck()
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
void gameOfLifeDrawFrameBufferCustom( const unsigned short* colour1, const unsigned short* colour2, const unsigned short* colour3, const unsigned short* colour4 )
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
void gameOfLifeDrawFrameBuffer( void )
{

	// render pixels
	unsigned char x, y;
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
	unsigned short* bufferPtr;

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
	gameOfLifeDrawFrameBuffer();
	send();

}
#endif // GAME_ENABLE_GAME_OF_LIFE
