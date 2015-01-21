/*********************************************************
 * File Description
 *
 *   $Revision: 2057 $
 *   $Author: huhai $
 *   $Date: 2014-07-11 14:24:25 +0800 (周五, 11 七月 2014) $
 *
 *********************************************************/

#include "banlance_battery/banlancebattery.h"
#include "voltage/voltage.h"
#include "middleLayer/appToDriver.h"
#include "public/public.h"
#include "public/types.h"
#include "public/systemState.h"
#include "driver/timer/timer.h"
#include <string.h>
//#include "timer/timer.h"

#define DEBUG_BANLANCE mprintf
#define DEGREE
#define MV_TO_100UV( mv )  (mv)*10
#define OPEN_BANLANCE_BY_LTC6804(index)   opBalance( index, 1)//enable_bypass_resistor((index/6),(index%6))//
#define CLOSE_BANLANCE_BY_LTC6804(index)    opBalance( index, 0)//disable_bypass_resistor((index/6),(index%6))//
static UINT16 THREASHOLD_V_BANLANCE = 20; //MV_TO_100UV(20);

static UINT8 n_open_balance = 0;
//static int timer_balance = -1;
//static const INT8 T0 = 45;
static struct balance_t gb;
//static UINT16 v[TOTAL_VOLTAGES];
//static INT8 t[TOTAL_VOLTAGES / 2];
static TYPE_VOLTAGE v_temp[MAX_ALLOWED_CELLS];
static UINT8 validBalance[MAX_ALLOWED_CELLS];
//static const INT8 T_board = 50;

static void set_threshold_v(UINT16 v)
{
	THREASHOLD_V_BANLANCE = v;
}

static UINT16 get_threshold_v(void)
{
	return (THREASHOLD_V_BANLANCE);
}

static int opBalance(UINT8 id, UINT8 state)
{
	UINT8 buff[2];
	buff[0] = id;
	buff[1] = state;
	DriverWrite(FD_BANLANCE_BY_INDEX, (char *) buff, 2, 0);
}

/*********************************************************
 *
 *********************************************************/
static int get_voltage(TYPE_VOLTAGE *destbuff)
{
	UINT8 i = 0;
	assert(destbuff);
	for (i = 0; i < TOTAL_VOLTAGES; i++)
	{
		destbuff[i] = getVoltageValueByCellIndex(i);
	}
	return 0;
}

static UINT8 select_max_from_array(const TYPE_VOLTAGE *array, UINT8 len)
{
	UINT8 i = 0, ret = 0;
	TYPE_VOLTAGE temp = 0;
	temp = array[0];
	for (i = 1; i < len; i++)
	{
		if (temp < array[i])
		{
			temp = array[i];
			ret = i;
		}
	}
	return (ret);
}
/*
 * select n max of data from array, and store the index to outbuff
 * */
static UINT8 select_nMaxFromArray(TYPE_VOLTAGE *array, UINT8 len,
		UINT8 *outbuff, UINT8 n)
{
	UINT8 i = 0, ret = 0;
	for (i = 0; i < n; i++)
	{
		ret = select_max_from_array(array, len - i);
		*outbuff++ = ret;
		array[ret] = 0;
	}
	return (0);
}

static int close_all_balance(void)
{
	UINT8 i = 0;

	for (i = 0; i < TOTAL_VOLTAGES; i++)
	{
		CLOSE_BANLANCE_BY_LTC6804(i);

	}
	return 0;
}

int balance_cal(void)
{
	//UINT8 iMax = 0,
	UINT8 i = 0, j = 0;
	static int n = 0;
	TYPE_TEMP T_board = BMS_GetTemperatureValueAverage();
	memset(validBalance, 0, TOTAL_VOLTAGES);
	if (n_open_balance == 5)
	{
		n_open_balance = 0;
		get_voltage((TYPE_VOLTAGE *) v_temp);
		n = 2; //(TOTAL_BALANCES - TOTAL_BALANCES * T_board / 75);
		if (n <= 0)
			goto end;
		select_nMaxFromArray(v_temp, TOTAL_VOLTAGES, validBalance, n);
		memset(&gb, 0, sizeof(gb));

		for (i = 0; i < n; i++)
		{
			if (((getVoltageValueByCellIndex(validBalance[i])
					- BMS_GetVoltageValueMin()) > THREASHOLD_V_BANLANCE)
					&& (BMS_GetVoltageValueMin() >= VALUE_TH_BALANCE_VOL))
			{
				gb.flags[validBalance[i]] = 1;
			}
		}

		for (i = 0; i < TOTAL_VOLTAGES; i++)
		{
			if (gb.flags[i])
			{
				OPEN_BANLANCE_BY_LTC6804(i);
			}
			else
			{
				CLOSE_BANLANCE_BY_LTC6804(i);
			}
		}

//		battery_blance_sw_get(gb.Bytebalances);
//		mprintf("balance cal result:%02x%02x%02x\r\n", gb.Bytebalances[0],
//				gb.Bytebalances[1], gb.Bytebalances[2]);
	}
	end: n_open_balance++;
}

//int balance_timer_init(void)
//{
//	struct timerspec ts;
//	ts.time_ms = 0;
//	ts.time_s = 5;
//	timer_balance = timer_get(&ts, (timer_process) balance_cal1, 0);
//	return (timer_balance);
//}

//struct balance_t * balance_read(void)
//{
//	return ((struct balance_t *) &gb);
//}

UINT16 get_balance_current_by_index(UINT8 index)
{
	return ((UINT16) (getVoltageValueByCellIndex(index) / V_2_100UV(R_BANLANCE)));
}

/********************************************************
 * set balance sw by ptr[3]
 * by mgc 20140725
 ********************************************************/
//enum UCT_ERR_CODE battery_balance_sw_set(const UINT8 *balance_ptr)
//{
//	UINT8 i, j;
//	enum UCT_ERR_CODE rv = UCT_SUCCESS;
//	struct balance_t * const gb = balance_read();
//
//	for (j = 0; j < 3; j++)
//	{
//		for (i = 0; i < 8; i++)
//		{
//			gb->flags[i + 8 * j] = ((balance_ptr[j] >> i) & 0x01);
//		}
//	}
//
//	return (rv);
//}
/********************************************************
 * read balance status to ptr
 * by mgc 20140725
 ********************************************************/
//enum UCT_ERR_CODE battery_blance_sw_get(UINT8 *balance_ptr)
//{
//	UINT8 i, j;
//	enum UCT_ERR_CODE rv = UCT_SUCCESS;
//	const struct balance_t * gb = balance_read();
//
//	for (j = 0; j < 3; j++)
//	{
//		balance_ptr[j] = 0;
//
//		for (i = 0; i < 8; i++)
//		{
//			balance_ptr[j] |= gb->flags[i + 8 * j] << i;
//		}
//	}
//
//	//balance_ptr[0] = 0;
//	//balance_ptr[1] = 0;
//	//balance_ptr[2] = 0;
//
//	return (rv);
//}
int balance_init(void)
{
	memset(&gb, 0, sizeof(struct balance_t));
	return (0);	// balance_timer_init();
}
int balanceProcess(void)
{
	int systemstate = getSystemState();
	if (systemstate == SYSTEM_MODE_SLEEP)
		return balance_cal();
	else
	{
		close_all_balance();
		return -1;
	}
}

int testbalance(void)
{
	UINT8 i = 0, n = 0, count = 0;
	while (1)
	{
		for (i = 0; i < TOTAL_VOLTAGES; i++)
		{
			if (count < 5)
			{
				OPEN_BANLANCE_BY_LTC6804(i);
				if (i)
					CLOSE_BANLANCE_BY_LTC6804(i-1);
				delay_ms(200);
			}
			if (count > 5)
			{
				if (count % 2 == 0)
					n = 1 - n;
				if ((i + n) % 2 == 0)
				{
					OPEN_BANLANCE_BY_LTC6804(i);
				}
				else
				{
					CLOSE_BANLANCE_BY_LTC6804(i);
				}
			}
		}
		if (count > 15)
			count = 0;
		count++;
	}
}
