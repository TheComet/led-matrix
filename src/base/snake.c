// ----------------------------------------------------------------------
// Snake
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "snake.h"
#include "framework.h"
#include "uart.h"

struct Snake_t Snake;

// ----------------------------------------------------------------------
// load snake
void loadSnake( unsigned char* frameBuffer )
{
  
  // player 1  Start Position
  Snake.Pos_X = 8;
  Snake.Pos_Y = 15;
  Snake.Direction = SNAKE_DIRECTION_UP;  
  
     
	// set up screen
	cls();
	send();

	// set refresh rate
	setRefreshRate( 42 );
}

// ----------------------------------------------------------------------
// process snake loop
void processSnakeLoop( void )
{
        

  switch(Snake.Direction){
    case      SNAKE_DIRECTION_UP: 
      Snake.Pos_Y --;
    break;
    case      SNAKE_DIRECTION_DOWN: 
      Snake.Pos_Y ++;
    break;
    case      SNAKE_DIRECTION_LEFT: 
      Snake.Pos_X --;
    break;
    case      SNAKE_DIRECTION_RIGHT: 
      Snake.Pos_X ++;
    break;
    
  }
  
    Snake.Pos_X &= 0x0F;
    Snake.Pos_Y &= 0x0F;
   
        unsigned short colour = 0x00E, clearColour = 0;
        dot( &Snake.Pos_X, &Snake.Pos_Y, &colour);
        send();       
}

// ----------------------------------------------------------------------
// process snake input
void processSnakeInput( void )
{
 
         if (player1ButtonLeft())
          Snake.Direction = SNAKE_DIRECTION_LEFT;
        
        if (player1ButtonRight())
          Snake.Direction = SNAKE_DIRECTION_RIGHT;
        
        if (player1ButtonUp())
          Snake.Direction  = SNAKE_DIRECTION_UP;        

        if (player1ButtonDown())
          Snake.Direction  = SNAKE_DIRECTION_DOWN;  
}