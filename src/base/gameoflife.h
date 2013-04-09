// ----------------------------------------------------------------------
// Game of Life
// ----------------------------------------------------------------------

#ifndef _GAMEOFLIFE_H_
	#define _GAMEOFLIFE_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct GameOfLife_Cursor_t
{
	unsigned char x;
	unsigned char y;
};

struct GameOfLife_t
{
	unsigned char* frameBuffer;
	unsigned char state;
	unsigned char bufferOffset;
	struct GameOfLife_Cursor_t cursor;
	struct GameOfLife_Cursor_t oldCursor;
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
void processGameOfLifeLoop( void );
void processGameOfLifeInput( void );
void randomizeFrameBuffer( void );
void prepareFrameBufferForEditing( void );
void drawFrameBuffer( const unsigned short* colour, unsigned char forceDraw );

#endif // _GAMEOFLIFE_H_