/*
 * ColdHeatManager.c
 *
 *  Created on: Nov 13, 2014
 *      Author: mgc
 */
#include "public/types.h"
#include "public/public.h"
#include "ctl/ColdHeatManager.h"
#include "temp/temperature.h"
#include "ctl/relay.h"
#include "public/drivers.h"

struct s_TempThreasholdValue
{
	TYPE_TEMP gValueFanOpen;
	TYPE_TEMP gValueFanClose;
	TYPE_TEMP gValueHeatOpen;
	TYPE_TEMP gValueHeatClose;
};

static const struct s_TempThreasholdValue gTempThValue=
		{
			VALUE_TH_TEMP_FAN_OPEN,
			VALUE_TH_TEMP_FAN_CLOSE,
			VALUE_TH_TEMP_HEAT_OPEN,
			VALUE_TH_TEMP_HEAT_CLOSE		
//				35,30,5,15
		};


int ColdHeatControler(void)
{
	TYPE_TEMP t = BMS_GetTemperatureValueAverage();
	if( t >= gTempThValue.gValueFanOpen )
	{
		TEMP_OPEN_FAN();
	}
	
	if(t <= gTempThValue.gValueFanClose )
	{
		TEMP_CLOSE_FAN();
	}
	
	if( t <= gTempThValue.gValueHeatOpen )
	{
		TEMP_OPEN_HEAT();
	}
		
	if( t >= gTempThValue.gValueHeatClose )
	{
		TEMP_CLOSE_HEAT();
	}
	
	return 0;
}
