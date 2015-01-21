/*
 * control.h
 * err check and state update check
 * add by mgc 2014-11-10
 * */

#include "public\public.h"
#ifndef _CONTROL_H_
#define _CONTROL_H_

enum type_state_fault
{
	STATE_ERR_NONE = 0,
	STATE_ERR_BMSRD6804_VOL,
	STATE_ERR_BMSRD6804_TEMP,
	STATE_ERR_BMSRD_CURRENT,
	STATE_ERR_BMSCHECKSELF,
	STATE_ERR_TOTAL
};
int BMS_GetErrStatusBMScheckselfByType( UINT8 types);
int getBMScheckselfState(void);

int AlarmUpdateVoltage(void);	
int AlarmUpdateTemperature(void);	
int AlarmUpdateCurrent(void);	
int AlarmUpdateSoc(void);	
int AlarmUpdateInsulutionResistance(void);	
int AlarmUpdateRelayModule(void);	
int AlarmUpdateBmsSelfCheck(void);	
int AlarmUpdatePTCAN(void);
int AlarmUpdateBSUcomunication(void);
int AlarmUpdateCbcuCheck(void);
#endif
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
