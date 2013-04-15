// ----------------------------------------------------------------------
// Snake
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "snake.h"
#include "framework.h"
#include "uart.h"
#include "gameenable.h"

#ifdef GAME_ENABLE_SNAKE

static struct Snake_t Snake;

// ----------------------------------------------------------------------
// load snake
void loadSnake( unsigned short* frameBuffer, unsigned char* playerCount )
{

	// get frame buffer
	Snake.frameBuffer = (unsigned char*) frameBuffer;

	// initialise buffer points
	Snake.player[0].cornerPoint.readPtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_0;
	Snake.player[0].cornerPoint.writePtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_0;
	Snake.player[1].cornerPoint.readPtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_1;
	Snake.player[1].cornerPoint.writePtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_1;
	Snake.player[2].cornerPoint.readPtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_2;
	Snake.player[2].cornerPoint.writePtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_2;
	Snake.player[3].cornerPoint.readPtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3;
	Snake.player[3].cornerPoint.writePtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3;

	// set up players (common)
	for( unsigned char i = 0; i != 4; i++ )
	{
		if( ((*playerCount) & ((1<<i)>>1)) || i == 0 ) Snake.player[i].alive = 1; else Snake.player[i].alive = 0;
	}

	// set up players (specific)
	Snake.player[0].direction = SNAKE_PLAYER_DIRECTION_UP;
	Snake.player[1].direction = SNAKE_PLAYER_DIRECTION_DOWN;
	Snake.player[2].direction = SNAKE_PLAYER_DIRECTION_RIGHT;
	Snake.player[3].direction = SNAKE_PLAYER_DIRECTION_LEFT;

	// get player count
	Snake.playerCount = playerCount;
}

// ----------------------------------------------------------------------
// process snake loop
void processSnakeLoop( void )
{

	// local variables
	unsigned char x, y;

	// process players
	for( unsigned char i = 0; i != 1; i++ )
	{
		if( ((*Snake.playerCount) & ((1<<i)>>1)) || i == 0 )
		{

			// last position is stored in write pointer
			x = (*Snake.player[i].cornerPoint.writePtr);
			y = x&0x0F;
			x >>= 4;

			// update positions
			switch(	Snake.player[i].direction )
			{
				case SNAKE_PLAYER_DIRECTION_UP    : y--; break;
				case SNAKE_PLAYER_DIRECTION_DOWN  : y++; break;
				case SNAKE_PLAYER_DIRECTION_LEFT  : x--; break;
				case SNAKE_PLAYER_DIRECTION_RIGHT : x++; break;
				default: break;
			}

			// wrap positions
			x &= 0x0F;
			y &= 0x0F;

			// write new positions into frame buffer
			(*Snake.player[i].cornerPoint.writePtr) = (x<<4)|y;

			// write to screen
			dot( &x, &y, &RED );

		}
	}

	// update screen
	send();
		
}

// ----------------------------------------------------------------------
// process snake input
void processSnakeInput( void )
{

	// process for player 1
	if( player1ButtonUp() ) Snake.player[0].direction = SNAKE_PLAYER_DIRECTION_UP;
	if( player1ButtonDown() ) Snake.player[0].direction = SNAKE_PLAYER_DIRECTION_DOWN;
	if( player1ButtonLeft() ) Snake.player[0].direction = SNAKE_PLAYER_DIRECTION_LEFT;
	if( player1ButtonRight() ) Snake.player[0].direction = SNAKE_PLAYER_DIRECTION_RIGHT;
}
#endif // GAME_ENABLE_SNAKE
