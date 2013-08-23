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

	//2-4 Players allowed
	if(*playerCount==0)
	{
		endGame();
		return;
	}

	// get frame buffer
	Snake.frameBuffer = (unsigned char*) frameBuffer;

	// initialise buffer points
	Snake.player[0].cornerPoint.readPtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_0;
	Snake.player[0].cornerPoint.writePtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_0+1;
	Snake.player[1].cornerPoint.readPtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_1;
	Snake.player[1].cornerPoint.writePtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_1+1;
	Snake.player[2].cornerPoint.readPtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_2;
	Snake.player[2].cornerPoint.writePtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_2+1;
	Snake.player[3].cornerPoint.readPtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3;
	Snake.player[3].cornerPoint.writePtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3+1;

	// set starting directions of players
	Snake.player[0].direction = SNAKE_PLAYER_DIRECTION_UP;
	Snake.player[1].direction = SNAKE_PLAYER_DIRECTION_DOWN;
	Snake.player[2].direction = SNAKE_PLAYER_DIRECTION_RIGHT;
	Snake.player[3].direction = SNAKE_PLAYER_DIRECTION_LEFT;

	// set starting positions of players
	(*Snake.player[0].cornerPoint.writePtr) = 0x7C;
	(*Snake.player[1].cornerPoint.writePtr) = 0x83;
	(*Snake.player[2].cornerPoint.writePtr) = 0x37;
	(*Snake.player[3].cornerPoint.writePtr) = 0xC8;
	(*Snake.player[0].cornerPoint.readPtr)  = 0x7E;
	(*Snake.player[1].cornerPoint.readPtr)  = 0x81;
	(*Snake.player[2].cornerPoint.readPtr)  = 0x17;
	(*Snake.player[3].cornerPoint.readPtr)  = 0xE8;

	// colour of players
	Snake.player[0].colour = RED;
	Snake.player[1].colour = GREEN;
	Snake.player[2].colour = BLUE;
	Snake.player[3].colour = YELLOW;

	// get player count
	Snake.playerCount = playerCount;

	// spawn food
	unsigned char x1, y1, x2, y2;
	Snake.foodPosition = rnd();
	x1 = Snake.foodPosition; y1 = x1&0x0F; x1 >>= 4;
	if( x1 == 0 ) x1++;
	if( x1 == 15 ) x1--;
	if( y1 == 0 ) y1++;
	if( y1 == 15 ) y1--;
	Snake.foodPosition = (x1<<4)|y1;

	// set up playing field
	setRefreshRate(40);
	cls();
	x1=0; y1=15;
	box( &x1, &x1, &y1, &y1, &ORANGE );
	x1 = Snake.foodPosition; y1 = x1&0x0F; x1 >>= 4;
	dot( &x1, &y1, &WHITE );

	// set up common properties of players
	for( unsigned char i = 0; i != 4; i++ )
	{
		if( ((*playerCount) & ((1<<i)>>1)) || i == 0 ) Snake.player[i].alive = 1; else Snake.player[i].alive = 0;
		Snake.player[i].oldDirection = Snake.player[i].direction;
		x1 = *Snake.player[i].cornerPoint.readPtr; y1 = x1&0x0F; x1 >>= 4;
		x2 = *Snake.player[i].cornerPoint.writePtr; y2 = x2&0x0F; x2 >>= 4;
		if( ((*Snake.playerCount) & ((1<<i)>>1)) || i == 0 )
			line( &x1, &y1, &x2, &y2, &Snake.player[i].colour );
	}

	// update screen
	send();
}

// ----------------------------------------------------------------------
// process snake loop
void processSnakeLoop( void )
{

	// local variables
	unsigned char x, y, x2, y2;
	unsigned char* ptr;

	// process players
	for( unsigned char i = 0; i != 4; i++ )
	{
		if( (((*Snake.playerCount) & ((1<<i)>>1)) || i == 0) && Snake.player[i].alive )
		{

			// snake head position is stored in write pointer
			x = (*Snake.player[i].cornerPoint.writePtr);
			y = x&0x0F;
			x >>= 4;

			// update positions of head
			switch(	Snake.player[i].direction )
			{
				case SNAKE_PLAYER_DIRECTION_UP    : y--; break;
				case SNAKE_PLAYER_DIRECTION_DOWN  : y++; break;
				case SNAKE_PLAYER_DIRECTION_LEFT  : x--; break;
				case SNAKE_PLAYER_DIRECTION_RIGHT : x++; break;
				default: break;
			}

			// collision with boundaries of level
                        if( x > 0x0E || x < 0x01 || y > 0x0E || y < 0x01 )
                        {
				Snake.player[i].alive = 0;
				continue;
			}

			// collision with other players
			for( unsigned char n = 0; n != 4; n++ )
			{
				unsigned char x1, y1;
				if( ((*Snake.playerCount) & ((1<<n)>>1)) || n == 0 )
				{

					// begin iterating over all corner points
					ptr = Snake.player[n].cornerPoint.readPtr;
					do
					{

						// get first corner point
						x1 = (*ptr); y1 = x1&0x0F; x1 >>= 4;

						// get next corner
						ptr++;
						if( ptr == Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_1 )
							ptr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_0;
						if( ptr == Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_2 )
							ptr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_1;
						if( ptr == Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3 )
							ptr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_2;
						if( ptr == Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3 + 128 )
							ptr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3;
						x2 = (*ptr); y2 = x2&0x0F; x2 >>= 4;

						// check if player collides
						if( x1 == x2 && x == x1 ){
							if( y1 < y2 ){
								if( y >= y1 && y <= y2 ){
									Snake.player[i].alive = 0;
									continue;
								}
							}else{
								if( y >= y2 && y <= y1 ){
									Snake.player[i].alive = 0;
									continue;
								}
							}
						}
						if( y1 == y2 && y == y1 ){
							if( x1 < x2 ){
								if( x >= x1 && x <= x2 ){
									Snake.player[i].alive = 0;
									continue;
								}
							}else{
								if( x >= x2 && x <= x1 ){
									Snake.player[i].alive = 0;
									continue;
								}
							}
						}

					}while( ptr != Snake.player[n].cornerPoint.writePtr );
				}
			}

			// if player's direction has changed, a new corner point must be inserted
			if( Snake.player[i].oldDirection != Snake.player[i].direction )
			{
				Snake.player[i].cornerPoint.writePtr++;

				// wrap buffers
				if( Snake.player[0].cornerPoint.writePtr == Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_1 )
					Snake.player[0].cornerPoint.writePtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_0;
				if( Snake.player[1].cornerPoint.writePtr == Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_2 )
					Snake.player[1].cornerPoint.writePtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_1;
				if( Snake.player[2].cornerPoint.writePtr == Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3 )
					Snake.player[2].cornerPoint.writePtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_2;
				if( Snake.player[3].cornerPoint.writePtr == Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3 + 128 )
					Snake.player[3].cornerPoint.writePtr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3;
			}

			// write new positions into frame buffer
			(*Snake.player[i].cornerPoint.writePtr) = (x<<4)|y;

			// write to screen
			dot( &x, &y, &Snake.player[i].colour );

			// update old position
			Snake.player[i].oldDirection = Snake.player[i].direction;

			// check if colliding with food
			x2 = Snake.foodPosition; y2 = x2&0x0F; x2 >>= 4;
			if( x == x2 && y == y2 )
			{
				Snake.foodPosition = rnd();
				x2 = Snake.foodPosition; y2 = x2&0x0F; x2 >>= 4;
				if( x2 == 0 ) x2++;
				if( x2 == 15 ) x2--;
				if( y2 == 0 ) y2++;
				if( y2 == 15 ) y2--;
				Snake.foodPosition = (x2<<4)|y2;
				continue;
			}

			// snake tail position is stored in read pointer
			x = (*Snake.player[i].cornerPoint.readPtr);
			y = x&0x0F;
			x >>= 4;

			// get second last corner point
			ptr = Snake.player[i].cornerPoint.readPtr+1;
			if( ptr == Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_1 )
				ptr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_0;
			if( ptr == Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_2 )
				ptr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_1;
			if( ptr == Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3 )
				ptr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_2;
			if( ptr == Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3 + 128 )
				ptr = Snake.frameBuffer + SNAKE_CORNER_DATA_OFFSET_3;
			x2 = (*ptr);
			y2 = x2&0x0F;
			x2 >>= 4;

			// erase tail
			dot( &x, &y, &BLACK );

			// shift tail towards second last corner point
			if( x2 == x )
			{
				if( y2 > y ) y++; else y--;
			}else{
				if( x2 > x ) x++; else x--;
			}

			// tail is on top of second last corner point -> erase last corner point
			if( x2 == x && y2 == y ) Snake.player[i].cornerPoint.readPtr = ptr;

			// write new positions into frame buffer
			(*Snake.player[i].cornerPoint.readPtr) = (x<<4)|y;
		}
	}

	// draw food
	x = Snake.foodPosition; y = x&0x0F; x >>= 4;
	dot( &x, &y, &WHITE );

	// update screen
	send();

}

// ----------------------------------------------------------------------
// process snake input
void processSnakeInput( void )
{

	// process input for all players
	for( unsigned char x = 0; x != 4; x++ )
	{
		if( globalPlayerButtonUp( x ) ) Snake.player[x].direction = SNAKE_PLAYER_DIRECTION_UP;
		if( globalPlayerButtonDown( x ) ) Snake.player[x].direction = SNAKE_PLAYER_DIRECTION_DOWN;
		if( globalPlayerButtonLeft( x ) ) Snake.player[x].direction = SNAKE_PLAYER_DIRECTION_LEFT;
		if( globalPlayerButtonRight( x ) ) Snake.player[x].direction = SNAKE_PLAYER_DIRECTION_RIGHT;
	}

	// exit game
	if( globalPlayer1ButtonMenu() ) endGame();
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

