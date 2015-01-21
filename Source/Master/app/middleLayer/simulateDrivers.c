/*
 * simulateDrivers.c
 *
 *  Created on: Dec 27, 2014
 *      Author: Administrator
 */

#include "public/types.h"
#include "middleLayer/simulateDrivers.h"
#include "public/drivers.h"
struct simulateAds
{
	UINT8 enable;
	UINT16 adMv[ID_ADC_TOTAL];			
};

struct simulateGpios
{
	UINT8 enable;
	UINT8 gpioPort[ID_IO_BMS_END];
};

struct simulateTemps
{
	UINT8 enable;
	TYPE_TEMP tempValues[MAX_ALLOWED_TEMPSENSORS];
};

struct simulateVols
{
	UINT8 enable;
	TYPE_VOLTAGE volValues[MAX_ALLOWED_CELLS];
};

struct simulateValues
{
	struct simulateAds ad;
	struct simulateGpios gpios;
	struct simulateTemps gtemps;
	struct simulateVols gvols;
};

static struct simulateValues gSim;

int setSimulateVolEnable( UINT8 enable )
{
	gSim.gvols.enable = enable;
}
int getSimulateVolEnable( void )
{
	return gSim.gvols.enable;
}
int setSimulateTempEnable( UINT8 enable )
{
	gSim.gtemps.enable = enable;
}
int getSimulateTempEnable( void )
{
	return gSim.gtemps.enable;
}
int setSimulateGpioEnable( UINT8 enable )
{
	gSim.gpios.enable = enable;
}
int getSimulateGpioEnable( void )
{
	return gSim.gpios.enable;
}

int setSimulateAdEnable( UINT8 enable )
{
	gSim.ad.enable = enable;
}

int getSimulateAdState(void)
{
	return gSim.ad.enable;
}


UINT16 GetSimulateAdmvById(UINT8 id)
{
	assert(id<ID_ADC_TOTAL);
	return gSim.ad.adMv[id];
}

int SetSimulateAdmvById(UINT8 id, UINT16 admv)
{
	assert(id<ID_ADC_TOTAL);
	gSim.ad.adMv[id] = admv;
	return 0;
}

int SetSimulateGpioById(UINT8 id, UINT8 state)
{
	assert(id<ID_IO_BMS_END);
	gSim.gpios.gpioPort[id] = state;
	return 0;
}
int getSimulateGpioById(UINT8 id)
{
	assert(id<ID_IO_BMS_END);
	return gSim.gpios.gpioPort[id];
}


int SetSimulateTempById(UINT8 id, TYPE_TEMP temp)
{
	assert(id<MAX_ALLOWED_TEMPSENSORS);
	gSim.gtemps.tempValues[id] = temp;
	return 0;
}
TYPE_TEMP getSimulateTempById(UINT8 id)
{
	assert(id<MAX_ALLOWED_TEMPSENSORS);
	return gSim.gtemps.tempValues[id];
}

int SetSimulateVolById(UINT8 id, TYPE_VOLTAGE vol)
{
	assert(id<MAX_ALLOWED_CELLS);
	gSim.gvols.volValues[id] = vol;
	return 0;
}
TYPE_VOLTAGE getSimulateVolById(UINT8 id)
{
	assert(id<MAX_ALLOWED_CELLS);
	return gSim.gvols.volValues[id];
}

int testSimulatorValue(void)
{
	UINT8 i = 0; 
	for(i=0; i<MAX_ALLOWED_TEMPSENSORS;i++)
	{
		SetSimulateTempById(i, (TYPE_TEMP)(-20+i*2));
	}
	
	for(i=0; i<ID_ADC_TOTAL;i++)
	{
		SetSimulateAdmvById(i, (UINT16)(2500+i*200));
	}
	
	for(i = 0; i<ID_IO_BMS_END;i ++ )
	{
		SetSimulateGpioById(i, i%2);
	}
	
	for( i=0; i<TOTAL_VOLTAGES; i++)
	{
		SetSimulateVolById(i, (TYPE_VOLTAGE)(i+1000));
	}
	
	return 0;
}
