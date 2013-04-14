// ----------------------------------------------------------------------
// UART handling
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// NOTES
// received data is always sent back when the command was processed
// ----------------------------------------------------------------------

// include files
#include "common.h"
#include "uart.h"
#include "drawUtils.h"

// global variables
struct UART_t UART;

// ------------------------------------------------------------------------------------------------------------------
// process received data
unsigned char processCommand( void )
{

	// extract command to execute, if ready
	if( UART.commandStateGroup == CMD_STATE_NOP )
	{
		if( UCA0RXBUF == CMD_CLS ){
			cls();
			return 1;
		}
		if( UCA0RXBUF == CMD_DOT ){
			UART.commandState = CMD_STATE_DOT__POSITION;
			UART.commandStateGroup = CMD_DOT;
			return 1;
		}
		if( UCA0RXBUF ==  CMD_BLEND_COLOUR_BOX ){
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__POSITION_A;
			UART.commandStateGroup = CMD_BLEND_COLOUR_BOX;
			return 1;
		}
		if( UCA0RXBUF ==  CMD_BLEND_COLOUR_FILL_BOX ){
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_A;
			UART.commandStateGroup = CMD_BLEND_COLOUR_FILL_BOX;
			return 1;
		}
		if( UCA0RXBUF ==  CMD_BOX){
			UART.commandState = CMD_STATE_BOX__POSITION_A;
			UART.commandStateGroup = CMD_BOX;
			return 1;
		}
		if( UCA0RXBUF == CMD_FILL_BOX){
			UART.commandState = CMD_STATE_FILL_BOX__POSITION_A;
			UART.commandStateGroup = CMD_FILL_BOX;
			return 1;
		}
		if( UCA0RXBUF == CMD_BLEND_COLOUR_LINE ){
			UART.commandState = CMD_STATE_BLEND_COLOUR_LINE__POSITION_A;
			UART.commandStateGroup = CMD_BLEND_COLOUR_LINE;
			return 1;
		}
		if( UCA0RXBUF == CMD_LINE ){
			UART.commandState = CMD_STATE_LINE__POSITION_A;
			UART.commandStateGroup = CMD_LINE;
			return 1;
		}
		if( UCA0RXBUF == CMD_CIRCLE ){
			UART.commandState = CMD_STATE_CIRCLE__POSITION;
			UART.commandStateGroup = CMD_CIRCLE;
			return 1;
		}
		if( UCA0RXBUF == CMD_FILL_CIRCLE ){
			UART.commandState = CMD_STATE_FILL_CIRCLE__POSITION;
			UART.commandStateGroup = CMD_FILL_CIRCLE;
			return 1;
		}
		if( UCA0RXBUF == CMD_BLEND_COLOUR_FILL_CIRCLE ){
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__POSITION;
			UART.commandStateGroup = CMD_BLEND_COLOUR_FILL_CIRCLE;
			return 1;
		}
		if( UCA0RXBUF == CMD_SET_BLEND_MODE__REPLACE ){
			drawUtils_SetBlendMode( BLEND_MODE_REPLACE );
			return 1;
		}
		if( UCA0RXBUF == CMD_SET_BLEND_MODE__ADD ){
			drawUtils_SetBlendMode( BLEND_MODE_ADD );
			return 1;
		}
		if( UCA0RXBUF == CMD_SET_BLEND_MODE__SUBTRACT ){
			drawUtils_SetBlendMode( BLEND_MODE_SUBTRACT );
			return 1;
		}
		if( UCA0RXBUF == CMD_SET_BLEND_MODE__MULTIPLY ){
			drawUtils_SetBlendMode( BLEND_MODE_MULTIPLY );
			return 1;
		}
	
		// transmission or synchronization failure, reset
		return error();
	}

	// ------------------------------------------------------------------------------------------
	// dot
	if( UART.commandStateGroup == CMD_DOT )
	{
		if( UART.commandState == CMD_STATE_DOT__POSITION ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_DOT__COLOUR_MSB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_DOT__COLOUR_MSB ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			if( (UART.cA&0xF00) > 0xE00 ) return error(); // error check
			UART.commandState = CMD_STATE_DOT__COLOUR_LSB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_DOT__COLOUR_LSB ){
			UART.cA |= (UCA0RXBUF>>4);
			if( (UART.cA&0x0F0)>0x0E0 || (UART.cA&0x00F)>0x00E ) return error(); // error check
			dot( &UART.x1, &UART.y1, &UART.cA );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return 1;
		}

		// transmission or synchronization failure, reset
		return error();
	}

	// ------------------------------------------------------------------------------------------
	// blend colour box
	if( UART.commandStateGroup == CMD_BLEND_COLOUR_BOX )
	{
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__POSITION_A ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__POSITION_B;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__POSITION_B ){
			UART.x2 = UCA0RXBUF>>4;
			UART.y2 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_A;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__COLOUR_A ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			if( (UART.cA&0xF00)>0xE00 || (UART.cA&0x0F0)>0x0E0 ) return error(); // error check
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_AB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__COLOUR_AB ){
			UART.cA |= UCA0RXBUF>>4;
			if( (UART.cA&0x00F)>0x00E ) return error(); // error check
			UART.cB = UCA0RXBUF&0x0F;
			UART.cB <<= 8;
			if( (UART.cB&0xF00)>0xE00 ) return error(); // error check
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_B;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__COLOUR_B ){
			UART.cB |= UCA0RXBUF;
			if( (UART.cB&0x0F0)>0x0E0 || (UART.cB&0x00F)>0x00E ) return error(); // error check
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_C;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__COLOUR_C ){
			UART.cC = UCA0RXBUF;
			UART.cC <<= 4;
			if( (UART.cC&0xF00)>0xE00 || (UART.cC&0x0F0)>0x0E0 ) return error(); // error check
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_CD;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__COLOUR_CD ){
			UART.cC |= UCA0RXBUF>>4;
			if( (UART.cC&0x00F)>0x00E ) return error(); // error check
			UART.cD = UCA0RXBUF&0x0F;
			UART.cD <<= 8;
			if( (UART.cD&0xF00)>0xE00 ) return error(); // error check
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_D;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__COLOUR_D ){
			UART.cD |= UCA0RXBUF;
			if( (UART.cD&0x0F0)>0x0E0 || (UART.cD&0x00F)>0x00E ) return error(); // error check
			blendColourBox( &UART.x1, &UART.y1, &UART.x2, &UART.y2, &UART.cA, &UART.cB, &UART.cC, &UART.cD );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return 1;
		}

		// transmission or synchronization failure, reset
		return error();
	}

	// ------------------------------------------------------------------------------------------
	// blend colour fill box
	if( UART.commandStateGroup == CMD_BLEND_COLOUR_FILL_BOX )
	{
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_A ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_B;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_B ){
			UART.x2 = UCA0RXBUF>>4;
			UART.y2 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_A;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_A ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_AB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_AB ){
			UART.cA |= UCA0RXBUF>>4;
			UART.cB = UCA0RXBUF&0x0F;
			UART.cB <<= 8;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_B;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_B ){
			UART.cB |= UCA0RXBUF;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_C;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_C ){
			UART.cC = UCA0RXBUF;
			UART.cC <<= 4;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_CD;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_CD ){
			UART.cC |= UCA0RXBUF>>4;
			UART.cD = UCA0RXBUF&0x0F;
			UART.cD <<= 8;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_D;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_D ){
			UART.cD |= UCA0RXBUF;
			blendColourFillBox( &UART.x1, &UART.y1, &UART.x2, &UART.y2, &UART.cA, &UART.cB, &UART.cC, &UART.cD );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return 1;
		}

		// transmission or synchronization failure, reset
		return error();
	}

	// ------------------------------------------------------------------------------------------
	// box
	if( UART.commandStateGroup == CMD_BOX )
	{
		if( UART.commandState == CMD_STATE_BOX__POSITION_A ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BOX__POSITION_B;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BOX__POSITION_B ){
			UART.x2 = UCA0RXBUF>>4;
			UART.y2 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BOX__COLOUR_MSB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BOX__COLOUR_MSB ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_BOX__COLOUR_LSB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BOX__COLOUR_LSB ){
			UART.cA |= UCA0RXBUF>>4;
			box( &UART.x1, &UART.y1, &UART.x2, &UART.y2, &UART.cA );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return 1;
		}

		// transmission or synchronization failure, reset
		return error();
	}

	// ------------------------------------------------------------------------------------------
	// fill box
	if( UART.commandStateGroup == CMD_FILL_BOX )
	{
		if( UART.commandState == CMD_STATE_FILL_BOX__POSITION_A ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_FILL_BOX__POSITION_B;
			return 1;
		}
		if( UART.commandState == CMD_STATE_FILL_BOX__POSITION_B ){
			UART.x2 = UCA0RXBUF>>4;
			UART.y2 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_FILL_BOX__COLOUR_MSB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_FILL_BOX__COLOUR_MSB ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_FILL_BOX__COLOUR_LSB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_FILL_BOX__COLOUR_LSB ){
			UART.cA |= UCA0RXBUF>>4;
			fillBox( &UART.x1, &UART.y1, &UART.x2, &UART.y2, &UART.cA );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return 1;
		}

		// transmission or synchronization failure, reset
		return error();
	}

	// ------------------------------------------------------------------------------------------
	// blend colour line
	if( UART.commandStateGroup == CMD_BLEND_COLOUR_LINE )
	{
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_LINE__POSITION_A ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_LINE__POSITION_B;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_LINE__POSITION_B ){
			UART.x2 = UCA0RXBUF>>4;
			UART.y2 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_LINE__COLOUR_A;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_LINE__COLOUR_A ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_BLEND_COLOUR_LINE__COLOUR_AB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_LINE__COLOUR_AB ){
			UART.cA |= UCA0RXBUF>>4;
			UART.cB = UCA0RXBUF&0x0F;
			UART.cB <<= 8;
			UART.commandState = CMD_STATE_BLEND_COLOUR_LINE__COLOUR_B;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_LINE__COLOUR_B ){
			UART.cB |= UCA0RXBUF;
			blendColourLine( &UART.x1, &UART.y1, &UART.x2, &UART.y2, &UART.cA, &UART.cB );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return 1;
		}

		// transmission or synchronization failure, reset
		return error();
	}

	// ------------------------------------------------------------------------------------------
	// line
	if( UART.commandStateGroup == CMD_LINE )
	{
		if( UART.commandState == CMD_STATE_LINE__POSITION_A ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_LINE__POSITION_B;
			return 1;
		}
		if( UART.commandState == CMD_STATE_LINE__POSITION_B ){
			UART.x2 = UCA0RXBUF>>4;
			UART.y2 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_LINE__COLOUR_MSB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_LINE__COLOUR_MSB ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_LINE__COLOUR_LSB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_LINE__COLOUR_LSB ){
			UART.cA |= UCA0RXBUF>>4;
			line( &UART.x1, &UART.y1, &UART.x2, &UART.y2, &UART.cA );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return 1;
		}

		// transmission or synchronization failure, reset
		return error();
	}

	// ------------------------------------------------------------------------------------------
	// circle
	if( UART.commandStateGroup == CMD_CIRCLE )
	{
		if( UART.commandState == CMD_STATE_CIRCLE__POSITION ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_CIRCLE__RADIUS;
			return 1;
		}
		if( UART.commandState == CMD_STATE_CIRCLE__RADIUS ){
			UART.x2 = UCA0RXBUF;
			UART.commandState = CMD_STATE_CIRCLE__COLOUR_MSB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_CIRCLE__COLOUR_MSB ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_CIRCLE__COLOUR_LSB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_CIRCLE__COLOUR_LSB ){
			UART.cA |= UCA0RXBUF>>4;
			circle( &UART.x1, &UART.y1, &UART.x2, &UART.cA );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return 1;
		}

		// transmission or synchronization failure, reset
		return error();
	}

	// ------------------------------------------------------------------------------------------
	// fill circle
	if( UART.commandStateGroup == CMD_FILL_CIRCLE )
	{
		if( UART.commandState == CMD_STATE_FILL_CIRCLE__POSITION ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_FILL_CIRCLE__RADIUS;
			return 1;
		}
		if( UART.commandState == CMD_STATE_FILL_CIRCLE__RADIUS ){
			UART.x2 = UCA0RXBUF;
			UART.commandState = CMD_STATE_FILL_CIRCLE__COLOUR_MSB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_FILL_CIRCLE__COLOUR_MSB ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_FILL_CIRCLE__COLOUR_LSB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_FILL_CIRCLE__COLOUR_LSB ){
			UART.cA |= UCA0RXBUF>>4;
			fillCircle( &UART.x1, &UART.y1, &UART.x2, &UART.cA );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return 1;
		}

		// transmission or synchronization failure, reset
		return error();
	}

	// ------------------------------------------------------------------------------------------
	// blend colour fill circle
	if( UART.commandStateGroup == CMD_BLEND_COLOUR_FILL_CIRCLE )
	{
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__POSITION ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__RADIUS;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__RADIUS ){
			UART.x2 = UCA0RXBUF;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_A;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_A ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_AB;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_AB ){
			UART.cA |= UCA0RXBUF>>4;
			UART.cB = UCA0RXBUF&0x0F;
			UART.cB <<= 8;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_B;
			return 1;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_B ){
			UART.cB |= UCA0RXBUF;
			blendColourFillCircle( &UART.x1, &UART.y1, &UART.x2, &UART.cA, &UART.cB );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return 1;
		}

		// transmission or synchronization failure, reset
		return error();
	}

	// transmission or synchronization failure, reset
	return error();
}

// ------------------------------------------------------------------------------------------------------------------
// error, resets the state machine
unsigned char error( void )
{
	UART.commandState = CMD_STATE_NOP;
	UART.commandStateGroup = CMD_STATE_NOP;
	return 0;
}

// ------------------------------------------------------------------------------------------------------------------
// Interrupt for receiving data
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR( void )
{

	// process received data, if any
	switch(__even_in_range(UCA0IV,4))
	{
		case 0:break;				// Vector 0 - no interrupt
		case 2:					// Vector 2 - RXIFG

			// process data and send it back
			processCommand();
			UCA0TXBUF = UCA0RXBUF;

			break;

		case 4:break;				// Vector 4 - TXIFG
	default: break;
	}
}