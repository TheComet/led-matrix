// ----------------------------------------------------------------------
// Main Menu
// ----------------------------------------------------------------------

#ifndef _MENU_H_
	#define _MENU_H_

#include "moduleenable.h"

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct Menu_t
{
	unsigned char toggleArrow;
	unsigned char selected;
	unsigned char playerList;
	unsigned char gameCount;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void initMenu( void );
void loadMenu( unsigned short* frameBuffer, unsigned char* playerCount );
void processMenuLoop( void );
void processMenuInput( void );
void drawMenuIconDummy( void );
void menuDrawFrame( void );
void menuDrawLeftArrow( unsigned char clear );
void menuDrawRightArrow( unsigned char clear );
void menuDrawStartArrow( unsigned char offset );
void menuDrawJoinArrows( unsigned char* playerList );

#endif // _MENU_H_
