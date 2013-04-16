// ----------------------------------------------------------------------
// Tron
// ----------------------------------------------------------------------

#ifndef _TRON_H_
	#define _TRON_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct Tron_t
{
 unsigned char Pos_X;
 unsigned char Pos_Y;
 unsigned char Pos_X_Hinten;
 unsigned char Pos_Y_Hinten;
 unsigned char Direction;
};

enum Tron_Direction_e
{
  TRON_DIRECTION_UP,
  TRON_DIRECTION_DOWN,
  TRON_DIRECTION_LEFT,
  TRON_DIRECTION_RIGHT
};

// ----------------------------------------------------------------------
// Function prototypes
// ----------------------------------------------------------------------

void loadTron( unsigned short* frameBuffer, unsigned char* playerCount );
void processTronLoop( void );
void processTronInput( void );

#endif // _TRON_H_