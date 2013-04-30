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
	Tron.Player[1].X = 7;
	Tron.Player[1].Y = 0;
	Tron.Player[1].Direction = TRON_DIRECTION_DOWN;
	dot(&Tron.Player[1].X, &Tron.Player[1].Y, &GREEN);

  	// player 3  Start Position
	Tron.Player[2].X = 0;
	Tron.Player[2].Y = 8;
	Tron.Player[2].Direction = TRON_DIRECTION_RIGHT;
	dot(&Tron.Player[2].X, &Tron.Player[2].Y, &YELLOW);

  	// player 4  Start Position
	Tron.Player[3].X = 15;
	Tron.Player[3].Y = 7;
	Tron.Player[3].Direction = TRON_DIRECTION_LEFT;
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

unsigned char j;
for (j=0;j!=4;j++){        
	switch(Tron.Player[j].Direction){
	    case      TRON_DIRECTION_UP: 
	      Tron.Player[j].Y --;
	    break;
	    case      TRON_DIRECTION_DOWN: 
	      Tron.Player[j].Y ++;
	    break;
	    case      TRON_DIRECTION_LEFT: 
	      Tron.Player[j].X --;
	    break;
	    case      TRON_DIRECTION_RIGHT: 
	      Tron.Player[j].X ++;
	    break;
	}

  
    	Tron.Player[j].X &= 0x0F;
    	Tron.Player[j].Y &= 0x0F;

	//Speed

	if(Tron.Speed != 80)
	Tron.Speed ++;

	setRefreshRate( Tron.Speed );

	//Collision
	if( (*(Tron.frameBuffer+Tron.Player[j].Y+(Tron.Player[j].X<<4))) & (1<<j) ){
		

	}
	

	(*(Tron.frameBuffer+Tron.Player[j].Y+(Tron.Player[j].X<<4))) |= (1<<j);

}   
	// set new position to frame buffer Player 1
	*(Tron.frameBuffer+Tron.Player[0].Y+(Tron.Player[0].X<<4)) |=  1;
        dot( &Tron.Player[0].X, &Tron.Player[0].Y, &MAGENTA);
        send();

	// set new position to frame buffer Player 2
	*(Tron.frameBuffer+Tron.Player[1].Y+(Tron.Player[1].X<<4)) |=  1;
        dot( &Tron.Player[1].X, &Tron.Player[1].Y, &GREEN);
        send();

	// set new position to frame buffer Player 3
	*(Tron.frameBuffer+Tron.Player[2].Y+(Tron.Player[2].X<<4)) |=  1;
        dot( &Tron.Player[2].X, &Tron.Player[2].Y, &YELLOW);
        send();

	// set new position to frame buffer Player 4
	*(Tron.frameBuffer+Tron.Player[3].Y+(Tron.Player[3].X<<4)) |=  1;
        dot( &Tron.Player[3].X, &Tron.Player[3].Y, &BLUE);
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


         if (player2ButtonLeft())
          Tron.Player[1].Direction = TRON_DIRECTION_RIGHT;
        
        if (player2ButtonRight())
          Tron.Player[1].Direction = TRON_DIRECTION_LEFT;
        
        if (player2ButtonUp())
          Tron.Player[1].Direction = TRON_DIRECTION_DOWN;        

        if (player2ButtonDown())
          Tron.Player[1].Direction = TRON_DIRECTION_UP; 


         if (player3ButtonLeft())
          Tron.Player[2].Direction = TRON_DIRECTION_UP;
        
        if (player3ButtonRight())
          Tron.Player[2].Direction = TRON_DIRECTION_DOWN;
        
        if (player3ButtonUp())
          Tron.Player[2].Direction = TRON_DIRECTION_RIGHT;        

        if (player3ButtonDown())
          Tron.Player[2].Direction = TRON_DIRECTION_LEFT; 

         if (player4ButtonLeft())
          Tron.Player[3].Direction = TRON_DIRECTION_DOWN;
        
        if (player4ButtonRight())
          Tron.Player[3].Direction = TRON_DIRECTION_UP;
        
        if (player4ButtonUp())
          Tron.Player[3].Direction = TRON_DIRECTION_LEFT;        

        if (player4ButtonDown())
          Tron.Player[3].Direction = TRON_DIRECTION_RIGHT; 
	

	// end game with menu button
	if( player1ButtonMenu() ) endGame();
}

// ----------------------------------------------------------------------
// draws the menu icon for tron
void drawTronMenuIcon( void )
{
	unsigned char x1=4, y1=12, x2=4, y2=4;
	line( &x1, &y1, &x2, &y2, &RED );
	x1=7; y1=4;
	line( &x2, &y2, &x1, &y2, &RED );
	x2=7; y2=6;
	line( &x1, &y1, &x2, &y2, &RED );
	x1=12; y1=8; x2=6; y2=8;
	line( &x1, &y1, &x2, &y2, &GREEN );
	x1=3; x2=12;
	box( &x1, &x1, &x2, &x2, &PURPLE );
}
#endif // GAME_ENABLE_TRON
