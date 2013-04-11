// ----------------------------------------------------------------------
// Main Menu
// ----------------------------------------------------------------------

#ifndef _MENU_H_
	#define _MENU_H_

#include "gameenable.h"

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct Menu_t
{
	unsigned char state;
	unsigned char toggleArrow;
	unsigned char selected;
	unsigned char playerList;
	unsigned char gameCount;
};

// ----------------------------------------------------------------------
// Enumerators
// ----------------------------------------------------------------------

// menu states
enum Menu_State_e
{
	MENU_STATE_SELECT_GAME
};

// list of games
enum Menu_Games_e
{
#ifdef GAME_ENABLE_COLOUR_DEMO
	MENU_SELECT_COLOUR_DEMO,
#endif
#ifdef GAME_ENABLE_SNAKE
	MENU_SELECT_SNAKE,
#endif
#ifdef GAME_ENABLE_GAME_OF_LIFE
	MENU_SELECT_GAME_OF_LIFE,
#endif
#ifdef GAME_ENABLE_TRON
	MENU_SELECT_TRON,
#endif
#ifdef GAME_ENABLE_SPACE_INVADERS
	MENU_SELECT_SPACE_INVADERS,
#endif
#ifdef GAME_ENABLE_TETRIS
	MENU_SELECT_TETRIS,
#endif
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void initMenu( void );
void loadMenu( unsigned char* frameBuffer );
void processMenuLoop( void );
void processMenuInput( void );
void menuDrawFrame( void );
void menuDrawLeftArrow( unsigned char clear );
void menuDrawRightArrow( unsigned char clear );
void menuDrawStartArrow( unsigned char offset );
void menuDrawJoinArrows( unsigned char* playerList );
void menuClearIcon( void );
void menuUpdateIcon( unsigned char* selected );

#ifdef GAME_ENABLE_COLOUR_DEMO
void menuDrawColourDemoIcon( void );
#endif
#ifdef GAME_ENABLE_SNAKE
void menuDrawSnakeIcon( void );
#endif
#ifdef GAME_ENABLE_GAME_OF_LIFE
void menuDrawGameOfLifeIcon( void );
#endif
#ifdef GAME_ENABLE_TRON
void menuDrawTronIcon( void );
#endif
#ifdef GAME_ENABLE_SPACE_INVADERS
void menuDrawSpaceInvadersIcon( void );
#endif
#ifdef GAME_ENABLE_TETRIS
void menuDrawTetrisIcon( void );
#endif


#endif // _MENU_H_
