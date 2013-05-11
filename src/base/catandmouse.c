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

#ifdef MODULE_ENABLE_CAT_AND_MOUSE

static struct CatAndMouse_t CatAndMouse;

// Icon
static const unsigned short catAndMouseIcon[100] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xD0,0xD0,0x0,0x0,0xD0,0xD0,0x0,0x0,0x0,0x0,0xD0,0xD0,0xD0,0xD0,0xD0,0xD0,0x0,0x0,0xD0,0x0,0xD0,0x0,0x0,0x0,0x0,0xD0,0x0,0xD0,0x0,0xD0,0x0,0xE00,0x0,0x0,0xE00,0x0,0xD0,0x0,0xD0,0xD0,0x0,0x0,0x0,0x0,0x0,0x0,0xD0,0xD0,0x0,0x0,0xD0,0x0,0x0,0x0,0x0,0xD0,0x0,0x0,0x0,0x0,0x0,0xD0,0xD0,0xD0,0xD0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};

// ----------------------------------------------------------------------
// load cat and mouse
void loadCatAndMouse( unsigned short* frameBuffer, unsigned char* playerCount )
{
	//2-4 Players allowed
	if(*playerCount==0)
	{
		endGame();
		return;
	}
	cls();
	
	CatAndMouse.frameBuffer=frameBuffer;
	CatAndMouse.playerCount=playerCount;	
	
	unsigned char x,x2;	//local variable for drawing functions and other stuff
	unsigned char y,y2;
	unsigned short LIGHTORANGE=0xA50;	//define the color for the box

	//set boundary
	x=0x00; y=0x00; x2=0x0F; y2=0x0F;
	box(&x, &y, &x2, &y2, &LIGHTORANGE);	

	for(unsigned char player=0; player!=4; player++)
	{

		// check if player is active
		if( ( (*CatAndMouse.playerCount) & (0x01 << (player-1) ))==0 && player) continue;
		
		// set initial color
		CatAndMouse.player[player].color=GREEN;

		// set positions
		do
		{
			// set random position
			CatAndMouse.player[player].pos_x=rnd()&0x0F;
			CatAndMouse.player[player].pos_y=rnd()&0x0F;

			clamp(&CatAndMouse.player[player].pos_x, 0x01, 0x0E);	//clamp the positions to the gamebox
			clamp(&CatAndMouse.player[player].pos_y, 0x01, 0x0E);

			// player 0 doesn't need checking
			if(player==0) break;

			// check if position isn't colliding with other player
			for(x=0; x!=player; x++)
			{	
				// check if player is active
				if( ( (*CatAndMouse.playerCount) & (0x01 << (x-1) ))==0 && x) continue;
				
				if(CatAndMouse.player[player].pos_x==CatAndMouse.player[x].pos_x
					&& CatAndMouse.player[player].pos_y==CatAndMouse.player[x].pos_y) break;
			}

		}while(x!=player);
		
		//draw all players 
		dot(&CatAndMouse.player[player].pos_x, &CatAndMouse.player[player].pos_y, &CatAndMouse.player[player].color);
				
		CatAndMouse.player[player].old_pos_x=CatAndMouse.player[player].pos_x;		//initialise old position
		CatAndMouse.player[player].old_pos_y=CatAndMouse.player[player].pos_y;
	}

	//set a random player as the cat
	do
	{
		x=rnd()&0x03;

	}while( ( (*CatAndMouse.playerCount) & (0x01 << (x-1) ))==0 && x);	// check if player is active

	CatAndMouse.player[x].color=RED;
	dot(&CatAndMouse.player[x].pos_x, &CatAndMouse.player[x].pos_y, &CatAndMouse.player[x].color);

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

	unsigned char x;
	for(x=0;x!=4;x++)
	{
		// check if player is active
		if( ( (*CatAndMouse.playerCount) & (0x01 << (x-1) ))==0 && x) continue;

		// process input from all players
		if( globalPlayerButtonLeft( x ) )
		{
			if( CatAndMouse.player[x].pos_x > 0x01 ) CatAndMouse.player[x].pos_x--;
		}
		if( globalPlayerButtonRight( x ) )
		{
			if( CatAndMouse.player[x].pos_x < 0x0E ) CatAndMouse.player[x].pos_x++;
		}
		if( globalPlayerButtonUp( x ) )
		{
			if( CatAndMouse.player[x].pos_y > 0x01 ) CatAndMouse.player[x].pos_y--;
		}
		if( globalPlayerButtonDown( x ) )
		{
			if( CatAndMouse.player[x].pos_y < 0x0E ) CatAndMouse.player[x].pos_y++;
		}
		/*if( globalPlayerButtonFire( x ) )
		{
		}*/

		//I have to clear here all the positions otherwise dots with higher drawing priority are cleared
		if(CatAndMouse.player[x].old_pos_x!=CatAndMouse.player[x].pos_x)
		{
			dot(&CatAndMouse.player[x].old_pos_x, &CatAndMouse.player[x].old_pos_y, &BLACK);
		}

		if(CatAndMouse.player[x].old_pos_y!=CatAndMouse.player[x].pos_y)
		{
			dot(&CatAndMouse.player[x].old_pos_x, &CatAndMouse.player[x].old_pos_y, &BLACK);
		}	
	}

	// draw new positions and do other updates
	for(x=0;x!=4;x++)
	{
		
		// check if player is active
		if( ( (*CatAndMouse.playerCount) & (0x01 << (x-1) ))==0 && x) continue;
		
		CatAndMouse.player[x].pos_x&=0x0F;	//set player x on the other side of the matrix if player x cross the line
		CatAndMouse.player[x].pos_y&=0x0F;
		
		CatAndMouse.player[x].old_pos_x=CatAndMouse.player[x].pos_x;
		CatAndMouse.player[x].old_pos_y=CatAndMouse.player[x].pos_y;

		dot(&CatAndMouse.player[x].pos_x, &CatAndMouse.player[x].pos_y, &CatAndMouse.player[x].color);
	}

	for(x=0;x!=4;x++)
	{
		if(CatAndMouse.player[x].color == RED)
		{
			unsigned char y;
			for(y=0;y!=4;y++)
			{
				
				// check if player is active
				if( ( (*CatAndMouse.playerCount) & (0x01 << (y-1) ))==0 && y) continue;
				
				if(CatAndMouse.player[x].pos_x == CatAndMouse.player[y].pos_x && CatAndMouse.player[x].pos_y 
					== CatAndMouse.player[y].pos_y && CatAndMouse.player[y].color==GREEN)
				{
					CatAndMouse.player[y].color=RED;
					y=0;
				}
			}
		}
	}
	if( globalPlayer1ButtonMenu() ) endGame();	//if player 1 press the the Button Up and Down in the same time the game end and you will jump to the menu

	send();
}

// -----------------------------------------------------------------------
// draw cat and mouse menu icon
void drawCatAndMouseMenuIcon( void )
{
	unsigned char x, y, index = 0;
	for( y = 3; y != 13; y++ )
	{
		for( x = 3; x != 13; x++ )
		{
			dot( &x, &y, &catAndMouseIcon[index] );
			index++;
		}
	}
}
#endif // GAME_ENABLE_CAT_AND_MOUSE