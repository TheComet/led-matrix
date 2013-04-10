// ----------------------------------------------------------------------
// Colour Demo
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "colourdemo.h"
#include "framework.h"
#include "uart.h"

static struct ColourDemo_t ColourDemo;

// ----------------------------------------------------------------------
// load colour demo
void loadColourDemo( unsigned char* frameBuffer, unsigned char* playerCount )
{

	// clear screen
	cls();
	send();

	// set refresh rate
	setRefreshRate( 255 );
}

// ----------------------------------------------------------------------
// process colour demo loop
void processColourDemoLoop( void )
{
	unsigned char fx, fy;
	signed char x=7, y=7;
	x += (sin( ColourDemo.angle )/16);
	y += (sin( ColourDemo.angle+90 )/16);
	fx = (unsigned char) x;
	fy = (unsigned char) y;
	dot( &fx, &fy, &BLUE );
	ColourDemo.angle += 32;
	send();
}

// ----------------------------------------------------------------------
// process colour demo input
void processColourDemoInput( void )
{
	
	// exit game
	if( player1ButtonMenu() ) endGame();
}