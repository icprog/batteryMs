/*
 * intc.c
 *
 *  Created on: Nov 11, 2014
 *      Author: Administrator
 */

#include "MPC5634M.h"
#include "public/types.h"
#include "driver/intc/intc.h"
#include "driver/timer/timer.h"
#include "IntcInterrupts.h" 
#include "driver/Gpio/gpio.h"
#include "driver/adc/adc.h"
#include "public/drivers.h"
//#include "gpio.h"
static UINT32 CP_time, CP_time1, CP_time2,CP_time3,CP_high;
static UINT8 CP_flag = 0,Sys_flag=0;
static UINT16 CP_NB = 50;
static UINT8 CP_state,CC_state;
UINT8 
Sys_wakeup;
#define PRECISION  10
#define VOL_12V  3000
#define VOL_9V  2250
#define VOL_6V  1500
//#define sleep_onehour    3600000


enum ACCP_CONNECT
	 {	
	     ACCP_NOLINK = 0,
		 CUR_16A ,
		 CUR_32A ,
		 UN_KNOWM
	 };

static void init_INTC_CP_C(void)
{
	SIU.DIRER.B.EIRE11 = 1;
	SIU.IFEER.B.IFEE11 = 1;
	SIU.IREER.B.IREE11 = 1;
	SIU.PCR[137].R = 0x0900;
}

static void INTC_CP_C(void)
{
	CP_flag++;
	if (CP_flag == 1)
	{
		CP_time1 = getSystemTimeMs();
	}
	else if (CP_flag == 2)
	{
		CP_time2 = getSystemTimeMs();
		if(SIU .GPDO[IO_STATUS_CP_C].R == 0)
		CP_high = TIME_TRHESHOLD_CAL(CP_time2, CP_time1);
	}
	else if(CP_flag == 3)
	{
	if(SIU .GPDO[IO_STATUS_CP_C].R == 0)
	CP_high = TIME_TRHESHOLD_CAL(CP_time3, CP_time2);
	CP_time3 = getSystemTimeMs();
	CP_time = TIME_TRHESHOLD_CAL(CP_time3, CP_time1);
	CP_flag = 0;
	Sys_flag = 0;
	}
	SIU .EISR.B.EIF11 = 1;
}

 void start_PWM_MONITOR(void)
{
    Sys_flag++;
	init_INTC_CP_C();
	INTC_InstallINTCInterruptHandler(INTC_CP_C, 50, 1);
	if(!CP_flag)    // 3周期没有信号
	{
		if(Sys_flag>=3)
			CP_state = ACCP_NOLINK;
	}
}

 static void stop_PWM_MONITOR(void)
 {
	 SIU.DIRER.B.EIRE11 = 0;
 }
 
void CP_test()
{
	SIU.PCR[135].R = 0x0203;
	while(1)
	{
	start_PWM_MONITOR();
	SIU.GPDO[135].R = 0;
	delay_ms(10);
	SIU.GPDO[135].R = 1;
	delay_ms(10);
	}
}

UINT16 getPwm(void)
{
    CP_state = (UINT8)(0x000000ff&((CP_high*60)/CP_time));
    if(CP_state == 0)
    CP_state = ACCP_NOLINK;
    else if((14<CP_state)&&(CP_state<18))
    CP_state =  CUR_16A;
    else if((CP_state>30)&&(CP_state<34))
    {
    CP_state = CUR_32A;
    }
    else
    CP_state = UN_KNOWM;
	return CP_state;
}

UINT16 getCC_state(void)
{
	UINT16 result_mv;
	result_mv = getMvByAdc(ID_ADC_PORT_ACCIN);
	if((result_mv<((VOL_12V/10)+VOL_12V))&&(result_mv< (VOL_12V-(VOL_12V/10))))
	CC_state = ACCP_NOLINK;
	else if((result_mv<((VOL_9V/10)+VOL_9V))&&(result_mv< (VOL_9V-(VOL_9V/10))))
	{
	CC_state = CUR_16A;
	}
	else if((result_mv<((VOL_6V/10)+VOL_6V))&&(result_mv< (VOL_6V-(VOL_6V/10))))
	{
	CC_state = CUR_32A;
	}
	else
	CC_state = UN_KNOWM;		
	return CC_state;
}
enum SYS_WAKEUP
{
	time_wakeup = 0,
	acc_wakeup = 1
};

static void INTC_wakeup(void)
{
	SIU .EISR.B.EIF9 = 1;
	Sys_wakeup = acc_wakeup;
	//printf("sending system wake up\r\n");
}
static void TIME_wakeup(void)
{
	timer_stop(TIMER_ID_WAKE_UP);
	Sys_wakeup = time_wakeup;
	//printf("sending system wake up\r\n");
}
static void init_INTC_ON(void)
{
	SIU .DIRER.B.EIRE9 = 1;
	SIU .IFEER.B.IFEE9 = 1;
	SIU.PCR[13].R = 0x0900;
	INTC_InstallINTCInterruptHandler(INTC_wakeup, 50, 1);
}

UINT8 SYS_wakeupstate()
{
	return Sys_wakeup;
}

UINT8 SYS_sleep(UINT32 sleeptime)
{
	init_PIT(TIMER_ID_WAKE_UP);
	init_INTC_ON();
	timer_add(TIMER_ID_WAKE_UP,TIME_wakeup, sleeptime);
//	SIU.HLT.B.CNASTP = 1;
	SIU.HLT.R = 0x01b45603;	
//	if(SIU.HLTACK.B.CPUACK == 1)
//	 return 0;
//	else
//     return 1;
}
UINT8 FLAG = 1;
void sleeptest()
{
   while(FLAG){
	SYS_sleep(1000);
	FLAG =0 ;}
	
}
