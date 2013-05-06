// ----------------------------------------------------------------------
// Counts how many modules were enabled
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include Files
// ----------------------------------------------------------------------

#include "moduleenable.h"

// ----------------------------------------------------------------------
// counts modules enabled
unsigned char getModuleCount( void )
{
	unsigned char moduleCount = 0;
#ifdef MODULE_ENABLE_COLOUR_DEMO
	moduleCount++;
#endif
#ifdef MODULE_ENABLE_SNAKE
	moduleCount++;
#endif
#ifdef MODULE_ENABLE_GAME_OF_LIFE
	moduleCount++;
#endif
#ifdef MODULE_ENABLE_TRON
	moduleCount++;
#endif
#ifdef MODULE_ENABLE_SPACE_INVADERS
	moduleCount++;
#endif
#ifdef MODULE_ENABLE_TETRIS
	moduleCount++;
#endif
#ifdef MODULE_ENABLE_PONG
	moduleCount++;
#endif
#ifdef MODULE_ENABLE_BURGLER
	moduleCount++;
#endif
#ifdef MODULE_ENABLE_CAT_AND_MOUSE
	moduleCount++;
#endif
	return moduleCount-1;
}