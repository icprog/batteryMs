#ifndef _SYSTEM_STATE_H_
#define _SYSTEM_STATE_H_
#include "public/types.h"

struct s_state_check
{
	int state;
	int (*update)(void);
	int (*notify)(void);
	int (*getState)(void);
};


enum system_mode
{
	SYSTEM_MODE_DISCHARGE = 1<<1,
	SYSTEM_MODE_DC_CHARGE = 1<<2,
	SYSTEM_MODE_AC_CHARGE = 1<<3,
	SYSTEM_MODE_DEEPSLEEP = 1<<4, // 16
	SYSTEM_MODE_SLEEP = 1<<5, //32
	SYSTEM_MODE_WAKEUPSELF = 1<<6
};

static const char* system_mode_name[]=
{
		"SYSTEM_MODE_DISCHARGE",
		"SYSTEM_MODE_DC_CHARGE",
		"SYSTEM_MODE_AC_CHARGE",
		"SYSTEM_MODE_DEEPSLEEP",
		"SYSTEM_MODE_SLEEP"	
};

static const char *modeToStr(int mode)
{
	UINT8 i = 0;
//	assert(mode<=SYSTEM_MODE_SLEEP);
	switch(mode)
	{
		case SYSTEM_MODE_DISCHARGE:
		{
			i = 0;
			break;
		}
		case SYSTEM_MODE_DC_CHARGE:
		{
			i = 1;
			break;
		}
		case SYSTEM_MODE_AC_CHARGE:
		{
			i = 2;
			break;
		}
		case SYSTEM_MODE_DEEPSLEEP:
		{
			i = 3;
			break;
		}
		case SYSTEM_MODE_SLEEP:
		{
			i = 4;
			break;
		}
		default:
			break;
	}
	return system_mode_name[i];
}

//#define modeToStr(mode) system_mode_name[mode]
int getSystemState(void);
int SystemStateMachine(UINT32 tnow);
int IsTimeOfReposeEnough(void);
//int SetSystemMode(UINT8 mode);

#endif

