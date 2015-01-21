/*********************************************************
 *
 *public.h
 *by mgc 20141124
 *
 *
 *********************************************************/

#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#include "public/types.h"
#include "middleLayer/appToDriver.h"
#include "public/ParameterManager.h"
//#include "init_class.h"

#define assert(v)  while(!(v))

#define BMS_VERSION "$Revision: 3404 $"
#define HARDWARE_VER 0


enum CANINFORPARAM
{

	COMMBYTE = 256,  //       
	//BARTYPE = 3, 锟斤拷  //  
	CHARGI = 1800,   //  0.1A    最大充电电流
	SCHGV = 420,      // 0.01 v  单体最高
	AWEIGHT = 900,   //  0.1 A*h   
	ACHATEV = 3360,   //0.1v  最高电压
	VRATING = 2880, //0.1V  额定电压
	ALTEMP =  65,     //1o 最高温度
	IPOWER = 26      //  kw*h      AWEIGHT*VRATING/100000      
    
};
/*for high vol relays*/
enum HIGH_VOL_RELAY_STATUS
{
 HIGH_VOL_RELAY_OP_OPEN  = 1,
 HIGH_VOL_RELAY_OP_SHUTDOWN = 0,
 HIGH_VOL_RELAY_FEEDBACK_LEVEL_CONNECT = 0,
 HIGH_VOL_RELAY_FEEDBACK_LEVEL_DISCONNECT = 1
};

/*for cans*/
static const UINT8 CAN_ADDR_BMS = 244;
static const UINT8 CAN_ADDR_VCU = 39;
static const UINT8 CAN_BROAD_CAST = 255;  

#define AH(ah) ah
static const UINT32 DEFUALT_Q_SUM  = AH(90);
static const  kprechargeAdcVToHighV = 83;
//static const float kprechargeAdcVToHighV = (float)83.00; //
static const float kcurrentAdcVToA = (float)76.3886;

/*
 * for ltc68042
 * */
static const UINT8 DEFUALT_NCLEES_EACH_SLAVE = 12;
static const UINT8 DEFUALT_NTEMPS_EACH_SLAVE= 4;
static const UINT8 DEFUALT_N_SLAVES= 7;
static const UINT8 DEFUALT_MAX_CELLS= 80 ; //((N_SLAVES)*(NCLEES_EACH_SLAVE))

static const UINT8 MAX_ALLOWED_NCLEES_EACH_SLAVE = 12;
static const UINT8 MAX_ALLOWED_N_SLAVES = 7;
static const UINT8 MAX_ALLOWED_NTEMPS_EACH_SLAVE = 5;
//static const UINT8 MAX_ALLOWED_TEMPSENSORS = (MAX_ALLOWED_NTEMPS_EACH_SLAVE*MAX_ALLOWED_NCLEES_EACH_SLAVE);
#define MAX_ALLOWED_TEMPSENSORS  (MAX_ALLOWED_NTEMPS_EACH_SLAVE*MAX_ALLOWED_NCLEES_EACH_SLAVE)


/*
 * for soc 
 * */
#define TOTAL_SOC_NUM 1
#define TOTAL_SOH_NUM 1
#define TOTAL_CHARGE_STATUS_NUM 1
#define TOTAL_BALANCE_NUM TOTAL_VOLTAGE_NUM

/*
 *for cold control (degrees)
 *35,30,5,15
 * */
#define TOTAL_FANS_NUM 1
#define TOTAL_HEATPANNALS_NUM 1

static const TYPE_TEMP VALUE_TH_TEMP_FAN_OPEN = 35;
static const TYPE_TEMP VALUE_TH_TEMP_FAN_CLOSE= 30;
static const TYPE_TEMP VALUE_TH_TEMP_HEAT_OPEN= 5;
static const TYPE_TEMP VALUE_TH_TEMP_HEAT_CLOSE= 10;
static const TYPE_VOLTAGE  VALUE_TH_BALANCE_VOL=3000;

/*
 * for voltage warning check(mv)
 * */
static const TYPE_VOLTAGE VALUE_TH_OV_CELL = 4200; // the threshold value that cell over voltage
static const TYPE_VOLTAGE VALUE_TH_OV_CELL_LEVEL1 = 4300; //the warning lever 1 threshold value of cell over voltage 
static const TYPE_VOLTAGE VALUE_TH_OV_CELL_LEVEL2 = 4250; //the  warning lever 2threshold value of cell over voltage 
static const TYPE_VOLTAGE VALUE_TH_UV_CELL = 2800; //the warning threshold value of cell under voltage
static const TYPE_VOLTAGE VALUE_TH_UV_CELL_LEVEL1 = 2700; //the warning lever 1 threshold value of cell under voltage
static const TYPE_VOLTAGE VALUE_TH_UV_CELL_LEVEL2 = 2750; //the warning lever 2 threshold value of cell under voltage

static const TYPE_VOLTAGE VALUE_TH_OV_SUM = 336000; //UINT16 vol_low;
static const TYPE_VOLTAGE VALUE_TH_OV_SUM_LEVEL1 = 344000; //UINT16 vol_low_level1;
static const TYPE_VOLTAGE VALUE_TH_OV_SUM_LEVEL2 = 340000; //UINT16 vol_low_level2;
static const TYPE_VOLTAGE VALUE_TH_UV_SUM = 224000;//87000; //UINT16 vol_high;
static const TYPE_VOLTAGE VALUE_TH_UV_SUM_LEVEL1 =216000;// 84000; //UINT16 vol_high_level1;
static const TYPE_VOLTAGE VALUE_TH_UV_SUM_LEVEL2 = 220000;//86000; //UINT16 vol_high_level2;

static const TYPE_VOLTAGE VALUE_TH_DIFF_VOL_CELL_LEVEL1 = 380;
static const TYPE_VOLTAGE VALUE_TH_DIFF_VOL_CELL_LEVEL2 = 300;

/*
 * for temperature warning check(degrees)
 * */
static const TYPE_TEMP VALUE_TH_UT = -20;
static const TYPE_TEMP VALUE_TH_UT_LEVEL1 = -28;
static const TYPE_TEMP VALUE_TH_UT_LEVEL2 = -25;
static const TYPE_TEMP VALUE_TH_OT = 55;
static const TYPE_TEMP VALUE_TH_OT_LEVEL1 = 65;
static const TYPE_TEMP VALUE_TH_OT_LEVEL2 = 60;

static const TYPE_TEMP VALUE_TH_DIFF_TEMP_CELL_LEVEL1 =12 ;
static const TYPE_TEMP VALUE_TH_DIFF_TEMP_CELL_LEVEL2 = 8;

/*
 * for current warning check(A)
 * */
static const TYPE_CURRENT THREATHOLD_VALUE_CAHRGE_OC_LEVEL1 =90;//280 ;
static const TYPE_CURRENT THREATHOLD_VALUE_CAHRGE_OC_LEVEL2 = 85;//275;
static const TYPE_CURRENT THREATHOLD_VALUE_DISCAHRGE_OC_LEVEL1 = 90;//280;
static const TYPE_CURRENT THREATHOLD_VALUE_DISCAHRGE_OC_LEVEL2 =  85;//275;
static const TYPE_CURRENT THREATHOLD_VALUE_FEEDBACK_OC_LEVEL1 = 90;//190;
static const TYPE_CURRENT THREATHOLD_VALUE_FEEDBACK_OC_LEVEL2 = 85;//185;

/*
 * for soc warning check (1%)
 * */
static const TYPE_SOC VALUE_TH_SOC_LEVEL1 = 10;
static const TYPE_SOC VALUE_TH_SOC_LEVEL2 = 20;


/*
 * for device info
 * */
static const UINT16 DEFUALT_COMMBYTE  = 256; //通信版本     V1.0					
static const UINT16 DEFUALT_BARTYPE = 0;     //电池类型   NCM					
static const TYPE_CURRENT DEFUALT_CHARGE_RATECURRENT = 45;    //额定电流  充电45A，放电65A；			
static const TYPE_CURRENT DEFUALT_DISCHARGE_RATECURRENT = 65;//
static const TYPE_CURRENT DEFUALT_CHARGI = 180;// = ,//最高允许充电电流                180A				
static const TYPE_VOLTAGE DEFUALT_RATEVOLTE = 288;// = ,      //额定电压  288V					
static const TYPE_VOLTAGE DEFUALT_SCHGV = 4200;// = , //单体动力蓄电池最高允许充电电压  4.2V					
static const TYPE_VOLTAGE DEFUALT_ALVOLT = 0; //整车动力蓄电池总电压  这个就是我们测得的总电压					
static const TYPE_VOLTAGE DEFUALT_VREQUEST = 0;//,  //电压需求        ？？这块不知道				
static const TYPE_VOLTAGE DEFUALT_ACHATEV = 336000;// = ,//最高允许充电总电压          4.2x80=336v				
static const UINT32 DEFUALT_AWEIGHT = 0;// = ,//动力蓄电池标称总重量   目前不知道        								
#define DEFUALT_ALTEMP   VALUE_TH_OT_LEVEL1 // = ,//最高允许温度         一级报警温度
static const TYPE_SOC DEFUALT_ELCSTATE = 0;// = ; //整车动力蓄电池荷电状态  这个就是SOC		
static const UINT32 DEFUALT_BATTERY_Q = 90;//电池额定容量			90Ah		

static const  UINT32 DEFUALT_TIME_POWEROFF_TO_SLEEP = 6000; //下电到进入休眠的时间 单位ms
static const  UINT32 DEFUALT_TIME_SLEEP_TO_SOBER = 36000; //休眠总时间 单位ms
static const  UINT32 DEFUALT_TIME_SOBER_TO_SLEEP = 10000;	//自唤醒时间 单位ms



enum flowdown_state
{
	STATE_BLOW = 1, STATE_DOWN = 3, STATE_IDLE = 0, STATE_SETUP = 2
};



/*bit set--add by mgc 20140730*/

static int mgcSetBit(int b, UINT8 nbit)
{
	int ret = ((b) & (~(1 << (nbit)))) | (1 << (nbit));
	return ret;
}
static int mgcResetBit(int b, UINT8 nbit)
{
	int ret = (b) & (~(1 << (nbit)));
	return ret;
}

#define MGC_SET_BIT(b,n)  ((b) = ((b)&(~(1<<(n))))|(1<<(n))) //(b) = mgcSetBit(b,n) //do{b = mgcSetBit(b,n);}while(0)//((b) = ((b)&(~(1<<(n))))|(1<<(n))) 
#define MGC_RESET_BIT(b, n) (b) = (b)&(~(1<<(n))) //(b) = mgcResetBit(b,n) // do{b = mgcResetBit(b,n);}while(0)//(b) = (b)&(~(1<<(n)))
#define SET_BIT(b,n,v) (((v)==0)?MGC_RESET_BIT((b),(n)):MGC_SET_BIT((b),(n)))


UINT8
ver_hardware_get(void);
int
set_simulator_mode(UINT8 mode);
int
get_simulator_mode(void);

void MCU_Reset(void);
#endif

extern void
mprintf(const char *format, ...);
extern void
print2bank(const char *fmt, ...);
