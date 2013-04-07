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

struct FrameWork_t FrameWork;

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
		case FRAMEWORK_STATE_LOAD_MENU          : loadMenu();             FrameWork.state = FRAMEWORK_STATE_MENU;             break;
		case FRAMEWORK_STATE_MENU               : processMenu();                                                              break;
		case FRAMEWORK_STATE_LOAD_SNAKE         : loadSnake();            FrameWork.state = FRAMEWORK_STATE_SNAKE;            break;
		case FRAMEWORK_STATE_SNAKE              : processSnake();                                                             break;

		// error, reset to main menu
		default: FrameWork.state = FRAMEWORK_STATE_LOAD_MENU; break;
	}
}