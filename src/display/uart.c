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
void processCommand( void )
{

	// extract command to execute, if ready
	if( UART.commandStateGroup == CMD_STATE_NOP )
	{
		if( UCA0RXBUF == CMD_CLS ){
			cls();
			return;
		}
		if( UCA0RXBUF == CMD_DOT ){
			UART.commandState = CMD_STATE_DOT__POSITION;
			UART.commandStateGroup = CMD_DOT;
			return;
		}
		if( UCA0RXBUF ==  CMD_BLEND_COLOUR_BOX ){
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__POSITION_A;
			UART.commandStateGroup = CMD_BLEND_COLOUR_BOX;
			return;
		}
		if( UCA0RXBUF ==  CMD_BLEND_COLOUR_FILL_BOX ){
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_A;
			UART.commandStateGroup = CMD_BLEND_COLOUR_FILL_BOX;
			return;
		}
		if( UCA0RXBUF ==  CMD_BOX){
			UART.commandState = CMD_STATE_BOX__POSITION_A;
			UART.commandStateGroup = CMD_BOX;
			return;
		}
		if( UCA0RXBUF == CMD_FILL_BOX){
			UART.commandState = CMD_STATE_FILL_BOX__POSITION_A;
			UART.commandStateGroup = CMD_FILL_BOX;
			return;
		}
		if( UCA0RXBUF == CMD_BLEND_COLOUR_LINE ){
			UART.commandState = CMD_STATE_BLEND_COLOUR_LINE__POSITION_A;
			UART.commandStateGroup = CMD_BLEND_COLOUR_LINE;
			return;
		}
		if( UCA0RXBUF == CMD_LINE ){
			UART.commandState = CMD_STATE_LINE__POSITION_A;
			UART.commandStateGroup = CMD_LINE;
			return;
		}
		if( UCA0RXBUF == CMD_CIRCLE ){
			UART.commandState = CMD_STATE_CIRCLE__POSITION;
			UART.commandStateGroup = CMD_CIRCLE;
			return;
		}
		if( UCA0RXBUF == CMD_FILL_CIRCLE ){
			UART.commandState = CMD_STATE_FILL_CIRCLE__POSITION;
			UART.commandStateGroup = CMD_FILL_CIRCLE;
			return;
		}
		if( UCA0RXBUF == CMD_BLEND_COLOUR_FILL_CIRCLE ){
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__POSITION;
			UART.commandStateGroup = CMD_BLEND_COLOUR_FILL_CIRCLE;
			return;
		}
		if( UCA0RXBUF == CMD_SET_BLEND_MODE__REPLACE ){
			drawUtils_SetBlendMode( BLEND_MODE_REPLACE );
			return;
		}
		if( UCA0RXBUF == CMD_SET_BLEND_MODE__ADD ){
			drawUtils_SetBlendMode( BLEND_MODE_ADD );
			return;
		}
		if( UCA0RXBUF == CMD_SET_BLEND_MODE__SUBTRACT ){
			drawUtils_SetBlendMode( BLEND_MODE_SUBTRACT );
			return;
		}
		if( UCA0RXBUF == CMD_SET_BLEND_MODE__MULTIPLY ){
			drawUtils_SetBlendMode( BLEND_MODE_MULTIPLY );
			return;
		}
	}

	// ------------------------------------------------------------------------------------------
	// dot
	if( UART.commandStateGroup == CMD_DOT )
	{
		if( UART.commandState == CMD_STATE_DOT__POSITION ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_DOT__COLOUR_MSB;
			return;
		}
		if( UART.commandState == CMD_STATE_DOT__COLOUR_MSB ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_DOT__COLOUR_LSB;
			return;
		}
		if( UART.commandState == CMD_STATE_DOT__COLOUR_LSB ){
			UART.cA |= (UCA0RXBUF>>4);
			dot( &UART.x1, &UART.y1, &UART.cA );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return;
		}

		// transmission or synchronization failure, reset
		UART.commandState = CMD_STATE_NOP;
		UART.commandStateGroup = CMD_STATE_NOP;
	}

	// ------------------------------------------------------------------------------------------
	// blend colour box
	if( UART.commandStateGroup == CMD_BLEND_COLOUR_BOX )
	{
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__POSITION_A ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__POSITION_B;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__POSITION_B ){
			UART.x2 = UCA0RXBUF>>4;
			UART.y2 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_A;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__COLOUR_A ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_AB;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__COLOUR_AB ){
			UART.cA |= UCA0RXBUF>>4;
			UART.cB = UCA0RXBUF&0x0F;
			UART.cB <<= 8;
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_B;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__COLOUR_B ){
			UART.cB |= UCA0RXBUF;
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_C;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__COLOUR_C ){
			UART.cC = UCA0RXBUF;
			UART.cC <<= 4;
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_CD;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__COLOUR_CD ){
			UART.cC |= UCA0RXBUF>>4;
			UART.cD = UCA0RXBUF&0x0F;
			UART.cD <<= 8;
			UART.commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_D;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_BOX__COLOUR_D ){
			UART.cD |= UCA0RXBUF;
			blendColourBox( &UART.x1, &UART.y1, &UART.x2, &UART.y2, &UART.cA, &UART.cB, &UART.cC, &UART.cD );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return;
		}

		// transmission or synchronization failure, reset
		UART.commandState = CMD_STATE_NOP;
		UART.commandStateGroup = CMD_STATE_NOP;
	}

	

	// ------------------------------------------------------------------------------------------
	// blend colour fill box
	if( UART.commandStateGroup == CMD_BLEND_COLOUR_FILL_BOX )
	{
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_A ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_B;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_B ){
			UART.x2 = UCA0RXBUF>>4;
			UART.y2 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_A;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_A ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_AB;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_AB ){
			UART.cA |= UCA0RXBUF>>4;
			UART.cB = UCA0RXBUF&0x0F;
			UART.cB <<= 8;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_B;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_B ){
			UART.cB |= UCA0RXBUF;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_C;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_C ){
			UART.cC = UCA0RXBUF;
			UART.cC <<= 4;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_CD;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_CD ){
			UART.cC |= UCA0RXBUF>>4;
			UART.cD = UCA0RXBUF&0x0F;
			UART.cD <<= 8;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_D;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_D ){
			UART.cD |= UCA0RXBUF;
			blendColourFillBox( &UART.x1, &UART.y1, &UART.x2, &UART.y2, &UART.cA, &UART.cB, &UART.cC, &UART.cD );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return;
		}

		// transmission or synchronization failure, reset
		UART.commandState = CMD_STATE_NOP;
		UART.commandStateGroup = CMD_STATE_NOP;
	}

	// ------------------------------------------------------------------------------------------
	// box
	if( UART.commandStateGroup == CMD_BOX )
	{
		if( UART.commandState == CMD_STATE_BOX__POSITION_A ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BOX__POSITION_B;
			return;
		}
		if( UART.commandState == CMD_STATE_BOX__POSITION_B ){
			UART.x2 = UCA0RXBUF>>4;
			UART.y2 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BOX__COLOUR_MSB;
			return;
		}
		if( UART.commandState == CMD_STATE_BOX__COLOUR_MSB ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_BOX__COLOUR_LSB;
			return;
		}
		if( UART.commandState == CMD_STATE_BOX__COLOUR_LSB ){
			UART.cA |= UCA0RXBUF>>4;
			box( &UART.x1, &UART.y1, &UART.x2, &UART.y2, &UART.cA );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return;
		}

		// transmission or synchronization failure, reset
		UART.commandState = CMD_STATE_NOP;
		UART.commandStateGroup = CMD_STATE_NOP;
	}

	// ------------------------------------------------------------------------------------------
	// fill box
	if( UART.commandStateGroup == CMD_FILL_BOX )
	{
		if( UART.commandState == CMD_STATE_FILL_BOX__POSITION_A ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_FILL_BOX__POSITION_B;
			return;
		}
		if( UART.commandState == CMD_STATE_FILL_BOX__POSITION_B ){
			UART.x2 = UCA0RXBUF>>4;
			UART.y2 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_FILL_BOX__COLOUR_MSB;
			return;
		}
		if( UART.commandState == CMD_STATE_FILL_BOX__COLOUR_MSB ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_FILL_BOX__COLOUR_LSB;
			return;
		}
		if( UART.commandState == CMD_STATE_FILL_BOX__COLOUR_LSB ){
			UART.cA |= UCA0RXBUF>>4;
			fillBox( &UART.x1, &UART.y1, &UART.x2, &UART.y2, &UART.cA );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return;
		}

		// transmission or synchronization failure, reset
		UART.commandState = CMD_STATE_NOP;
		UART.commandStateGroup = CMD_STATE_NOP;
	}

	// ------------------------------------------------------------------------------------------
	// blend colour line
	if( UART.commandStateGroup == CMD_BLEND_COLOUR_LINE )
	{
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_LINE__POSITION_A ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_LINE__POSITION_B;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_LINE__POSITION_B ){
			UART.x2 = UCA0RXBUF>>4;
			UART.y2 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_LINE__COLOUR_A;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_LINE__COLOUR_A ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_BLEND_COLOUR_LINE__COLOUR_AB;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_LINE__COLOUR_AB ){
			UART.cA |= UCA0RXBUF>>4;
			UART.cB = UCA0RXBUF&0x0F;
			UART.cB <<= 8;
			UART.commandState = CMD_STATE_BLEND_COLOUR_LINE__COLOUR_B;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_LINE__COLOUR_B ){
			UART.cB |= UCA0RXBUF;
			blendColourLine( &UART.x1, &UART.y1, &UART.x2, &UART.y2, &UART.cA, &UART.cB );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return;
		}

		// transmission or synchronization failure, reset
		UART.commandState = CMD_STATE_NOP;
		UART.commandStateGroup = CMD_STATE_NOP;
	}

	// ------------------------------------------------------------------------------------------
	// line
	if( UART.commandStateGroup == CMD_LINE )
	{
		if( UART.commandState == CMD_STATE_LINE__POSITION_A ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_LINE__POSITION_B;
			return;
		}
		if( UART.commandState == CMD_STATE_LINE__POSITION_B ){
			UART.x2 = UCA0RXBUF>>4;
			UART.y2 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_LINE__COLOUR_MSB;
			return;
		}
		if( UART.commandState == CMD_STATE_LINE__COLOUR_MSB ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_LINE__COLOUR_LSB;
			return;
		}
		if( UART.commandState == CMD_STATE_LINE__COLOUR_LSB ){
			UART.cA |= UCA0RXBUF>>4;
			line( &UART.x1, &UART.y1, &UART.x2, &UART.y2, &UART.cA );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return;
		}

		// transmission or synchronization failure, reset
		UART.commandState = CMD_STATE_NOP;
		UART.commandStateGroup = CMD_STATE_NOP;
	}

	// ------------------------------------------------------------------------------------------
	// circle
	if( UART.commandStateGroup == CMD_CIRCLE )
	{
		if( UART.commandState == CMD_STATE_CIRCLE__POSITION ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_CIRCLE__RADIUS;
			return;
		}
		if( UART.commandState == CMD_STATE_CIRCLE__RADIUS ){
			UART.x2 = UCA0RXBUF;
			UART.commandState = CMD_STATE_CIRCLE__COLOUR_MSB;
			return;
		}
		if( UART.commandState == CMD_STATE_CIRCLE__COLOUR_MSB ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_CIRCLE__COLOUR_LSB;
			return;
		}
		if( UART.commandState == CMD_STATE_CIRCLE__COLOUR_LSB ){
			UART.cA |= UCA0RXBUF>>4;
			circle( &UART.x1, &UART.y1, &UART.x2, &UART.cA );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return;
		}

		// transmission or synchronization failure, reset
		UART.commandState = CMD_STATE_NOP;
		UART.commandStateGroup = CMD_STATE_NOP;
	}

	// ------------------------------------------------------------------------------------------
	// fill circle
	if( UART.commandStateGroup == CMD_FILL_CIRCLE )
	{
		if( UART.commandState == CMD_STATE_FILL_CIRCLE__POSITION ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_FILL_CIRCLE__RADIUS;
			return;
		}
		if( UART.commandState == CMD_STATE_FILL_CIRCLE__RADIUS ){
			UART.x2 = UCA0RXBUF;
			UART.commandState = CMD_STATE_FILL_CIRCLE__COLOUR_MSB;
			return;
		}
		if( UART.commandState == CMD_STATE_FILL_CIRCLE__COLOUR_MSB ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_FILL_CIRCLE__COLOUR_LSB;
			return;
		}
		if( UART.commandState == CMD_STATE_FILL_CIRCLE__COLOUR_LSB ){
			UART.cA |= UCA0RXBUF>>4;
			fillCircle( &UART.x1, &UART.y1, &UART.x2, &UART.cA );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return;
		}

		// transmission or synchronization failure, reset
		UART.commandState = CMD_STATE_NOP;
		UART.commandStateGroup = CMD_STATE_NOP;
	}

	// ------------------------------------------------------------------------------------------
	// blend colour fill circle
	if( UART.commandStateGroup == CMD_BLEND_COLOUR_FILL_CIRCLE )
	{
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__POSITION ){
			UART.x1 = UCA0RXBUF>>4;
			UART.y1 = UCA0RXBUF&0x0F;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__RADIUS;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__RADIUS ){
			UART.x2 = UCA0RXBUF;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_A;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_A ){
			UART.cA = UCA0RXBUF;
			UART.cA <<= 4;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_AB;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_AB ){
			UART.cA |= UCA0RXBUF>>4;
			UART.cB = UCA0RXBUF&0x0F;
			UART.cB <<= 8;
			UART.commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_B;
			return;
		}
		if( UART.commandState == CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_B ){
			UART.cB |= UCA0RXBUF;
			blendColourFillCircle( &UART.x1, &UART.y1, &UART.x2, &UART.cA, &UART.cB );
			UART.commandState = CMD_STATE_NOP;
			UART.commandStateGroup = CMD_STATE_NOP;
			return;
		}

		// transmission or synchronization failure, reset
		UART.commandState = CMD_STATE_NOP;
		UART.commandStateGroup = CMD_STATE_NOP;
	}

	// transmission or synchronization failure, reset
	UART.commandState = CMD_STATE_NOP;
	UART.commandStateGroup = CMD_STATE_NOP;

	// return
	return;
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

			// process data
			processCommand();

			// ready for next
			UCA0TXBUF = UCA0RXBUF;

			break;

		case 4:break;				// Vector 4 - TXIFG
	default: break;
	}
}