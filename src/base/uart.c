// ----------------------------------------------------------------------
// Serial communication
// ----------------------------------------------------------------------

// header files
#include "uart.h"
#include "common.h"

// structs
struct UART_t UART;

// ----------------------------------------------------------------------
// returns 1 on buffer overflow
unsigned char _buffer_overflow( void )
{

	// cache next write pointer, because we're using it more than once
	unsigned short ptr = UART.bufferWritePtr+1;

	// check if we're exceeding buffer size
	if( ptr != UART_BUFFER_SIZE )
	{

		// if write pointer is equal to read pointer, buffer has overflown
		if( ptr == UART.bufferReadPtr ) return 1;
	}else{

		// if after wrapping the pointer back to 0 it equals the read pointer, buffer has overflown
		if( UART.bufferReadPtr ){}else{ return 1; }
	}
	return 0;
}

// ----------------------------------------------------------------------
// increases a buffer pointer by 1 and wraps
void _increase_buffer_pointer( unsigned short* ptr )
{
	(*ptr)++;
	if( (*ptr) == UART_BUFFER_SIZE )
		(*ptr) = 0;
}

// ----------------------------------------------------------------------
// writes data into the buffer
void _write_to_buffer( unsigned char* data )
{
	
	// overflow? force sending
	while( _buffer_overflow() == 1 )
	{
		if( UART.isSending == 0 ) send();
	}

	// write
	UART.buffer[ UART.bufferWritePtr ] = *data;

	// increase pointer
	_increase_buffer_pointer( &UART.bufferWritePtr );

	// return
	return;
}

// ----------------------------------------------------------------------
// initiates sending the buffer - this causes a chain reaction
// which lasts until the buffer is empty
void send( void )
{
	if( UART.isSending == 0 )
	{
		UART.isSending = 1;
		UCA1TXBUF = UART.buffer[ UART.bufferReadPtr ];
	}
}

// ----------------------------------------------------------------------
// clear screen
void cls( void )
{
	
	// write clear command
	unsigned char n;
	n = CMD_CLS;            _write_to_buffer( &n );

	// return
	return;

}

// ----------------------------------------------------------------------
// dot
void dot( unsigned char* x, unsigned char* y, unsigned short* rgb )
{

	// write dot command to buffer
	unsigned char n;
	n = CMD_DOT;          _write_to_buffer( &n );
	n = (((*x)<<4)|(*y)); _write_to_buffer( &n );
	n = ((*rgb)>>4);      _write_to_buffer( &n );
	n = ((*rgb)<<4);      _write_to_buffer( &n );

	// return
	return;
}

// ----------------------------------------------------------------------
// blend colour box
void blendColourBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* topLeftColour, unsigned short* bottomLeftColour, unsigned short* topRightColour, unsigned short* bottomRightColour )
{

	// write command to buffer
	unsigned char n;
	n = CMD_BLEND_COLOUR_BOX;                                _write_to_buffer( &n );
	n = (((*x1)<<4)|(*y1));                                  _write_to_buffer( &n );
	n = (((*x2)<<4)|(*y2));                                  _write_to_buffer( &n );
	n = ((*topLeftColour)>>4);                               _write_to_buffer( &n );
	n = ((*topLeftColour)<<4)|((*bottomLeftColour)>>8);      _write_to_buffer( &n );
	n = (*bottomLeftColour);                                 _write_to_buffer( &n );
	n = ((*topRightColour)>>4);                              _write_to_buffer( &n );
	n = ((*topRightColour)<<4)|((*bottomRightColour)>>8);    _write_to_buffer( &n );
	n = (*bottomRightColour);                                _write_to_buffer( &n );

	// return
	return;
}

// ----------------------------------------------------------------------
// blend colour fill box
void blendColourFillBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* topLeftColour, unsigned short* bottomLeftColour, unsigned short* topRightColour, unsigned short* bottomRightColour )
{

	// write command to buffer
	unsigned char n;
	n = CMD_BLEND_COLOUR_FILL_BOX;                           _write_to_buffer( &n );
	n = (((*x1)<<4)|(*y1));                                  _write_to_buffer( &n );
	n = (((*x2)<<4)|(*y2));                                  _write_to_buffer( &n );
	n = ((*topLeftColour)>>4);                               _write_to_buffer( &n );
	n = ((*topLeftColour)<<4)|((*bottomLeftColour)>>8);      _write_to_buffer( &n );
	n = (*bottomLeftColour);                                 _write_to_buffer( &n );
	n = ((*topRightColour)>>4);                              _write_to_buffer( &n );
	n = ((*topRightColour)<<4)|((*bottomRightColour)>>8);    _write_to_buffer( &n );
	n = (*bottomRightColour);                                _write_to_buffer( &n );

	// return
	return;
}

// ----------------------------------------------------------------------
// box
void box( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour )
{

	// write command to buffer
	unsigned char n;
	n = CMD_BOX;                                             _write_to_buffer( &n );
	n = (((*x1)<<4)|(*y1));                                  _write_to_buffer( &n );
	n = (((*x2)<<4)|(*y2));                                  _write_to_buffer( &n );
	n = ((*colour)>>4);                                      _write_to_buffer( &n );
	n = ((*colour)<<4);                                      _write_to_buffer( &n );

	// return
	return;
}

// ----------------------------------------------------------------------
// fill box
void fillBox( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour )
{

	// write command to buffer
	unsigned char n;
	n = CMD_FILL_BOX;                                        _write_to_buffer( &n );
	n = (((*x1)<<4)|(*y1));                                  _write_to_buffer( &n );
	n = (((*x2)<<4)|(*y2));                                  _write_to_buffer( &n );
	n = ((*colour)>>4);                                      _write_to_buffer( &n );
	n = ((*colour)<<4);                                      _write_to_buffer( &n );

	// return
	return;
}

// ----------------------------------------------------------------------
// blend colour line
void blendColourLine( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour1, unsigned short* colour2 )
{

	// write command to buffer
	unsigned char n;
	n = CMD_BLEND_COLOUR_LINE;                               _write_to_buffer( &n );
	n = (((*x1)<<4)|(*y1));                                  _write_to_buffer( &n );
	n = (((*x2)<<4)|(*y2));                                  _write_to_buffer( &n );
	n = ((*colour1)>>4);                                     _write_to_buffer( &n );
	n = (((*colour1)<<4)|((*colour2)>>8));                   _write_to_buffer( &n );
	n = (*colour2);                                          _write_to_buffer( &n );

	// return
	return;
}

// ----------------------------------------------------------------------
// line
void line( unsigned char* x1, unsigned char* y1, unsigned char* x2, unsigned char* y2, unsigned short* colour )
{

	// write command to buffer
	unsigned char n;
	n = CMD_LINE;                                            _write_to_buffer( &n );
	n = (((*x1)<<4)|(*y1));                                  _write_to_buffer( &n );
	n = (((*x2)<<4)|(*y2));                                  _write_to_buffer( &n );
	n = ((*colour)>>4);                                      _write_to_buffer( &n );
	n = ((*colour)<<4);                                      _write_to_buffer( &n );

	// return
	return;
}

// ----------------------------------------------------------------------
// circle
void circle( unsigned char* x, unsigned char* y, unsigned char* radius, unsigned short* colour )
{

	// write command to buffer
	unsigned char n;
	n = CMD_CIRCLE;                                          _write_to_buffer( &n );
	n = (((*x)<<4)|(*y));                                    _write_to_buffer( &n );
	n = (*radius);                                           _write_to_buffer( &n );
	n = ((*colour)>>4);                                      _write_to_buffer( &n );
	n = ((*colour)<<4);                                      _write_to_buffer( &n );

	// return
	return;
}

// ----------------------------------------------------------------------
// fill circle
void fillCircle( unsigned char* x, unsigned char* y, unsigned char* radius, unsigned short* colour )
{

	// write command to buffer
	unsigned char n;
	n = CMD_FILL_CIRCLE;                                     _write_to_buffer( &n );
	n = (((*x)<<4)|(*y));                                    _write_to_buffer( &n );
	n = (*radius);                                           _write_to_buffer( &n );
	n = ((*colour)>>4);                                      _write_to_buffer( &n );
	n = ((*colour)<<4);                                      _write_to_buffer( &n );

	// return
	return;
}

// ----------------------------------------------------------------------
// blend colour fill circle
void blendColourFillCircle( unsigned char* x, unsigned char* y, unsigned char* radius, unsigned short* insideColour, unsigned short* outsideColour )
{

	// write command to buffer
	unsigned char n;
	n = CMD_BLEND_COLOUR_FILL_CIRCLE;                        _write_to_buffer( &n );
	n = (((*x)<<4)|(*y));                                    _write_to_buffer( &n );
	n = (*radius);                                           _write_to_buffer( &n );
	n = ((*insideColour)>>4);                                _write_to_buffer( &n );
	n = (((*insideColour)<<4)|((*outsideColour)>>8));        _write_to_buffer( &n );
	n = (*outsideColour);                                    _write_to_buffer( &n );

	// return
	return;
}

// ----------------------------------------------------------------------
// set blend mode
void setBlendMode( unsigned char blendMode )
{

	// determine blend mode
	unsigned char n;
	switch( blendMode )
	{
		case BLEND_MODE_REPLACE  : n = CMD_SET_BLEND_MODE__REPLACE;  break;
		case BLEND_MODE_ADD      : n = CMD_SET_BLEND_MODE__ADD;      break;
		case BLEND_MODE_SUBTRACT : n = CMD_SET_BLEND_MODE__SUBTRACT; break;
		case BLEND_MODE_MULTIPLY : n = CMD_SET_BLEND_MODE__MULTIPLY; break;
		default: return;
	}

	// write command to buffer
	write_to_buffer( &n );

	// return
	return;
}
	

// ----------------------------------------------------------------------
// RxD interrupt
#pragma vector=USCIAB1RX_VECTOR
__interrupt void USCI1RX_ISR(void)
{

	// reset timeout
	TAR = 0x00;
	
	// increase and wrap pointer if it's the same as sent data
	if( UART.buffer[ UART.bufferReadPtr ] == UCA1RXBUF )
	{
		_increase_buffer_pointer( &UART.bufferReadPtr );
	}

	// send next block of data, if any
	if( UART.bufferReadPtr != UART.bufferWritePtr )
	{

		// send data
		UCA1TXBUF = UART.buffer[ UART.bufferReadPtr ];

	// buffer is empty
	}else{
		UART.isSending = 0;
	}
}