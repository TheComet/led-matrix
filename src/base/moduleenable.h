// ----------------------------------------------------------------------
// Module Enable File
// ----------------------------------------------------------------------

#ifndef _MODULEENABLE_H_
	#define _MODULEENABLE_H_

// ----------------------------------------------------------------------
// Comment in the modules you would like enabled
// ----------------------------------------------------------------------

#define MAX_MODULES 12

#define MODULE_ENABLE_COLOUR_DEMO
//#define MODULE_ENABLE_SNAKE
//#define MODULE_ENABLE_GAME_OF_LIFE
//#define MODULE_ENABLE_TRON
//#define MODULE_ENABLE_TETRIS
//#define MODULE_ENABLE_SPACE_INVADERS
//#define MODULE_ENABLE_PONG
//#define MODULE_ENABLE_BURGLER
#define MODULE_ENABLE_CAT_AND_MOUSE

// ----------------------------------------------------------------------
// Will count the commented modules so the menu knows
// ----------------------------------------------------------------------

unsigned char getModuleCount( void );

#endif // _MODULEENABLE_H_