// ----------------------------------------------------------------------
// Common declarations
// ----------------------------------------------------------------------

#ifndef _COMMON_H_
	#define _COMMON_H_

// ----------------------------------------------------------------------
// include files
// ----------------------------------------------------------------------

#include "msp430f2418.h"

// ----------------------------------------------------------------------
// definitions
// ----------------------------------------------------------------------

// map player ports
#define MAP_PLAYER1_BUTTON         (~P1IN)&0x1F
#define MAP_PLAYER3_BUTTON         (~P2IN)&0x1F
#define MAP_PLAYER2_BUTTON         (~P3IN)&0x1F
#define MAP_PLAYER4_BUTTON         (~P4IN)&0x1F

// map virtual buttons
#define MAP_PLAYER_BUTTON_MENU     0x20
#define MAP_PLAYER_BUTTON_CLEAR    0x40

// map player buttons
#define MAP_PLAYER1_BUTTON_FIRE    0x01
#define MAP_PLAYER1_BUTTON_RIGHT   0x02
#define MAP_PLAYER1_BUTTON_DOWN    0x04
#define MAP_PLAYER1_BUTTON_LEFT    0x08
#define MAP_PLAYER1_BUTTON_UP      0x10

#define MAP_PLAYER2_BUTTON_FIRE    0x10
#define MAP_PLAYER2_BUTTON_RIGHT   0x08
#define MAP_PLAYER2_BUTTON_DOWN    0x04
#define MAP_PLAYER2_BUTTON_LEFT    0x02
#define MAP_PLAYER2_BUTTON_UP      0x01

#define MAP_PLAYER3_BUTTON_FIRE    0x10
#define MAP_PLAYER3_BUTTON_RIGHT   0x08
#define MAP_PLAYER3_BUTTON_DOWN    0x04
#define MAP_PLAYER3_BUTTON_LEFT    0x02
#define MAP_PLAYER3_BUTTON_UP      0x01

#define MAP_PLAYER4_BUTTON_FIRE    0x10
#define MAP_PLAYER4_BUTTON_RIGHT   0x08
#define MAP_PLAYER4_BUTTON_DOWN    0x04
#define MAP_PLAYER4_BUTTON_LEFT    0x02
#define MAP_PLAYER4_BUTTON_UP      0x01

// ----------------------------------------------------------------------
// global variables
// ----------------------------------------------------------------------

#endif // _COMMON_H_