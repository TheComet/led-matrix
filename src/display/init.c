// ----------------------------------------------------------------------
// Initialisations
// ----------------------------------------------------------------------

// header files
#include "common.h"
#include "init.h"
#include "uart.h"
#include "drawUtils.h"

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

	// configure UART serial interface
	cfgUART();

	// initialise some variables
	UART.commandBufferWritePtr = 0;
	UART.commandBufferReadPtr = 0;
	UART.timeToProcessCommandBuffer = 0;
	drawUtils.blendMode = BLEND_MODE_REPLACE;

	// enable global interrupts
	__bis_SR_register(GIE);
}

void cfgPort1( void )
{
	P1SEL = 0x06;			// select TxD and RxD
}

void cfgPort2( void )
{
	P2DIR |= 0x1F;
	P2SEL = 0xE0;
	P2OUT = 0x00;
}

void cfgPort3( void )
{
	P3DIR |= 0xFF;
	P3SEL = 0x00;
	ROW_EN = ENABLE_ROW;
}

void cfgSystemClock( void )
{

	// set up internal clock
	UCSCTL3 |= SELREF_2;				// Set DCO FLL reference = REFO
	UCSCTL4 |= SELA_2;				// Set ACLK = REFO

	__bis_SR_register(SCG0);			// Disable the FLL control loop
	UCSCTL0 = 0x0000;				// Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_5;				// Select DCO range 24MHz operation
	UCSCTL2 = 0x0300;				// Set DCO Multiplier for 30MHz
	
	__bic_SR_register(SCG0);			// re-enable the FLL control loop   
	
	// Worst-case settling time for the DCO when the DCO range bits have been
	// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
	// UG for optimization.
	// 32 x 32 x 12 MHz / 32,768 Hz = 375000 = MCLK cycles for DCO to settle
	__delay_cycles(375000);

	// Loop until XT1 & DCO fault flag is cleared
	do
	{
		UCSCTL7 &= ~(XT1LFOFFG + XT1HFOFFG + DCOFFG);
							// Clear XT1,DCO fault flags
		SFRIFG1 &= ~OFIFG;			// Clear fault flags
	}while (SFRIFG1&OFIFG);				// Test oscillator fault flag
}

void cfgTimerA( void )
{
	TA0CCTL0 = CCIE;				// CCR0 interrupt enabled
	TA0CCR0 = 35000;
	TA0CTL = TASSEL_2 + MC_1 + TACLR + 0x00C0;	// SMCLK, upmode, clear TAR
}

void cfgUART( void )
{

	// configure UART
	UCA0CTL1 |= UCSWRST;				// **Put state machine in reset**
	UCA0CTL1 |= UCSSEL_2;				// SMCLK (29088000)
	UCA0BR0 = 0xDD;					// 0xDD for Baud 115'200
	UCA0BR1 = 0x00;
	UCA0MCTL |= UCBRS_1 + UCBRF_0;			// Modulation UCBRSx=1, UCBRFx=0
	UCA0CTL1 &= ~UCSWRST;				// **Initialize USCI state machine**
	UCA0IE |= UCRXIE;				// Enable USCI_A0 RX interrupt
}