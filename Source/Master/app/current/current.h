/*********************************************************
*
*********************************************************/


#ifndef _CURRENT_H_
#define _CURRENT_H_
#include "current/currentAbstract.h"
#include "temp/temperature.h"

TYPE_CURRENT BMS_GetMaxAlowedDisChargeCurrent(void);
TYPE_CURRENT BMS_GetMaxAlowedChargeCurrent(void);
TYPE_CURRENT getMaxAllowedCurrent(void);
TYPE_CURRENT BMS_GetMaxAllowedFeedbackCurrent(void);
TYPE_CURRENT getMaxAllowedFeedbackCurByTempAndSoc( TYPE_TEMP t, TYPE_SOC s);
int test_getMaxAllowedCurrent(void);
#endif
