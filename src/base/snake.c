// ----------------------------------------------------------------------
// Snake
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "snake.h"
#include "framework.h"
#include "uart.h"
#include "moduleenable.h"

#ifdef MODULE_ENABLE_SNAKE

static struct Snake_t Snake;

static const unsigned short snakeIcon[100] = {0x0,0x0,0xA71,0x972,0x852,0x751,0x0,0x0,0x0,0x0,0x0,0xB92,0xA91,0x972,0x862,0x851,0x730,0x0,0x0,0x0,0x0,0xBA2,0xA82,0x420,0x0,0x851,0x750,0x0,0x0,0x0,0x0,0x0,0xD00,0x0,0x0,0x852,0x851,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x972,0x851,0x0,0x0,0x0,0x0,0x0,0x0,0x420,0xA81,0x972,0x0,0x0,0x0,0x0,0x0,0x0,0xCA3,0xB92,0x420,0x0,0x0,0x100,0x740,0x0,0x0,0xDC4,0xCB3,0xBA2,0x0,0x0,0x0,0x0,0x0,0x730,0x0,0xDC4,0xCC3,0xBB3,0x0,0x0,0x0,0x420,0x851,0x730,0x0,0x0,0xDC4,0xCA3,0xBA2,0xA92,0x971,0x972,0x420,0x0};

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
	for( unsigned char i = 0; i != 4; i++ )
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

	// exit game
	if( player1ButtonMenu() ) endGame();
}

// ----------------------------------------------------------------------
// draws the menu icon for snake
void drawSnakeMenuIcon( void )
{
	unsigned char x, y, index = 0;
	for( y = 3; y != 13; y++ )
	{
		for( x = 3; x != 13; x++ )
		{
			dot( &x, &y, &snakeIcon[index] );
			index++;
		}
	}
}
#endif // GAME_ENABLE_SNAKE

