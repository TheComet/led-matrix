// ----------------------------------------------------------------------
// Tron
// ----------------------------------------------------------------------

#ifndef _TRON_H_
	#define _TRON_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct Tron_Player_t
{
	unsigned char Y;
	unsigned char X;
	unsigned char Direction;
};

struct Tron_t
{
 struct Tron_Player_t Player[4];
 unsigned char Speed;
 unsigned short* frameBuffer; 
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