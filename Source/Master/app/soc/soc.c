#include "soc/soc.h"
#include "public/types.h"
#include "public/public.h"
#include "voltage/voltage.h"
#include "temp/temperature.h"
#include "current/current.h"
#include "driver/timer/timer.h"

static struct gValue
{
	TYPE_SOC soc;
	UINT8 errStatus;
};

static struct s_socConstvalue
{
	TYPE_SOC soc_th_level1;
	TYPE_SOC soc_th_level2;
};

static struct gValue gSocValue = {100,0};
static struct s_socConstvalue gSocConstValue = 
		{
				VALUE_TH_SOC_LEVEL1,
				VALUE_TH_SOC_LEVEL2
		};
static UINT8 socInitFlag = 0;

UINT8 getSocValue(void)
{
	
	return gSocValue.soc;
}


static int setSocErr( UINT8 err)
{
	assert(err<STATE_SOC_TOTAL_STATUS);
//	SET_BIT(gSocValue.errStatus,1<<err, 1);
	return 0;
}
static int resetSocErr(UINT8 err)
{
	assert(err<STATE_SOC_TOTAL_STATUS);
//	SET_BIT(gSocValue.errStatus,1<<err, 0);
	return 0;
}

static int getSocErr(void)
{
	return gSocValue.errStatus;
}

static int updateSocErrStatus(void)
{
//
//	STATE_SOC_NORMAL,
//	STATE_SOC_LOW,
//	STATE_SOC_LOW_LEVEL1,
//	STATE_SOC_LOW_LEVEL2
	UINT8  soc = getSocValue();
	if(soc < gSocConstValue.soc_th_level1)
	{
		setSocErr(STATE_SOC_LOW_LEVEL1);
	}
	else
	{
		resetSocErr(STATE_SOC_LOW_LEVEL1);
		if(soc <gSocConstValue.soc_th_level2)
		{
			setSocErr(STATE_SOC_LOW_LEVEL2);
		}
		else
		{
			resetSocErr(STATE_SOC_LOW_LEVEL2);
			setSocErr(STATE_SOC_NORMAL);
		}
	}
	
    
	return 0;
}

UINT8 getSohValue(void)
{
	//TODO
	return 100;
}

int BMS_GetErrStatusSocByType(UINT8 types)
{
	assert(types<STATE_SOC_TOTAL_STATUS);
	if( ((1<<types) & getSocErr()) != 0 )
		return 1;
	else
	{
		if( types == STATE_SOC_NORMAL)
			return 1;
		else
		return 0;
	}
}

int BMS_GetErrStatusSOCByType(UINT8 types)
{
	return BMS_GetErrStatusSocByType(types);
}

int socInit(void) //TODO
{
	 Closed_Loop_NCM_29Ah_initialize();
	 socInitFlag = 1;
	return 0;
}
/* Customized model step function */
//	extern void Closed_Loop_NCM_29Ah_custom(real32_T IN_U, real32_T IN_DT, real32_T
//	  IN_I, int8_T IN_I_Direct, real32_T *soc);
int socProcess(void) //TODO
{
	static UINT32 lastTimeMs = 0;
	UINT32 timeMsNow = 0;
	UINT32 timeTh = 0;
	TYPE_CURRENT current = 0;
	TYPE_VOLTAGE vol = 0;
//	float volf  = 0.0 , currentf = 0.0;
	
	if( socInitFlag != 1 )
			return -1;
	timeMsNow = getSystemTimeMs();
	timeTh = TIME_TRHESHOLD_CAL(lastTimeMs, timeMsNow);
	lastTimeMs = timeMsNow;
	vol = BMS_GetVoltageValueMin();
//	volf = (float)volf;
	current = BMS_GetCurrentValueBUS();
//	currentf = (float)currentf;
	
	Closed_Loop_NCM_29Ah_custom((int)vol, (int)timeTh,current,  &gSocValue.soc);
//	extern void Closed_Loop_NCM_29Ah_custom(real32_T IN_U, real32_T IN_I, real32_T
//	  *arg_In_Dt, uint8_T *arg_soc);
//	Closed_Loop_NCM_29Ah_custom(volf, currentf, (float)timeTh,  &gSocValue.soc);
	
	updateSocErrStatus();
	
	return gSocValue.soc;
}


UINT8 BMS_GetSOCValue(void)  //0~100
{
	return getSocValue();
}

UINT8 BMS_GetSOHValue(void)	 //0~100
{
	return getSohValue();
}

