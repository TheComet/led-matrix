// ----------------------------------------------------------------------
// Snake
// ----------------------------------------------------------------------

#ifndef _SNAKE_H_
	#define _SNAKE_H_

// ----------------------------------------------------------------------
// Definitions
// ----------------------------------------------------------------------

// Offsets to the snake corner points for each player
// The data is stored in the frame buffer
#define SNAKE_CORNER_DATA_OFFSET_0 0
#define SNAKE_CORNER_DATA_OFFSET_1 128
#define SNAKE_CORNER_DATA_OFFSET_2 256
#define SNAKE_CORNER_DATA_OFFSET_3 384

// ----------------------------------------------------------------------
// Enumerators
// ----------------------------------------------------------------------

enum Snake_Player_Direction_e
{
	SNAKE_PLAYER_DIRECTION_UP,
	SNAKE_PLAYER_DIRECTION_DOWN,
	SNAKE_PLAYER_DIRECTION_LEFT,
	SNAKE_PLAYER_DIRECTION_RIGHT
};

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct Snake_Corner_Points_t
{
	unsigned char* readPtr;
	unsigned char* writePtr;
};

struct Snake_Player_t
{
	struct Snake_Corner_Points_t cornerPoint;
	unsigned char direction;
	unsigned char oldDirection;
	unsigned char alive;
	unsigned short colour;
};

struct Snake_t
{
	unsigned char* frameBuffer; // explicitly cast to (unsigned char*), so we have 512 bytes
	unsigned char* playerCount;
	struct Snake_Player_t player[4];
	unsigned char foodPosition;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadSnake( unsigned short* frameBuffer, unsigned char* playerCount );
void processSnakeLoop( void );
void processSnakeInput( void );
void drawSnakeMenuIcon( void );

#endif // _SNAKE_H_
