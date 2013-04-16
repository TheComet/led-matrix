// ----------------------------------------------------------------------
// Tron
// ----------------------------------------------------------------------

// ----------------------------------------------------------------------
// Include files
// ----------------------------------------------------------------------

#include "tron.h"
#include "framework.h"
#include "uart.h"
#include "gameenable.h"

#ifdef GAME_ENABLE_TRON

static struct Tron_t Tron;

// ----------------------------------------------------------------------
// load tron
void loadTron( unsigned short* frameBuffer, unsigned char* playerCount )
{
	//Clean Screen
	cls();

	// get frame buffer
	Tron.frameBuffer = frameBuffer;

  	// player 1  Start Position
	Tron.Player[0].X = 8;
	Tron.Player[0].Y = 15;
	Tron.Player[0].Direction = TRON_DIRECTION_UP;
	dot(&Tron.Player[0].X, &Tron.Player[0].Y, &MAGENTA);

  	// player 2  Start Position
	Tron.Player[1].X = 15;
	Tron.Player[1].Y = 8;
	Tron.Player[1].Direction = TRON_DIRECTION_UP;
	dot(&Tron.Player[1].X, &Tron.Player[1].Y, &GREEN);

  	// player 3  Start Position
	Tron.Player[2].X = 8;
	Tron.Player[2].Y = 0;
	Tron.Player[2].Direction = TRON_DIRECTION_UP;
	dot(&Tron.Player[2].X, &Tron.Player[2].Y, &YELLOW);

  	// player 4  Start Position
	Tron.Player[3].X = 0;
	Tron.Player[3].Y = 8;
	Tron.Player[3].Direction = TRON_DIRECTION_UP;
	dot(&Tron.Player[3].X, &Tron.Player[3].Y, &BLUE);
  
     
	// set up screen
	send();

	// set refresh rate
	Tron.Speed = 12;
	setRefreshRate( Tron.Speed );
}

// ----------------------------------------------------------------------
// process tron loop
void processTronLoop( void )
{
        
switch(Tron.Player[0].Direction){
    case      TRON_DIRECTION_UP: 
      Tron.Player[0].Y --;
    break;
    case      TRON_DIRECTION_DOWN: 
      Tron.Player[0].Y ++;
    break;
    case      TRON_DIRECTION_LEFT: 
      Tron.Player[0].X --;
    break;
    case      TRON_DIRECTION_RIGHT: 
      Tron.Player[0].X ++;
    break;
}
switch(Tron.Player[1].Direction){
    case      TRON_DIRECTION_UP: 
      Tron.Player[1].Y --;
    break;
    case      TRON_DIRECTION_DOWN: 
      Tron.Player[1].Y ++;
    break;
    case      TRON_DIRECTION_LEFT: 
      Tron.Player[1].X --;
    break;
    case      TRON_DIRECTION_RIGHT: 
      Tron.Player[1].X ++;
    break;
    
  }
  
    Tron.Player[0].X &= 0x0F;
    Tron.Player[0].Y &= 0x0F;

	//Speed

	if(Tron.Speed != 80)
	Tron.Speed ++;

	setRefreshRate( Tron.Speed );
   
	// set new position to frame buffer
	*(Tron.frameBuffer+Tron.Player[0].Y+(Tron.Player[0].X<<4)) |=  1;
        dot( &Tron.Player[0].X, &Tron.Player[0].Y, &MAGENTA);
        send();
}

// ----------------------------------------------------------------------
// process tron input
void processTronInput( void )
{
 
         if (player1ButtonLeft())
          Tron.Player[0].Direction = TRON_DIRECTION_LEFT;
        
        if (player1ButtonRight())
          Tron.Player[0].Direction = TRON_DIRECTION_RIGHT;
        
        if (player1ButtonUp())
          Tron.Player[0].Direction = TRON_DIRECTION_UP;        

        if (player1ButtonDown())
          Tron.Player[0].Direction = TRON_DIRECTION_DOWN; 

	// end game with menu button
	if( player1ButtonMenu() ) endGame();
}
#endif // GAME_ENABLE_TRON
