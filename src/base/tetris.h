// ----------------------------------------------------------------------
// Tetris
// ----------------------------------------------------------------------

#ifndef _TETRIS_H_
	#define _TETRIS_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct Tetris_t
{
	unsigned char* frameBuffer;
	unsigned char* playerCount;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadTetris( unsigned char* frameBuffer, unsigned char* playerCount );
void processTetrisLoop( void );
void processTetrisInput( void );

#endif // _TETRIS_H_