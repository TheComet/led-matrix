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
	setRefreshRate( 24 );
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
   
        unsigned short colour = 0x00E, clearColour = 0;
        dot( &Snake.Pos_X, &Snake.Pos_Y, &colour);
        dot( &Snake.Pos_X_Hinten, &Snake.Pos_Y_Hinten, &clearColour);
        send();       
}

// ----------------------------------------------------------------------
// process snake input
void processSnakeInput( void )
{
 
         if (player1ButtonLeft())
          Snake.Pos_X = Snake.Pos_X-1;
        
        if (player1ButtonRight())
          Snake.Pos_X = Snake.Pos_X+1;
        
        if (player1ButtonUp())
          Snake.Pos_X = Snake.Pos_Y+1;        

        if (player1ButtonDown())
          Snake.Pos_X = Snake.Pos_Y-1;  
}