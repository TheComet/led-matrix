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
#include "moduleenable.h"

// added games
#include "snake.h"
#include "colourdemo.h"
#include "gameoflife.h"
#include "tron.h"
#include "spaceinvaders.h"
#include "tetris.h"
#include "pong.h"
#include "burgler.h"
#include "catandmouse.h"

static struct FrameWork_t FrameWork;

// ----------------------------------------------------------------------
// initialise framework
void initFrameWork( void )
{

	// set initial values
	FrameWork.updateCounter = 0;
	FrameWork.updateDivider = 1;
	FrameWork.updateFlag = 0;
	FrameWork.gamesRegistered = 0;

	// register start screen
	registerModule( loadStartUpScreen, processStartUpScreenLoop, processStartUpScreenInput, drawStartUpScreenIconDummy );

	// register menu
	registerModule( loadMenu, processMenuLoop, processMenuInput, drawMenuIconDummy );

	// register user added games
#ifdef GAME_ENABLE_COLOUR_DEMO
	registerModule( loadColourDemo, processColourDemoLoop, processColourDemoInput, drawColourDemoMenuIcon );
#endif
#ifdef GAME_ENABLE_SNAKE
	registerModule( loadSnake, processSnakeLoop, processSnakeInput, drawSnakeMenuIcon );
#endif
#ifdef GAME_ENABLE_GAME_OF_LIFE
	registerModule( loadGameOfLife, processGameOfLifeLoop, processGameOfLifeInput, drawGameOfLifeMenuIcon );
#endif
#ifdef GAME_ENABLE_TRON
	registerModule( loadTron, processTronLoop, processTronInput, drawTronMenuIcon );
#endif
#ifdef GAME_ENABLE_TETRIS
	registerModule( loadTetris, processTetrisLoop, processTetrisInput, drawTetrisMenuIcon );
#endif
#ifdef GAME_ENABLE_SPACE_INVADERS
	registerModule( loadSpaceInvaders, processSpaceInvadersLoop, processSpaceInvadersInput, drawSpaceInvadersMenuIcon );
#endif
#ifdef GAME_ENABLE_PONG
	registerModule( loadPong, processPongLoop, processPongInput, drawPongMenuIcon );
#endif
#ifdef GAME_ENABLE_BURGLER
	registerModule( loadBurgler, processBurglerLoop, processBurglerInput, drawBurglerMenuIcon );
#endif
#ifdef GAME_ENABLE_CAT_AND_MOUSE
	registerModule( loadCatAndMouse, processCatAndMouseLoop, processCatAndMouseInput, drawCatAndMouseMenuIcon );
#endif

	// load startup screen
	unsigned char gameSelected = 0;
	unsigned char playerCount = 0;
	startGame( &gameSelected, &playerCount );
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
// register a game with the framework
void registerModule( loadFunction_cb_t loadFunction, processLoopFunction_cb_t processLoopFunction, processInputFunction_cb_t processInputFunction, drawMenuIconFunction_cb_t drawMenuIconFunction )
{

	// check for free slots
	if( FrameWork.gamesRegistered == MAX_MODULES ) return;

	// register game
	FrameWork.game[ FrameWork.gamesRegistered ].load = loadFunction;
	FrameWork.game[ FrameWork.gamesRegistered ].processLoop = processLoopFunction;
	FrameWork.game[ FrameWork.gamesRegistered ].processInput = processInputFunction;
	FrameWork.game[ FrameWork.gamesRegistered ].drawMenuIcon = drawMenuIconFunction;
	FrameWork.gamesRegistered++;
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
void clearFrameBuffer( unsigned short* frameBuffer )
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
// starts a game
void startGame( unsigned char* gameSelected, unsigned char* playerCount )
{
	FrameWork.game[ *gameSelected ].load( FrameWork.frameBuffer, playerCount );
	FrameWork.gameSelected = *gameSelected;
}

// ----------------------------------------------------------------------
// end the game
void endGame( void )
{

	// load the menu
	FrameWork.gameSelected = 1; unsigned char discard;
	FrameWork.game[ 1 ].load( FrameWork.frameBuffer, &discard );
}

// ----------------------------------------------------------------------
// updates menu icon
void menuUpdateIcon( unsigned char* selected )
{

	// clear icon space
	unsigned char x1=3, x2=12;
	fillBox( &x1, &x1, &x2, &x2, &BLACK );
	menuDrawLeftArrow( 0 );
	menuDrawRightArrow( 0 );

	// call draw function
	FrameWork.game[ *selected ].drawMenuIcon();
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
unsigned char sqrt( unsigned short* value )
{

        // prepare first guess
        register unsigned char result;
        register unsigned char currentBitMask;
        if( 0x4000 > *value )
        {
            result = 0;
            currentBitMask = 0x40;
        }else{
            currentBitMask = 0x80;
        }

        // loop through all 8 bits in result
        while( currentBitMask )
        {

                // add next bit
                result |= currentBitMask;

                // if squared result is larger than value, remove bit again
                if( result*result > *value ) result ^= currentBitMask;

                // select next bit
                currentBitMask >>= 1;
        }

        // return result
        return result;
}

// ----------------------------------------------------------------------
// call update loop of current game running - passes the process on to
// the current "main loop" by using a callback
// this allows expandability for more games/demos in the future
void frameWorkUpdateProcessLoop( void )
{
	
	// callback selected game's main loop
	FrameWork.game[ FrameWork.gameSelected ].processLoop();
}

// ----------------------------------------------------------------------
// call input loop of current running game
void frameWorkUpdateInputLoop( void )
{

	// callback selected game's input loop
	FrameWork.game[ FrameWork.gameSelected ].processInput();
}

// ----------------------------------------------------------------------
// Update interrupt
#pragma vector=TIMERA0_VECTOR
__interrupt void Timer_A( void )
{

	// call UART timeout update (for resend)
	UARTUpdateTimeOut();

	// divide update rate
	unsigned char temp = (FrameWork.updateCounter++);
	if( temp != FrameWork.updateDivider ) return;
	FrameWork.updateCounter = 0;

	// set update flag (this is caught in the main loop)
	FrameWork.updateFlag = 1;
}
