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
			frameWorkUpdateProcessLoop();
			FrameWork.updateFlag = 0;
		}

	}
}

// ----------------------------------------------------------------------
// poll all ports
void pollPorts( void )
{

	// read in new button states
	FrameWork.player[0].buttonState = MAP_PLAYER1_BUTTON;
	FrameWork.player[1].buttonState = MAP_PLAYER2_BUTTON;
	FrameWork.player[2].buttonState = MAP_PLAYER3_BUTTON;
	FrameWork.player[3].buttonState = MAP_PLAYER4_BUTTON;

	// process positive edges and save old states
	unsigned char updateFlag = 0;
	for( unsigned char i = 0; i != 4; i++ )
	{
		FrameWork.player[i].buttonPositiveEdge = ((~FrameWork.player[i].oldButtonState) & FrameWork.player[i].buttonState);
		FrameWork.player[i].oldButtonState = FrameWork.player[i].buttonState;
		if( FrameWork.player[i].buttonPositiveEdge ) updateFlag = 1;

		// change
		if( FrameWork.player[i].buttonState&0x1F )
			FrameWork.randomSeed++;
	}

	// update any inputs
	if( updateFlag ) frameWorkUpdateInputLoop();

}

// ----------------------------------------------------------------------
// sets the refresh rate
void setRefreshRate( unsigned char refresh )
{
	FrameWork.updateDivider = 255/refresh;
}

// ----------------------------------------------------------------------
// clears a frame buffer
void clearFrameBuffer( unsigned char* frameBuffer )
{
	unsigned char x = 0;
	do{
		frameBuffer[x] = 0;
		x++;
	}while( x != 0 );
}

// ----------------------------------------------------------------------
// gets a random number
unsigned char rnd( void )
{
	FrameWork.randomSeed++;
	unsigned short x = FrameWork.randomSeed;
	x = (x<<7) ^ x;
	x = (unsigned short)(( 34071 - ( ( x * ( x * x * 15731 + 7881 ) + 13763 ) & 0x7FFF ))/9);
	unsigned char y = x>>8;
	unsigned char z = x;
	return ((y^z)*x)^FrameWork.randomSeed;
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
// call update loop of current game running - passes the process on to
// the current "main loop" using a state machine for different modes
// this allows expandability for more games/demos in the future
void frameWorkUpdateProcessLoop( void )
{
	switch( FrameWork.state )
	{

		// main menu
		case FRAMEWORK_STATE_LOAD_MENU :
			loadMenu( &FrameWork.frameBuffer[0] );
			FrameWork.state = FRAMEWORK_STATE_MENU;
			break;
		case FRAMEWORK_STATE_MENU :
			processMenuLoop();
			break;

		// snake
		case FRAMEWORK_STATE_LOAD_SNAKE :
			loadSnake( &FrameWork.frameBuffer[0] );
			FrameWork.state = FRAMEWORK_STATE_SNAKE;
			break;
		case FRAMEWORK_STATE_SNAKE :
			processSnakeLoop();
			break;

		// colour demo
		case FRAMEWORK_STATE_LOAD_COLOUR_DEMO :
			loadColourDemo( &FrameWork.frameBuffer[0] );
			FrameWork.state = FRAMEWORK_STATE_COLOUR_DEMO;
			break;
		case FRAMEWORK_STATE_COLOUR_DEMO :
			processColourDemoLoop();
			break;

		// game of life
		case FRAMEWORK_STATE_LOAD_GAME_OF_LIFE :
			loadGameOfLife( &FrameWork.frameBuffer[0] );
			FrameWork.state = FRAMEWORK_STATE_GAME_OF_LIFE;
			break;
		case FRAMEWORK_STATE_GAME_OF_LIFE :
			processGameOfLifeLoop();
			break;

		// error, reset to main menu
		default: FrameWork.state = FRAMEWORK_STATE_LOAD_MENU; break;
	}
}

// ----------------------------------------------------------------------
// call input loop of current running game
void frameWorkUpdateInputLoop( void )
{

	// call input loop of current game
	switch( FrameWork.state )
	{
		case FRAMEWORK_STATE_MENU                  : processMenuInput();               break;
		case FRAMEWORK_STATE_COLOUR_DEMO           : processColourDemoInput();         break;
		case FRAMEWORK_STATE_SNAKE                 : processSnakeInput();              break;
		case FRAMEWORK_STATE_GAME_OF_LIFE          : processGameOfLifeInput();         break;
		default: break;
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