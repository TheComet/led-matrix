// ----------------------------------------------------------------------
// Cat and Mouse
// by Benjamin Ostermayer
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// include files

#include "catandmouse.h"
#include "uart.h"
#include "framework.h"
#include "moduleenable.h"

#ifdef GAME_ENABLE_CAT_AND_MOUSE

static struct CatAndMouse_t CatAndMouse;

// ----------------------------------------------------------------------
// load cat and mouse
void loadCatAndMouse( unsigned short* frameBuffer, unsigned char* playerCount )
{
	cls();
	
	CatAndMouse.frameBuffer=frameBuffer;
	CatAndMouse.playerCount=playerCount;	

	for(unsigned char player=0; player!=4; player++)
	{
		unsigned char x;

		// set initial color
		CatAndMouse.player[player].color=GREEN;

		// set positions
		do
		{
			// set position
			CatAndMouse.player[player].pos_x=rnd()&0x0F;
			CatAndMouse.player[player].pos_y=rnd()&0x0F;

			// player 0 doesn't need checking
			if(player==0) break;

			// check if position isn't colliding with other player
			for(x=0; x!=player; x++)
			{
				if(CatAndMouse.player[player].pos_x!=CatAndMouse.player[x].pos_x) break;
				if(CatAndMouse.player[player].pos_y!=CatAndMouse.player[x].pos_y) break;
			}

		}while(x==player);

		dot(&CatAndMouse.player[player].pos_x, &CatAndMouse.player[player].pos_y, &CatAndMouse.player[player].color);
	}

	send();
	
}

// ----------------------------------------------------------------------
// process cat and mouse main loop
void processCatAndMouseLoop( void )
{	

}	
// ----------------------------------------------------------------------
// process cat and mouse input
void processCatAndMouseInput( void )
{
	if( player1ButtonMenu() ) endGame();
}

// -----------------------------------------------------------------------
// draw cat and mouse menu icon
void drawCatAndMouseMenuIcon( void )
{
}
#endif // GAME_ENABLE_CAT_AND_MOUSE