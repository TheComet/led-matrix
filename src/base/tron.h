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
	unsigned char* frameBuffer;
	unsigned char* playerCount;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void loadTron( unsigned char* frameBuffer, unsigned char* playerCount );
void processTronLoop( void );
void processTronInput( void );

#endif // _TRON_H_