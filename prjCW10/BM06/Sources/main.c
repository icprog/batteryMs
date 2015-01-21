/*
 * main.c
 * by mgc 20150105
 * */

#include "MPC5634M.h"
#include "IntcInterrupts.h"  
#include <stdio.h>
#include <string.h>
#include "public/systemState.h"
#include "public/types.h"
#include "public/err.h"
#include "ctl/control.h"
#include "voltage/voltage.h"
#include "temp/temperature.h"
#include "current/currentAbstract.h"
#include "protocol/accp_protocol.h"
#include "protocol/cpcu_protocol.h"
#include "protocol/vcu_protocol.h"
#include "protocol/uart_protocol.h"
#include "ctl/ColdHeatManager.h"
#include "banlance_battery/banlancebattery.h"
#include "ctl/relay.h"
#include "public/drivers.h"
#include "app/selftest/selftest.h"

typedef int
(*type_monitor_func)(int argc, char *argv[]);
int BMS_selfCheck(void);
enum running_interval_timems
{
	TIMEMS_50, TIMEMS_100, TIMEMS_200, TIMEMS_500, TIMES_1
};

struct hardware_monitor
{
	type_monitor_func func;
	UINT32 interval_timems;
	UINT32 time_last;
};

#define MONITOR_START { 
#define MONITOR_END };
#define SET_HARDWARE_MONITOR( func, timems, startTimems ) {(type_monitor_func)(func),timems, ((timems)+(startTimems))}

static struct hardware_monitor hardware_monitors[] =
{
		SET_HARDWARE_MONITOR( ltc6804_voltage_monitor, 1000, 0),
		SET_HARDWARE_MONITOR( ltc6804_temp_monitor, 5500, 20 ),
		SET_HARDWARE_MONITOR( adc_update, 50, 50),
		SET_HARDWARE_MONITOR( balanceProcess, 1000, 500),
//		SET_HARDWARE_MONITOR( charge_current_calculat, 50, 50),
//		SET_HARDWARE_MONITOR( vcu_protocol, 10, 0),
//		SET_HARDWARE_MONITOR( dccp_protocol, 10, 0),
//		SET_HARDWARE_MONITOR( soc_calculate, 100, 100),

	};

static const hardware_monitors_size = sizeof(hardware_monitors)
		/ sizeof(struct hardware_monitor);

static int sensorCapture(UINT32 timems)
{
	struct hardware_monitor *m_now = NULL;
	UINT8 i = 0;
	int ret = 0;
	UINT32 tnow = timems;
	/*monitors*/
	for (i = 0; i < hardware_monitors_size; i++)
	{
		m_now = &hardware_monitors[i];
		if (TIME_TRHESHOLD_CAL(tnow, m_now->time_last) > m_now->interval_timems)
		{
			m_now->time_last = tnow;
			ret = m_now->func(0, 0);
			if (ret < 0)
			{
				//  system err;          
			}
		}
	}

	if (getLtc6804VoltageUpdate() == 1)
	{
		observerVoltageUpdate();
	}

	if (getLtc6804TempUpdate() == 1)
	{
		observerTemperatureUpdate();
		ColdHeatControler();
	}

	observerCurrentUpdate();
	return 0;
}
struct gloable_flags
{
	int selfCheckFlag;
};
static struct gloable_flags gflags =
{
	0 };

int BMS_GetErrStatusBMSCheckSelf(void)
{
	if (gflags.selfCheckFlag != ERR_BMS_NONE)
		return 1;
	else
		return 0;
}

int BMS_selfCheck(void)
{
	return gflags.selfCheckFlag;
}

typedef int (*typefaultProcess)(void);
static typefaultProcess vcufp[] =
{
	bms_self_detection_update,
	AlarmUpdateVoltage,
	AlarmUpdateTemperature,
	AlarmUpdateCurrent,
	AlarmUpdateSoc,
	AlarmUpdateInsulutionResistance,
	AlarmUpdateRelayModule,
	AlarmUpdateBmsSelfCheck };
static typefaultProcess accufp[] =
{
	0 }; //TODO
static typefaultProcess dccufp[] =
{
	0 }; //TODO

/*err process*/
static int faultProcess(UINT32 timemsNow)
{
	UINT8 i = 0;
	int ret = 0, len = 0;
	typefaultProcess *fp = NULL;
	(void) timemsNow;
	ret = getSystemState();
//	if( ret == SYSTEM_MODE_DISCHARGE)
//	{
	// always do this fault check!!!
	fp = vcufp;
	len = sizeof(vcufp) / sizeof(typefaultProcess);
	for (i = 0; i < len; i++)
	{
		if (fp[i])
		{
			ret = fp[i]();
			if (ret < 0)
			{
				//  err
			}
		}
	}

	if (ret == SYSTEM_MODE_AC_CHARGE)
	{
		fp = accufp;
		len = sizeof(accufp) / sizeof(typefaultProcess);
	}
	else if (ret == SYSTEM_MODE_DC_CHARGE)
	{
		fp = dccufp;
		len = sizeof(dccufp) / sizeof(typefaultProcess);
	}
	else
	{
		return 0;
	}

	ret = 0;

	for (i = 0; i < len; i++)
	{
		if (fp[i])
		{
			ret = fp[i]();
			if (ret < 0)
			{
				//  err
			}
		}
	}
	return ret; // 
}

/*protocol process*/
static int CommunicationProcess(UINT32 timemsNow)
{
	int i = getSystemState();
	(void) timemsNow;
	if (i == SYSTEM_MODE_DISCHARGE)
	{
		i = VCU_communication();
	}
	else if (i == SYSTEM_MODE_AC_CHARGE)
	{
		i = ACCP_communication();
	}
	else if (i == SYSTEM_MODE_DC_CHARGE)
	{
		i = DCCP_communication();
	}

//	if (debugByUartEn()) // 
	{
		uart_communication(); // 
	}
	return i;	// 
}

static int algorithmProcess(UINT32 timemsNow)
{
	int i = getSystemState();
	(void) timemsNow;
	socProcess(); // 
//	sohProcess(); //TODO
	if (i == SYSTEM_MODE_DISCHARGE)
	{
	}
	else if (i == SYSTEM_MODE_AC_CHARGE)
	{
	}
	else if (i == SYSTEM_MODE_DC_CHARGE)
	{
	}
	else
	{
	}
	return 0; // 
}

/*controls*/
static int systemControl(UINT32 timemsNow)
{
	(void) timemsNow;
	DriverUpdate(FD_GPIO_BY_INDEX);
	return 0;
}

//
//static int systemWakeUp(void)
//{
//	return 0; //TODO
//}

static int systemSleep(UINT32 timemsNow)
{
	int i = 0;
	static UINT32 timemsLast = 0;
	static int last_state = SYSTEM_MODE_SLEEP;
	(void) timemsNow;

	i = getSystemState();
	if (last_state != i)
	{
		if (last_state != SYSTEM_MODE_SLEEP && i == SYSTEM_MODE_SLEEP)
		{
//			systemEntrySleep(); // 
		}
		else if (last_state == SYSTEM_MODE_SLEEP && i != SYSTEM_MODE_SLEEP)
		{
//			systemWakeUp(); // 
		}
		last_state = i;
	}
	return i;
}

static int parameterManager(UINT32 tnow)
{
	storeAllParametersByTimer(tnow, 2000);
}

static int systemInit(void)
{

	Config_PLL();
	Adc_InitADC0();
	INTC_InitINTCInterrupts();
	initPIT_RTI();
	scia_init();
	spi_init();
	gpioFuncsInit();
	eepromInit();
	uart_protocol_init();
	initCAN();

	bms_self_test();

	socInit();
	balance_init();
	eepromParametersInit();
	bms_self_detection_init();
	enableIrq();
}

struct s_main_function
{
	UINT8 enable;
	UINT32 timeInterval;
	int (*func)(UINT32 timemsNow);
	UINT32 timemsLast;
	const char *name;
	int OP_MODE;
};
#define getStr(str) #str
#define REGISTER_MAIN_FUNCTION( func, time, mode )\
{ 1, time, func, 0, getStr(func),mode }

static struct s_main_function gFunctionCtl[] =
{
		REGISTER_MAIN_FUNCTION(sensorCapture, 0,SYSTEM_MODE_DEEPSLEEP|SYSTEM_MODE_SLEEP|SYSTEM_MODE_DC_CHARGE|SYSTEM_MODE_AC_CHARGE|SYSTEM_MODE_DISCHARGE),
		REGISTER_MAIN_FUNCTION(faultProcess, 0,SYSTEM_MODE_SLEEP|SYSTEM_MODE_DC_CHARGE|SYSTEM_MODE_AC_CHARGE|SYSTEM_MODE_DISCHARGE),
		REGISTER_MAIN_FUNCTION(algorithmProcess, 0,SYSTEM_MODE_SLEEP|SYSTEM_MODE_DC_CHARGE|SYSTEM_MODE_AC_CHARGE|SYSTEM_MODE_DISCHARGE),
		REGISTER_MAIN_FUNCTION(CommunicationProcess, 0,SYSTEM_MODE_SLEEP|SYSTEM_MODE_DC_CHARGE|SYSTEM_MODE_AC_CHARGE|SYSTEM_MODE_DISCHARGE),
		REGISTER_MAIN_FUNCTION(systemControl, 0,SYSTEM_MODE_SLEEP|SYSTEM_MODE_DC_CHARGE|SYSTEM_MODE_AC_CHARGE|SYSTEM_MODE_DISCHARGE),
		REGISTER_MAIN_FUNCTION(parameterManager, 2000,SYSTEM_MODE_SLEEP|SYSTEM_MODE_DC_CHARGE|SYSTEM_MODE_AC_CHARGE|SYSTEM_MODE_DISCHARGE),
		REGISTER_MAIN_FUNCTION(systemSleep, 0,SYSTEM_MODE_SLEEP|SYSTEM_MODE_DEEPSLEEP)
};
static const int sizeof_gfunctionCtl = sizeof(gFunctionCtl)
		/ sizeof(struct s_main_function);

/*
 * param:
 * 	@ctlValue: 0 disable, 1 enable
 * 	@name:
 * 	
 * */
static int CtlFunctionByName(const char *name, UINT8 cltValue)
{
	int i = 0;
	
	if(strcmp(name,"all") == 0 )
	{
		for (i = 0; i < sizeof_gfunctionCtl; i++)
		{
			gFunctionCtl[i].enable = cltValue;
		}
		return 0;
	}

	for (i = 0; i < sizeof_gfunctionCtl; i++)
	{
		if (strcmp(name, gFunctionCtl[i].name) == 0)
		{
			gFunctionCtl[i].enable = cltValue;
		}
	}
	return -1;
}


int main(void)
{
	int i = 0, ret = 0, enable;
	int state = 0;
	UINT32 tnow = 0, tlast = 0;
	systemInit();
	
	CtlFunctionByName("all",1);

	/*VERSION 2*/
	for (;;)
	{
		/*get time*/
		tnow = getSystemTimeMs();
		
		/*get system state*/
		SystemStateMachine(tnow);
		state = getSystemState();
		mprintf("%d  \r\n",state );
		
		for (i = 0; i < sizeof_gfunctionCtl; i++)
		{
			enable = 0;
			if((state &gFunctionCtl[i].OP_MODE) != 0)
			{
				gFunctionCtl[i].enable = 1;
			}
			else
			{
				gFunctionCtl[i].enable = 0;
			}
			
			if (gFunctionCtl[i].enable && gFunctionCtl[i].func !=0  )
			{
				/*get system time*/
				tnow = getSystemTimeMs();

				if (gFunctionCtl[i].timeInterval == 0)
				{
					enable = 1;
				}
				else if (TIME_TRHESHOLD_CAL(tnow, gFunctionCtl[i].timemsLast)
						> gFunctionCtl[i].timeInterval)
				{
					gFunctionCtl[i].timemsLast = tnow;
					enable = 1;
				}
				if (enable)
				{
					tlast = tnow;
					mprintf("run function<%s(%dms)> ", gFunctionCtl[i].name, tlast);
					gFunctionCtl[i].func(tnow);
					tnow = getSystemTimeMs();
					mprintf("takes %d milliseconds!\r\n",
							TIME_TRHESHOLD_CAL(tnow, tlast));
				}
			}
		}
	}
	/*VERSION 1*/
	for (;;)
	{

		/*get system time*/
		tnow = getSystemTimeMs();

		/*sensor info capture*/
		sensorCapture(tnow);

		/*get system state*/
		SystemStateMachine(tnow);

		/*err process*/
		faultProcess(tnow);

		/*algorithm*/
		algorithmProcess(tnow);

		/*protocol process*/
		CommunicationProcess(tnow);

		/*controls*/
		systemControl(tnow);

//		wdog(); //TODO

		systemSleep(tnow); //TODO

		/*store it per 2 second*/
		parameterManager(tnow);

		mprintf("running... time:%dms state:%d\r\n", tnow, getSystemState());
	}
}

