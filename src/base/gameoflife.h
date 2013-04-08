// ----------------------------------------------------------------------
// Game of Life
// ----------------------------------------------------------------------

#ifndef _GAMEOFLIFE_H_
	#define _GAMEOFLIFE_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct GameOfLife_t
{
	unsigned char* frameBuffer;
	unsigned char state;
	unsigned char bufferOffset;
};

// ----------------------------------------------------------------------
// Enumerators
// ----------------------------------------------------------------------

enum GameOfLife_States_e
{
	GAMEOFLIFE_STATE_PLAY,
	GAMEOFLIFE_STATE_EDIT
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadGameOfLife( unsigned char* frameBuffer );
void processGameOfLife( void );

#endif // _GAMEOFLIFE_H_