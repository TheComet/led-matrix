// ----------------------------------------------------------------------
// Main Menu
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "menu.h"
#include "uart.h"
#include "framework.h"
#include "gameenable.h"

static struct Menu_t Menu;

// icon images
#ifdef GAME_ENABLE_SNAKE
static const unsigned short snakeIcon[100] = {0x0,0x0,0xA71,0x972,0x852,0x751,0x0,0x0,0x0,0x0,0x0,0xB92,0xA91,0x972,0x862,0x851,0x730,0x0,0x0,0x0,0x0,0xBA2,0xA82,0x420,0x0,0x851,0x750,0x0,0x0,0x0,0x0,0x0,0xD00,0x0,0x0,0x852,0x851,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x972,0x851,0x0,0x0,0x0,0x0,0x0,0x0,0x420,0xA81,0x972,0x0,0x0,0x0,0x0,0x0,0x0,0xCA3,0xB92,0x420,0x0,0x0,0x100,0x740,0x0,0x0,0xDC4,0xCB3,0xBA2,0x0,0x0,0x0,0x0,0x0,0x730,0x0,0xDC4,0xCC3,0xBB3,0x0,0x0,0x0,0x420,0x851,0x730,0x0,0x0,0xDC4,0xCA3,0xBA2,0xA92,0x971,0x972,0x420,0x0};
#endif

// ----------------------------------------------------------------------
// draws the menu frame
void menuDrawFrame( void )
{
	unsigned char x1=0, x2=15;
	blendColourBox( &x1, &x1, &x2, &x2, &LIGHTBLUE, &BLUE, &BLUE, &LIGHTBLUE );
}

// ----------------------------------------------------------------------
// draws the left and right game selection arrows
void menuDrawLeftArrow( unsigned char clear )
{
	unsigned char i;
	for( i = 0; i != 2; i++ )
	{
		unsigned char x1=i, y1=7-i, y2=8+i;
		unsigned short cA=0xEE0*clear;
		line( &x1, &y1, &x1, &y2, &cA );
	}
}
void menuDrawRightArrow( unsigned char clear )
{
	unsigned char i;
	for( i = 0; i != 2; i++ )
	{
		unsigned char x1=15-i, y1=7-i, x2=15-i, y2=8+i;
		unsigned short cA=0xEE0*clear;
		line( &x1, &y1, &x2, &y2, &cA );
	}
}

// ----------------------------------------------------------------------
// draw join arrows
void menuDrawJoinArrows( unsigned char* playerList )
{
	
	// player 1 (always green)
	unsigned char x1=12, y1=14, x2=14;
	line( &x1, &y1, &x2, &y1, &GREEN );
	x1=13; y1=15;
	dot( &x1, &y1, &GREEN );

	// player 2
	unsigned short cA;
	x1=1; y1=1; x2=3;
	if( (*playerList)&0x01 ) cA = 0x0E0; else cA = 0xE00;
	line( &x1, &y1, &x2, &y1, &cA );
	x1=2; y1=0;
	dot( &x1, &y1, &cA );

	// player 3
	y1=1; x1=12; x2=14;
	if( (*playerList)&0x02 ) cA = 0x0E0; else cA = 0xE00;
	line( &y1, &x1, &y1, &x2, &cA ),
	x1=0; y1=13;
	dot( &x1, &y1, &cA );

	// player 4
	y1=14; x1=1; x2=3;
	if( (*playerList)&0x04 ) cA = 0x0E0; else cA = 0xE00;
	line( &y1, &x1, &y1, &x2, &cA );
	x1=15; y1=2;
	dot( &x1, &y1, &cA );
}

// ----------------------------------------------------------------------
// clears the icon space
void menuClearIcon( void )
{
	unsigned char x1=3, x2=12;
	fillBox( &x1, &x1, &x2, &x2, &BLACK );
}

/*
// ----------------------------------------------------------------------
// draws the colour demo icon
#ifdef GAME_ENABLE_COLOUR_DEMO
void menuDrawColourDemoIcon( void )
{
	unsigned char x1=3, x2=12;
	unsigned short cA=0xE00, cB=0x0E0, cC=0x00E, cD=0xEE0;
	blendColourFillBox( &x1, &x1, &x2, &x2, &cA, &cB, &cC, &cD );
}
#endif

// ----------------------------------------------------------------------
// draws the snake icon
#ifdef GAME_ENABLE_SNAKE
void menuDrawSnakeIcon( void )
{
	unsigned char x, y, index = 0;
	for( y = 3; y != 13; y++ )
	{
		for( x = 3; x != 13; x++ )
		{
			dot( &x, &y, &snakeIcon[index] );
			index++;
		}
	}
}
#endif

// ----------------------------------------------------------------------
// draws game of life icon
#ifdef GAME_ENABLE_GAME_OF_LIFE
void menuDrawGameOfLifeIcon( void )
{
	unsigned char x, y, i;
	for( i = 0; i != 40; i++ )
	{
		x = rnd() & 0x0F;
		y = rnd() & 0x0F;
		if( x < 3 ) x = 3;
		if( x > 12 ) x = 3+(x-12);
		if( y < 3 ) y = 3;
		if( y > 12 ) y = 3+(y-12);
		dot( &x, &y, &GREEN );
	}
		
}
#endif

// ----------------------------------------------------------------------
// draws the tron icon
#ifdef GAME_ENABLE_TRON
void menuDrawTronIcon( void )
{
	unsigned char x1=4, y1=12, x2=4, y2=4;
	line( &x1, &y1, &x2, &y2, &RED );
	x1=7; y1=4;
	line( &x2, &y2, &x1, &y2, &RED );
	x2=7; y2=6;
	line( &x1, &y1, &x2, &y2, &RED );
	x1=12; y1=8; x2=6; y2=8;
	line( &x1, &y1, &x2, &y2, &GREEN );
	x1=3; x2=12;
	box( &x1, &x1, &x2, &x2, &PURPLE );
}
#endif

// ----------------------------------------------------------------------
// draws the space invaders icon
#ifdef GAME_ENABLE_SPACE_INVADERS
void menuDrawSpaceInvadersIcon( void )
{
}
#endif

// ----------------------------------------------------------------------
// draws the tetris icon
#ifdef GAME_ENABLE_TETRIS
void menuDrawTetrisIcon( void )
{
	unsigned char x1=5, y1=4, x2=7, y2=9;
	fillBox( &x1, &y1, &x2, &y2, &WHITE );
	x1=8; y1=7; x2=10; y2=12;
	fillBox( &x1, &y1, &x2, &y2, &WHITE );
}
#endif

// ----------------------------------------------------------------------
#ifdef GAME_ENABLE_PONG
void menuDrawPongIcon( void )
{
}
#endif

// ----------------------------------------------------------------------
#ifdef GAME_ENABLE_BURGLER
void menuDrawBurglerIcon( void )
{
}
#endif
*/

// ----------------------------------------------------------------------
// initialises some values for the menu
void initMenu( void )
{
	Menu.selected  = 0;
	Menu.gameCount = getGameCount();
}

// ----------------------------------------------------------------------
// load menu
void loadMenu( void )
{

	// initial state
	Menu.state = MENU_STATE_SELECT_GAME;

	// set refresh rate
	setRefreshRate( 50 );

	// reset values
	Menu.toggleArrow = 0;
	Menu.playerList = 0;

	// draw current game
	cls();
	menuDrawFrame();
	menuDrawRightArrow(0);
	menuDrawLeftArrow(0);
	menuDrawJoinArrows( &Menu.playerList );
	//menuUpdateIcon( &Menu.selected );
	send();
}

// ----------------------------------------------------------------------
// process menu loop
void processMenuLoop( void )
{

	// toggle
	Menu.toggleArrow = 1 - Menu.toggleArrow;

	// draw different menu screens
	switch( Menu.state )
	{

		// select game
		case MENU_STATE_SELECT_GAME :

			// control left and right arrow blinking
			if( Menu.selected ) menuDrawLeftArrow( Menu.toggleArrow );
			if( Menu.selected < Menu.gameCount ) menuDrawRightArrow( Menu.toggleArrow );
			send();

			break;

		default: break;
	}
}

// ----------------------------------------------------------------------
// process menu input
void processMenuInput( void )
{

	// process players joining/leaving
	if( player2ButtonFire() ){ Menu.playerList ^= 0x01; menuDrawJoinArrows( &Menu.playerList ); send(); }
	if( player3ButtonFire() ){ Menu.playerList ^= 0x02; menuDrawJoinArrows( &Menu.playerList ); send(); }
	if( player4ButtonFire() ){ Menu.playerList ^= 0x04; menuDrawJoinArrows( &Menu.playerList ); send(); }

	// draw different menu screens
	switch( Menu.state )
	{

		// select game
		case MENU_STATE_SELECT_GAME :

			// select previous game
			if( player1ButtonLeft() && Menu.selected )
			{
				Menu.selected--;
				
				// reset players joined
				Menu.playerList = 0;
				menuDrawJoinArrows( &Menu.playerList );

				// draw appropriate icon
				menuUpdateIcon( &Menu.selected );
				send();
			}

			// select next game
			if( player1ButtonRight() && Menu.selected < Menu.gameCount )
			{
				Menu.selected++;

				// reset players joined
				Menu.playerList = 0;
				menuDrawJoinArrows( &Menu.playerList );

				// draw appropriate icon
				menuUpdateIcon( &Menu.selected );
				send();
			}

			// start a game
			if( player1ButtonFire() )
			{
				startGame( &Menu.selected, &Menu.playerList );
			}

			break;

		default: break;
	}
}
