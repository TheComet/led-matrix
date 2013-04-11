// ----------------------------------------------------------------------
// Framework
// ----------------------------------------------------------------------

#ifndef _FRAMEWORK_H_
	#define _FRAMEWORK_H_

// ----------------------------------------------------------------------
// global constants
// ----------------------------------------------------------------------

// common colours
static const unsigned short BLACK     = 0x000;
static const unsigned short WHITE     = 0xEEE;
static const unsigned short RED       = 0xE00;
static const unsigned short GREEN     = 0x0E0;
static const unsigned short BLUE      = 0x00E;
static const unsigned short YELLOW    = 0xEE0;
static const unsigned short MAGENTA   = 0xE0E;
static const unsigned short LIGHTBLUE = 0x0EE;
static const unsigned short PINK      = 0xE07;
static const unsigned short PURPLE    = 0x70E;
static const unsigned short ORANGE    = 0xE70;

// states
#define ZERO 0
#define ONE 1

// sin() lookup table
static const signed char sinus[30] = { 0x0,0x1A,0x33,0x4A,0x5E,0x6D,0x78,0x7E,0x7E,0x78,0x6D,0x5E,0x4A,0x33,0x1A,0x0,0xE6,0xCD,0xB6,0xA2,0x93,0x88,0x82,0x82,0x88,0x93,0xA2,0xB6,0xCD,0xE6 };

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct FrameWork_Buttons_t
{
	unsigned char oldButtonState;
	unsigned char buttonState;
	unsigned char buttonPositiveEdge;
	unsigned char menuButtonTracker;
};

struct FrameWork_t
{
	struct FrameWork_Buttons_t player[4];
	unsigned char updateDivider;
	unsigned char updateCounter;
	unsigned char updateFlag;
	unsigned char state;
	unsigned char frameBuffer[256];
	unsigned short randomSeed;
	unsigned char menuButtonFlags;
};

// ----------------------------------------------------------------------
// Enumerators
// ----------------------------------------------------------------------

// states
enum FrameWork_State_e
{
	FRAMEWORK_STATE_MENU,
	FRAMEWORK_STATE_START_UP_SCREEN,
	FRAMEWORK_STATE_SNAKE,
	FRAMEWORK_STATE_COLOUR_DEMO,
	FRAMEWORK_STATE_GAME_OF_LIFE
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void initFrameWork( void );
void startFrameWork( void );
void pollPorts( void );
void frameWorkUpdateProcessLoop( void );
void frameWorkUpdateInputLoop( void );

// change applications
void startColourDemo( unsigned char* playerCount );
void startSnake( unsigned char* playerCount );
void startGameOfLife( unsigned char* playerCount );
void endGame( void );

// misc
void setRefreshRate( unsigned char refresh );
void clearFrameBuffer( unsigned char* frameBuffer );
unsigned char rnd( void );
extern inline signed char sin( unsigned short angle );
extern inline void wrap( unsigned short* value, unsigned char wrap );

// player specific input
extern inline unsigned char player1ButtonFire( void );
extern inline unsigned char player1ButtonLeft( void );
extern inline unsigned char player1ButtonRight( void );
extern inline unsigned char player1ButtonUp( void );
extern inline unsigned char player1ButtonDown( void );
extern inline unsigned char player1ButtonMenu( void );
extern inline unsigned char player1ButtonClear( void );

extern inline unsigned char player2ButtonFire( void );
extern inline unsigned char player2ButtonLeft( void );
extern inline unsigned char player2ButtonRight( void );
extern inline unsigned char player2ButtonUp( void );
extern inline unsigned char player2ButtonDown( void );
extern inline unsigned char player2ButtonMenu( void );
extern inline unsigned char player2ButtonClear( void );

extern inline unsigned char player3ButtonFire( void );
extern inline unsigned char player3ButtonLeft( void );
extern inline unsigned char player3ButtonRight( void );
extern inline unsigned char player3ButtonUp( void );
extern inline unsigned char player3ButtonDown( void );
extern inline unsigned char player3ButtonMenu( void );
extern inline unsigned char player3ButtonClear( void );

extern inline unsigned char player4ButtonFire( void );
extern inline unsigned char player4ButtonLeft( void );
extern inline unsigned char player4ButtonRight( void );
extern inline unsigned char player4ButtonUp( void );
extern inline unsigned char player4ButtonDown( void );
extern inline unsigned char player4ButtonMenu( void );
extern inline unsigned char player4ButtonClear( void );

// player general input
unsigned char playerButtonFire( unsigned char playerID );
unsigned char playerButtonLeft( unsigned char playerID );
unsigned char playerButtonRight( unsigned char playerID );
unsigned char playerButtonUp( unsigned char playerID );
unsigned char playerButtonDown( unsigned char playerID );
extern inline unsigned char playerButtonMenu( unsigned char playerID );
extern inline unsigned char playerButtonClear( unsigned char playerID );

#endif // _FRAMEWORK_H_