#ifndef _GPIO_FUNCS_H_
#define _GPIO_FUNCS_H_
/*
 * file:gpioFuncs.c
 * create by mgc 20141118
 * modified by mgc 20150113
 * */
#include "null.h"
#include "driver/Gpio/gpio.h"
#include "driver/Gpio/gpioFuncs.h"
#include "driver/Gpio/port_defs.h"
#include "driver/intc/intc.h"
#include "driver/timer/timer.h"
#include "app/ctl/relay.h"
#include "app/ctl/charge.h"

struct ioInfos
{
	union
	{
		UINT8 BYTES;
		struct
		{
			UINT8 ioStatusNow :1;
			UINT8 ioStatusLast :1;
			UINT8 ioTrigerNow :1;
			UINT8 ioStatusLookFor :1;
			UINT8 reversed :4;
		} BITS;
	} ioInfo;
};

struct gpioPortStatus
{
	struct ioInfos gpioPortStatus[sizeof(gpioPorts)];
};

struct relayFaults
{
	UINT8 RelayNegative;
	UINT8 RelayPositive;
	UINT8 RelayPrecharge;
	UINT8 RelayCharge;
	UINT8 RelayErrStatus;
};

static struct gpioPortStatus myGpioStatus;

/*
 * monitor the io status
 * */

int BMSIOReadMonitor(void)
{
	UINT8 i = 0, pin = 0;
	struct ioInfos *pIo = NULL;
	for (i = 0; i < sizeof(gpioPorts); i++)
	{
		pIo = &myGpioStatus.gpioPortStatus[i];
		pin = gpio_read(gpioPorts[i]);
		{
			pIo->ioInfo.BITS.ioStatusNow = pin;
			if (pIo->ioInfo.BITS.ioStatusLast != pin)
			{
				pIo->ioInfo.BITS.ioStatusLast = pin;
				if (pin == IO_STATUS_HIGH)
				{
					pIo->ioInfo.BITS.ioTrigerNow = IO_RISING_EDG;
				}
				else
				{
					pIo->ioInfo.BITS.ioTrigerNow = IO_FALLING_EDG;
				}
			}
		}
	}
	return sizeof(gpioPorts);
}
static int BMSIOControlmonitor(void)
{
	UINT8 i = 0, pin = 0;
	struct ioInfos *pIo = NULL;
	for (i = 0; i < sizeof(g_out_port); i++)
	{
		pIo = &myGpioStatus.gpioPortStatus[i];
		pin = gpio_read(g_out_port[i]);
//		if (pIo->ioInfo.BITS.ioStatusLookFor != pin)
		{
			gpio_set(g_out_port[i], pIo->ioInfo.BITS.ioStatusLookFor);
		}
	}
}

UINT8 BMS_IO_GET(UINT8 ioId)
{
	assert(ioId<ID_IO_BMS_END);
	return myGpioStatus.gpioPortStatus[ioId].ioInfo.BITS.ioStatusNow;
}
void BMS_IO_SET(UINT8 ioId, UINT8 ioStatus)
{
	assert(ioId<ID_IO_BMS_END);
	if (ioStatus == 0)
		myGpioStatus.gpioPortStatus[ioId].ioInfo.BITS.ioStatusLookFor = 0;
	else
		myGpioStatus.gpioPortStatus[ioId].ioInfo.BITS.ioStatusLookFor = 1;
	return;
}
int getGpioLeverLeverById(UINT8 ioId)
{
	assert(ioId<ID_IO_BMS_END);
	return BMS_IO_GET(ioId);
}

int getGpioLeverChangeStatusById(UINT8 ioId)
{
	assert(ioId<ID_IO_BMS_END);
	if (myGpioStatus.gpioPortStatus[ioId].ioInfo.BITS.ioTrigerNow == 1)
		return 1;
	else
		return 0;
}
int gpioFuncsInit(void)
{
	UINT8 i = 0;
	for (i = 0; i < sizeof(g_out_port); i++)
	{
		gpio_init(g_out_port[i], GPIO_INANDOUT);
	}
	for (i = 0; i < sizeof(g_in_port); i++)
	{
		gpio_init(g_in_port[i], GPIO_INPUT);
	}
	BMSIOControlmonitor();
	BMSIOReadMonitor();
}

int gpioControl(void)
{
	BMSIOControlmonitor();
	BMSIOReadMonitor();
	return 0;
}

#define ID_LED_ALARM ID_IO_CTL_LED2
static void timer_led_isr(void)
{
	BMS_IO_SET(ID_LED_ALARM, 0);
	timer_stop(TIMER_ID_STOP_ALARM_LED);
}

int ledAlarmOnByTimers(UINT32 times)
{
	BMS_IO_SET(ID_LED_ALARM, 1);
	timer_add(TIMER_ID_STOP_ALARM_LED, timer_led_isr, times * 1000);
	return 0;
}

int SystemLedFlash(UINT32 timemsInterval)
{
	static UINT32 lastTimeMs = 0;
	static UINT8 flag = 0;
	UINT32 timeNow = getSystemTimeMs();
	if (TIME_TRHESHOLD_CAL(timeNow, lastTimeMs) > timemsInterval)
	{
		lastTimeMs = timeNow;
		flag = 1 - flag;
		BMS_IO_SET(ID_IO_CTL_LED1, flag);
	}
	return 0;
}

int OpenRelayNegative(void)
{
	BMS_IO_SET(ID_IO_CTL_DISCHARGE_NEGATIVE, HIGH_VOL_RELAY_OP_OPEN);
	if (BMS_IO_GET(ID_IO_STATUS_DISCHARGE_NEGATIVE)
			== HIGH_VOL_RELAY_FEEDBACK_LEVEL_CONNECT)
		return 0;
	else
		return -1;
}

int OpenRelayPositive(void)
{
	BMS_IO_SET(ID_IO_CTL_DISCHARGE_POSITIVE, HIGH_VOL_RELAY_OP_OPEN);
	if (BMS_IO_GET(ID_IO_STATUS_DISCHARGE_POSITIVE)
			== HIGH_VOL_RELAY_FEEDBACK_LEVEL_CONNECT)
		return 0;
	else
		return -1;
}

int ShutdownRelayNegative(void)
{

	BMS_IO_SET(ID_IO_CTL_DISCHARGE_NEGATIVE, HIGH_VOL_RELAY_OP_SHUTDOWN);
	if (BMS_IO_GET(ID_IO_STATUS_DISCHARGE_NEGATIVE)
			== HIGH_VOL_RELAY_FEEDBACK_LEVEL_DISCONNECT)
		return 0;
	else
		return -1;
}

int ShutdownRelayPositive(void)
{
	BMS_IO_SET(ID_IO_CTL_DISCHARGE_POSITIVE, HIGH_VOL_RELAY_OP_SHUTDOWN);
	if (BMS_IO_GET(ID_IO_STATUS_DISCHARGE_POSITIVE)
			== HIGH_VOL_RELAY_FEEDBACK_LEVEL_DISCONNECT)
	{

		return 0;
	}
	else
		return -1;
}

enum relay_op_result
{
	RELAY_OP_SUCCESS = 0
};

int OpenRelayPrecharge(void)
{
	BMS_IO_SET(ID_IO_CTL_PRECHARGE, HIGH_VOL_RELAY_OP_OPEN);
	return (RELAY_OP_SUCCESS);
}

static void timer_isr_shutdownRelayPrecharge(void)
{
	BMS_IO_SET(ID_IO_CTL_PRECHARGE, HIGH_VOL_RELAY_OP_SHUTDOWN);
	timer_stop(TIMER_ID_SHUTDOWN_RELAY_PRECAHRGE);
}

int ShutdownRelayPrechargeByTimerS(UINT32 times)
{
	int ret = timer_add(TIMER_ID_SHUTDOWN_RELAY_PRECAHRGE,
			timer_isr_shutdownRelayPrecharge, times * 1000);
	return ret;
}

int OpenRelayCharge(void)
{

	BMS_IO_SET(ID_IO_CTL_CHARGE, HIGH_VOL_RELAY_OP_OPEN);
	return (RELAY_OP_SUCCESS);

}

int shutdownRelayCharge(void)
{
	BMS_IO_SET(ID_IO_CTL_CHARGE, HIGH_VOL_RELAY_OP_SHUTDOWN);
	return (RELAY_OP_SUCCESS);
}

int keyToOn(void)
{
	static UINT8 laststatus = 0;
	UINT8 status = BMS_IO_GET(ID_IO_STATUS_ON);
	if (laststatus != status)
	{
		laststatus = status;
		if (status == 1)
			return 1;
	}
	return 0;
}

int keyToACC(void)
{
	static UINT8 laststatus = 0;
	UINT8 status = gpio_read(IO_STATUS_ACC);
	if (laststatus != status)
	{
		laststatus = status;
		if (status == 1)
			return 1;
	}
	return 0;
}

int OpenVEH_Charger_CPC(void)
{
	BMS_IO_SET(ID_IO_STATUS_CP_C, HIGH_VOL_RELAY_OP_OPEN);
}
int CloseVEH_Charger_CPC(void)
{
	BMS_IO_SET(ID_IO_STATUS_CP_C, HIGH_VOL_RELAY_OP_SHUTDOWN);
}

int testIosForHardware(UINT32 timeMsNow)
{
	static UINT32 timeMslast = 0;
	static UINT8 count200Ms = 0;
	UINT8 i = 0;
	UINT32 t = 0;
	t = TIME_TRHESHOLD_CAL(timeMslast, timeMsNow);
	if (t > 200)
	{
		timeMslast = timeMsNow;
		count200Ms++;
		for (i = 0; i < sizeof(gpioPorts); i++)
		{
			/*space and time:for and if...*/
			if (count200Ms == 10)
			{
				gpio_set(gpioPorts[i], GPIO_HIGH);
			}
			else if (count200Ms == 20)
			{
				gpio_set(gpioPorts[i], GPIO_LOW);
			}

			if (count200Ms % 4 == 0)
			{
				mprintf("[GPIO%03d(%01d)]", gpioPorts[i],
						gpio_read(gpioPorts[i]));
				if (i % 6 == 0)
					mprintf("\r\n");
			}
		}
		if (count200Ms == 20)
			count200Ms = 0;
	}
}

#endif

