// ----------------------------------------------------------------------
// UART handling
// ----------------------------------------------------------------------

#ifndef _UART_H_
	#define _UART_H_

// buffer size
#define COMMAND_BUFFER_SIZE 12

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

// command states
enum commandState_e
{
	CMD_STATE_NOP,
	CMD_STATE_CLS,

	CMD_STATE_DOT__POSITION,
	CMD_STATE_DOT__COLOUR_MSB,
	CMD_STATE_DOT__COLOUR_LSB,

	CMD_STATE_BLEND_COLOUR_BOX__POSITION_A,
	CMD_STATE_BLEND_COLOUR_BOX__POSITION_B,
	CMD_STATE_BLEND_COLOUR_BOX__COLOUR_A,
	CMD_STATE_BLEND_COLOUR_BOX__COLOUR_AB,
	CMD_STATE_BLEND_COLOUR_BOX__COLOUR_B,
	CMD_STATE_BLEND_COLOUR_BOX__COLOUR_C,
	CMD_STATE_BLEND_COLOUR_BOX__COLOUR_CD,
	CMD_STATE_BLEND_COLOUR_BOX__COLOUR_D,

	CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_A,
	CMD_STATE_BLEND_COLOUR_FILL_BOX__POSITION_B,
	CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_A,
	CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_AB,
	CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_B,
	CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_C,
	CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_CD,
	CMD_STATE_BLEND_COLOUR_FILL_BOX__COLOUR_D,

	CMD_STATE_BOX__POSITION_A,
	CMD_STATE_BOX__POSITION_B,
	CMD_STATE_BOX__COLOUR_MSB,
	CMD_STATE_BOX__COLOUR_LSB,

	CMD_STATE_FILL_BOX__POSITION_A,
	CMD_STATE_FILL_BOX__POSITION_B,
	CMD_STATE_FILL_BOX__COLOUR_MSB,
	CMD_STATE_FILL_BOX__COLOUR_LSB,

	CMD_STATE_BLEND_COLOUR_LINE__POSITION_A,
	CMD_STATE_BLEND_COLOUR_LINE__POSITION_B,
	CMD_STATE_BLEND_COLOUR_LINE__COLOUR_A,
	CMD_STATE_BLEND_COLOUR_LINE__COLOUR_AB,
	CMD_STATE_BLEND_COLOUR_LINE__COLOUR_B,

	CMD_STATE_LINE__POSITION_A,
	CMD_STATE_LINE__POSITION_B,
	CMD_STATE_LINE__COLOUR_MSB,
	CMD_STATE_LINE__COLOUR_LSB,

	CMD_STATE_CIRCLE__POSITION,
	CMD_STATE_CIRCLE__RADIUS,
	CMD_STATE_CIRCLE__COLOUR_MSB,
	CMD_STATE_CIRCLE__COLOUR_LSB,

	CMD_STATE_FILL_CIRCLE__POSITION,
	CMD_STATE_FILL_CIRCLE__RADIUS,
	CMD_STATE_FILL_CIRCLE__COLOUR_MSB,
	CMD_STATE_FILL_CIRCLE__COLOUR_LSB,

	CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__POSITION,
	CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__RADIUS,
	CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_A,
	CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_AB,
	CMD_STATE_BLEND_COLOUR_FILL_CIRCLE__COLOUR_B,

	CMD_STATE_SET_BLEND_MODE__REPLACE,
	CMD_STATE_SET_BLEND_MODE__ADD,
	CMD_STATE_SET_BLEND_MODE__SUBTRACT,
	CMD_STATE_SET_BLEND_MODE__MULTIPLY
};

// structs
struct UART_t
{

	// buffer
	volatile unsigned char commandBuffer[ COMMAND_BUFFER_SIZE ];
	volatile unsigned char commandBufferWritePtr;
	unsigned char commandBufferReadPtr;
	volatile unsigned char timeToProcessCommandBuffer;
};
extern struct UART_t UART;

// function prototypes
void processCommandBuffer( void );

#endif // _UART_H_