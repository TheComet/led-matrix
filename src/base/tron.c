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
   
        dot( &Tron.Pos_X, &Tron.Pos_Y, &BLUE);
        dot( &Tron.Pos_X_Hinten, &Tron.Pos_Y_Hinten, &BLACK);
        send();       
}

// ----------------------------------------------------------------------
// process tron input
void processTronInput( void )
{
 
         if (player1ButtonLeft())
          Tron.Pos_X = Tron.Pos_X-1;
        
        if (player1ButtonRight())
          Tron.Pos_X = Tron.Pos_X+1;
        
        if (player1ButtonUp())
          Tron.Pos_X = Tron.Pos_Y+1;        

        if (player1ButtonDown())
          Tron.Pos_X = Tron.Pos_Y-1;  

	// end game with menu button
	if( player1ButtonMenu() ) endGame();
}
#endif // GAME_ENABLE_TRON
