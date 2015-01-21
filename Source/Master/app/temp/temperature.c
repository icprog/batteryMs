/*********************************************************
 *
 * (UCT)
 *
 * File Description
 *
 *   $Revision: 3405 $
 *   $Author: huhai $
 *   $Date: 2014-09-01 17:02:40 +0800 (周一, 01 九月 2014) $
 *
 *********************************************************/
#include "temp/temperature.h"
#include "string.h"
#include "public/public.h"
//#include "LTC68042/ltc6804.h"




struct state_temperature
{
	int state[MAX_ALLOWED_TEMPSENSORS];
	int bitState;  //each bit indicate the id of temperatures err or ok
	int cell_temp_diff_state; //the state of difference of temperature 
	UINT8 sumState;  // each bit indicate the type of  type_state_temperature 
};

struct temperature_obj
{
	INT16 temp;
	UINT8 id;
};

struct temperature_vars
{
	struct state_temperature temp_state;
	struct temperature_obj temp_max;
	struct temperature_obj temp_min;
//	struct simulator_temp temp_simulator;
	TYPE_TEMP TempAverageValue;
};

static struct temperature_vars g_temp_var;
static  struct temperature_const_vars g_const_temp_var = 
		{
				{

						VALUE_TH_UT, //UINT16 vol_low;
						VALUE_TH_UT_LEVEL1,//UINT16 vol_low_level1;
						VALUE_TH_UT_LEVEL2,//UINT16 vol_low_level2;
						VALUE_TH_OT,//UINT16 vol_high;
						VALUE_TH_OT_LEVEL1,//UINT16 vol_high_level1;
						VALUE_TH_OT_LEVEL2,//UINT16 vol_high_level2;
					},
				{
						VALUE_TH_DIFF_TEMP_CELL_LEVEL1,
						VALUE_TH_DIFF_TEMP_CELL_LEVEL2
				}
		};


static int getTemperatureState(void)
{
	return g_temp_var.temp_state.bitState;
}

//TYPE_TEMP g_UCT_temperature[TOTAL_TEMP_NUM] = {0};
//TYPE_TEMP g_simulator_temperature[TOTAL_TEMP_NUM] = {0};
//
//int set_simulator_temp( const TYPE_TEMP *ptemps, UINT8 n_temps)
//{
//	memcpy(  g_temp_var.temp_simulator.m_temps, ptemps, n_temps*sizeof(TYPE_TEMP) );
//	return 0;
//}



TYPE_TEMP BMS_GetTemperatureValueById(int id)
{
	TYPE_TEMP temp = 0;
	if( DriverRead( FD_TEMP_BY_INDEX, (char *)&temp, sizeof(TYPE_TEMP), id) < 0 )
	{
			;//TODO err
	}
	return temp;
}

/*
 * in: cell voltage
 * out: state_voltage
 * get cell voltage state
 * by mgc 20141030
 * */
static int getCellTemperatureState(int temperature)
{
	int ret=0;
	if (temperature < g_const_temp_var.cell_temp_value.temp_low_level1)
	{
		//todo
		ret = STATE_TEMP_LOW_LEVEL1;
	}
	else if (temperature < g_const_temp_var.cell_temp_value.temp_low_level2)
	{
		ret = STATE_TEMP_LOW_LEVEL2;
	}
	else if (temperature < g_const_temp_var.cell_temp_value.temp_low)
	{
		ret = STATE_TEMP_LOW;
	}

	if (temperature > g_const_temp_var.cell_temp_value.temp_high_level1)
	{
		ret = STATE_TEMP_HIGH_LEVEL1;
	}
	else if (temperature > g_const_temp_var.cell_temp_value.temp_high_level2)
	{
		ret = STATE_TEMP_HIGH_LEVEL2;
	}
	else if (temperature > g_const_temp_var.cell_temp_value.temp_high)
	{
		ret = STATE_TEMP_HIGH;
	}
	return ret;
}
static int checkTemperatureDiffState(INT16 min_temp, INT16 max_temp)
{
	int ret = STATE_TEMP_NORMAL;
	UINT16 diff = max_temp-min_temp ;
	if( diff > g_const_temp_var.cell_diff_value.temp_diff_danger1 )
	{
		ret = STATE_TEMP_CELL_DIFF_BIG_LEVEL1;
	}
	else if(diff > g_const_temp_var.cell_diff_value.temp_diff_danger2)
	{
		ret = STATE_TEMP_CELL_DIFF_BIG_LEVEL2;
	}
	return ret;
}
int observerTemperatureUpdate(void)
{
	int rv = UCT_SUCCESS;
	int state;
   UINT16 t_temp_state=0;
	INT16 t_temperature;
	UINT8 temperature_sensor_id = 0;
	INT32 temp_sum = 0;
//	count = get_simulator_mode();
//	if ((count & MASK_BIT_SIMULATOR_MODE_TEMP) == 0)
//	{
//		memcpy(g_UCT_temperature, g_simulator_temperature, TOTAL_TEMP_NUM);
//	}
    #if SIMULATE_TEMPERATURE
	g_const_temp_var.cell_temp_value.temp_low_level1 = -5;
	g_const_temp_var.cell_temp_value.temp_low_level2 = -2;
	g_const_temp_var.cell_temp_value.temp_low = 0;
	g_const_temp_var.cell_temp_value.temp_high_level1 = 40;
	g_const_temp_var.cell_temp_value.temp_high_level2 = 38;
	g_const_temp_var.cell_temp_value.temp_high = 37;
	g_const_temp_var.cell_diff_value.temp_diff_danger1 =80;
	g_const_temp_var.cell_diff_value.temp_diff_danger2= 30;
    #endif
	g_temp_var.temp_max.temp = g_temp_var.temp_min.temp = BMS_GetTemperatureValueById(0);
	for (temperature_sensor_id = 0; temperature_sensor_id < TOTAL_TEMPERATURE_SENSORS_NUM; temperature_sensor_id++)
	{
		t_temperature = BMS_GetTemperatureValueById(temperature_sensor_id);
		temp_sum += t_temperature;
		if (g_temp_var.temp_max.temp < t_temperature)
		{
			g_temp_var.temp_max.temp = t_temperature;
			g_temp_var.temp_max.id = temperature_sensor_id;
		}

		if (g_temp_var.temp_min.temp > t_temperature)
		{
			g_temp_var.temp_min.temp = t_temperature;
			g_temp_var.temp_max.id = temperature_sensor_id;
		}
       
		state = getCellTemperatureState(t_temperature);
//		if (state != g_temp_var.temp_state.state[temperature_sensor_id])
//		{
			g_temp_var.temp_state.state[temperature_sensor_id] = state;
			MGC_SET_BIT(t_temp_state, state);
			//TODO
//		}

	}
	
	state  = checkTemperatureDiffState(g_temp_var.temp_min.temp, g_temp_var.temp_max.temp);
//	if(state != g_temp_var.temp_state.cell_temp_diff_state)
//	{
		g_temp_var.temp_state.cell_temp_diff_state = state ;
		MGC_SET_BIT(t_temp_state, state);
		//todo update()
//	}
	
	g_temp_var.TempAverageValue = (TYPE_TEMP)(temp_sum/TOTAL_TEMPERATURE_SENSORS_NUM);
	g_temp_var.temp_state.bitState = t_temp_state;
	return rv;
}

TYPE_TEMP BMS_GetTemperatureValueMinValue(void)
{
	return g_temp_var.temp_min.temp;
}

TYPE_TEMP BMS_GetTemperaturValueMaxValue(void)
{
	return g_temp_var.temp_max.temp;
}

TYPE_TEMP BMS_GetTemperatureValueAverage(void)
{
	return g_temp_var.TempAverageValue;
}

int BMS_GetTemperatureMaxValueSensorId(void)
{
	return g_temp_var.temp_max.id;
}

int BMS_GetTemperatureMinValueSensorId(void)
{
	return g_temp_var.temp_min.id;
}

/*
 * if have return 1 else 0 
 * */
int BMS_GetErrStatusTemperatureByType( UINT8 types)
{
	assert(types<STATE_TEMP_TOTAL);
	if( types == 0 )
		return (getTemperatureState()==1);
	if( ((1<<types) & getTemperatureState()) == 0)
		return 0;
	return 1;
}

