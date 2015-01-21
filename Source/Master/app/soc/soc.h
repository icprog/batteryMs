#ifndef _SOC_H_
#define _SOC_H_

#include "public/types.h"
#include "soc/soc/Closed_Loop_NCM_29Ah_change.h"

enum type_soc_state
{
	STATE_SOC_NORMAL,
	STATE_SOC_LOW,
	STATE_SOC_LOW_LEVEL1,
	STATE_SOC_LOW_LEVEL2,
	STATE_SOC_TOTAL_STATUS
};

int BMS_GetErrStatusSocByType(UINT8 types);
int BMS_GetErrStatusSOCByType(UINT8 types);

UINT8 getSocValue(void);
UINT8 getSohValue(void);
UINT8 BMS_GetSOCValue(void);  //0~100
UINT8 BMS_GetSOHValue(void);	 //0~100

int socInit(void); //
int socProcess(void); //

/* Model entry point functions */


#endif

