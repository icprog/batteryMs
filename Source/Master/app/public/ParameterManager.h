/*
 * ParameterManager.h
 *
 *  Created on: Dec 18, 2014
 *      Author: Administrator
 */

#ifndef PARAMETERMANAGER_H_
#define PARAMETERMANAGER_H_
#include "public/public.h"
#include "current/CurrentAbstract.h"
#include "temp/temperature.h"
#include "voltage/voltage.h"

#define MAX_ALLOWED_CELLS_PERSLAVE (12)
#define MAX_ALLOWED_SLAVES (8)
#define MAX_ALLOWED_CELLS (MAX_ALLOWED_SLAVES*MAX_ALLOWED_CELLS_PERSLAVE)

#pragma pack(1)
struct time_sleep //4*3
{
	UINT32 time_PowerOffToSleep;	
	UINT32 time_SleepToSober;
	UINT32 time_SoberToSleep;
};

struct dev_info_slaves //4
{
	UINT8 nCellsEachSave;
	UINT8 nTempsEachSlave;
	UINT8 nSalves;
	UINT8 totalCells;
};
struct dev_info
{
	UINT16 COMMBYTE ; //通信版本     V1.0					
	UINT16 BARTYPE;     //电池类型   NCM					
	TYPE_CURRENT CHARGE_RATECURRENT;    //额定电流  充电45A，放电65A；			
	TYPE_CURRENT DISCHARGE_RATECURRENT;//
	TYPE_CURRENT CHARGI;// = ,//最高允许充电电流                180A				// 5*2  = 10
	TYPE_VOLTAGE RATEVOLTE;// = ,      //额定电压  288V					
	TYPE_VOLTAGE SCHGV;// = , //单体动力蓄电池最高允许充电电压  4.2V					
	TYPE_VOLTAGE ALVOLT; //整车动力蓄电池总电压  这个就是我们测得的总电压					
	TYPE_VOLTAGE VREQUEST ;//,  //电压需求        ？？这块不知道				
	TYPE_VOLTAGE ACHATEV;// = ,//最高允许充电总电压          4.2x80=336v				
	UINT32 AWEIGHT;// = ,//动力蓄电池标称总重量   目前不知道        				//6*4		 = 24		
	TYPE_TEMP ALTEMP;// = ,//最高允许温度         一级报警温度					//2
	TYPE_SOC ELCSTATE;// = ; //整车动力蓄电池荷电状态  这个就是SOC		    //1
	UINT32 BATTERY_Q;//电池额定容量			90Ah			       //4
	struct time_sleep sleepCtlTime;								//12
	struct dev_info_slaves devSlaveInfo;						//4				..=23
	float COVARANCE[9];// 初始误差协方差			3x3矩阵，单精度		    //4*9 = 36 
};  // =83
struct cell_voltage_threashold
{
	TYPE_VOLTAGE vol_low;
	TYPE_VOLTAGE vol_low_level1;
	TYPE_VOLTAGE vol_low_level2;
	TYPE_VOLTAGE vol_high;
	TYPE_VOLTAGE vol_high_level1;
	TYPE_VOLTAGE vol_high_level2;
};
struct sum_voltage_threashold
{
	TYPE_VOLTAGE vol_low;
	TYPE_VOLTAGE vol_low_level1;
	TYPE_VOLTAGE vol_low_level2;
	TYPE_VOLTAGE vol_high;
	TYPE_VOLTAGE vol_high_level1;
	TYPE_VOLTAGE vol_high_level2;
};

struct voltage_cell_diff_threshold 
{
	TYPE_VOLTAGE cell_diff_danger1;
	TYPE_VOLTAGE cell_diff_danger2;
};

struct voltage_const_vars
{
	struct cell_voltage_threashold cell_vol_value;
	struct sum_voltage_threashold sum_vol_value;
	struct voltage_cell_diff_threshold cell_diff_value;
};
struct cell_temperature_threashold
{
	INT16 temp_low;
	INT16 temp_low_level1;
	INT16 temp_low_level2;
	INT16 temp_high;
	INT16 temp_high_level1;
	INT16 temp_high_level2;
};

struct temperature_cell_diff_threshold
{
	UINT16 temp_diff_danger1;
	UINT16 temp_diff_danger2;
};

struct temperature_const_vars
{
	struct cell_temperature_threashold cell_temp_value;
	struct temperature_cell_diff_threshold cell_diff_value;
};

struct CCurrentThresholdValue
{
	TYPE_CURRENT mThreatholdOCLEVEL1;
	TYPE_CURRENT mThreatholdOCLEVEL2;
	UINT8 adChannel;
};

struct CCurrentsThresholdConstValue
{
	struct CCurrentThresholdValue mCurrentThreshold[4];
};

typedef struct S_CParameter
{
	struct voltage_const_vars e_vol;
	struct temperature_const_vars e_temp;
	struct CCurrentsThresholdConstValue e_cur;
	struct dev_info e_devInfo;
}Cparameter;


enum paramIds
{
	/*voltage*/
	PARAM_ID_OV_CELL , //= 4200, // the threshold value that cell over voltage
	PARAM_ID_OV_CELL_LEVEL1 , //= 4300; //the warning lever 1 threshold value of cell over voltage 
	PARAM_ID_OV_CELL_LEVEL2 , //= 4250; //the  warning lever 2threshold value of cell over voltage 
	PARAM_ID_UV_CELL, // = 2800, //; //the warning threshold value of cell under voltage
	PARAM_ID_UV_CELL_LEVEL1, // = 2700; //the warning lever 1 threshold value of cell under voltage
	PARAM_ID_UV_CELL_LEVEL2, //= 2750; //the warning lever 2 threshold value of cell under voltage

	PARAM_ID_OV_SUM , //= 336000; //UINT16 vol_low;
	PARAM_ID_OV_SUM_LEVEL1, // = 344000; //UINT16 vol_low_level1;
	PARAM_ID_OV_SUM_LEVEL2, // = 340000; //UINT16 vol_low_level2;
	PARAM_ID_UV_SUM, // = 224000;//87000; //UINT16 vol_high;
	PARAM_ID_UV_SUM_LEVEL1, // =216000;// 84000; //UINT16 vol_high_level1;
	PARAM_ID_UV_SUM_LEVEL2, // = 220000;//86000; //UINT16 vol_high_level2;

	PARAM_ID_DIFF_VOL_CELL_LEVEL1 , //= 380;
	PARAM_ID_DIFF_VOL_CELL_LEVEL2 , //= 300;
	
	
	/*temperatures*/
	PARAM_ID_UT , //= -20;
	PARAM_ID_UT_LEVEL1, // = -28;
	PARAM_ID_UT_LEVEL2, // = -25;
	PARAM_ID_OT , //= 55;
	PARAM_ID_OT_LEVEL1, // = 65;
	PARAM_ID_OT_LEVEL2, // = 60;
	PARAM_ID_DIFF_TEMP_CELL_LEVEL1, // =12 ;
	PARAM_ID_DIFF_TEMP_CELL_LEVEL2, // = 8;

	PARAM_ID_TEMP_FAN_OPEN, // = 35;
	PARAM_ID_TEMP_FAN_CLOSE, //= 30;
	PARAM_ID_TEMP_HEAT_OPEN, //= 5;
	PARAM_ID_TEMP_HEAT_CLOSE, //= 10;

	/*
	 * for ltc68042
	 * */
	 PARAM_ID_NCLEES_EACH_SLAVE, // 12
	 PARAM_ID_NTEMPS_EACH_SLAVE, // 4
	 PARAM_ID_N_SLAVES, // 7
	 PARAM_ID_MAX_CELLS, // 80  //((N_SLAVES)*(NCLEES_EACH_SLAVE))

	/*
	 * for soc 
	 * */
	 PARAM_ID_TOTAL_SOC_NUM, // 1
	 PARAM_ID_TOTAL_SOH_NUM, // 1
	 PARAM_ID_TOTAL_CHARGE_STATUS_NUM, // 1
	 PARAM_ID_TOTAL_BALANCE_NUM, // TOTAL_VOLTAGE_NUM
	 PARAM_ID_INITAL_VALUE, // TODO

	/*
	 *for cold control (degrees)
	 *35,30,5,15
	 * */
	 PARAM_ID_TOTAL_FANS_NUM, // 1
	 PARAM_ID_TOTAL_HEATPANNALS_NUM // 1
};

#define N_SLAVES gParameters.e_devInfo.devSlaveInfo.nSalves
#define NTEMPS_EACH_SLAVE 	gParameters.e_devInfo.devSlaveInfo.nTempsEachSlave
#define MAX_CELLS gParameters.e_devInfo.devSlaveInfo.totalCells
#define NCLEES_EACH_SLAVE gParameters.e_devInfo.devSlaveInfo.nCellsEachSave
#define TOTAL_TEMPERATURE_SENSORS_NUM (N_SLAVES*NTEMPS_EACH_SLAVE)

#define g_const_vol_value gParameters.e_vol
#define Q_SUM gParameters.e_devInfo.BATTERY_Q


int setParamById( UINT8 ids,  DF64 paramter);
DF64 getParamById(UINT8 ids);
int eepromParametersInit(void);
int storeParameterByAddr( UINT8 addrOffset, UINT8 *paramters, int len );
int storeAllParameters(void);
int storeAllParametersByTimer(UINT32 timemsNow, UINT32 timeTH);
extern Cparameter gParameters;

#endif /* PARAMETERMANAGER_H_ */
