// ----------------------------------------------------------------------
// Drawing Utilities
// ----------------------------------------------------------------------

#ifndef _DRAWUTILS_H_
	#define _DRAWUTILS_H_

// blend modes
enum blendMode_e
{
	BLEND_MODE_REPLACE,
	BLEND_MODE_ADD,
	BLEND_MODE_SUBTRACT,
	BLEND_MODE_MULTIPLY
};

// struct
struct drawUtils_t
{
	unsigned char blendMode;
};
extern struct drawUtils_t drawUtils;

// function prototypes
void cls( void );
void dot( unsigned char* x, unsigned char* y, unsigned short* rgb );
void blendColourBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* topLeftColour, unsigned short* bottomLeftColour, unsigned short* topRightColour, unsigned short* bottomRightColour );
void blendColourFillBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* topLeftColour, unsigned short* bottomLeftColour, unsigned short* topRightColour, unsigned short* bottomRightColour );
void box( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour );
void fillBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour );
void blendColourLine( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour1, unsigned short* colour2 );
void line( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour );
void _circle_draw8points( unsigned char* cx, unsigned char* cy, signed char* x, signed char* y, unsigned short* colour );
void circle( unsigned char* x, unsigned char* y, unsigned char* radius, unsigned short* colour );
void fillCircle( unsigned char* x, unsigned char* y, unsigned char* radius, unsigned short* colour );
void blendColourFillCircle( unsigned char* x, unsigned char* y, unsigned char* radius, unsigned short* insideColour, unsigned short* outsideColour );
void drawUtils_SetBlendMode( unsigned char blendMode );
extern inline void _process_blend_mode( volatile unsigned char* pixelArray, unsigned char* blendMode, unsigned char* colour, unsigned char* pwm );

#endif // _DRAWUTILS_H_