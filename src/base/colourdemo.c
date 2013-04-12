// ----------------------------------------------------------------------
// Colour Demo
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "colourdemo.h"
#include "framework.h"
#include "uart.h"
#include "gameenable.h"

#ifdef GAME_ENABLE_COLOUR_DEMO

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

	// locals
	register unsigned char cx, cy;
	register signed short sx, sy;
	unsigned short colour;
	unsigned char r, g, b;

	// slowly rotate centre point
	ColourDemo.angle += 5;
	
	// convert angle to coordinates
	sx=60, sy=60;
	sx += (sin( ColourDemo.angle )>>2);
	sy += (sin( ColourDemo.angle+90 )>>2);
	cx = (unsigned char) sx;
	cy = (unsigned char) sy;

	// computer colours of every dot
	for( unsigned char x = 0; x != 16; x++ )
	{
		for( unsigned char y = 0; y != 16; y++ )
		{

			// get distance
			sx = (cx - (x<<3)); sy = (cy - (y<<3));
			sx *= sx; sy *= sy;
			sx += sy;
			colour = sqrt( (unsigned short) sx );

			// compute colours
			colour >>= 1;
			r = 14; g = 0; b = 0;
			while( colour )
			{
				colour--;
				if( r == 14 && g <  14 ) g++;
				if( g == 14 && r >  0  ) r--;
				if( g == 14 && b <  14 ) b++;
				if( b == 14 && g >  0  ) g--;
			}

			// final colour
			colour = ((r<<8)|(g<<4)|b);

			dot( &x, &y, &colour );
		}
	}

	// update display
	send();
}

// ----------------------------------------------------------------------
// process colour demo input
void processColourDemoInput( void )
{
	
	// exit game
	if( player1ButtonMenu() ) endGame();
}
#endif // GAME_ENABLE_COLOUR_DEMO
