// ----------------------------------------------------------------------
// Snake
// ----------------------------------------------------------------------

#ifndef _SNAKE_H_
	#define _SNAKE_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct Snake_t
{
 unsigned char Pos_X;
 unsigned char Pos_Y;
 unsigned char Pos_X_Hinten;
 unsigned char Pos_Y_Hinten;
 unsigned char Direction;
};

enum Snake_Direction_e
{
  SNAKE_DIRECTION_UP,
  SNAKE_DIRECTION_DOWN,
  SNAKE_DIRECTION_LEFT,
  SNAKE_DIRECTION_RIGHT
};

// ----------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------

void loadSnake( unsigned char* frameBuffer, unsigned char* playerCount );
void processSnakeLoop( void );
void processSnakeInput( void );

#endif // _SNAKE_H_
