// ----------------------------------------------------------------------
// Framework
// ----------------------------------------------------------------------

#ifndef _FRAMEWORK_H_
	#define _FRAMEWORK_H_

#include "moduleenable.h"

// ----------------------------------------------------------------------
// global constants
// ----------------------------------------------------------------------

// common colours
static const unsigned short BLACK      = 0x000;
static const unsigned short WHITE      = 0xEEE;
static const unsigned short RED        = 0xE00;
static const unsigned short GREEN      = 0x0E0;
static const unsigned short BLUE       = 0x00E;
static const unsigned short YELLOW     = 0xEE0;
static const unsigned short MAGENTA    = 0xE0E;
static const unsigned short LIGHTBLUE  = 0x0EE;
static const unsigned short PINK       = 0xE07;
static const unsigned short PURPLE     = 0x70E;
static const unsigned short ORANGE     = 0xE70;
static const unsigned short LIGHTGREEN = 0x7E0;
static const unsigned short BLUEGREEN  = 0x0E7;
static const unsigned short LIGHTYELLOW= 0xEE7;

// states
#define ZERO 0
#define ONE 1

// sin() lookup table
static const signed char sinus[30] = { 0x0,0x1A,0x33,0x4A,0x5E,0x6D,0x78,0x7E,0x7E,0x78,0x6D,0x5E,0x4A,0x33,0x1A,0x0,0xE6,0xCD,0xB6,0xA2,0x93,0x88,0x82,0x82,0x88,0x93,0xA2,0xB6,0xCD,0xE6 };

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

// for callback registration
typedef void (*loadFunction_cb_t)(unsigned short* frameBuffer, unsigned char* userData);
typedef void (*processLoopFunction_cb_t)(void);
typedef void (*processInputFunction_cb_t)(void);
typedef void (*drawMenuIconFunction_cb_t)(void);
struct FrameWork_Registered_Games_t
{
	loadFunction_cb_t load;
	processLoopFunction_cb_t processLoop;
	processInputFunction_cb_t processInput;
	drawMenuIconFunction_cb_t drawMenuIcon;
};

// for input
struct FrameWork_Buttons_t
{
	unsigned char oldButtonState;
	unsigned char buttonState;
	unsigned char buttonPositiveEdge;
	unsigned char menuButtonTracker;
};

// Framework struct
struct FrameWork_t
{

	// input
	struct FrameWork_Buttons_t player[4];
	unsigned char menuButtonFlags;

	// game registration
	struct FrameWork_Registered_Games_t game[MAX_MODULES];
	unsigned char gamesRegistered;

	// frame rate
	volatile unsigned char updateDivider;
	volatile unsigned char updateCounter;
	volatile unsigned char updateFlag;

	// misc
	unsigned short frameBuffer[256];
	unsigned short randomSeed;
	unsigned char gameSelected;
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

// framework specific functions, which shouldn't be called
// by anything else other than the framework
void initFrameWork( void );
void startFrameWork( void );
void pollPorts( void );
void frameWorkUpdateProcessLoop( void );
void frameWorkUpdateInputLoop( void );
void menuUpdateIcon( unsigned char* selected );

// used to register a game to the framework for callbacks
void registerModule( loadFunction_cb_t loadFunction, processLoopFunction_cb_t processLoopFunction, processInputFunction_cb_t processInputFunction, drawMenuIconFunction_cb_t drawMenuIconFunction );

// change applications
void startGame( unsigned char* gameSelected, unsigned char* playerCount );
void endGame( void );

// misc
void setRefreshRate( unsigned char refresh );
void clearFrameBuffer( unsigned short* frameBuffer );
unsigned char rnd( void );
extern inline signed char sin( unsigned short angle );
extern inline void wrap( unsigned short* value, unsigned char wrap );
extern inline void clamp( unsigned char* value, unsigned char lower, unsigned char higher );
unsigned char sqrt( unsigned short* value );

// player specific input with local orientation
extern inline unsigned char localPlayer1ButtonFire( void );
extern inline unsigned char localPlayer1ButtonLeft( void );
extern inline unsigned char localPlayer1ButtonRight( void );
extern inline unsigned char localPlayer1ButtonUp( void );
extern inline unsigned char localPlayer1ButtonDown( void );
extern inline unsigned char localPlayer1ButtonMenu( void );
extern inline unsigned char localPlayer1ButtonClear( void );

extern inline unsigned char localPlayer2ButtonFire( void );
extern inline unsigned char localPlayer2ButtonLeft( void );
extern inline unsigned char localPlayer2ButtonRight( void );
extern inline unsigned char localPlayer2ButtonUp( void );
extern inline unsigned char localPlayer2ButtonDown( void );
extern inline unsigned char localPlayer2ButtonMenu( void );
extern inline unsigned char localPlayer2ButtonClear( void );

extern inline unsigned char localPlayer3ButtonFire( void );
extern inline unsigned char localPlayer3ButtonLeft( void );
extern inline unsigned char localPlayer3ButtonRight( void );
extern inline unsigned char localPlayer3ButtonUp( void );
extern inline unsigned char localPlayer3ButtonDown( void );
extern inline unsigned char localPlayer3ButtonMenu( void );
extern inline unsigned char localPlayer3ButtonClear( void );

extern inline unsigned char localPlayer4ButtonFire( void );
extern inline unsigned char localPlayer4ButtonLeft( void );
extern inline unsigned char localPlayer4ButtonRight( void );
extern inline unsigned char localPlayer4ButtonUp( void );
extern inline unsigned char localPlayer4ButtonDown( void );
extern inline unsigned char localPlayer4ButtonMenu( void );
extern inline unsigned char localPlayer4ButtonClear( void );

// player general input with local orientation
unsigned char localPlayerButtonFire( unsigned char playerID );
unsigned char localPlayerButtonLeft( unsigned char playerID );
unsigned char localPlayerButtonRight( unsigned char playerID );
unsigned char localPlayerButtonUp( unsigned char playerID );
unsigned char localPlayerButtonDown( unsigned char playerID );
extern inline unsigned char localPlayerButtonMenu( unsigned char playerID );
extern inline unsigned char localPlayerButtonClear( unsigned char playerID );

// player specific input with global orientation
extern inline unsigned char globalPlayer1ButtonFire( void );
extern inline unsigned char globalPlayer1ButtonLeft( void );
extern inline unsigned char globalPlayer1ButtonRight( void );
extern inline unsigned char globalPlayer1ButtonUp( void );
extern inline unsigned char globalPlayer1ButtonDown( void );
extern inline unsigned char globalPlayer1ButtonMenu( void );
extern inline unsigned char globalPlayer1ButtonClear( void );

extern inline unsigned char globalPlayer2ButtonFire( void );
extern inline unsigned char globalPlayer2ButtonLeft( void );
extern inline unsigned char globalPlayer2ButtonRight( void );
extern inline unsigned char globalPlayer2ButtonUp( void );
extern inline unsigned char globalPlayer2ButtonDown( void );
extern inline unsigned char globalPlayer2ButtonMenu( void );
extern inline unsigned char globalPlayer2ButtonClear( void );

extern inline unsigned char globalPlayer3ButtonFire( void );
extern inline unsigned char globalPlayer3ButtonLeft( void );
extern inline unsigned char globalPlayer3ButtonRight( void );
extern inline unsigned char globalPlayer3ButtonUp( void );
extern inline unsigned char globalPlayer3ButtonDown( void );
extern inline unsigned char globalPlayer3ButtonMenu( void );
extern inline unsigned char globalPlayer3ButtonClear( void );

extern inline unsigned char globalPlayer4ButtonFire( void );
extern inline unsigned char globalPlayer4ButtonLeft( void );
extern inline unsigned char globalPlayer4ButtonRight( void );
extern inline unsigned char globalPlayer4ButtonUp( void );
extern inline unsigned char globalPlayer4ButtonDown( void );
extern inline unsigned char globalPlayer4ButtonMenu( void );
extern inline unsigned char globalPlayer4ButtonClear( void );

// player general input with global orientation
unsigned char globalPlayerButtonFire( unsigned char playerID );
unsigned char globalPlayerButtonLeft( unsigned char playerID );
unsigned char globalPlayerButtonRight( unsigned char playerID );
unsigned char globalPlayerButtonUp( unsigned char playerID );
unsigned char globalPlayerButtonDown( unsigned char playerID );
extern inline unsigned char globalPlayerButtonMenu( unsigned char playerID );
extern inline unsigned char globalPlayerButtonClear( unsigned char playerID );

#endif // _FRAMEWORK_H_
