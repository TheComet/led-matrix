// ----------------------------------------------------------------------
// Game Enable File
// ----------------------------------------------------------------------

#ifndef _GAMEENABLE_H_
	#define _GAMEENABLE_H_

// ----------------------------------------------------------------------
// Comment in the games you would like enabled
// ----------------------------------------------------------------------

#define GAME_ENABLE_COLOUR_DEMO
//#define GAME_ENABLE_SNAKE
//#define GAME_ENABLE_GAME_OF_LIFE
//#define GAME_ENABLE_TRON
//#define GAME_ENABLE_TETRIS
//#define GAME_ENABLE_SPACE_INVADERS
//#define GAME_ENABLE_PONG
//#define GAME_ENABLE_BURGLER

// ----------------------------------------------------------------------
// Will count the commented games so the menu knows
// ----------------------------------------------------------------------

unsigned char getGameCount( void );

#endif // _GAMEENABLE_H_