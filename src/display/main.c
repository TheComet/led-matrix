// ----------------------------------------------------------------------
// LED Matrix Driver
// ----------------------------------------------------------------------
// Programmed by      : Alex Murray
//                      Marcel Kaltenrieder
// ----------------------------------------------------------------------
// This program will render a 16x16 matrix of RGB LEDs.
//
// This module can communicate via UART with other devices.
// Check the documentation for protocol details.
// ----------------------------------------------------------------------

/*
-------------------------------------------------------------------------
   Pin Layout
-------------------------------------------------------------------------

                                 _______________________
                                |     MSP430F5172       |
                                |                       |
                          ------| P1.0             P3.0 |------ DS_R_0                 OUT/D
                      TxD ------| P1.1             P3.1 |------ DS_R_1                 OUT/D
                      RxD ------| P1.2             P3.2 |------ DS_G_0                 OUT/D
                          ------| P1.3             P3.3 |------ DS_G_1                 OUT/D
                          ------| P1.4             P3.4 |------ DS_B_0                 OUT/D
                          ------| P1.5             P3.5 |------ DS_B_1                 OUT/D
                          ------| P1.6             P3.6 |------ SHCP                   OUT/D
                          ------| P1.7             P3.7 |------ STCP                   OUT/D
                                |                       |
  OUT/D             ROW_0 ------| P2.0                  |
  OUT/D             ROW_1 ------| P2.1                  |
  OUT/D             ROW_2 ------| P2.2                  |
  OUT/D             ROW_3 ------| P2.3                  |
  OUT/D           /ROW_EN ------| P2.4                  |
                          ------| P2.5                  |
                          ------| P2.6                  |
                          ------| P2.7                  |
                                |                       |
                                |_______________________|


-------------------------------------------------------------------------
   Pin description
-------------------------------------------------------------------------

     Pin Name           | Description
   ---------------------+-------------------------------------------------------------
     TxD                | For serial communication
     RxD                |
   ---------------------+-------------------------------------------------------------
     ROW_0              | Selects the active row to write to.
     ROW_1              | These 4 bits are externally demultiplexed to 16 bits.
     ROW_2              |
     ROW_3              |
   ---------------------+-------------------------------------------------------------
     /ROW_EN            | When set to 1, all LEDs are deactivated
   ---------------------+-------------------------------------------------------------
     DS_R_0             | Lower byte for Red colour data to write to shift registers
     DS_R_1             | Higher byte for Red colour data to write to shift registers
     DS_G_0             | Lower byte for Green colour data to write to shift registers
     DS_G_1             | Higher byte for Green colour data to write to shift registers
     DS_B_0             | Lower byte for Blue colour data to write to shift registers
     DS_B_1             | Higher byte for Blue colour data to write to shift registers
   ---------------------+-------------------------------------------------------------
     SHCP               | Serial data is read in on positive edge
     STCP               | Serial data is applied to outputs on positive edge

*/

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

// main header
#include "main.h"

// ----------------------------------------------------------------------
// global variables & arrays
// ----------------------------------------------------------------------

// see main.h for more info
unsigned char volatile pixelArray[8][16][PWM_RESOLUTION];
unsigned char ROW_EN;

// ----------------------------------------------------------------------
// Main entry point
// ----------------------------------------------------------------------

void main( void )
{

	// Initialse device
	initDevice();

	// initialise screen
	cls();
/*
	// annoy zingg
	dot( 0x2F, 0xEEE );
	dot( 0x1E, 0xEEE );
	dot( 0x0D, 0xEEE );
	dot( 0x0C, 0xEEE );
	dot( 0x0B, 0xEEE );
	dot( 0x0A, 0xEEE );
	dot( 0x19, 0xEEE );
	dot( 0x28, 0xEEE );
	dot( 0x38, 0xEEE );
	dot( 0x48, 0xEEE );
	dot( 0x58, 0xEEE );
	dot( 0x69, 0xEEE );
	dot( 0x7A, 0xEEE );
	dot( 0x7B, 0xEEE );
	dot( 0x7C, 0xEEE );
	dot( 0x7D, 0xEEE );
	dot( 0x6E, 0xEEE );
	dot( 0x5F, 0xEEE );
	dot( 0x4F, 0xEEE );
	dot( 0x3F, 0xEEE );

	dot( 0x2A, 0x00E );
	dot( 0x5A, 0x00E );
	dot( 0x2B, 0x00E );
	dot( 0x5B, 0x00E );

	dot( 0x2D, 0x0EE );
	dot( 0x3E, 0x0EE );
	dot( 0x4E, 0x0EE );
	dot( 0x5D, 0x0EE );
*/
        
        unsigned char x1, y1, x2, y2;
        unsigned short colour1, colour2, colour3, colour4;
        x1 = 0; y1 = 0; x2 = 15; y2 = 15;
        colour1 = 0xEE0; colour2 = 0xE00; colour3 = 0x0E0; colour4 = 0x00E;
        blendColourFillBox( &x1, &y1, &x2, &y2, &colour1, &colour2, &colour3, &colour4 );
        x1 = 7; y1 = 7; x2 = 7; colour1 = 0xEEE;
        circle( &x1, &y1, &x2, &colour1 );

	// main loop
	while( 1 )
	{
		if( UART.timeToProcessCommandBuffer == 2 ) processCommandBuffer();
	}
}

// ----------------------------------------------------------------------
// Utility functions and other stuff
// ----------------------------------------------------------------------

// returns the absolute value of a number
unsigned char absolute( signed char value )
{
	if( value & 0x80 ) return 0-value;
	return value;
}

// The question is: Will it Blend?
extern inline unsigned short blendColours( unsigned short colour1, unsigned short colour2, unsigned char startPosition, unsigned char endPosition, unsigned char position, unsigned char blendDistance )
{
	return ((((colour1&0xF00)>>8)*(endPosition-position)/blendDistance + ((colour2&0xF00)>>8)*(position-startPosition)/blendDistance)<<8) | ((((colour1&0x0F0)>>4)*(endPosition-position)/blendDistance + ((colour2&0x0F0)>>4)*(position-startPosition)/blendDistance)<<4) | ((colour1&0x00F)*(endPosition-position)/blendDistance + (colour2&0x00F)*(position-startPosition)/blendDistance);
}

// returns true if the point is out of bounds of the screen
extern inline unsigned char isOffScreen( signed char x, signed char y )
{
	return (x&0xF0 || y&0xF0);
}