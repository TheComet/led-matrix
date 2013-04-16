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
void loadColourDemo( unsigned short* frameBuffer, unsigned char* playerCount )
{

	// get frame buffer
	ColourDemo.frameBuffer = frameBuffer;

	// clear screen
	cls();
	send();

	// starting colour
	ColourDemo.startR = 14;
	ColourDemo.startG = 0;
	ColourDemo.startB = 0;

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

	// set starting colour
	if( ColourDemo.startR == 14 && ColourDemo.startG <  14 ) ColourDemo.startG+=2;
	if( ColourDemo.startG == 14 && ColourDemo.startR >  0  ) ColourDemo.startR-=2;
	if( ColourDemo.startG == 14 && ColourDemo.startB <  14 ) ColourDemo.startB+=2;
	if( ColourDemo.startB == 14 && ColourDemo.startG >  0  ) ColourDemo.startG-=2;
	if( ColourDemo.startB == 14 && ColourDemo.startR <  14 ) ColourDemo.startR+=2;
	if( ColourDemo.startR == 14 && ColourDemo.startB >  0  ) ColourDemo.startB-=2;
	
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
			colour = sx + sy;
			colour = sqrt( &colour );

			// compute colours
			colour >>= 1;
			r = ColourDemo.startR; g = ColourDemo.startG; b = ColourDemo.startB;
			while( colour )
			{
				colour--;
				if( r == 14 && g <  14 && b == 0 ) g++;
				if( g == 14 && r >  0  ) r--;
				if( g == 14 && b <  14 ) b++;
				if( b == 14 && g >  0  ) g--;
				if( b == 14 && r <  14 ) r++;
				if( r == 14 && b >  0  ) b--;
			}

			// final colour
			colour = ((r<<8)|(g<<4)|b);

			// save in frame buffer
			(*(ColourDemo.frameBuffer+y+(x<<4))) = colour;
		}
	}

	// update display
	colourDemoDrawFrameBuffer();
	send();
}

// ----------------------------------------------------------------------
// process colour demo input
void processColourDemoInput( void )
{
	
	// exit game
	if( player1ButtonMenu() ) endGame();
}

// ----------------------------------------------------------------------
// draws the frame buffer to the screen
void colourDemoDrawFrameBuffer()
{
	unsigned short* bufferPtr;
	for( unsigned char x = 0; x != 16; x++ )
	{
		bufferPtr = (ColourDemo.frameBuffer+(x<<4));
		for( unsigned char y = 0; y != 16; y++ )
		{
			dot( &x, &y, bufferPtr+y );
		}
	}
}
#endif // GAME_ENABLE_COLOUR_DEMO
