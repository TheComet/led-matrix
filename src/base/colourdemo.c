// ----------------------------------------------------------------------
// Colour Demo
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "colourdemo.h"
#include "framework.h"
#include "uart.h"

// ----------------------------------------------------------------------
// load colour demo
void loadColourDemo( unsigned char* frameBuffer )
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
}

// ----------------------------------------------------------------------
// process colour demo input
void processColourDemoInput( void )
{
}