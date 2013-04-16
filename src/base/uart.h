// ----------------------------------------------------------------------
// Serial communication
// ----------------------------------------------------------------------

#ifndef _UART_H_
	#define _UART_H_

// ----------------------------------------------------------------------
// Definitions
// ----------------------------------------------------------------------

#define UART_BUFFER_SIZE 1024

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

// structs
struct UART_t
{
	volatile unsigned short bufferReadPtr;
	volatile unsigned short bufferWritePtr;
	volatile unsigned char buffer[ UART_BUFFER_SIZE ];
	volatile unsigned char isSending;
	volatile unsigned char timeOut;
};

// ----------------------------------------------------------------------
// Enumerators
// ----------------------------------------------------------------------

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

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void initUART( void );
unsigned char _buffer_overflow( void );
void _increase_buffer_pointer( volatile unsigned short* ptr );
void _write_to_buffer( unsigned char* data );
void UARTUpdateTimeOut( void );
void send( void );
void cls( void );
void dot( unsigned char* x, unsigned char* y, const unsigned short* rgb );
void blendColourBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, const unsigned short* topLeftColour, const unsigned short* bottomLeftColour, const unsigned short* topRightColour, const unsigned short* bottomRightColour );
void blendColourFillBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, const unsigned short* topLeftColour, const unsigned short* bottomLeftColour, const unsigned short* topRightColour, const unsigned short* bottomRightColour );
void box( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, const unsigned short* colour );
void fillBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, const unsigned short* colour );
void blendColourLine( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, const unsigned short* colour1, const unsigned short* colour2 );
void line( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, const unsigned short* colour );
void circle( unsigned char* x, unsigned char* y, unsigned char* radius, const unsigned short* colour );
void fillCircle( unsigned char* x, unsigned char* y, unsigned char* radius, const unsigned short* colour );
void blendColourFillCircle( unsigned char* x, unsigned char* y, unsigned char* radius, const unsigned short* insideColour, const unsigned short* outsideColour );
void setBlendMode( unsigned char blendMode );

#endif // _UART_H_
