// ----------------------------------------------------------------------
// Framework
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include Files
// ----------------------------------------------------------------------

#include "common.h"
#include "framework.h"
#include "uart.h"
#include "menu.h"
#include "snake.h"
#include "colourdemo.h"
#include "gameoflife.h"

struct FrameWork_t FrameWork;

// ----------------------------------------------------------------------
// initialise framework
void initFrameWork( void )
{
	FrameWork.updateCounter = 0;
	FrameWork.updateDivider = 1;
	FrameWork.updateFlag = 0;
}

// ----------------------------------------------------------------------
// main loop for entire micro controller
void startFrameWork( void )
{

	// main loop
	while( 1 )
	{

		// read input
		pollPorts();

		// update
		if( FrameWork.updateFlag ){
			frameWorkUpdate();
			clearPorts();
			FrameWork.updateFlag = 0;
		}

	}
}

// ----------------------------------------------------------------------
// poll all ports
void pollPorts( void )
{

	// store old button states
	FrameWork.player[0].oldButtonState = FrameWork.player[0].buttonState;
	FrameWork.player[1].oldButtonState = FrameWork.player[1].buttonState;
	FrameWork.player[2].oldButtonState = FrameWork.player[2].buttonState;
	FrameWork.player[3].oldButtonState = FrameWork.player[3].buttonState;

	// read in new button states
	FrameWork.player[0].buttonState = MAP_PLAYER1_BUTTON;
	FrameWork.player[1].buttonState = MAP_PLAYER2_BUTTON;
	FrameWork.player[2].buttonState = MAP_PLAYER3_BUTTON;
	FrameWork.player[3].buttonState = MAP_PLAYER4_BUTTON;

	// process positive edges
	FrameWork.player[0].buttonPositiveEdge |= ((~FrameWork.player[0].oldButtonState) & FrameWork.player[0].buttonState);
	FrameWork.player[1].buttonPositiveEdge |= ((~FrameWork.player[1].oldButtonState) & FrameWork.player[1].buttonState);
	FrameWork.player[2].buttonPositiveEdge |= ((~FrameWork.player[2].oldButtonState) & FrameWork.player[2].buttonState);
	FrameWork.player[3].buttonPositiveEdge |= ((~FrameWork.player[3].oldButtonState) & FrameWork.player[3].buttonState);
}

// ----------------------------------------------------------------------
// clears the ports again, because they were processed
void clearPorts( void )
{
	FrameWork.player[0].buttonPositiveEdge = 0;
	FrameWork.player[1].buttonPositiveEdge = 0;
	FrameWork.player[2].buttonPositiveEdge = 0;
	FrameWork.player[3].buttonPositiveEdge = 0;
}

// ----------------------------------------------------------------------
// sets the refresh rate
void setRefreshRate( unsigned char refresh )
{
	FrameWork.updateDivider = 255/refresh;
}

// ----------------------------------------------------------------------
// starts the colour demo
void startColourDemo( void )
{
	FrameWork.state = FRAMEWORK_STATE_LOAD_COLOUR_DEMO;
}

// ----------------------------------------------------------------------
// starts snake
void startSnake( void )
{
	FrameWork.state = FRAMEWORK_STATE_LOAD_SNAKE;
}

// ----------------------------------------------------------------------
// starts the game of life
void startGameOfLife( void )
{
	FrameWork.state = FRAMEWORK_STATE_LOAD_GAME_OF_LIFE;
}

// ----------------------------------------------------------------------
// end the game
void endGame( void )
{
	FrameWork.state = FRAMEWORK_STATE_LOAD_MENU;
}

// ----------------------------------------------------------------------
// gets the button state of a specific player (positive edge only)
// unfortunately, I see no way to compress this, because it is dependant on global definitions
extern inline unsigned char player1ButtonFire ( void ){ return FrameWork.player[0].buttonPositiveEdge & MAP_PLAYER1_BUTTON_FIRE;  }
extern inline unsigned char player1ButtonLeft ( void ){ return FrameWork.player[0].buttonPositiveEdge & MAP_PLAYER1_BUTTON_LEFT;  }
extern inline unsigned char player1ButtonRight( void ){ return FrameWork.player[0].buttonPositiveEdge & MAP_PLAYER1_BUTTON_RIGHT; }
extern inline unsigned char player1ButtonUp   ( void ){ return FrameWork.player[0].buttonPositiveEdge & MAP_PLAYER1_BUTTON_UP;    }
extern inline unsigned char player1ButtonDown ( void ){ return FrameWork.player[0].buttonPositiveEdge & MAP_PLAYER1_BUTTON_DOWN;  }

extern inline unsigned char player2ButtonFire ( void ){ return FrameWork.player[1].buttonPositiveEdge & MAP_PLAYER2_BUTTON_FIRE;  }
extern inline unsigned char player2ButtonLeft ( void ){ return FrameWork.player[1].buttonPositiveEdge & MAP_PLAYER2_BUTTON_LEFT;  }
extern inline unsigned char player2ButtonRight( void ){ return FrameWork.player[1].buttonPositiveEdge & MAP_PLAYER2_BUTTON_RIGHT; }
extern inline unsigned char player2ButtonUp   ( void ){ return FrameWork.player[1].buttonPositiveEdge & MAP_PLAYER2_BUTTON_UP;    }
extern inline unsigned char player2ButtonDown ( void ){ return FrameWork.player[1].buttonPositiveEdge & MAP_PLAYER2_BUTTON_DOWN;  }

extern inline unsigned char player3ButtonFire ( void ){ return FrameWork.player[2].buttonPositiveEdge & MAP_PLAYER3_BUTTON_FIRE;  }
extern inline unsigned char player3ButtonLeft ( void ){ return FrameWork.player[2].buttonPositiveEdge & MAP_PLAYER3_BUTTON_LEFT;  }
extern inline unsigned char player3ButtonRight( void ){ return FrameWork.player[2].buttonPositiveEdge & MAP_PLAYER3_BUTTON_RIGHT; }
extern inline unsigned char player3ButtonUp   ( void ){ return FrameWork.player[2].buttonPositiveEdge & MAP_PLAYER3_BUTTON_UP;    }
extern inline unsigned char player3ButtonDown ( void ){ return FrameWork.player[2].buttonPositiveEdge & MAP_PLAYER3_BUTTON_DOWN;  }

extern inline unsigned char player4ButtonFire ( void ){ return FrameWork.player[3].buttonPositiveEdge & MAP_PLAYER4_BUTTON_FIRE;  }
extern inline unsigned char player4ButtonLeft ( void ){ return FrameWork.player[3].buttonPositiveEdge & MAP_PLAYER4_BUTTON_LEFT;  }
extern inline unsigned char player4ButtonRight( void ){ return FrameWork.player[3].buttonPositiveEdge & MAP_PLAYER4_BUTTON_RIGHT; }
extern inline unsigned char player4ButtonUp   ( void ){ return FrameWork.player[3].buttonPositiveEdge & MAP_PLAYER4_BUTTON_UP;    }
extern inline unsigned char player4ButtonDown ( void ){ return FrameWork.player[3].buttonPositiveEdge & MAP_PLAYER4_BUTTON_DOWN;  }

// ----------------------------------------------------------------------
// update loop - everything happens here
// using a state machine for different modes
// this allows expandability for more games/demos in the future
void frameWorkUpdate( void )
{
	switch( FrameWork.state )
	{

		// main menu
		case FRAMEWORK_STATE_LOAD_MENU :
			loadMenu( &FrameWork.frameBuffer[0] );
			FrameWork.state = FRAMEWORK_STATE_MENU;
			break;
		case FRAMEWORK_STATE_MENU :
			processMenu();
			break;

		// snake
		case FRAMEWORK_STATE_LOAD_SNAKE :
			loadSnake( &FrameWork.frameBuffer[0] );
			FrameWork.state = FRAMEWORK_STATE_SNAKE;
			break;
		case FRAMEWORK_STATE_SNAKE :
			processSnake();
			break;

		// colour demo
		case FRAMEWORK_STATE_LOAD_COLOUR_DEMO :
			loadColourDemo( &FrameWork.frameBuffer[0] );
			FrameWork.state = FRAMEWORK_STATE_COLOUR_DEMO;
			break;
		case FRAMEWORK_STATE_COLOUR_DEMO :
			processColourDemo();
			break;

		// game of life
		case FRAMEWORK_STATE_LOAD_GAME_OF_LIFE :
			loadGameOfLife( &FrameWork.frameBuffer[0] );
			FrameWork.state = FRAMEWORK_STATE_GAME_OF_LIFE;
			break;
		case FRAMEWORK_STATE_GAME_OF_LIFE :
			processGameOfLife();
			break;

		// error, reset to main menu
		default: FrameWork.state = FRAMEWORK_STATE_LOAD_MENU; break;
	}
}

// ----------------------------------------------------------------------
// Update interrupt
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A( void )
{
	// divide update rate
	if( (FrameWork.updateCounter++) != FrameWork.updateDivider ) return;
	FrameWork.updateCounter = 0;

	// set update flag (this is caught in the main loop)
	FrameWork.updateFlag = 1;
}