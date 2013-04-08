// ----------------------------------------------------------------------
// Main Menu
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "menu.h"
#include "common.h"
#include "uart.h"
#include "framework.h"

struct Menu_t Menu;

// colour data for snake
unsigned short snake[100] = {0x0,0x0,0xA71,0x972,0x852,0x751,0x0,0x0,0x0,0x0,0x0,0xB92,0xA91,0x972,0x862,0x851,0x730,0x0,0x0,0x0,0x0,0xBA2,0xA82,0x420,0x0,0x851,0x750,0x0,0x0,0x0,0x0,0x0,0xD00,0x0,0x0,0x852,0x851,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x972,0x851,0x0,0x0,0x0,0x0,0x0,0x0,0x420,0xA81,0x972,0x0,0x0,0x0,0x0,0x0,0x0,0xCA3,0xB92,0x420,0x0,0x0,0x100,0x740,0x0,0x0,0xDC4,0xCB3,0xBA2,0x0,0x0,0x0,0x0,0x0,0x730,0x0,0xDC4,0xCC3,0xBB3,0x0,0x0,0x0,0x420,0x851,0x730,0x0,0x0,0xDC4,0xCA3,0xBA2,0xA92,0x971,0x972,0x420,0x0};

// ----------------------------------------------------------------------
// draws an arrow pointing to player 1's port
void menuDrawStartArrow( unsigned char offset )
{

	// draw arrow
	for( unsigned char i = 0; i != 5; i++ )
	{
		unsigned char x1=7-i, y1=15-offset-i, x2=8+i;
		unsigned short cA=(0x00E|((i<<4)*3));
		line( &x1, &y1, &x2, &y1, &cA );
	}
}

// ----------------------------------------------------------------------
// draws the menu frame
void menuDrawFrame( void )
{
	unsigned char x1=0, x2=15;
	unsigned short cA=0x0EE, cB=0x00E;
	blendColourBox( &x1, &x1, &x2, &x2, &cA, &cB, &cB, &cA );
}

// ----------------------------------------------------------------------
// draws the left and right game selection arrows
void menuDrawLeftArrow( unsigned char clear )
{
	for( unsigned char i = 0; i != 2; i++ )
	{
		unsigned char x1=i, y1=7-i, y2=8+i;
		unsigned short cA=0xEE0*clear;
		line( &x1, &y1, &x1, &y2, &cA );
	}
}
void menuDrawRightArrow( unsigned char clear )
{
	for( unsigned char i = 0; i != 2; i++ )
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
	unsigned short cA=0x0E0;
	line( &x1, &y1, &x2, &y1, &cA );
	x1=13; y1=15;
	dot( &x1, &y1, &cA );

	// player 2
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
	unsigned short cA=0;
	fillBox( &x1, &x1, &x2, &x2, &cA );
}

// ----------------------------------------------------------------------
// draws the colour demo icon
void menuDrawColourDemoIcon( void )
{
	unsigned char x1=3, x2=12;
	unsigned short cA=0xE00, cB=0x0E0, cC=0x00E, cD=0xEE0;
	blendColourFillBox( &x1, &x1, &x2, &x2, &cA, &cB, &cC, &cD );
}

// ----------------------------------------------------------------------
// draws the snake icon
void menuDrawSnakeIcon( void )
{
	unsigned char index = 0;
	for( unsigned char y = 3; y != 13; y++ )
	{
		for( unsigned char x = 3; x != 13; x++ )
		{
			dot( &x, &y, &snake[index] );
			index++;
		}
	}
}

// ----------------------------------------------------------------------
// updates the icon
void menuUpdateIcon( unsigned char* selected )
{
	menuClearIcon();
	menuDrawLeftArrow(0);
	menuDrawRightArrow(0);
	if( *selected == MENU_SELECT_COLOUR_DEMO ){ menuDrawColourDemoIcon(); return; }
	if( *selected == MENU_SELECT_SNAKE )      { menuDrawSnakeIcon();      return; }
	return;
}

// ----------------------------------------------------------------------
// load menu
void loadMenu( void )
{

	// initial state
	Menu.state = MENU_STATE_PRESS_START;
	Menu.toggleArrow = 0;

	// reset player list
	Menu.playerList = 0;

	// slow refresh rate
	setRefreshRate( 50 );
}

// ----------------------------------------------------------------------
// process menu
void processMenu( void )
{

	// toggle arrow
	Menu.toggleArrow = 1-Menu.toggleArrow;

	// process players joining/leaving
	if( player2ButtonFire() ){ Menu.playerList ^= 0x01; menuDrawJoinArrows( &Menu.playerList ); send(); }
	if( player3ButtonFire() ){ Menu.playerList ^= 0x02; menuDrawJoinArrows( &Menu.playerList ); send(); }
	if( player4ButtonFire() ){ Menu.playerList ^= 0x04; menuDrawJoinArrows( &Menu.playerList ); send(); }

	// different menu screens
	switch( Menu.state )
	{

		// press start
		case MENU_STATE_PRESS_START :

			// draw start arrow
			cls();
			menuDrawStartArrow( Menu.toggleArrow );
			send();

			// start is pressed
			if( player1ButtonFire() )
			{

				// draw first game
				cls();
				menuDrawColourDemoIcon();
				menuDrawFrame();
				menuDrawRightArrow(0);
				menuDrawLeftArrow(0);
				menuDrawJoinArrows( &Menu.playerList );
				send();
				
				// switch states
				Menu.selected = MENU_SELECT_COLOUR_DEMO;
				Menu.state = MENU_STATE_SELECT_GAME;
			}

			break;

		// select game
		case MENU_STATE_SELECT_GAME :

			// control left and right arrow blinking
			if( Menu.selected ) menuDrawLeftArrow( Menu.toggleArrow );
			if( Menu.selected < GAME_COUNT ) menuDrawRightArrow( Menu.toggleArrow );
			send();

			// select previous game
			if( player1ButtonLeft() && Menu.selected )
			{
				Menu.selected--;
				
				// draw appropriate icon
				menuUpdateIcon( &Menu.selected );
				send();

				// reset players joined
				Menu.playerList = 0;
			}

			// select next game
			if( player1ButtonRight() && Menu.selected < GAME_COUNT )
			{
				Menu.selected++;

				// draw appropriate icon
				menuUpdateIcon( &Menu.selected );
				send();

				// reset players joined
				Menu.playerList = 0;
			}

			// select a game
			if( player1ButtonFire() )
			{
				switch( Menu.selected )
				{
					case MENU_SELECT_COLOUR_DEMO : startColourDemo();           break;
					case MENU_SELECT_SNAKE       : startSnake();                break;
					default: break;
				}
			}

			break;

		default: Menu.state = MENU_STATE_PRESS_START; break;
	}
}