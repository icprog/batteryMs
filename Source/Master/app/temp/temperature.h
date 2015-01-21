/*********************************************************
*
* File Description
*
*   $Revision: 3405 $
*   $Author: huhai $
*   $Date: 2014-09-01 17:02:40 +0800 $
*
*********************************************************/


#ifndef _TEMP_H_
#define _TEMP_H_

#include "public/public.h"
#include "public/types.h"
//#include "log/log.h"

//#define TOTAL_TEMP_POINT_NUM              91

//enum TEMP_EVENT_TYPE_DEF {
//  TEMP_EVENT_OK = 0,
//  TEMP_EVENT_HIGH_TEMP,
//  TEMP_EVENT_LOW_TEMP
//};

enum type_state_temperature
{
	STATE_TEMP_NORMAL,
	STATE_TEMP_LOW,
	STATE_TEMP_LOW_LEVEL1,
	STATE_TEMP_LOW_LEVEL2,
	STATE_TEMP_HIGH,
	STATE_TEMP_HIGH_LEVEL1,
	STATE_TEMP_HIGH_LEVEL2,
	STATE_TEMP_CELL_DIFF_BIG_LEVEL1,
	STATE_TEMP_CELL_DIFF_BIG_LEVEL2,
	STATE_TEMP_TOTAL
};




//int set_simulator_temp( const TYPE_TEMP *ptemps, UINT8 n_temps);
//TYPE_TEMP getTemperatureByIndex( UINT8 index );
//int setTemperatureByIndex( UINT8 index, TYPE_TEMP temperature);
TYPE_TEMP BMS_GetTemperatureValueAverage(void);
TYPE_TEMP BMS_GetTemperaturValueMaxValue(void);
TYPE_TEMP BMS_GetTemperatureValueMinValue(void);
TYPE_TEMP BMS_GetTemperatureValueById(int id);

int BMS_GetTemperatureMaxValueSensorId(void);
int BMS_GetTemperatureMinValueSensorId(void);

int BMS_GetErrStatusTemperatureByType( UINT8 types);     //1:err, 0 OK

//int getTemperatureState(void);
int BMS_GetErrStatusTemperatureByType( UINT8 types);
int observerTemperatureUpdate(void); 

#endif



