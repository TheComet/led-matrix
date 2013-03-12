// ----------------------------------------------------------------------
// UART handling
// ----------------------------------------------------------------------

// include files
#include "main.h"

// global variables
struct UART_t UART;

// ------------------------------------------------------------------------------------------------------------------
// process data in command buffer
void processCommandBuffer( void )
{

	// clear flag
	UART.timeToProcessCommandBuffer = 0;

	// copy volatile variables into non volatile ones for processing
	unsigned char cpy_commandBufferWritePtr = UART.commandBufferWritePtr;

	// state of the command
	unsigned char commandState = CMD_STATE_NOP;

	// data to be extracted
	unsigned char x1, y1, x2, y2;
	unsigned short cA, cB, cC, cD;
	unsigned char data;

	// disable UART interrupt
	//UCA0IE &= ~UCRXIE;

	// extract command to execute
	switch( UART.commandBuffer[ UART.commandBufferReadPtr ] )
	{
		case CMD_CLS				: commandState = CMD_STATE_CLS; 					break;
		case CMD_DOT 				: commandState = CMD_STATE_DOT__POSITION;				break;
		case CMD_BLEND_COLOUR_BOX		: commandState = CMD_STATE_BLEND_COLOUR_BOX__POSITION_A;		break;
		case CMD_BLEND_COLOUR_FILL_BOX		: commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_A;		break;
		case CMD_BOX				: commandState = CMD_STATE_BOX__POSITION_A;				break;
		case CMD_FILL_BOX			: commandState = CMD_STATE_FILL_BOX__POSITION_A;			break;
		case CMD_BLEND_COLOUR_LINE		: commandState = CMD_STATE_BLEND_COLOUR_LINE__POSITION_A;		break;
		case CMD_LINE				: commandState = CMD_STATE_LINE__POSITION_A;				break;
		case CMD_CIRCLE				: commandState = CMD_STATE_CIRCLE__POSITION;				break;
		case CMD_FILL_CIRCLE			: commandState = CMD_STATE_FILL_CIRCLE__POSITION;			break;
		case CMD_BLEND_COLOUR_FILL_CIRCLE	: commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__POSITION;		break;
		case CMD_SET_BLEND_MODE__REPLACE	: commandState = CMD_STATE_SET_BLEND_MODE__REPLACE;			break;
		case CMD_SET_BLEND_MODE__ADD		: commandState = CMD_STATE_SET_BLEND_MODE__ADD;				break;
		case CMD_SET_BLEND_MODE__SUBTRACT	: commandState = CMD_STATE_SET_BLEND_MODE__SUBTRACT;			break;
		case CMD_SET_BLEND_MODE__MULTIPLY	: commandState = CMD_STATE_SET_BLEND_MODE__MULTIPLY;			break;
		default 				: commandState = CMD_STATE_NOP;						break;
	}

	if( commandState == CMD_STATE_NOP )
	{
		UART.commandBufferReadPtr = UART.commandBufferWritePtr;
		//UCA0IE |= UCRXIE;
		return;
	}
	UART.commandBufferReadPtr++;

	// loop through buffer
	for( ; UART.commandBufferReadPtr != cpy_commandBufferWritePtr; UART.commandBufferReadPtr++ )
	{

		// get next set of data
		data = UART.commandBuffer[ UART.commandBufferReadPtr ];

		// process command states
		switch( commandState )
		{

			// ------------------------------------------------------------------------------------------
			// clear screen
			case CMD_STATE_CLS :
				cls();
				commandState = CMD_STATE_NOP;
				break;

			// ------------------------------------------------------------------------------------------
			// dot
			case CMD_STATE_DOT__POSITION :
				x1 = data>>4;
				y1 = data&0x0F;
				commandState = CMD_STATE_DOT__COLOUR_MSB;
				break;
			case CMD_STATE_DOT__COLOUR_MSB :
				cA = data;
				cA <<= 4;
				commandState = CMD_STATE_DOT__COLOUR_LSB;
				break;
			case CMD_STATE_DOT__COLOUR_LSB :
				cA |= (data>>4);
				dot( &x1, &y1, &cA );
				commandState = CMD_STATE_NOP;
				break;

			// ------------------------------------------------------------------------------------------
			// blend colour box
			case CMD_STATE_BLEND_COLOUR_BOX__POSITION_A :
				x1 = data>>4;
				y1 = data&0x0F;
				commandState = CMD_STATE_BLEND_COLOUR_BOX__POSITION_B;
				break;
			case CMD_STATE_BLEND_COLOUR_BOX__POSITION_B :
				x2 = data>>4;
				y2 = data&0x0F;
				commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_A;
				break;
			case CMD_STATE_BLEND_COLOUR_BOX__COLOUR_A :
				cA = data;
				cA <<= 4;
				commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_AB;
				break;
			case CMD_STATE_BLEND_COLOUR_BOX__COLOUR_AB :
				cA |= data>>4;
				cB = data&0x0F;
				cB <<= 8;
				commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_B;
				break;
			case CMD_STATE_BLEND_COLOUR_BOX__COLOUR_B :
				cB |= data;
				commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_C;
				break;
			case CMD_STATE_BLEND_COLOUR_BOX__COLOUR_C :
				cC = data;
				cC <<= 4;
				commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_CD;
				break;
			case CMD_STATE_BLEND_COLOUR_BOX__COLOUR_CD :
				cC |= data>>4;
				cD = data&0x0F;
				cD <<= 8;
				commandState = CMD_STATE_BLEND_COLOUR_BOX__COLOUR_D;
				break;
			case CMD_STATE_BLEND_COLOUR_BOX__COLOUR_D :
				cD |= data;
				blendColourBox( &x1, &y1, &x2, &y2, &cA, &cB, &cC, &cD );
				commandState = CMD_STATE_NOP;
				break;

			// ------------------------------------------------------------------------------------------
			// blend colour fill box
			case CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_A :
				x1 = data>>4;
				y1 = data&0x0F;
				commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_B;
				break;
			case CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_B :
				x2 = data>>4;
				y2 = data&0x0F;
				commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_A;
				break;
			case CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_A :
				cA = data;
				cA <<= 4;
				commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_AB;
				break;
			case CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_AB :
				cA |= data>>4;
				cB = data&0x0F;
				cB <<= 8;
				commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_B;
				break;
			case CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_B :
				cB |= data;
				commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_C;
				break;
			case CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_C :
				cC = data;
				cC <<= 4;
				commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_CD;
				break;
			case CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_CD :
				cC |= data>>4;
				cD = data&0x0F;
				cD <<= 8;
				commandState = CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_D;
				break;
			case CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_D :
				cD |= data;
				blendColourFillBox( &x1, &y1, &x2, &y2, &cA, &cB, &cC, &cD );
				commandState = CMD_STATE_NOP;
				break;

			// ------------------------------------------------------------------------------------------
			// box
			case CMD_STATE_BOX__POSITION_A :
				x1 = data>>4;
				y1 = data&0x0F;
				commandState = CMD_STATE_BOX__POSITION_B;
				break;
			case CMD_STATE_BOX__POSITION_B :
				x2 = data>>4;
				y2 = data&0x0F;
				commandState = CMD_STATE_BOX__COLOUR_MSB;
				break;
			case CMD_STATE_BOX__COLOUR_MSB :
				cA = data;
				cA <<= 4;
				commandState = CMD_STATE_BOX__COLOUR_LSB;
				break;
			case CMD_STATE_BOX__COLOUR_LSB :
				cA |= data>>4;
				box( &x1, &y1, &x2, &y2, &cA );
				commandState = CMD_STATE_NOP;
				break;

			// ------------------------------------------------------------------------------------------
			// fill box
			case CMD_STATE_FILL_BOX__POSITION_A :
				x1 = data>>4;
				y1 = data&0x0F;
				commandState = CMD_STATE_FILL_BOX__POSITION_B;
				break;
			case CMD_STATE_FILL_BOX__POSITION_B :
				x2 = data>>4;
				y2 = data&0x0F;
				commandState = CMD_STATE_FILL_BOX__COLOUR_MSB;
				break;
			case CMD_STATE_FILL_BOX__COLOUR_MSB :
				cA = data;
				cA <<= 4;
				commandState = CMD_STATE_FILL_BOX__COLOUR_LSB;
				break;
			case CMD_STATE_FILL_BOX__COLOUR_LSB :
				cA |= data>>4;
				fillBox( &x1, &y1, &x2, &y2, &cA );
				commandState = CMD_STATE_NOP;
				break;

			// ------------------------------------------------------------------------------------------
			// blend colour line
			case CMD_STATE_BLEND_COLOUR_LINE__POSITION_A :
				x1 = data>>4;
				y1 = data&0x0F;
				commandState = CMD_STATE_BLEND_COLOUR_LINE__POSITION_B;
				break;
			case CMD_STATE_BLEND_COLOUR_LINE__POSITION_B :
				x2 = data>>4;
				y2 = data&0x0F;
				commandState = CMD_STATE_BLEND_COLOUR_LINE__COLOUR_A;
				break;
			case CMD_STATE_BLEND_COLOUR_LINE__COLOUR_A :
				cA = data;
				cA <<= 4;
				commandState = CMD_STATE_BLEND_COLOUR_LINE__COLOUR_AB;
				break;
			case CMD_STATE_BLEND_COLOUR_LINE__COLOUR_AB :
				cA |= data>>4;
				cB = data&0x0F;
				cB <<= 8;
				commandState = CMD_STATE_BLEND_COLOUR_LINE__COLOUR_B;
				break;
			case CMD_STATE_BLEND_COLOUR_LINE__COLOUR_B :
				cB |= data;
				blendColourLine( &x1, &y1, &x2, &y2, &cA, &cB );
				commandState = CMD_STATE_NOP;
				break;

			// ------------------------------------------------------------------------------------------
			// line
			case CMD_STATE_LINE__POSITION_A :
				x1 = data>>4;
				y1 = data&0x0F;
				commandState = CMD_STATE_LINE__POSITION_B;
				break;
			case CMD_STATE_LINE__POSITION_B :
				x2 = data>>4;
				y2 = data&0x0F;
				commandState = CMD_STATE_LINE__COLOUR_MSB;
				break;
			case CMD_STATE_LINE__COLOUR_MSB :
				cA = data;
				cA <<= 4;
				commandState = CMD_STATE_LINE__COLOUR_LSB;
				break;
			case CMD_STATE_LINE__COLOUR_LSB :
				cA |= data>>4;
				line( &x1, &y1, &x2, &y2, &cA );
				commandState = CMD_STATE_NOP;
				break;

			// ------------------------------------------------------------------------------------------
			// circle
			case CMD_STATE_CIRCLE__POSITION :
				x1 = data>>4;
				y1 = data&0x0F;
				commandState = CMD_STATE_CIRCLE__RADIUS;
				break;
			case CMD_STATE_CIRCLE__RADIUS :
				x2 = data;
				commandState = CMD_STATE_CIRCLE__COLOUR_MSB;
				break;
			case CMD_STATE_CIRCLE__COLOUR_MSB :
				cA = data;
				cA <<= 4;
				commandState = CMD_STATE_CIRCLE__COLOUR_LSB;
				break;
			case CMD_STATE_CIRCLE__COLOUR_LSB :
				cA |= data>>4;
				circle( &x1, &y1, &x2, &cA );
				commandState = CMD_STATE_NOP;
				break;

			// ------------------------------------------------------------------------------------------
			// fill circle
			case CMD_STATE_FILL_CIRCLE__POSITION :
				x1 = data>>4;
				y1 = data&0x0F;
				commandState = CMD_STATE_FILL_CIRCLE__RADIUS;
				break;
			case CMD_STATE_FILL_CIRCLE__RADIUS :
				x2 = data;
				commandState = CMD_STATE_FILL_CIRCLE__COLOUR_MSB;
				break;
			case CMD_STATE_FILL_CIRCLE__COLOUR_MSB :
				cA = data;
				cA <<= 4;
				commandState = CMD_STATE_FILL_CIRCLE__COLOUR_LSB;
				break;
			case CMD_STATE_FILL_CIRCLE__COLOUR_LSB :
				cA |= data>>4;
				fillCircle( &x1, &y1, &x2, &cA );
				commandState = CMD_STATE_NOP;
				break;

			// ------------------------------------------------------------------------------------------
			// blend colour fill circle
			case CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__POSITION :
				x1 = data>>4;
				y1 = data&0x0F;
				commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__RADIUS;
				break;
			case CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__RADIUS :
				x2 = data;
				commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_A;
				break;
			case CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_A :
				cA = data;
				cA <<= 4;
				commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_AB;
				break;
			case CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_AB :
				cA |= data>>4;
				cB = data&0x0F;
				cB <<= 8;
				commandState = CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_B;
				break;
			case CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_B :
				cB |= data;
				blendColourFillCircle( &x1, &y1, &x2, &cA, &cB );
				commandState = CMD_STATE_NOP;
				break;

			// ------------------------------------------------------------------------------------------
			// set blend modes
			case CMD_STATE_SET_BLEND_MODE__REPLACE :
				drawUtils_SetBlendMode( BLEND_MODE_REPLACE );
				commandState = CMD_STATE_NOP;
				break;
			case CMD_STATE_SET_BLEND_MODE__ADD :
				drawUtils_SetBlendMode( BLEND_MODE_ADD );
				commandState = CMD_STATE_NOP;
				break;
			case CMD_STATE_SET_BLEND_MODE__SUBTRACT :
				drawUtils_SetBlendMode( BLEND_MODE_SUBTRACT );
				commandState = CMD_STATE_NOP;
				break;
			case CMD_STATE_SET_BLEND_MODE__MULTIPLY :
				drawUtils_SetBlendMode( BLEND_MODE_MULTIPLY );
				commandState = CMD_STATE_NOP;
				break;

			// nothing
			default: break;
		}

		// wrap buffer
		if( UART.commandBufferReadPtr+1 == COMMAND_BUFFER_SIZE ) UART.commandBufferReadPtr = 255;

	}

	// enable UART interrupt
	//UCA0IE |= UCRXIE;

	// return
	return;
}

// ------------------------------------------------------------------------------------------------------------------
// Interrupt for receiving data
#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR( void )
{

	// process received data, if any
	__bic_SR_register( GIE );
	switch(__even_in_range(UCA0IV,4))
	{
		case 0:break;				// Vector 0 - no interrupt
		case 2:					// Vector 2 - RXIFG

			// store received command
			UART.commandBuffer[ UART.commandBufferWritePtr ] = UCA0RXBUF;

			// check for ending signature (0xFFFF)
			if( UCA0RXBUF == 0xFF ) UART.timeToProcessCommandBuffer++; else UART.timeToProcessCommandBuffer = 0;

			// increase pointer
			if( UART.commandBufferWritePtr+1 == COMMAND_BUFFER_SIZE ) UART.commandBufferWritePtr = 255;
			UART.commandBufferWritePtr++;
                        
			break;

		case 4:break;				// Vector 4 - TXIFG
	default: break;
	}
	__bis_SR_register( GIE );
}