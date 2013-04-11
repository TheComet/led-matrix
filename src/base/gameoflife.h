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

struct GameOfLife_Player_t
{
	struct GameOfLife_Cursor_t cursor;
	struct GameOfLife_Cursor_t oldCursor;
};

struct GameOfLife_t
{
	unsigned char* frameBuffer;
	unsigned char state;
	unsigned char bufferOffset;
	struct GameOfLife_Player_t player[4];
	unsigned char* playerCount;
};

// ----------------------------------------------------------------------
// Enumerators
// ----------------------------------------------------------------------

enum GameOfLife_States_e
{
	GAMEOFLIFE_STATE_PLAY_SINGLE,
	GAMEOFLIFE_STATE_EDIT_SINGLE,
	GAMEOFLIFE_STATE_PLAY_MULTI
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadGameOfLife( unsigned char* frameBuffer, unsigned char* playerCount );
void processGameOfLifeLoop( void );
void processGameOfLifeInput( void );
void randomizeFrameBuffer( void );
void drawFrameBufferNoCheck( void );
void drawFrameBuffer( void );
void drawFrameBufferCustom( const unsigned short* colour1, const unsigned short* colour2, const unsigned short* colour3, const unsigned short* colour4 );
void computeNextCycle( void );

#endif // _GAMEOFLIFE_H_
