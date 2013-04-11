// ----------------------------------------------------------------------
// Initialisations
// ----------------------------------------------------------------------

// header files
#include "init.h"
#include "uart.h"
#include "framework.h"
#include "common.h"
#include "menu.h"

// ------------------------------------------------------------------------------------------------------------------
// call this to initialise the device
void initDevice( void )
{

  	// Stop watchdog timer to prevent time out reset
	WDTCTL = WDTPW + WDTHOLD;

	// setup clock
	cfgSystemClock();

	// configure timers
	cfgTimerA();

	// configure ports
	cfgPort1();
	cfgPort2();
	cfgPort3();
	cfgPort4();
	cfgPort5();
	cfgPort6();

	// configure UART serial interface
	cfgUART();

	// initial values
	initUART();
	initFrameWork();
	initMenu();

	// enable global interrupts
	__bis_SR_register( GIE );
}

void cfgPort1( void )
{
	P1DIR = ~0x1F;
	P1SEL = 0x00;
}

void cfgPort2( void )
{
	P2DIR = ~0x1F;
	P2SEL = 0x00;
}

void cfgPort3( void )
{
	P3DIR = ~0x1F;
	P3SEL = 0xC0;
}

void cfgPort4( void )
{
	P4DIR = ~0x1F;
	P4DIR = 0x00;
}

void cfgPort5( void )
{
}

void cfgPort6( void )
{
}

void cfgSystemClock( void )
{

	// setup external clock (14.74560 MHz)
	BCSCTL1 &= ~0x80;			// Turn on XT2 oscillator
	BCSCTL3 |= 0x80;			// Select range 3-16 MHz
	BCSCTL2 |= 0x08;			// select XT2CLK
	BCSCTL2 &= ~0x06;			// divider to 1
}

void cfgTimerA( void )
{
	CCTL0 = CCIE;                             // CCR0 interrupt enabled
	CCR0 = 50000;
	TACTL = TASSEL_2 + MC_3 + ID_3;           // SMCLK, upmode, divide by 8
}

void cfgUART( void )
{

	// configure UART
	UCA1CTL1 |= UCSWRST;			// **Put state machine in reset**
	UCA1CTL1 |= 0xC0;			// Select SMCLK for BRCLK
	UCA1BR0 = 0x04;				// 14.7456 MHz divided by 0x04 = Baud 3'686'400
	UCA1BR1 = 0x00;
	UCA1CTL1 &= ~UCSWRST;			// **Initialise USCI state machine**

	// enable interrupts
	UC1IE |= UCA1RXIE;			// Enable receive interrupt

}
