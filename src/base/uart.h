// ----------------------------------------------------------------------
// Serial communication
// ----------------------------------------------------------------------

#ifndef _UART_H_
	#define _UART_H_

// definitions
#define UART_BUFFER_SIZE 1024

// structs
struct UART_t
{
	unsigned short bufferReadPtr;
	unsigned short bufferWritePtr;
	unsigned char buffer[ UART_BUFFER_SIZE ];
	unsigned char isSending;
};
extern struct UART_t UART;

// blend modes
enum blendMode_e
{
	BLEND_MODE_REPLACE,
	BLEND_MODE_ADD,
	BLEND_MODE_SUBTRACT,
	BLEND_MODE_MULTIPLY
};

// command list
enum commandList_e
{
	CMD_CLS,
	CMD_DOT,
	CMD_BLEND_COLOUR_BOX,
	CMD_BLEND_COLOUR_FILL_BOX,
	CMD_BOX,
	CMD_FILL_BOX,
	CMD_BLEND_COLOUR_LINE,
	CMD_LINE,
	CMD_CIRCLE,
	CMD_FILL_CIRCLE,
	CMD_BLEND_COLOUR_FILL_CIRCLE,

	CMD_SET_BLEND_MODE__REPLACE,
	CMD_SET_BLEND_MODE__ADD,
	CMD_SET_BLEND_MODE__SUBTRACT,
	CMD_SET_BLEND_MODE__MULTIPLY
};

// function prototypes
unsigned char _buffer_overflow( void );
void _increase_buffer_pointer( unsigned short* ptr );
void _write_to_buffer( unsigned char* data );
void send( void );
void cls( void );
void dot( unsigned char* x, unsigned char* y, unsigned short* rgb );
void blendColourBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* topLeftColour, unsigned short* bottomLeftColour, unsigned short* topRightColour, unsigned short* bottomRightColour );
void blendColourFillBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* topLeftColour, unsigned short* bottomLeftColour, unsigned short* topRightColour, unsigned short* bottomRightColour );
void box( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour );
void fillBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour );
void blendColourLine( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour1, unsigned short* colour2 );
void line( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour );
void circle( unsigned char* x, unsigned char* y, unsigned char* radius, unsigned short* colour );
void fillCircle( unsigned char* x, unsigned char* y, unsigned char* radius, unsigned short* colour );
void blendColourFillCircle( unsigned char* x, unsigned char* y, unsigned char* radius, unsigned short* insideColour, unsigned short* outsideColour );
void setBlendMode( unsigned char blendMode );

#endif // _UART_H_