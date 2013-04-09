// ----------------------------------------------------------------------
// Snake
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "snake.h"
#include "framework.h"
#include "uart.h"

// ----------------------------------------------------------------------
// load snake
void loadSnake( unsigned char* frameBuffer )
{
  
 unsigned char Pos_X;
 unsigned char Pos_Y;
 unsigned char Pos_X_Hinten;
 unsigned char Pos_Y_Hinten;
     

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
  	FrameWork.state = FRAMEWORK_STATE_LOAD_MENU;
        
        if (player1BottomLeft())
          Pos_X = Pos_X-1;
        
        if (player1BottomRight())
          Pos_X = Pos_X+1;
        
        if (player1BottomUp())
          Pos_X = Pos_Y+1;        

        if (player1BottomDown())
          Pos_X = Pos_Y-1; 

   
        
        dot(Pos_X, Pos_Y, 0x00E)
        dot(Pos_X_Hinten, Pos_Y_Hinten, 0)
}

// ----------------------------------------------------------------------
// process snake input
void processSnakeInput( void )
{ 
}