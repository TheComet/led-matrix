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
	unsigned short* frameBuffer;
	unsigned char* playerCount;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadTron( unsigned short* frameBuffer, unsigned char* playerCount );
void processTronLoop( void );
void processTronInput( void );

#endif // _TRON_H_