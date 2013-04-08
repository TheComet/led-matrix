// ----------------------------------------------------------------------
// Framework
// ----------------------------------------------------------------------

#ifndef _FRAMEWORK_H_
	#define _FRAMEWORK_H_

// ----------------------------------------------------------------------
// Structs
// ----------------------------------------------------------------------

struct FrameWork_Buttons_t
{
	unsigned char oldButtonState;
	unsigned char buttonState;
	unsigned char buttonPositiveEdge;
};

struct FrameWork_t
{
	struct FrameWork_Buttons_t player[4];
	unsigned char updateDivider;
	unsigned char updateCounter;
	unsigned char updateFlag;
	unsigned char state;
	unsigned char frameBuffer[256];
};

// ----------------------------------------------------------------------
// Enumerators
// ----------------------------------------------------------------------

// states
enum FrameWork_State_e
{
	FRAMEWORK_STATE_LOAD_MENU,
	FRAMEWORK_STATE_MENU,
	FRAMEWORK_STATE_LOAD_SNAKE,
	FRAMEWORK_STATE_SNAKE,
	FRAMEWORK_STATE_LOAD_COLOUR_DEMO,
	FRAMEWORK_STATE_COLOUR_DEMO,
	FRAMEWORK_STATE_LOAD_GAME_OF_LIFE,
	FRAMEWORK_STATE_GAME_OF_LIFE
};

// ----------------------------------------------------------------------
// Function Prototypes
// ----------------------------------------------------------------------

void initFrameWork( void );
void startFrameWork( void );
void pollPorts( void );
void clearPorts( void );
void frameWorkUpdate( void );
void setRefreshRate( unsigned char refresh );

void startColourDemo( void );
void startSnake( void );
void startGameOfLife( void );
void endGame( void );

extern inline unsigned char player1ButtonFire( void );
extern inline unsigned char player1ButtonLeft( void );
extern inline unsigned char player1ButtonRight( void );
extern inline unsigned char player1ButtonUp( void );
extern inline unsigned char player1ButtonDown( void );
extern inline unsigned char player2ButtonFire( void );
extern inline unsigned char player2ButtonLeft( void );
extern inline unsigned char player2ButtonRight( void );
extern inline unsigned char player2ButtonUp( void );
extern inline unsigned char player2ButtonDown( void );
extern inline unsigned char player3ButtonFire( void );
extern inline unsigned char player3ButtonLeft( void );
extern inline unsigned char player3ButtonRight( void );
extern inline unsigned char player3ButtonUp( void );
extern inline unsigned char player3ButtonDown( void );
extern inline unsigned char player4ButtonFire( void );
extern inline unsigned char player4ButtonLeft( void );
extern inline unsigned char player4ButtonRight( void );
extern inline unsigned char player4ButtonUp( void );
extern inline unsigned char player4ButtonDown( void );

#endif // _FRAMEWORK_H_