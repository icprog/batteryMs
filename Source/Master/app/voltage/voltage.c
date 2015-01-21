/*********************************************************
 *
 * (UCT)
 *
 * File Description
 *
 *   $Revision: 3405 $
 *   $Author: huhai $
 *   $Date: 2014-09-01 17:02:40 +0800  $
 *
 *********************************************************/

#include <string.h>
#include "app/public/public.h"
#include "app/voltage/voltage.h"
#include "app/mgclib/mgcbytesorder.h"
#include "app/public/ParameterManager.h"

/*types */
enum err_level
{
	err_level_none, err_level1, err_level2
};

struct state_cell_voltage
{
	int cell[MAX_ALLOWED_CELLS];
};

struct state_voltage
{
	struct state_cell_voltage cell_vol_state;  //each cell state
	int sum_vol_state; // the sum of voltage state
	int cell_vol_diff_state; //the state of difference of voltages 
	int voltage_state; //each bit show the sum state of this battery as type vol state
};

struct simulateVoltage
{
	UINT8 simulatorEnableFlag;
	TYPE_VOLTAGE cellVoltage[MAX_ALLOWED_CELLS];
	TYPE_VOLTAGE sumVoltage;
};

struct voltage_vars
{
	TYPE_VOLTAGE g_battery_singlevol_max;
	TYPE_VOLTAGE g_battery_singlevol_min;
	TYPE_VOLTAGE g_battery_vol_sum;
	struct simulateVoltage sVol;
	UINT8 singleVolMaxId;
	UINT8 singleVolMinId;

	/*single cell voltage state*/
	struct state_voltage g_volatage_state;
//  UINT16 g_sleep_voltage_allowed ;
//  UINT32 g_ov_status ;
//  UINT32 g_uv_status ;
//  UINT32 timer_OV[TOTAL_VOLTAGES];
//  UINT32 timer_UV[TOTAL_VOLTAGES];
//  UINT16 g_UCT_voltage[TOTAL_VOLTAGES] ;
//  UINT16 g_simulator_vols[TOTAL_VOLTAGES] ;
};

static struct voltage_vars g_voltage_vars;

//int SetSimulateVoltageById(UINT8 id, TYPE_VOLTAGE vol)
//{
//	g_voltage_vars.sVol.cellVoltage[id] = vol;
//	return 0;
//}
//
//TYPE_VOLTAGE GetSimulateVoltageById(UINT8 id)
//{
//	return g_voltage_vars.sVol.cellVoltage[id];
//}
//
//int SetSimulateVoltage(TYPE_VOLTAGE *vol, int nVol)
//{
//	memcpy(g_voltage_vars.sVol.cellVoltage, vol, nVol * sizeof(TYPE_VOLTAGE));
//	return 0;
//}
//
//int SetSimulateVoltageEnable(UINT8 enable)
//{
//	g_voltage_vars.sVol.simulatorEnableFlag = enable;
//	return 0;
//}
///*
// * if return 1 means use simulate voltages
// * else use realvoltage by ltc6804
// * */
//int GetSimulateVoltageEnable(void)
//{
//	return g_voltage_vars.sVol.simulatorEnableFlag;
//}

TYPE_VOLTAGE getVoltageValueByCellIndex(UINT8 index)
{
	TYPE_VOLTAGE vol = 0;
//	if (GetSimulateVoltageEnable())
//	{
//		return GetSimulateVoltageById(index);
//	}
	if (DriverRead(FD_VOL_BY_INDEX, (char *) &vol, sizeof(TYPE_VOLTAGE), index)
			< 0)
	{
		; //TODO err
	}
	return vol;
}

TYPE_VOLTAGE BMS_GetVoltageValueBatteryTotal(void)
{
	return g_voltage_vars.g_battery_vol_sum;
}

TYPE_VOLTAGE BMS_GetVoltageValueMin(void)
{
	return g_voltage_vars.g_battery_singlevol_min;
}
TYPE_VOLTAGE BMS_GetVoltageValueMax(void)
{
	return g_voltage_vars.g_battery_singlevol_max;
}

int BMS_GetVolValueMaxCellId(void)
{
	return g_voltage_vars.singleVolMaxId;
}
int BMS_GetVolValueMinCellId(void)
{
	return g_voltage_vars.singleVolMinId;
}
/*
 * get the BMS batteries voltage state
 * state:
 bit0:STATE_VOL_NORMAL = 0,
 STATE_CELL_VOL_LOW,
 STATE_CELL_VOL_LOW_LEVEL1,
 STATE_CELL_VOL_LOW_LEVEL2,
 STATE_CELL_VOL_HIGH,
 bit5:STATE_CELL_VOL_HIGH_LEVEL1 = 5,
 bit6:STATE_CELL_VOL_HIGH_LEVEL2,
 STATE_SUM_VOL_LOW,
 STATE_SUM_VOL_LOW_LEVEL1,
 STATE_SUM_VOL_LOW_LEVEL2,
 STATE_SUM_VOL_HIGH, // =10
 bit11:STATE_SUM_VOL_HIGH_LEVEL1 = 11,
 STATE_SUM_VOL_HIGH_LEVEL2,
 STATE_VOL_CELL_DIFF_BIG_LEVEL1,
 STATE_VOL_CELL_DIFF_BIG_LEVEL2,
 * by mgc-2014-10-31
 * */
int getVoltageState(void)
{
	return g_voltage_vars.g_volatage_state.voltage_state;
}
/*
 * in: cell voltage
 * out: state_voltage
 * get cell voltage state
 * by mgc 20141030
 * */
static int checkCellVoltageState(TYPE_VOLTAGE cell_voltage)
{
	int ret = 0;
	if (cell_voltage < g_const_vol_value.cell_vol_value.vol_low_level1)
	{
		//todo
		ret = STATE_CELL_VOL_LOW_LEVEL1;
	}
	else if (cell_voltage < g_const_vol_value.cell_vol_value.vol_low_level2)
	{
		ret = STATE_CELL_VOL_LOW_LEVEL2;
	}
	else if (cell_voltage < g_const_vol_value.cell_vol_value.vol_low)
	{
		ret = STATE_CELL_VOL_LOW;
	}

	if (cell_voltage > g_const_vol_value.cell_vol_value.vol_high_level1)
	{
		ret = STATE_CELL_VOL_HIGH_LEVEL1;
	}
	else if (cell_voltage > g_const_vol_value.cell_vol_value.vol_high_level2)
	{
		ret = STATE_CELL_VOL_HIGH_LEVEL2;
	}
	else if (cell_voltage > g_const_vol_value.cell_vol_value.vol_high)
	{
		ret = STATE_CELL_VOL_HIGH;
	}
	return ret;
}
/*
 * get the state of sum voltage
 * by mgc 20141030
 * */
static int checkSumVoltageState(UINT32 sum_voltage)
{
	int ret = 0;
	if (sum_voltage < g_const_vol_value.sum_vol_value.vol_low_level1)
	{
		//todo
		ret = STATE_SUM_VOL_LOW_LEVEL1;
	}
	else if (sum_voltage < g_const_vol_value.sum_vol_value.vol_low_level2)
	{
		ret = STATE_SUM_VOL_LOW_LEVEL2;
	}
	else if (sum_voltage < g_const_vol_value.sum_vol_value.vol_low)
	{
		ret = STATE_SUM_VOL_LOW;
	}

	if (sum_voltage > g_const_vol_value.sum_vol_value.vol_high_level1)
	{
		ret = STATE_SUM_VOL_HIGH_LEVEL1;
	}
	else if (sum_voltage > g_const_vol_value.sum_vol_value.vol_high_level2)
	{
		ret = STATE_SUM_VOL_HIGH_LEVEL2;
	}
	else if (sum_voltage > g_const_vol_value.sum_vol_value.vol_high)
	{
		ret = STATE_SUM_VOL_HIGH;
	}
	return ret;
}

static int checkVoltageDiffState(TYPE_VOLTAGE max_vol, TYPE_VOLTAGE min_vol)
{
	int ret = STATE_VOL_NORMAL;
	TYPE_VOLTAGE diff = max_vol - min_vol;
	if (diff > g_const_vol_value.cell_diff_value.cell_diff_danger1)
	{
		ret = STATE_VOL_CELL_DIFF_BIG_LEVEL1;
	}
	else if (diff > g_const_vol_value.cell_diff_value.cell_diff_danger2)
	{
		ret = STATE_VOL_CELL_DIFF_BIG_LEVEL2;
	}
	return ret;
}
/*
 * update for ltc68042 object
 * if it read new voltages from chips this update should be executed
 * by mgc 20141030
 * */
int observerVoltageUpdate(void)
{
	int rv = 0;
	UINT8 i = 0, j = 0, k = 0, temp_sleep = 0, flag_vol_state_change = 0;
	TYPE_VOLTAGE voltage = 0;
	TYPE_VOLTAGE temp_sum = 0;
	struct state_voltage *pvstate = NULL;
	int state;
	int t_vol_state = 0;
	UINT8 flag_cell_state_change = 0;

	/*the next simulator should move to driver layer*/

	g_voltage_vars.g_battery_singlevol_max =
			g_voltage_vars.g_battery_singlevol_min = getVoltageValueByCellIndex(
					0);
	pvstate = &g_voltage_vars.g_volatage_state;

	temp_sum = 0;
	for (i = 0; i < TOTAL_VOLTAGES; i++)
	{
		voltage = getVoltageValueByCellIndex(i);

		if (g_voltage_vars.g_battery_singlevol_max < voltage)
		{
			g_voltage_vars.g_battery_singlevol_max = voltage;
			g_voltage_vars.singleVolMaxId = i;
		}

		if (g_voltage_vars.g_battery_singlevol_min > voltage)
		{
			g_voltage_vars.g_battery_singlevol_min = voltage;
			g_voltage_vars.singleVolMinId = i;
		}

		/*cell voltage err state check*/
		state = checkCellVoltageState(voltage);

		pvstate->cell_vol_state.cell[i] = state;
		MGC_SET_BIT(t_vol_state, state);
		flag_cell_state_change = 1;

		temp_sum += voltage;
	}
	/*observe object1 cell voltage state*/
	if (flag_cell_state_change)
	{
		//todo update()
		flag_vol_state_change = 1;
	}
	/*observe object2 sum voltage state */
	state = checkSumVoltageState(temp_sum);
//	if (state !=  pvstate->sum_vol_state)
//	{
	pvstate->sum_vol_state = state;
	MGC_SET_BIT(t_vol_state, state);
	//todo update()
	flag_vol_state_change = 1;
//	}

	/*observer object3 voltage differences state*/
	state = checkVoltageDiffState(g_voltage_vars.g_battery_singlevol_max,
			g_voltage_vars.g_battery_singlevol_min);
//	if(state  != pvstate->cell_vol_diff_state)
//	{
	pvstate->cell_vol_diff_state = state;
	MGC_SET_BIT(t_vol_state, state);
	//todo update()
	flag_vol_state_change = 1;
//	}

	if (flag_vol_state_change == 1)
	{
		//TODO update

	}

	g_voltage_vars.g_battery_vol_sum = temp_sum;
	g_voltage_vars.g_volatage_state.voltage_state = t_vol_state;
	return (rv);

}

/*
 * check the voltage state  by state as ttype_state_voltage
 * in : state 		type_state_voltage
 * out: 
 * return 0 none, 1:yes
 * */
int BMS_GetErrStatusVoltageByType(UINT8 types)
{
	assert(types<STATE_VOL_TOTAL);
	if (types == 0)
		return (getVoltageState() == 1);
	if (((1 << types) & getVoltageState()) == 0)
		return 0;
	return 1;
}

static int VoltageInit(void)
{
	return 0;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
