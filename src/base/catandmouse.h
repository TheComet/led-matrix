// ----------------------------------------------------------------------
// Cat and Mouse
// by Benjamin Ostermayer
// ----------------------------------------------------------------------

#ifndef _CATANDMOUSE_H_
	#define _CATANDMOUSE_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------
struct CatAndMouse_Player_t
{
	unsigned char pos_x;
	unsigned char pos_y;
	unsigned char old_pos_x;
	unsigned char old_pos_y;
	unsigned short color;
};
struct CatAndMouse_t
{
	struct CatAndMouse_Player_t player[4];
	unsigned short* frameBuffer;
	unsigned char* playersActive;
	unsigned char playerCount;
	unsigned char catCount;
};
// ----------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------

void loadCatAndMouse( unsigned short* frameBuffer, unsigned char* playerCounter );
void processCatAndMouseLoop( void );
void processCatAndMouseInput( void );
void drawCatAndMouseMenuIcon( void );

#endif // _CATANDMOUSE_H_