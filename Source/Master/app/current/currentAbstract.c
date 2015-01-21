#include "current/currentAbstract.h"
#include "current/current.h"
#include "public/systemState.h"
#include "driver/Gpio/port_defs.h"
#include "public/public.h"
#include "public/ParameterManager.h"

struct c_current
{
	TYPE_CURRENT current;
	UINT8 AdPort;
};

struct CCurrent
{
	TYPE_CURRENT mCurrentCurrent;
	TYPE_CURRENT mSimulatorCurrent;
	UINT8 mCurrentStatus;
	UINT8 simulatorEnable;
	UINT32 timer_OC;
};

struct CCurrentsMember
{
	struct CCurrent mCurrents[TOTAL_CURRENTS];
	UINT8 currentDirect;
};
/*********************************************************************************
 * private gloable value
 * 
 * ******************************************************************************/
#define SET_CURRENT_CONSTVALUE(LEVEL1,LEVEL2,adc) {LEVEL1,LEVEL2,adc}
#define gThresholdValue gParameters.e_cur
static struct CCurrentsMember gCurrent;

/*
 * functions 
 * */
static int setCurrentValueById(int id, TYPE_CURRENT current)
{
	assert(id < TOTAL_CURRENTS);
	gCurrent.mCurrents[id].mCurrentCurrent = current;
	return 0;
}

static UINT16 getmvByAdId(int ad_id)
{
	UINT16 mv = 0;
	if (DriverRead(FD_AD_BY_INDEX, (char *) &mv, sizeof(UINT16), ad_id) < 0)
	{
		; //TODO err
	}
	return mv;
}

static struct CCurrent *getModleCurrentById(int id)
{
	assert(id<TOTAL_CURRENTS);
	return &gCurrent.mCurrents[id];
}

static struct CCurrentThresholdValue *getConstCurrentThById(int id)
{
	assert(id<TOTAL_CURRENTS);
	return &gThresholdValue.mCurrentThreshold[id];
}

static int setCurrentStateByEnvAndId(TYPE_CURRENT curReal,
		TYPE_CURRENT curMaxAllowedTh, int id)
{
	assert(id<TOTAL_CURRENTS);

	if (curReal
			> curMaxAllowedTh * getConstCurrentThById(id)->mThreatholdOCLEVEL1
					/ 100)  //discharge current too big
	{
		getModleCurrentById(id)->mCurrentStatus = SYSTEM_FAULT_LEVEL1;
	}
	else if (curReal
			> curMaxAllowedTh
					* getConstCurrentThById(id)->mThreatholdOCLEVEL2 / 100)
	{
		getModleCurrentById(id)->mCurrentStatus = SYSTEM_FAULT_LEVEL2;
	}
	else
	{
		getModleCurrentById(id)->mCurrentStatus = SYSTEM_FAULT_NONE;
	}
	return getModleCurrentById(id)->mCurrentStatus;
}

static void updateCurrentStatusValue(int SysState, TYPE_CURRENT CurBusCur)
{

	TYPE_CURRENT max_allowed_bus_cur = getMaxAllowedCurrent();
	TYPE_CURRENT max_allowed_fd_cur = BMS_GetMaxAllowedFeedbackCurrent();

	if (SYSTEM_MODE_DISCHARGE == SysState)
	{
		setCurrentStateByEnvAndId(CurBusCur, max_allowed_bus_cur,
				 CURRENT_ID_DISCHARGE);
		setCurrentStateByEnvAndId(CurBusCur, max_allowed_fd_cur,
				 CURRENT_ID_FEEDBACK);
	}

	else if (SYSTEM_MODE_AC_CHARGE == SysState
			|| SYSTEM_MODE_DC_CHARGE == SysState)
	{
		if (CurBusCur < 0)
			CurBusCur = 0 - CurBusCur;
		setCurrentStateByEnvAndId(CurBusCur, max_allowed_bus_cur,
				CURRENT_ID_CHARGE);
	}

//	for (i = 0; i < TOTAL_CURRENTS; i++)
//	{
////		pthresholdValue = &gParameters.e_cur.mCurrentThreshold[i];
//		pthresholdValue = &gThresholdValue.mCurrentThreshold[i];
//		this = &gCurrent.mCurrents[i];
//		if (this->mCurrentCurrent >= pthresholdValue->mThreatholdOCLEVEL1 )//* max_allowed_current) //TODO
//		{
//			this->mCurrentStatus = SYSTEM_FAULT_LEVEL1;
//		}
//		else if (this->mCurrentCurrent >= pthresholdValue->mThreatholdOCLEVEL2)// * max_allowed_current) //TODO
//		{
//			this->mCurrentStatus = SYSTEM_FAULT_LEVEL2;
//		}
//		else
//		{
//			this->mCurrentStatus = SYSTEM_FAULT_NONE;
//		}
//	}
}

int getCurrentDirection(void)
{
	return gCurrent.currentDirect;
}

static int setCurrentDirection(UINT8 dir)
{
	gCurrent.currentDirect = dir;
	return 0;
}

static TYPE_CURRENT getCurrentById(int id)
{
	assert(id<TOTAL_CURRENTS);
	return gCurrent.mCurrents[id].mCurrentCurrent;
}

static int getCurrentStatusById(int id)
{
	assert(id<TOTAL_CURRENTS);
	return gCurrent.mCurrents[id].mCurrentStatus;
}

static CURRENT_TYPE getCurrentBymV(UINT16 mv, float k)
{
	CURRENT_TYPE current = 0;
	double v = mv * 0.001;
	current = (CURRENT_TYPE) (v / k);
	return (current);
}

static CURRENT_TYPE get_current_discharge(void)
{
	return (getCurrentById(CURRENT_ID_DISCHARGE));
}

static CURRENT_TYPE get_current_charge(void)
{
	return (getCurrentById(CURRENT_ID_CHARGE));
}


static int updataFeedbackCurrentValue(int SysState)
{
	TYPE_CURRENT current = 0;
	if( SysState == SYSTEM_MODE_DISCHARGE )
	{
		current = getCurrentById(CURRENT_ID_DISCHARGE);
	}
	else
	{
		current = 0;
	}
	
	setCurrentValueById(CURRENT_ID_FEEDBACK, current);
	return current;
}

static TYPE_CURRENT updataBusCurrentValue(int SysState)
{

	TYPE_CURRENT currentCharge = 0, currentDis = 0;
	currentDis = getCurrentById(CURRENT_ID_DISCHARGE);
	currentCharge = getCurrentById(CURRENT_ID_CHARGE);
	if( SysState == SYSTEM_MODE_DISCHARGE )
	{
		if( currentDis < 0 )
			currentDis = 0;
		setCurrentValueById(CURRENT_ID_BUS, 0 - currentDis);
		setCurrentDirection(CURRENT_DIR_N);
	}
	else
	{
		setCurrentValueById(CURRENT_ID_BUS, currentCharge);
		setCurrentDirection(CURRENT_DIR_P);
	}

	return BMS_GetCurrentValueBUS();
}

static TYPE_CURRENT getCurrentByAdportAndId( int id )
{
	UINT16 mv = 0;
	float cv = 0.0;
	TYPE_CURRENT current = 0;
	assert(id<TOTAL_CURRENTS);
	mv = getmvByAdId(gThresholdValue.mCurrentThreshold[id].adChannel);
	cv = (float) (((float) mv) / 1000.0);
	current = (TYPE_CURRENT) (cv * kcurrentAdcVToA);
	return current;
}

int observerCurrentUpdate(void)
{
	enum UCT_ERR_CODE rv = UCT_SUCCESS;
	TYPE_CURRENT current = 0,buscur = 0;
	int i = 0;
	int SysState = getSystemState();

	current = getCurrentByAdportAndId(CURRENT_ID_CHARGE);
	setCurrentValueById(CURRENT_ID_CHARGE, current);

	current = getCurrentByAdportAndId(CURRENT_ID_DISCHARGE);
	setCurrentValueById(CURRENT_ID_DISCHARGE, current);
		
	buscur = updataBusCurrentValue(SysState);
	updataFeedbackCurrentValue(SysState);
	updateCurrentStatusValue(SysState, buscur);
	
	return (rv);
	
}

TYPE_CURRENT BMS_GetCurrentValueFeedback(void)
{
	return getCurrentById(CURRENT_ID_FEEDBACK);
}

TYPE_CURRENT BMS_GetDischargeCurrentValue(void)
{
	return getCurrentById(CURRENT_ID_DISCHARGE);
}

INT16 BMS_GetCurrentValueBUS(void)
{
	INT16 current = getCurrentById(CURRENT_ID_BUS);
	if (getCurrentDirection() == CURRENT_DIR_N)
	{
//		 return (0 - current); 
		return current;
	}
	else
		return current;
}

TYPE_CURRENT BMS_GetChargeCurrentValue(void)
{
	return getCurrentById(CURRENT_ID_CHARGE);
}
/*
 * if have this type, return 1
 * else return 0
 * */
int BMS_GetErrStatusCurrentByType(UINT8 types)
{

	int ret = 0;
	UINT8 i = 0;
	switch (types)
	{
		case STATE_CURRENT_NORMAL:
			for (i = 0; i < TOTAL_CURRENTS; i++)
			{
				if (gCurrent.mCurrents[i].mCurrentStatus != SYSTEM_FAULT_NONE)
					return 0;
			}
			return 1;
		case STATE_CURRENT_DISCHARGE_OC_LEVEL1:
			if (getCurrentStatusById(CURRENT_ID_DISCHARGE)
					== SYSTEM_FAULT_LEVEL1)
			{
				return 1;
			}
			else
				return 0;

		case STATE_CURRENT_DISCHARGE_OC_LEVEL2:
			if (getCurrentStatusById(CURRENT_ID_DISCHARGE)
					== SYSTEM_FAULT_LEVEL2)
			{
				return 1;
			}
			else
				return 0;
		case STATE_CURRENT_CHARGE_OC_LEVEL1:
			if (getCurrentStatusById(CURRENT_ID_CHARGE) == SYSTEM_FAULT_LEVEL1)
			{
				return 1;
			}
			else
				return 0;

		case STATE_CURRENT_CHARGE_OC_LEVEL2:
			if (getCurrentStatusById(CURRENT_ID_CHARGE) == SYSTEM_FAULT_LEVEL2)
			{
				return 1;
			}
			else
				return 0;
		case STATE_CURRENT_FEEDBACK_OC_LEVEL1:
			if (getCurrentStatusById(CURRENT_ID_FEEDBACK)
					== SYSTEM_FAULT_LEVEL1)
			{
				return 1;
			}
			else
				return 0;

		case STATE_CURRENT_FEEDBACK_OC_LEVEL2:
			if (getCurrentStatusById(CURRENT_ID_FEEDBACK)
					== SYSTEM_FAULT_LEVEL2)
			{
				return 1;
			}
			else
				return 0;
		default:
			return 0;
	}
}

static void printf_currents(void)
{
	UINT8 i = 0;
	for (i = 0; i < TOTAL_CURRENTS; i++)
	{
		mprintf("[current%u:%uA]", i, getCurrentById(i));
	}
	mprintf("\r\n");
}

//int testCurrents(void)
//{
//	Adc_InitADC0();
//	while(1)
//	{
//		adc_update();
//	observerCurrentUpdate();
//	printf_currents();
//	}
//}

