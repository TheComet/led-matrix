// ----------------------------------------------------------------------
// LED Matrix Base
// ----------------------------------------------------------------------
// Programmed by	: Alex Murray
//			  Marcel Kaltenrieder
// ----------------------------------------------------------------------
// This program interfaces with the LED driver by providing
// utility functions
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
	// Stop watchdog timer to prevent time out reset
	WDTCTL = WDTPW + WDTHOLD;

	return 0;
}
