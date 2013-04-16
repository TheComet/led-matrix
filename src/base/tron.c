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
  
  // player 1  Start Position
  Tron.Pos_X = 8;
  Tron.Pos_Y = 15;
  Tron.Direction = TRON_DIRECTION_UP;  
  
     
	// set up screen
	cls();
	send();

	// set refresh rate
	setRefreshRate( 24 );
}

// ----------------------------------------------------------------------
// process tron loop
void processTronLoop( void )
{
        
switch(Tron.Direction){
    case      TRON_DIRECTION_UP: 
      Tron.Pos_Y --;
    break;
    case      TRON_DIRECTION_DOWN: 
      Tron.Pos_Y ++;
    break;
    case      TRON_DIRECTION_LEFT: 
      Tron.Pos_X --;
    break;
    case      TRON_DIRECTION_RIGHT: 
      Tron.Pos_X ++;
    break;
    
  }
  
    Tron.Pos_X &= 0x0F;
    Tron.Pos_Y &= 0x0F;
   
        unsigned short colour = 0x00E, clearColour = 0;
        dot( &Tron.Pos_X, &Tron.Pos_Y, &colour);
        send();
}

// ----------------------------------------------------------------------
// process tron input
void processTronInput( void )
{
 
         if (player1ButtonLeft())
          Tron.Direction = TRON_DIRECTION_LEFT;
        
        if (player1ButtonRight())
          Tron.Direction = TRON_DIRECTION_RIGHT;
        
        if (player1ButtonUp())
          Tron.Direction  = TRON_DIRECTION_UP;        

        if (player1ButtonDown())
          Tron.Direction  = TRON_DIRECTION_DOWN; 

	// end game with menu button
	if( player1ButtonMenu() ) endGame();
}
#endif // GAME_ENABLE_TRON
