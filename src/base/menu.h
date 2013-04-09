// ----------------------------------------------------------------------
// Main Menu
// ----------------------------------------------------------------------

#ifndef _MENU_H_
	#define _MENU_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct Menu_t
{
	unsigned char state;
	unsigned char toggleArrow;
	unsigned char selected;
	unsigned char playerList;
};

// ----------------------------------------------------------------------
// Enumerators
// ----------------------------------------------------------------------

// menu states
enum Menu_State_e
{
	MENU_STATE_PRESS_START,
	MENU_STATE_SELECT_GAME
};

// games/demos - specify game count-1
#define GAME_COUNT 2
enum Menu_Games_e
{
	MENU_SELECT_COLOUR_DEMO,
	MENU_SELECT_SNAKE,
	MENU_SELECT_GAME_OF_LIFE
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadMenu( unsigned char* frameBuffer );
void processMenuLoop( void );
void processMenuInput( void );
void menuDrawFrame( void );
void menuDrawLeftArrow( unsigned char clear );
void menuDrawRightArrow( unsigned char clear );
void menuDrawStartArrow( unsigned char offset );
void menuDrawJoinArrows( unsigned char* playerList );
void menuClearIcon( void );
void menuDrawColourDemoIcon( void );
void menuDrawSnakeIcon( void );
void menuDrawGameOfLifeIcon( void );
void menuUpdateIcon( unsigned char* selected );

#endif // _MENU_H_