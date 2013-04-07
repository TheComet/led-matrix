// ----------------------------------------------------------------------
// Drawing Utilities
// ----------------------------------------------------------------------

// include files
#include "common.h"
#include "drawUtils.h"

// global variables
struct drawUtils_t drawUtils;

// ------------------------------------------------------------------------------------------------------------------
// Sets the blending mode. Available modes: BLEND_MODE_REPLACE *** BLEND_MODE_ADD *** BLEND_MODE_SUBTRACT *** BLEND_MODE_MULTIPLY
void drawUtils_SetBlendMode( unsigned char blendMode )
{
	drawUtils.blendMode = blendMode;
}

// ------------------------------------------------------------------------------------------------------------------
// Will draw a filled box from x1,y1 to x2,y2 with a different colour for each corner and blend between them
void blendColourFillBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* topLeftColour, unsigned short* bottomLeftColour, unsigned short* topRightColour, unsigned short* bottomRightColour )
{

	// local variables
	unsigned char x, y;
	unsigned short finalColour;
	unsigned char blendFactorX = *x2 - *x1;
	unsigned char blendFactorY = *y2 - *y1;

	// blend entire top and bottom rows and store in array
	unsigned char blendTable[3][2];
	for( x = *x1; x <= *x2; x++ )
	{
	}

	// calculates all pixels between the four points
	for( x = *x1; x <= *x2; x++ )
	{

		// calculate x blend
		blendTable[0][0] = (((*topLeftColour&0xF00)>>8)    * (*x2-x)/blendFactorX) + (((*topRightColour&0xF00)>>8)    * (x-*x1)/blendFactorX);
		blendTable[1][0] = (((*topLeftColour&0x0F0)>>4)    * (*x2-x)/blendFactorX) + (((*topRightColour&0x0F0)>>4)    * (x-*x1)/blendFactorX);
		blendTable[2][0] = (( *topLeftColour&0x00F)        * (*x2-x)/blendFactorX) + (( *topRightColour&0x00F)        * (x-*x1)/blendFactorX);
		blendTable[0][1] = (((*bottomLeftColour&0xF00)>>8) * (*x2-x)/blendFactorX) + (((*bottomRightColour&0xF00)>>8) * (x-*x1)/blendFactorX);
		blendTable[1][1] = (((*bottomLeftColour&0x0F0)>>4) * (*x2-x)/blendFactorX) + (((*bottomRightColour&0x0F0)>>4) * (x-*x1)/blendFactorX);
		blendTable[2][1] = (( *bottomLeftColour&0x00F)     * (*x2-x)/blendFactorX) + (( *bottomRightColour&0x00F)     * (x-*x1)/blendFactorX);
		for( y = *y1; y <= *y2; y++ )
		{

			// blend y coordinates
			finalColour =  (blendTable[0][0] * (*y2-y)/blendFactorY + (blendTable[0][1]) * (y-*y1)/blendFactorY)<<8;
			finalColour |= (blendTable[1][0] * (*y2-y)/blendFactorY + (blendTable[1][1]) * (y-*y1)/blendFactorY)<<4;
			finalColour |=  blendTable[2][0] * (*y2-y)/blendFactorY + (blendTable[2][1]) * (y-*y1)/blendFactorY;

			// set pixel
			dot( &x, &y, &finalColour );
		}
	}

	// return
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// Will draw the outline of a box from x1,y1 to x2,y2 with a different colour for each corner and blend between them
void blendColourBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* topLeftColour, unsigned short* bottomLeftColour, unsigned short* topRightColour, unsigned short* bottomRightColour )
{

	// local variables
	unsigned char x;
	unsigned char blendFactorX = *x2 - *x1;
	unsigned char blendFactorY = *y2 - *y1;
	unsigned short finalColour;

	// blend top and bottom rows
	for( x = *x1; x <= *x2; x++ )
	{
		finalColour = blendColours( *topLeftColour, *topRightColour, *x1, *x2, x, blendFactorX); dot( &x, y1, &finalColour );
		finalColour = blendColours( *bottomLeftColour, *bottomRightColour, *x1, *x2, x, blendFactorX); dot( &x, y2, &finalColour );
	}

	// blend left and right columns
	for( x = *y1; x <= *y2; x++ )
	{
		finalColour = blendColours( *topLeftColour, *bottomLeftColour, *y1, *y2, x, blendFactorY); dot( x1, &x, &finalColour );
		finalColour = blendColours( *topRightColour, *bottomRightColour, *y1, *y2, x, blendFactorY); dot( x2, &x, &finalColour );
	}

	// return
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// will draw the outline of a box with one colour
void box( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour )
{

	// local variables
	unsigned char x;

	// draw box
	for( x = *x1; x <= *x2; x++ )
	{
		dot( &x, y1, colour );
		dot( &x, y2, colour );
	}
	for( x = *y1; x <= *y2; x++ )
	{
		dot( x1, &x, colour );
		dot( x2, &x, colour );
	}

	// return
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// will draw a filled box with one colour
void fillBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour )
{

	// local variables
	unsigned char x, y;

	// draw box
	for( x = *x1; x <= *x2; x++ )
	{
		for( y = *y1; y <= *y2; y++ )
		{
			dot( &x, &y, colour );
		}
	}

	// return
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// will draw a line from point A to point B with one colour
void line( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour )
{

	// get delta
	unsigned char dx = absolute(*x2-*x1);
	unsigned char dy = absolute(*y2-*y1);
	signed char sx, sy;
	if( *x1 < *x2 ) sx=1; else sx=-1;
	if( *y1 < *y2 ) sy=1; else sy=-1;
	signed char err = (signed char)(dx - dy);
	signed char e2;

	// plot line
	while(1)
	{
		dot( x1, y1, colour );
		if( *x1 == *x2 && *y1 == *y2 ) break;
		e2 = 2*err;
		if( e2 > 0-dy )
		{
			err -= dy;
			*x1 += sx;
		}
		if( e2 < dx )
		{
			err += dx;
			*y1 += sy;
		}
	}

	// return
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// will draw a line from point A to point B and blend between two colours
void blendColourLine( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour1, unsigned short* colour2 )
{

	// get delta
	unsigned char dx = absolute(*x2-*x1);
	unsigned char dy = absolute(*y2-*y1);
	signed char sx, sy;
	if( *x1 < *x2 ) sx=1; else sx=-1;
	if( *y1 < *y2 ) sy=1; else sy=-1;
	signed char err = (signed char)(dx - dy);
	signed char e2;
	unsigned short finalColour;

	// plot line
	unsigned char x=*x1, y=*y1, blendStart, blendEnd;
	while(1)
	{

		// blend colours using longest distance and draw line
		if( dx > dy )
		{
			if( *x2 > *x1 ){ blendStart = *x1; blendEnd = *x2; }else{ blendStart = *x2; blendEnd = *x1; }
			finalColour = blendColours( *colour1, *colour2, blendStart, blendEnd, x, dx);
			dot( &x, &y, &finalColour );
		}else{
			if( *y2 > *y1 ){ blendStart = *y1; blendEnd = *y2; }else{ blendStart = *y2; blendEnd = *y1; }
			finalColour = blendColours( *colour1, *colour2, blendStart, blendEnd, y, dy);
			dot( &x, &y, &finalColour );
		}

		// draw line
		if( x == *x2 && y == *y2 ) break;
		e2 = 2*err;
		if( e2 > 0-dy )
		{
			err -= dy;
			x += sx;
		}
		if( e2 < dx )
		{
			err += dx;
			y += sy;
		}
	}

	// return
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// will draw an empty circle with one colour
void circle( unsigned char* cx, unsigned char* cy, unsigned char* radius, unsigned short* colour )
{

	// local variables
	signed char f = 1- *radius;
	signed char ddF_x = 1;
	signed char ddF_y = (-2)*(*radius);
	signed char x = 0;
	signed char y = *radius;

	// draw first 8 points
	_circle_draw8points( cx, cy, &x, &y, colour );
 
	// Bresenham Algorithm
	while ( x < y )
	{
		if( f >= 0 )
		{
			y--;
			ddF_y += 2;
			f += ddF_y;
 		}

		x++;
		ddF_x += 2;
		f += ddF_x;
		_circle_draw8points( cx, cy, &x, &y, colour );
	}

	// return
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// will draw a filled circle with one colour
void fillCircle( unsigned char* x, unsigned char* y, unsigned char* radius, unsigned short* colour )
{
	
	// draw circles with decreasing radius
	unsigned char x1, i1;
	for( unsigned char i = *radius; i != 0; i-- )
	{
		circle( x, y, &i, colour );
		x1 = *x+1;
		i1 = i-1;
		circle( &x1, y, &i1, colour );
	}

	// return
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// will draw a filled circle and blend between an outer and inner colour
void blendColourFillCircle( unsigned char* x, unsigned char* y, unsigned char* radius, unsigned short* insideColour, unsigned short* outsideColour )
{

	// draw circles with decreasing radius
	unsigned char x1, i1;
	unsigned short finalColour;
	for( unsigned char i = *radius; i != 0; i-- )
	{
		finalColour = blendColours( *insideColour, *outsideColour, 0, *radius, i, *radius );
		circle( x, y, &i, &finalColour );
		x1 = *x+1;
		i1 = i-1;
		circle( &x1, y, &i1, &finalColour );
	}

	// dot in centre
	dot( x, y, insideColour );

	// fix two dots (observant fix)
	finalColour = blendColours( *insideColour, *outsideColour, 0, *radius, 2, *radius );
	x1 = *x-1;
	i1 = *y-1; if( isOffScreen( x1, i1 ) ){}else{ dot( &x1, &i1, &finalColour ); }
	i1 = *y+1; if( isOffScreen( x1, i1 ) ){}else{ dot( &x1, &i1, &finalColour ); }

	// return
	return;
}

void _circle_draw8points( unsigned char* cx, unsigned char* cy, signed char* x, signed char* y, unsigned short* colour )
{
	unsigned char nx, ny;
	nx = *cx+*x; ny = *cy+*y; if( isOffScreen( nx, ny ) ){} else { dot( &nx, &ny, colour ); }
	nx = *cx-*x; ny = *cy+*y; if( isOffScreen( nx, ny ) ){} else { dot( &nx, &ny, colour ); }
	nx = *cx+*x; ny = *cy-*y; if( isOffScreen( nx, ny ) ){} else { dot( &nx, &ny, colour ); }
	nx = *cx-*x; ny = *cy-*y; if( isOffScreen( nx, ny ) ){} else { dot( &nx, &ny, colour ); }
	nx = *cx+*y; ny = *cy+*x; if( isOffScreen( nx, ny ) ){} else { dot( &nx, &ny, colour ); }
	nx = *cx-*y; ny = *cy+*x; if( isOffScreen( nx, ny ) ){} else { dot( &nx, &ny, colour ); }
	nx = *cx+*y; ny = *cy-*x; if( isOffScreen( nx, ny ) ){} else { dot( &nx, &ny, colour ); }
	nx = *cx-*y; ny = *cy-*x; if( isOffScreen( nx, ny ) ){} else { dot( &nx, &ny, colour ); }
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// will clear the screen
void cls( void )
{
	unsigned char x, y, i;
	for( x = 0; x != 8; x++ )
	{
		for( y = 0; y != 16; y++ )
		{
			for( i = 0; i != PWM_RESOLUTION; i++ ) pixelArray[x][y][i] = 0x00;
		}
	}
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// Will set a pixel to the specified colour
void dot( unsigned char* x, unsigned char* y, unsigned short* rgb )
{	

	// shift bit is set to one if x coordinate > 7 so the pixel is rendered to the other half of the display
	unsigned char shift = (*x & 0x08) || 0;
	unsigned char x_copy = *x & 0x07;

	// get x coordinate
	unsigned char x_inverse = 7-x_copy;

	// used to extract the data
	unsigned char i, colour, pwm;

	// disable interrupts during array manipulation
	__bic_SR_register( GIE );

	// clears only the pixels we're manipulating in the array
	// only need to do this with BLEND_MODE_REPLACE
	if( drawUtils.blendMode == BLEND_MODE_REPLACE )
	{
		unsigned char clr1 = 0xF5>>shift, clr2 = 0xDF>>shift;
		for( i = 0; i != PWM_RESOLUTION; i++ )
		{
			pixelArray[x_copy][*y][i] &= clr1;
			pixelArray[x_inverse][*y][i] &= clr2;
		}
	}

	// extracts blue
	pwm = (*rgb) & 0x000F;
	colour = 0x02 >> shift;
	if( drawUtils.blendMode != BLEND_MODE_REPLACE ) _process_blend_mode( &x_copy, y, &drawUtils.blendMode, &colour, &pwm );
	for( i = 0; i != pwm; i++ )
	{
		pixelArray[x_copy][*y][i] |= colour;
	}

	// extracts green
	unsigned char c_rgb = (*rgb)>>4;
	pwm = c_rgb & 0x000F;
	colour = 0x20 >> shift;
	if( drawUtils.blendMode != BLEND_MODE_REPLACE ) _process_blend_mode( &x_inverse, y, &drawUtils.blendMode, &colour, &pwm );
	for( i = 0; i != pwm; i++ )
	{
		pixelArray[x_inverse][*y][i] |= colour;
	}

	// extracts red
	pwm = c_rgb >> 4;
	colour = 0x08 >> shift;
	if( drawUtils.blendMode != BLEND_MODE_REPLACE ) _process_blend_mode( &x_copy, y, &drawUtils.blendMode, &colour, &pwm );
	for( i = 0; i != pwm; i++ )
	{
		pixelArray[x_copy][*y][i] |= colour;
	}

	// enable interrupts
	__bis_SR_register( GIE );

	// return
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// processes the colour according to the set blend mode
void _process_blend_mode( unsigned char* x, unsigned char* y, unsigned char* blendMode, unsigned char* colour, unsigned char* pwm )
{

	// get current pwm value in array
	unsigned char current_pwm, c;
	for( current_pwm = 0; current_pwm != PWM_RESOLUTION; current_pwm++ )
	{
		c = (pixelArray[*x][*y][current_pwm] & (*colour));
		if( c == 0 ) break;
	}

	// check which blend mode we're using
	switch( *blendMode )
	{
		// addition
		case BLEND_MODE_ADD :
			*pwm += current_pwm;
			if( *pwm > PWM_RESOLUTION ) *pwm = PWM_RESOLUTION;
			break;

		// subtraction
		case BLEND_MODE_SUBTRACT :
			*pwm -= current_pwm;
			if( *pwm > PWM_RESOLUTION ) *pwm = PWM_RESOLUTION;
			break;

		// multiplication
		case BLEND_MODE_MULTIPLY :
			*pwm *= current_pwm;
			if( *pwm > PWM_RESOLUTION ) *pwm = PWM_RESOLUTION;
			break;

		default:break;
	}

	// return
	return;
}