#include "current/current.h"
#include "public/public.h"
#include "public/ParameterManager.h"

/*
 * this  is a max allowable discharge current manager module
 * */

#define GetBmsQsum() (gParameters.e_devInfo.BATTERY_Q) 

static TYPE_CURRENT g_dischargeMaxAllowableCurrent = 0; //Q_SUM;
static const TYPE_TEMP thtemps[] =
{ -20, -10, 0, 10, 35, 45, 55 };
static const TYPE_SOC thsocs[] =
{ 0, 10, 20, 30, 70, 90, 100 };
static const TYPE_CURRENT maxAllowedDischargeCurrent30[6][6] =
{
{ 50, 100, 100, 100, 100, 100 }, //45~55
{ 80, 100, 100, 200, 200, 200 }, //35~45
{ 80, 100, 100, 200, 200, 200 }, //10~35
{ 50, 100, 100, 100, 100, 100 }, //0~10
{ 30, 50, 50, 80, 100, 100 }, //-10~0
{ 30, 30, 30, 30, 30, 30 }, //-20~-10
		};

static const TYPE_CURRENT maxAllowedDischargeCurrent10[6][6] =
{
{ 100, 105, 150, 150, 150, 150 }, //45~55
{ 150, 200, 200, 250, 250, 250 }, //35~45
{ 150, 200, 250, 300, 300, 300 }, //10~35
{ 150, 150, 150, 150, 150, 150 }, //0~10
{ 50, 80, 50, 80, 100, 100 }, //-10~0
{ 50, 50, 30, 30, 30, 30 }, //-20~-10
		};

static const UINT8 maxAllowedChargeCurrent10[2][6] =
{
{ 50, 100, 150, 150, 150, 50 }, //10~45¡æ
{ 50, 50, 100, 100, 100, 50 }  //0~10¡æ
};
/*
 * SOC 0~10% 10~20% 20~30% 30~70% 70~90% 90~100% 10~45¡æ
 0.3C 0.5C    1C     1C      1C     0.3C  0~10¡æ
 0.3C 0.5C   0.5C   0.5C    0.5C    0.3C
 * */
static const UINT8 maxAllowedChargeCurrent30[2][6] =
{
/*SOC 0~10% 10~20% 20~30% 30~70% 70~90% 90~100% */
{ 30, 50, 100, 100, 100, 30 },  // 10~45¡æ
{ 30, 50, 50, 50, 50, 30 } // 0~10¡æ
};

static const UINT16 feedbackCurrentTable[5][5] =
{ // <-5, 0,10,25,55
		{ 0, 0, 0, 0, 0 }, //>95-- soc
		{ 0, 0, 0, 100, 100 }, //90 
		{ 0, 0, 100, 100, 150 }, //80
		{ 0, 100, 100, 150, 200 }, //70
		{ 0, 100, 150, 200, 200 }, //<70
		};
static const TYPE_TEMP feedbackColTemp[] = {-5, 0, 10, 25, 55};
static const TYPE_SOC  feedbackRowSoc[] = {95,90,80,70};

static TYPE_CURRENT maxallowedcurrent30 = 0;
static TYPE_CURRENT maxallowedcurrent10 = 0;
static TYPE_CURRENT maxallowedFeedbackCurrent = 0;

struct cur_state_machine
{
	int machineState;
};

struct OC_time_manager
{
	UINT32 timems_start;
	UINT32 timems_oc_value;
	TYPE_CURRENT curMaxAllowed;
};

struct maxallowed_current
{
	struct cur_state_machine curStateMachine;
	TYPE_CURRENT realCurMaxAllowed;
	struct OC_time_manager sec10;
	struct OC_time_manager sec30;
};

static struct maxallowed_current gmaxAlowedCurrent[TOTAL_CURRENTS];

enum ctimer_ids
{
	timerSec10 = 4, timerSec30 = 5
};

static UINT8 getFeedbackCurRowBySoc( TYPE_SOC socs)
{
	UINT8 i = 0;
	for(i=0; i<sizeof(feedbackRowSoc);i++)
	{
		if( socs>=feedbackRowSoc[i])
			break;
	}
	assert(i<sizeof(feedbackRowSoc));
	return i;
}

static UINT8 getFeedbackCurColByTemp( TYPE_TEMP temps)
{
	UINT8 i = 0;
	for(i=0; i<sizeof(feedbackColTemp);i++)
	{
		if( temps<=feedbackColTemp[i] )
			return i;
	}
	return i-1;
}

static UINT16 getKFeedbackCurByTempAndSoc( TYPE_TEMP t, TYPE_SOC s)
{
	return feedbackCurrentTable[getFeedbackCurRowBySoc(s)][getFeedbackCurColByTemp(t)];
}



TYPE_CURRENT getMaxAllowedFeedbackCurByTempAndSoc( TYPE_TEMP t, TYPE_SOC s)
{

	maxallowedFeedbackCurrent = (TYPE_CURRENT) (((float) getKFeedbackCurByTempAndSoc(t, s))
				* ((float) GetBmsQsum()));
}

TYPE_CURRENT BMS_GetMaxAllowedFeedbackCurrent(void)
{
	TYPE_TEMP averageTemp = BMS_GetTemperatureValueAverage();
	TYPE_SOC socNow = BMS_GetSOCValue();
	int state = getSystemState();
	if( state == SYSTEM_MODE_DISCHARGE)
		return getMaxAllowedFeedbackCurByTempAndSoc(averageTemp, socNow);
	else
		return 0;
}

static TYPE_CURRENT getMaxAllowedCurrentByEnv(TYPE_SOC socNow, int state,
		TYPE_TEMP averageTemp)
{
	UINT8 i_t = 0, i_soc = 0;
	int maxallowedcurrent30_k, maxallowedcurrent10_k, ret = 0;
	TYPE_CURRENT maxallowedcurrent = 0;
	for (i_t = 0; i_t < sizeof(thtemps); i_t++)
	{
		if (averageTemp <= thtemps[i_t])
			break;
	}
	if (i_t >= sizeof(thtemps) || i_t == 0) //too high or too low 
		return 0;

	for (i_soc = 0; i_soc < sizeof(thsocs); i_soc++)
	{
		if (socNow <= thsocs[i_soc])
		{
			break;
		}
	}
	if (i_soc >= sizeof(thsocs))
		return 0; //err happen....
	/*discharge*/
	if (state == SYSTEM_MODE_DISCHARGE)
	{
		maxallowedcurrent30_k = maxAllowedDischargeCurrent30[i_t][i_soc];
		maxallowedcurrent10_k = maxAllowedDischargeCurrent10[i_t][i_soc];

		maxallowedcurrent30 = (TYPE_CURRENT) (((float) maxallowedcurrent30_k)
				* ((float) GetBmsQsum()));
		maxallowedcurrent10 = (TYPE_CURRENT) (((float) maxallowedcurrent10_k)
				* ((float) GetBmsQsum()));
	}

	/*charge*/
	if (state == SYSTEM_MODE_AC_CHARGE || state == SYSTEM_MODE_DC_CHARGE)
	{
		if (averageTemp < 0)
		{
			ret = -1;
		}
		else if (averageTemp < 10)
		{
			maxallowedcurrent30_k = maxAllowedChargeCurrent30[0][i_soc];
			maxallowedcurrent10_k = maxAllowedChargeCurrent10[0][i_soc];
		}
		else if (averageTemp < 45)
		{
			maxallowedcurrent30_k = maxAllowedChargeCurrent30[1][i_soc];
			maxallowedcurrent10_k = maxAllowedChargeCurrent10[1][i_soc];
		}
		else
		{
			ret = -1;
		}

		if (ret == -1)
		{
			maxallowedcurrent30 = maxallowedcurrent10 = 0;
		}
		else
		{
			maxallowedcurrent30 =
					(TYPE_CURRENT) (((float) maxallowedcurrent30_k)
							* ((float) GetBmsQsum()));
			maxallowedcurrent10 =
					(TYPE_CURRENT) (((float) maxallowedcurrent10_k)
							* ((float) GetBmsQsum()));
		}
	}

//	cur_state_machine( maxallowedcurrent30,maxallowedcurrent10 );
	//do process
//	if( SYSTEM_MODE_DISCHARGE == state )
//	{
//		if( dischargeCurrentNow > maxallowedcurrent30 )
//		{
//			if( timer_getState(gmaxAlowedCurrent[CURRENT_ID_DISCHARGE].OCtimer_30sec) != RUNNING)
//			{
//				timer_add(gmaxAlowedCurrent[CURRENT_ID_DISCHARGE].OCtimer_30sec,dischargeOCIsr,30000);
//			}
//			// count 30second
//			if( dischargeCurrentNow > maxallowedcurrent10 )
//			{
//				if(timer_getState(gmaxAlowedCurrent[CURRENT_ID_DISCHARGE].OCtimer_10sec) != RUNNING)
//				{
//					timer_add(gmaxAlowedCurrent[CURRENT_ID_DISCHARGE].OCtimer_10sec,dischargeOCIsr,10000);
//				}
//				//count 10 second
//			}
//			else
//			{
//				timer_stop(gmaxAlowedCurrent[CURRENT_ID_DISCHARGE].OCtimer_10sec);
//			}
//		}
//	}
//	else
//	{
//		timer_stop(gmaxAlowedCurrent[CURRENT_ID_DISCHARGE].OCtimer_10sec);
//		timer_stop(gmaxAlowedCurrent[CURRENT_ID_DISCHARGE].OCtimer_30sec);
//	}

	return maxallowedcurrent30;
}

TYPE_CURRENT getMaxAllowedCurrent(void)
{
	TYPE_TEMP averageTemp = BMS_GetTemperatureValueAverage();
	TYPE_SOC socNow = BMS_GetSOCValue();
	int state = getSystemState();
	return getMaxAllowedCurrentByEnv(socNow, state, averageTemp);
}

struct allowedCurrentTable
{
	TYPE_TEMP tempTh;
	float currentK;
};

static struct allowedCurrentTable dischargeTable[] =
{
{ 0, 1 },
{ -20, 0.3 },
{ -20, 0 } };
static const UINT8 sizeofdischargetable = sizeof(dischargeTable);
static struct allowedCurrentTable chargeTable[] =
{
{ 0, 0.3 },
{ 0, 0 } };

TYPE_CURRENT BMS_GetMaxAlowedDisChargeCurrent(void)
{
	return getMaxAllowedCurrent();
//	UINT8 i = 0;
//	TYPE_TEMP t = BMS_GetTemperatureValueAverage();
//	if( t > 0 )
//		return Q_SUM;
//	else if(t>-20)
//		return Q_SUM*3/10;
//	else
//		return 0;
//	
//	if(t<dischargeTable[sizeofdischargetable-1].tempTh)
//		return Q_SUM*dischargeTable[sizeofdischargetable-1].currentK;
//
//	for(i=0;i<sizeofdischargetable-1;i++)
//	{
//		if(t>dischargeTable[i].tempTh)
//			return Q_SUM*dischargeTable[i].currentK;
//	}

}

TYPE_CURRENT BMS_GetMaxAlowedChargeCurrent(void)
{
	return getMaxAllowedCurrent();
//	TYPE_TEMP t = BMS_GetTemperatureValueAverage();
//	if( t > 0)
//		return Q_SUM*3/10; 
//	else
//		return 0;
}
//
//static int SetCurrentMaxAllowableDischarge(TYPE_CURRENT current)
//{ 
//	(void)current;
//	return 0;
//}
//
//static int ReduceCurrentMaxAllowableDischarge(TYPE_CURRENT reduceCurrent)
//{ 
//	if(reduceCurrent>g_dischargeMaxAllowableCurrent)
//		g_dischargeMaxAllowableCurrent = 0;
//	else
//		g_dischargeMaxAllowableCurrent -= reduceCurrent;
//	return 0;
//}

int test_getMaxAllowedCurrent(void)
{
	TYPE_TEMP averageTemp = BMS_GetTemperatureValueAverage();
	TYPE_SOC socNow = BMS_GetSOCValue();
	int state = getSystemState();
	return getMaxAllowedCurrentByEnv(socNow, state, averageTemp);
}

