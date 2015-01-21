/*********************************************************
 * File Description
 *
 *   $Revision: 2057 $
 *   $Author: huhai $
 *   $Date: 2014-07-11 14:24:25 +0800 (2014) $
 *
 *********************************************************/

#ifndef _BANLANCE_BATTERY_H_
#define _BANLANCE_BATTERY_H_

#include <stdio.h>
#include "public/public.h"
#include "banlance_battery/banlancebattery.h"
#include "voltage/voltage.h"
#include "temp/temperature.h"

#define TOTAL_BALANCES TOTAL_VOLTAGES

struct balance_t
{
	UINT8 flags[MAX_ALLOWED_CELLS];
	UINT8 Bytebalances[MAX_ALLOWED_CELLS / 8 + (MAX_ALLOWED_CELLS % 8 == 0 ? 0 : 1)];
};

#define R_BANLANCE  100
//#define OPEN_BANLANCE_BY_LTC6804(index)   ltc6804_set_balance( index, 1)//enable_bypass_resistor((index/6),(index%6))//
//#define CLOSE_BANLANCE_BY_LTC6804(index)    ltc6804_set_balance( index, 0)//disable_bypass_resistor((index/6),(index%6))//
//struct balance_t *
//balance_read(void);
//void balance_cal1(void);
//enum UCT_ERR_CODE battery_blance_sw_get(UINT8 *balance_ptr);
int balance_init(void);
int balanceProcess(void);
int balance_cal(void);
int testbalance(void);

int close_all_balance(void);
UINT16 get_balance_current_by_index(UINT8 index);
//void test_union(void);
//void balance_cal(void);
//void set_threshold_v(UINT16 v);
//UINT16 get_threshold_v(void);

#endif

