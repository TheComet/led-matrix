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
#include "startupscreen.h"

// added games
#ifdef GAME_ENABLE_SNAKE
	#include "snake.h"
#endif
#ifdef GAME_ENABLE_COLOUR_DEMO
	#include "colourdemo.h"
#endif
#ifdef GAME_ENABLE_GAME_OF_LIFE
	#include "gameoflife.h"
#endif
#ifdef GAME_ENABLE_TRON
	#include "tron.h"
#endif
#ifdef GAME_ENABLE_SPACE_INVADERS
	#include "spaceinvaders.h"
#endif
#ifdef GAME_ENABLE_TETRIS
	#include "tetris.h"
#endif
#ifdef GAME_ENABLE_PONG
	#include "pong.h"
#endif
#ifdef GAME_ENABLE_BURGLER
	#include "burgler.h"
#endif

static struct FrameWork_t FrameWork;

// ----------------------------------------------------------------------
// initialise framework
void initFrameWork( void )
{
	FrameWork.updateCounter = 0;
	FrameWork.updateDivider = 1;
	FrameWork.updateFlag = 0;
	FrameWork.state = FRAMEWORK_STATE_START_UP_SCREEN;
	loadStartUpScreen();
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

	// add menu button to 6th bit
	if( (FrameWork.player[0].buttonState & (MAP_PLAYER1_BUTTON_UP | MAP_PLAYER1_BUTTON_DOWN)) == (MAP_PLAYER1_BUTTON_UP | MAP_PLAYER1_BUTTON_DOWN) )
		FrameWork.player[0].buttonState |= MAP_PLAYER_BUTTON_MENU;
	if( (FrameWork.player[1].buttonState & (MAP_PLAYER2_BUTTON_UP | MAP_PLAYER2_BUTTON_DOWN)) == (MAP_PLAYER2_BUTTON_UP | MAP_PLAYER2_BUTTON_DOWN) )
		FrameWork.player[1].buttonState |= MAP_PLAYER_BUTTON_MENU;
	if( (FrameWork.player[2].buttonState & (MAP_PLAYER3_BUTTON_UP | MAP_PLAYER3_BUTTON_DOWN)) == (MAP_PLAYER3_BUTTON_UP | MAP_PLAYER3_BUTTON_DOWN) )
		FrameWork.player[2].buttonState |= MAP_PLAYER_BUTTON_MENU;
	if( (FrameWork.player[3].buttonState & (MAP_PLAYER4_BUTTON_UP | MAP_PLAYER4_BUTTON_DOWN)) == (MAP_PLAYER4_BUTTON_UP | MAP_PLAYER4_BUTTON_DOWN) )
		FrameWork.player[3].buttonState |= MAP_PLAYER_BUTTON_MENU;

	// add clear button to 7th bit
	if( (FrameWork.player[0].buttonState & (MAP_PLAYER1_BUTTON_LEFT | MAP_PLAYER1_BUTTON_RIGHT)) == (MAP_PLAYER1_BUTTON_LEFT | MAP_PLAYER1_BUTTON_RIGHT) )
		FrameWork.player[0].buttonState |= MAP_PLAYER_BUTTON_CLEAR;
	if( (FrameWork.player[1].buttonState & (MAP_PLAYER2_BUTTON_LEFT | MAP_PLAYER2_BUTTON_RIGHT)) == (MAP_PLAYER2_BUTTON_LEFT | MAP_PLAYER2_BUTTON_RIGHT) )
		FrameWork.player[1].buttonState |= MAP_PLAYER_BUTTON_CLEAR;
	if( (FrameWork.player[2].buttonState & (MAP_PLAYER3_BUTTON_LEFT | MAP_PLAYER3_BUTTON_RIGHT)) == (MAP_PLAYER3_BUTTON_LEFT | MAP_PLAYER3_BUTTON_RIGHT) )
		FrameWork.player[2].buttonState |= MAP_PLAYER_BUTTON_CLEAR;
	if( (FrameWork.player[3].buttonState & (MAP_PLAYER4_BUTTON_LEFT | MAP_PLAYER4_BUTTON_RIGHT)) == (MAP_PLAYER4_BUTTON_LEFT | MAP_PLAYER4_BUTTON_RIGHT) )
		FrameWork.player[3].buttonState |= MAP_PLAYER_BUTTON_CLEAR;

	// process buttons
	unsigned char updateFlag = 0, i;
	for( i = 0; i != 4; i++ )
	{
		
		// process positive edges and save old states
		FrameWork.player[i].buttonPositiveEdge = ((~FrameWork.player[i].oldButtonState) & FrameWork.player[i].buttonState);
		FrameWork.player[i].oldButtonState = FrameWork.player[i].buttonState;
		if( FrameWork.player[i].buttonPositiveEdge ) updateFlag = 1;

		// change random seed as long as buttons are being pressed
		if( FrameWork.player[i].buttonState&0x1F )
			FrameWork.randomSeed++;
	}

	// update any input loops
	if( updateFlag ) frameWorkUpdateInputLoop();

}

// ----------------------------------------------------------------------
// sets the refresh rate
void setRefreshRate( unsigned char refresh )
{
	FrameWork.updateDivider = 255/refresh;
	FrameWork.updateCounter = 0;
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
#ifdef GAME_ENABLE_COLOUR_DEMO
void startColourDemo( unsigned char* playerCount )
{
	loadColourDemo( FrameWork.frameBuffer, playerCount );
	FrameWork.state = FRAMEWORK_STATE_COLOUR_DEMO;
}
#endif

// ----------------------------------------------------------------------
// starts snake
#ifdef GAME_ENABLE_SNAKE
void startSnake( unsigned char* playerCount )
{
	loadSnake( FrameWork.frameBuffer, playerCount );
	FrameWork.state = FRAMEWORK_STATE_SNAKE;
}
#endif

// ----------------------------------------------------------------------
// starts the game of life
#ifdef GAME_ENABLE_GAME_OF_LIFE
void startGameOfLife( unsigned char* playerCount )
{
	loadGameOfLife( FrameWork.frameBuffer, playerCount );
	FrameWork.state = FRAMEWORK_STATE_GAME_OF_LIFE;
}
#endif

// ----------------------------------------------------------------------
// starts tron
#ifdef GAME_ENABLE_TRON
void startTron( unsigned char* playerCount )
{
	loadTron( FrameWork.frameBuffer, playerCount );
	FrameWork.state = FRAMEWORK_STATE_TRON;
}
#endif

// ----------------------------------------------------------------------
// starts space invaders
#ifdef GAME_ENABLE_SPACE_INVADERS
void startSpaceInvaders( unsigned char* playerCount )
{
	loadSpaceInvaders( FrameWork.frameBuffer, playerCount );
	FrameWork.state = FRAMEWORK_STATE_SPACE_INVADERS;
}
#endif

// ----------------------------------------------------------------------
// starts tetris
#ifdef GAME_ENABLE_TRON
void startTetris( unsigned char* playerCount )
{
	loadTetris( FrameWork.frameBuffer, playerCount );
	FrameWork.state = FRAMEWORK_STATE_TETRIS;
}
#endif

// ----------------------------------------------------------------------
// starts pong
#ifdef GAME_ENABLE_PONG
void startPong( unsigned char* playerCount )
{
	loadPong( FrameWork.frameBuffer, playerCount );
	FrameWork.state = FRAMEWORK_STATE_PONG;
}
#endif

// ----------------------------------------------------------------------
// starts burgler
#ifdef GAME_ENABLE_BURGLER
void startBurgler( unsigned char* playerCount )
{
	loadBurgler( FrameWork.frameBuffer, playerCount );
	FrameWork.state = FRAMEWORK_STATE_BURGLER;
}
#endif

// ----------------------------------------------------------------------
// end the game
void endGame( void )
{
	loadMenu( FrameWork.frameBuffer );
	FrameWork.state = FRAMEWORK_STATE_MENU;
}

// ----------------------------------------------------------------------
// gets the button state of a specific player (positive edge only)
// unfortunately, I see no way to compress this, because it is dependant on global definitions
inline unsigned char player1ButtonFire ( void ){ return FrameWork.player[0].buttonPositiveEdge & MAP_PLAYER1_BUTTON_FIRE;  }
inline unsigned char player1ButtonLeft ( void ){ return FrameWork.player[0].buttonPositiveEdge & MAP_PLAYER1_BUTTON_LEFT;  }
inline unsigned char player1ButtonRight( void ){ return FrameWork.player[0].buttonPositiveEdge & MAP_PLAYER1_BUTTON_RIGHT; }
inline unsigned char player1ButtonUp   ( void ){ return FrameWork.player[0].buttonPositiveEdge & MAP_PLAYER1_BUTTON_UP;    }
inline unsigned char player1ButtonDown ( void ){ return FrameWork.player[0].buttonPositiveEdge & MAP_PLAYER1_BUTTON_DOWN;  }
inline unsigned char player1ButtonMenu ( void ){ return FrameWork.player[0].buttonPositiveEdge & MAP_PLAYER_BUTTON_MENU;   }
inline unsigned char player1ButtonClear( void ){ return FrameWork.player[0].buttonPositiveEdge & MAP_PLAYER_BUTTON_CLEAR;  }

inline unsigned char player2ButtonFire ( void ){ return FrameWork.player[1].buttonPositiveEdge & MAP_PLAYER2_BUTTON_FIRE;  }
inline unsigned char player2ButtonLeft ( void ){ return FrameWork.player[1].buttonPositiveEdge & MAP_PLAYER2_BUTTON_LEFT;  }
inline unsigned char player2ButtonRight( void ){ return FrameWork.player[1].buttonPositiveEdge & MAP_PLAYER2_BUTTON_RIGHT; }
inline unsigned char player2ButtonUp   ( void ){ return FrameWork.player[1].buttonPositiveEdge & MAP_PLAYER2_BUTTON_UP;    }
inline unsigned char player2ButtonDown ( void ){ return FrameWork.player[1].buttonPositiveEdge & MAP_PLAYER2_BUTTON_DOWN;  }
inline unsigned char player2ButtonMenu ( void ){ return FrameWork.player[1].buttonPositiveEdge & MAP_PLAYER_BUTTON_MENU;   }
inline unsigned char player2ButtonClear( void ){ return FrameWork.player[1].buttonPositiveEdge & MAP_PLAYER_BUTTON_CLEAR;  }

inline unsigned char player3ButtonFire ( void ){ return FrameWork.player[2].buttonPositiveEdge & MAP_PLAYER3_BUTTON_FIRE;  }
inline unsigned char player3ButtonLeft ( void ){ return FrameWork.player[2].buttonPositiveEdge & MAP_PLAYER3_BUTTON_LEFT;  }
inline unsigned char player3ButtonRight( void ){ return FrameWork.player[2].buttonPositiveEdge & MAP_PLAYER3_BUTTON_RIGHT; }
inline unsigned char player3ButtonUp   ( void ){ return FrameWork.player[2].buttonPositiveEdge & MAP_PLAYER3_BUTTON_UP;    }
inline unsigned char player3ButtonDown ( void ){ return FrameWork.player[2].buttonPositiveEdge & MAP_PLAYER3_BUTTON_DOWN;  }
inline unsigned char player3ButtonMenu ( void ){ return FrameWork.player[2].buttonPositiveEdge & MAP_PLAYER_BUTTON_MENU;   }
inline unsigned char player3ButtonClear( void ){ return FrameWork.player[2].buttonPositiveEdge & MAP_PLAYER_BUTTON_CLEAR;  }

inline unsigned char player4ButtonFire ( void ){ return FrameWork.player[3].buttonPositiveEdge & MAP_PLAYER4_BUTTON_FIRE;  }
inline unsigned char player4ButtonLeft ( void ){ return FrameWork.player[3].buttonPositiveEdge & MAP_PLAYER4_BUTTON_LEFT;  }
inline unsigned char player4ButtonRight( void ){ return FrameWork.player[3].buttonPositiveEdge & MAP_PLAYER4_BUTTON_RIGHT; }
inline unsigned char player4ButtonUp   ( void ){ return FrameWork.player[3].buttonPositiveEdge & MAP_PLAYER4_BUTTON_UP;    }
inline unsigned char player4ButtonDown ( void ){ return FrameWork.player[3].buttonPositiveEdge & MAP_PLAYER4_BUTTON_DOWN;  }
inline unsigned char player4ButtonMenu ( void ){ return FrameWork.player[3].buttonPositiveEdge & MAP_PLAYER_BUTTON_MENU;   }
inline unsigned char player4ButtonClear( void ){ return FrameWork.player[3].buttonPositiveEdge & MAP_PLAYER_BUTTON_CLEAR;  }

// ----------------------------------------------------------------------
// general player input for "fire"
unsigned char playerButtonFire( unsigned char playerID )
{
	unsigned char mask;
	switch( playerID )
	{
		case 0 : mask = MAP_PLAYER1_BUTTON_FIRE; break;
		case 1 : mask = MAP_PLAYER2_BUTTON_FIRE; break;
		case 2 : mask = MAP_PLAYER3_BUTTON_FIRE; break;
		case 3 : mask = MAP_PLAYER4_BUTTON_FIRE; break;
		default: break;
	}
	return FrameWork.player[playerID].buttonPositiveEdge & mask;
}

// ----------------------------------------------------------------------
// general player input for "left"
unsigned char playerButtonLeft( unsigned char playerID )
{
	unsigned char mask;
	switch( playerID )
	{
		case 0 : mask = MAP_PLAYER1_BUTTON_LEFT; break;
		case 1 : mask = MAP_PLAYER2_BUTTON_LEFT; break;
		case 2 : mask = MAP_PLAYER3_BUTTON_LEFT; break;
		case 3 : mask = MAP_PLAYER4_BUTTON_LEFT; break;
		default: break;
	}
	return FrameWork.player[playerID].buttonPositiveEdge & mask;
}

// ----------------------------------------------------------------------
// general player input for "right"
unsigned char playerButtonRight( unsigned char playerID )
{
	unsigned char mask;
	switch( playerID )
	{
		case 0 : mask = MAP_PLAYER1_BUTTON_RIGHT; break;
		case 1 : mask = MAP_PLAYER2_BUTTON_RIGHT; break;
		case 2 : mask = MAP_PLAYER3_BUTTON_RIGHT; break;
		case 3 : mask = MAP_PLAYER4_BUTTON_RIGHT; break;
		default: break;
	}
	return FrameWork.player[playerID].buttonPositiveEdge & mask;
}

// ----------------------------------------------------------------------
// general player input for "up"
unsigned char playerButtonUp( unsigned char playerID )
{
	unsigned char mask;
	switch( playerID )
	{
		case 0 : mask = MAP_PLAYER1_BUTTON_UP; break;
		case 1 : mask = MAP_PLAYER2_BUTTON_UP; break;
		case 2 : mask = MAP_PLAYER3_BUTTON_UP; break;
		case 3 : mask = MAP_PLAYER4_BUTTON_UP; break;
		default: break;
	}
	return FrameWork.player[playerID].buttonPositiveEdge & mask;
}

// ----------------------------------------------------------------------
// general player input for "down"
unsigned char playerButtonDown( unsigned char playerID )
{
	unsigned char mask;
	switch( playerID )
	{
		case 0 : mask = MAP_PLAYER1_BUTTON_DOWN; break;
		case 1 : mask = MAP_PLAYER2_BUTTON_DOWN; break;
		case 2 : mask = MAP_PLAYER3_BUTTON_DOWN; break;
		case 3 : mask = MAP_PLAYER4_BUTTON_DOWN; break;
		default: break;
	}
	return FrameWork.player[playerID].buttonPositiveEdge & mask;
}

// ----------------------------------------------------------------------
// general player input for "menu"
inline unsigned char playerButtonMenu( unsigned char playerID )
{
	return FrameWork.player[playerID].buttonPositiveEdge & MAP_PLAYER_BUTTON_MENU;
}

// ----------------------------------------------------------------------
// general player input for "clear"
inline unsigned char playerButtonClear( unsigned char playerID )
{
	return FrameWork.player[playerID].buttonPositiveEdge & MAP_PLAYER_BUTTON_MENU;
}

// ----------------------------------------------------------------------
// sinus
inline signed char sin( unsigned short angle )
{
	angle /= 12;
	wrap( &angle, 30 );
	return sinus[ angle ];
}

// ----------------------------------------------------------------------
// wraps a value between 0 and <wrap>
inline void wrap( unsigned short* value, unsigned char wrap )
{
	while( (*value) >= wrap ){ (*value) -= wrap; }
}

// ----------------------------------------------------------------------
// square root
inline unsigned char sqrt( unsigned short value )
{

	// first guess
	register unsigned char result = 0x80;
	unsigned char currentBitMask = 0x80;

	// loop through all 7 bits in result
	for( unsigned char i = 6; i != 0xFF; i-- )
	{

		// if squared result is smaller than value, add new bit
		if( result*result < value )
		{
			currentBitMask = (1<<i);
			result |= currentBitMask;

		// if squared result is larger than value, shift that bit right
		}else{
			result &= ~currentBitMask;
			currentBitMask = (1<<i);
			result |= currentBitMask;
		}
	}

	// return result
	return result;
}

// ----------------------------------------------------------------------
// call update loop of current game running - passes the process on to
// the current "main loop" using a state machine for different modes
// this allows expandability for more games/demos in the future
void frameWorkUpdateProcessLoop( void )
{
	switch( FrameWork.state )
	{

		// built in
		case FRAMEWORK_STATE_MENU : processMenuLoop();                          break;
		case FRAMEWORK_STATE_START_UP_SCREEN : processStartUpScreenLoop();      break;

		// added games
	#ifdef GAME_ENABLE_SNAKE
		case FRAMEWORK_STATE_SNAKE           : processSnakeLoop();              break;
	#endif
	#ifdef GAME_ENABLE_COLOUR_DEMO
		case FRAMEWORK_STATE_COLOUR_DEMO     : processColourDemoLoop();         break;
	#endif
	#ifdef GAME_ENABLE_GAME_OF_LIFE
		case FRAMEWORK_STATE_GAME_OF_LIFE    : processGameOfLifeLoop();         break;
	#endif
	#ifdef GAME_ENABLE_TRON
		case FRAMEWORK_STATE_TRON            : processTronLoop();               break;
	#endif
	#ifdef GAME_ENABLE_SPACE_INVADERS
		case FRAMEWORK_STATE_SPACE_INVADERS  : processSpaceInvadersLoop();      break;
	#endif
	#ifdef GAME_ENABLE_TETRIS
		case FRAMEWORK_STATE_TETRIS          : processTetrisLoop();             break;
	#endif
	#ifdef GAME_ENABLE_PONG
		case FRAMEWORK_STATE_PONG            : processPongLoop();               break;
	#endif
	#ifdef GAME_ENABLE_BURGLER
		case FRAMEWORK_STATE_BURGLER         : processBurglerLoop();            break;
	#endif

		// error, reset to main menu
		default:
			FrameWork.state = FRAMEWORK_STATE_MENU;
			break;
	}
}

// ----------------------------------------------------------------------
// call input loop of current running game
void frameWorkUpdateInputLoop( void )
{

	// call input loop of current game
	switch( FrameWork.state )
	{

		// built in
		case FRAMEWORK_STATE_MENU                  : processMenuInput();               break;
		case FRAMEWORK_STATE_START_UP_SCREEN       : processStartUpScreenInput();      break;

		// added games
	#ifdef GAME_ENABLE_SNAKE
		case FRAMEWORK_STATE_SNAKE           : processSnakeInput();              break;
	#endif
	#ifdef GAME_ENABLE_COLOUR_DEMO
		case FRAMEWORK_STATE_COLOUR_DEMO     : processColourDemoInput();         break;
	#endif
	#ifdef GAME_ENABLE_GAME_OF_LIFE
		case FRAMEWORK_STATE_GAME_OF_LIFE    : processGameOfLifeInput();         break;
	#endif
	#ifdef GAME_ENABLE_TRON
		case FRAMEWORK_STATE_TRON            : processTronInput();               break;
	#endif
	#ifdef GAME_ENABLE_SPACE_INVADERS
		case FRAMEWORK_STATE_SPACE_INVADERS  : processSpaceInvadersInput();      break;
	#endif
	#ifdef GAME_ENABLE_TETRIS
		case FRAMEWORK_STATE_TETRIS          : processTetrisInput();             break;
	#endif
	#ifdef GAME_ENABLE_PONG
		case FRAMEWORK_STATE_PONG            : processPongInput();               break;
	#endif
	#ifdef GAME_ENABLE_BURGLER
		case FRAMEWORK_STATE_BURGLER         : processBurglerInput();            break;
	#endif

		default: break;
	}
}

// ----------------------------------------------------------------------
// Update interrupt
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A( void )
{

	// divide update rate
	unsigned char temp = (FrameWork.updateCounter++);
	if( temp != FrameWork.updateDivider ) return;
	FrameWork.updateCounter = 0;

	// set update flag (this is caught in the main loop)
	FrameWork.updateFlag = 1;
}
