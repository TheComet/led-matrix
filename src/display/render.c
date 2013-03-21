// ----------------------------------------------------------------------
// Handels rendering of LEDs
// ----------------------------------------------------------------------

// include files
#include "common.h"
#include "render.h"

// ------------------------------------------------------------------------------------------------------------------
// refreshes the entire screen
void refreshScreen( void )
{

	// local variables
	unsigned char x, y, currentPWMCycle;
	
	for( y = 0; y != 16; y++ )
	{

		// output current row
		P2OUT = y | ROW_EN;

		// pwm control
		for( currentPWMCycle = 0; currentPWMCycle != PWM_RESOLUTION; currentPWMCycle++ )
		{

			// delay as to make LED brightness to PWM ratio linear
			// Lookup table: 255, 180, 128, 90, 64, 45, 32, 23, 16, 12, 8, 6, 4, 3, 2, 1,0
			switch( currentPWMCycle )
			{
				case 1 : __delay_cycles( PWM_DELAY_CYCLES*0 ); break;
				case 2 : __delay_cycles( PWM_DELAY_CYCLES*1 ); break;
				case 3 : __delay_cycles( PWM_DELAY_CYCLES*2 ); break;
				case 4 : __delay_cycles( PWM_DELAY_CYCLES*3 ); break;
				case 5 : __delay_cycles( PWM_DELAY_CYCLES*4 ); break;
				case 6 : __delay_cycles( PWM_DELAY_CYCLES*6 ); break;
				case 7 : __delay_cycles( PWM_DELAY_CYCLES*8 ); break;
				case 8 : __delay_cycles( PWM_DELAY_CYCLES*12 ); break;
				case 9 : __delay_cycles( PWM_DELAY_CYCLES*16 ); break;
				case 10 : __delay_cycles( PWM_DELAY_CYCLES*23 ); break;
				case 11 : __delay_cycles( PWM_DELAY_CYCLES*32 ); break;
				case 12 : __delay_cycles( PWM_DELAY_CYCLES*45 ); break;
				case 13 : __delay_cycles( PWM_DELAY_CYCLES*64 ); break;
				case 14 : __delay_cycles( PWM_DELAY_CYCLES*90 ); break;
				default: break;
			}

			// output serial data
			for( x = 0; x != 8; x++ )
			{

				// NOTE: my god, I hope the shift registers can handle this. It's torture I say, TORTURE
				// NOTE2: On second thought, they *were* a pain to solder. I guess it's only fair to punish them

				// output next 6 bits
				P3OUT = pixelArray[x][y][currentPWMCycle];

				// write them to shift registers
				P3OUT |= SHIFT_REGISTER_WRITE;
			}

			// output shift registers to LEDs
			P3OUT |= SHIFT_REGISTER_OUTPUT;

		}

		// reset shift registers to 0
		// stops signals from overlapping and causing half-on LEDs
		for( x = 0; x != 8; x++ )
		{
			P3OUT = 0x00;
			P3OUT |= SHIFT_REGISTER_WRITE;
		}
		P3OUT |= SHIFT_REGISTER_OUTPUT;

	}

	// return
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// for refreshing the display
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR( void )
{
	refreshScreen();
}
