// ----------------------------------------------------------------------
// Burgler - Avoid patrolling police and collect the gold
// ----------------------------------------------------------------------

#ifndef _BURGLER_H_
	#define _BURGLER_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct Burgler_t
{
	unsigned short* frameBuffer;
	unsigned char* playerCount;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadBurgler( unsigned short* frameBuffer, unsigned char* playerCount );
void processBurglerLoop( void );
void processBurglerInput( void );
void drawBurglerMenuIcon( void );

#endif // _BURGLER_H_