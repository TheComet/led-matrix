// ----------------------------------------------------------------------
// Start-Up Screen
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include Files
// ----------------------------------------------------------------------

#include "startupscreen.h"
#include "framework.h"
#include "uart.h"

static struct StartUpScreen_t StartUpScreen;

// ----------------------------------------------------------------------
// draws an arrow pointing to player 1's port
void drawStartArrow( void )
{

	// draw arrow
	for( unsigned char i = 0; i != 5; i++ )
	{
		unsigned char x1=7-i, y1=15-StartUpScreen.toggle-i, x2=8+i;
		unsigned short cA=(0x00E|((i<<4)*3));
		line( &x1, &y1, &x2, &y1, &cA );
	}
}

// ----------------------------------------------------------------------
// draws help so the user knows what to press
void drawButtonHelp( void )
{

	// draw non-changing buttons
	unsigned char x1, y1, x2, y2;
	x1=0x05; y1=0x03; x2=0x06; y2=0x04; fillBox( &x1, &y1, &x2, &y2, &BLUE );
	x1=0x09;          x2=0x0A;          fillBox( &x1, &y1, &x2, &y2, &BLUE );
	         y1=0x05;          y2=0x06; fillBox( &x1, &y1, &x2, &y2, &BLUE );

	// draw changing buttons
	x1=0x07; y1=0x01; x2=0x08; y2=0x02; if( StartUpScreen.toggle ) fillBox( &x1, &y1, &x2, &y2, &BLUE ); else fillBox( &x1, &y1, &x2, &y2, &YELLOW );
	         y1=0x03;          y2=0x04; if( StartUpScreen.toggle ) fillBox( &x1, &y1, &x2, &y2, &BLUE ); else fillBox( &x1, &y1, &x2, &y2, &YELLOW );
}

// ----------------------------------------------------------------------
// load start up screen
void loadStartUpScreen( void )
{

	// reset values
	StartUpScreen.toggle = 0;

	// set refresh rate
	setRefreshRate( 50 );

	// prepare screen
	cls();
	send();
}

// ----------------------------------------------------------------------
// process start up screen loop
void processStartUpScreenLoop( void )
{

	// toggle
	StartUpScreen.toggle = 1 - StartUpScreen.toggle;

	// draw graphics
	cls();
	drawStartArrow();
	drawButtonHelp();
	send();
}

// ----------------------------------------------------------------------
// process start up screen input
void processStartUpScreenInput( void )
{

	// start is pressed
	if( player1ButtonMenu() ) endGame();
}