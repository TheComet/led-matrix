// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#ifndef _MAIN_H_
	#define _MAIN_H_

#include "msp430f5172.h"
#include "init.h"
#include "render.h"
#include "drawUtils.h"
#include "uart.h"

// ----------------------------------------------------------------------
// definitions
// ----------------------------------------------------------------------

// PWM resolution
#define PWM_RESOLUTION	0x0E

// how many cycles to delay between shift register writes (is multiplied by inverse square root lookup table)
#define PWM_DELAY_CYCLES 16

// enable and disable rows
#define ENABLE_ROW	0x00
#define DISABLE_ROW	0x10

// shift register control
#define SHIFT_REGISTER_WRITE	0x40
#define SHIFT_REGISTER_OUTPUT	0x80

// ----------------------------------------------------------------------
// global variables
// ----------------------------------------------------------------------

// holds the data of the matrix
//
// The array is structured in a way so only one byte has to be written to the port every cycle in order to save time.
// The PWM data is precalculated in the ISR and is stored in the array as well.
// There are physically multiple entire frames of the matrix stored in memory. The number of frames is specified by PWM_RESOLUTION.
// Even though a lot of memory is being used, it's being exchanged for speed.
//
// bit0 - bit5 hold the next bits to be shifted into the shift registers
// bit6 - bit7 must be 0 to ensure SHCP and STCP can trigger the shift registers to write
// array is structured as [Column][Row][PWM vector], where [Column] holds a total of two pixels
extern volatile unsigned char pixelArray[8][16][PWM_RESOLUTION];

// variable for enabling and disabling rows.
// Should be set to either ENABLE_ROW or DISABLE_ROW
extern unsigned char ROW_EN;

// utility functions
unsigned char absolute( signed char value );
extern inline unsigned short blendColours( unsigned short colour1, unsigned short colour2, unsigned char startPosition, unsigned char endPosition, unsigned char position, unsigned char blendDistance );
extern inline unsigned char isOffScreen( signed char x, signed char y );

#endif // _MAIN_H_