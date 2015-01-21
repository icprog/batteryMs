/*********************************************************
 *
 * File Description
 *
 *   $Revision: 3405 $
 *   $Author: huhai $
 *   $Date: 2014-09-01 17:02:40 +0800 ( 01  2014) $
 *
 *********************************************************/
#ifndef _VOLTAGE_H_
#define _VOLTAGE_H_

#include "public/public.h"
#include "soc/soc.h"
//#include "LTC68042/ltc6804.h"

#define V_TO_100UV(v)  ((v)*10000)

#define TOTAL_VOLTAGES MAX_CELLS

enum VOL_EVENT_TYPE_DEF
{
	VOL_EVENT_OK = 0, VOL_EVENT_HIGH_VOL, VOL_EVENT_LOW_VOL
};

enum type_state_voltage
{
	STATE_VOL_NORMAL = 0,
	STATE_CELL_VOL_LOW,
	STATE_CELL_VOL_LOW_LEVEL1,
	STATE_CELL_VOL_LOW_LEVEL2,
	STATE_CELL_VOL_HIGH,
	STATE_CELL_VOL_HIGH_LEVEL1 = 5,
	STATE_CELL_VOL_HIGH_LEVEL2,
	STATE_SUM_VOL_LOW,
	STATE_SUM_VOL_LOW_LEVEL1,
	STATE_SUM_VOL_LOW_LEVEL2,
	STATE_SUM_VOL_HIGH, // =10
	STATE_SUM_VOL_HIGH_LEVEL1 = 11,
	STATE_SUM_VOL_HIGH_LEVEL2,
	STATE_VOL_CELL_DIFF_BIG_LEVEL1,
	STATE_VOL_CELL_DIFF_BIG_LEVEL2,
	STATE_VOL_TOTAL
};





#define STATE_VOLHIGH_BITS_MASK (1<<STATE_VOL_HIGH|1<<STATE_VOL_HIGH_LEVEL1|1<<STATE_VOL_HIGH_LEVEL2)
#define STATE_VOLLOW_BITS_MASK (1<<STATE_VOL_LOW|1<<STATE_VOL_LOW_LEVEL1|1<<STATE_VOL_LOW_LEVEL2)
#define STATE_VOLDIFF_BITS_MASK (1<<STATE_VOL_CELL_DIFF_BIG_LEVEL1 | 1<<STATE_VOL_CELL_DIFF_BIG_LEVER2)

UINT16 setVoltageByIndex(UINT16 index, UINT16 voltage); //this maybe called by CAN module
int set_simulator_vol(const UINT16 *pvols, UINT8 n_batterys);

/*
 * the next four functions is needed by vcu_protocol.h
 * */
UINT32 BMS_GetVoltageValueBatteryTotal(void);  //mv
TYPE_VOLTAGE BMS_GetVoltageValueMax(void); //mv
TYPE_VOLTAGE BMS_GetVoltageValueMin(void); //mv
TYPE_VOLTAGE getVoltageValueByCellIndex(UINT8 index);

int BMS_GetVolValueMaxCellId(void);
int BMS_GetVolValueMinCellId(void);

int BMS_GetErrStatusVoltageByType( UINT8 types);         //1:err, 0 OK
int getSumVoltageState(void);
int getCellVoltageState(void);
int getVoltageState(void);

//
//int SetSimulateVoltageById(UINT8 id, TYPE_VOLTAGE  vol);
//int SetSimulateVoltage(TYPE_VOLTAGE *vol, int nVol);
//int SetSimulateVoltageEnable(UINT8 enable);

int observerVoltageUpdate(void);


#define V_2_100UV(v)  ((v)*1000*10)
#define V_2_MV(v) ((v)*1000)
#endif

