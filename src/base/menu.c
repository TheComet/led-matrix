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

// initialises some values for the menu
void initMenu( void )
{
	Menu.selected  = 2;
	Menu.gameCount = getGameCount() + 2; // startup screen and menu are 0, 1
}

// ----------------------------------------------------------------------
// load menu
void loadMenu( unsigned short* frameBuffer, unsigned char* playerCount )
{

	// set refresh rate
	setRefreshRate( 70 );

	// reset values
	Menu.toggleArrow = 0;
	Menu.playerList = 0;

	// draw current game
	cls();
	menuDrawFrame();
	menuDrawRightArrow(0);
	menuDrawLeftArrow(0);
	menuDrawJoinArrows( &Menu.playerList );
	menuUpdateIcon( &Menu.selected );
	send();
}

// ----------------------------------------------------------------------
// process menu loop
void processMenuLoop( void )
{

	// toggle
	Menu.toggleArrow = 1 - Menu.toggleArrow;

	// control left and right arrow blinking
	if( Menu.selected > 2 ) menuDrawLeftArrow( Menu.toggleArrow );
	if( Menu.selected < Menu.gameCount ) menuDrawRightArrow( Menu.toggleArrow );
	send();

}

// ----------------------------------------------------------------------
// process menu input
void processMenuInput( void )
{

	// process players joining/leaving
	if( player2ButtonFire() ){ Menu.playerList ^= 0x01; menuDrawJoinArrows( &Menu.playerList ); send(); }
	if( player3ButtonFire() ){ Menu.playerList ^= 0x02; menuDrawJoinArrows( &Menu.playerList ); send(); }
	if( player4ButtonFire() ){ Menu.playerList ^= 0x04; menuDrawJoinArrows( &Menu.playerList ); send(); }

	// select previous game
	if( player1ButtonLeft() && Menu.selected > 2 )
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
}

// ----------------------------------------------------------------------
// Menu Icon dummy, so the framework is happy
void drawMenuIconDummy( void )
{
}
