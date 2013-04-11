// ----------------------------------------------------------------------
// Counts how many games were enabled
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include Files
// ----------------------------------------------------------------------

#include "gameenable.h"

// ----------------------------------------------------------------------
// counts games enabled
unsigned char getGameCount( void )
{
	unsigned char gameCount = 0;
#ifdef GAME_ENABLE_COLOUR_DEMO
	gameCount++;
#endif
#ifdef GAME_ENABLE_SNAKE
	gameCount++;
#endif
#ifdef GAME_ENABLE_GAME_OF_LIFE
	gameCount++;
#endif
#ifdef GAME_ENABLE_TRON
	gameCount++;
#endif
#ifdef GAME_ENABLE_SPACE_INVADERS
	gameCount++;
#endif
#ifdef GAME_ENABLE_TETRIS
	gameCount++;
#endif
	return gameCount-1;
}