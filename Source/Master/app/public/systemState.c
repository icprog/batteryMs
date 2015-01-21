/*
 * the next is the system state machine module
 * */
#include "public/systemState.h"
#include "public/drivers.h"
#include "public/err.h"
#include "public/types.h"
#include "app/middleLayer/appToDriver.h"
//#include "Gpio/gpioFuncs.h"
#include "ctl/charge.h"
#include "ctl/relay.h"

#define SIMULATE_CHARGE_DISCHARGE_EN   1
static const UINT32 VALUE_TH_TIMEMS_OF_BATTERY_REPOSE = (2 * 60 * 60); //2 hours

struct system_module
{
	UINT8 systemModule;
	UINT8 gSimulateSystemState;
};

struct s_time_of_repose
{
	int en;
	UINT32 timems_start;
	UINT32 timems_repose;
};

static struct system_module gSystemMode =
{
	SYSTEM_MODE_SLEEP };

static struct s_time_of_repose gTimeOfRepose =
{
	0,
	0,
	0 };

/*
 * calculate the time of repose of the package.
 * user can use the function IsTimeOfReposeEnough() to get 
 * if the time is enough for repose.(return 1, enough;else not).
 * the threshold value of this is defined by VALUE_TH_TIMEMS_OF_BATTERY_REPOSE
 * at the head of this file.
 * by mgc 20150108.
 * */
static void RecordTimeOfReposeStart(UINT32 timems)
{
	gTimeOfRepose.timems_start = timems;
}

static void RecordTimeOfReposeEnd(UINT32 timems)
{
	gTimeOfRepose.timems_start = timems;
	gTimeOfRepose.en = 0;
}

static void TimeOfReposeUpdate(UINT32 timems)
{
	if (gTimeOfRepose.en == 0)
	{
		gTimeOfRepose.timems_repose = TIME_TRHESHOLD_CAL(timems,
				gTimeOfRepose.timems_start);
		if (gTimeOfRepose.timems_repose > VALUE_TH_TIMEMS_OF_BATTERY_REPOSE)
		{
			gTimeOfRepose.en = 1;
		}
	}
}

int IsTimeOfReposeEnough(void)
{
	return gTimeOfRepose.en;
}

static UINT8 GetSystemMode(void)
{
	return gSystemMode.systemModule;
}

static int SetSystemMode(UINT8 mode)
{
	gSystemMode.systemModule = mode;
	return 0;
}

int getSystemState(void)
{
	return GetSystemMode();
}

/*
 * the next is the key activate and charge activate manage module
 * */
static int KeyActiveCheck(void)
{
	if (KEY_TO_ACC())
		return 1;
	return 0;
}

static int DCChargeInActiveCheck(void)
{
	if (KEY_TO_CRG())
	{
		return 1;
	}
	return 0;
}

static int ACChargeInActive(void)
{
	if (AC_CHARGER_INSERT())
	{
		return 1;
	}
	return 0;
}

static UINT8 gSysteActiveMode = 0;
int SetSystemActiveMode(UINT8 mode)
{
	gSysteActiveMode = mode;
	return 0;
}
UINT8 GetSystemActiveMode(void)
{
	return gSysteActiveMode;
}

static int SleepModeDetection(void)
{
	if (KeyActiveCheck())
	{
		SetSystemMode(SYSTEM_MODE_DISCHARGE);
//		SetSystemActiveMode(SYSTEM_ACTIVE_MODE_KEY_TO_ACC);
	}
	else if (ACChargeInActive())
	{
		SetSystemMode(SYSTEM_MODE_AC_CHARGE);
//		SetSystemActiveMode(SYSTEM_ACTIVE_MODE_ACCHARGER_IN);
	}
	else if (DCChargeInActiveCheck())
	{
//		SetSystemActiveMode(SYSTEM_ACTIVE_MODE_DCCHARGER_IN);
		SetSystemMode(SYSTEM_MODE_DC_CHARGE);
	}
	else
	{
//		SetSystemActiveMode(SYSTEM_ACTIVE_MODE_UNKOWN);
	}
	return GetSystemMode();
}

/*
 * */
static UINT8 record_pin[ID_IO_CTL_FANS + 1];
/*
 * close all ios
 * */
static int closeAllIos(void)
{
	UINT8 i = 0;
	for (i = ID_IO_CTL_LED5; i <= ID_IO_CTL_FANS; i++)
	{
		/*	record_output_pin_state */
		DriverRead(FD_GPIO_BY_INDEX, &record_pin[i], 1, i);
		/* close_output_pin_all */
		DriverWrite(FD_GPIO_BY_INDEX, (const char *) &i, 1, 0);
	}
	DriverUpdate(FD_GPIO_BY_INDEX);
}
/*
 * recover io
 * */
static int recoverAllIos(void)
{
	UINT8 i = 0;
	for (i = ID_IO_CTL_LED5; i <= ID_IO_CTL_FANS; i++)
	{
		/* close_output_pin_all */
		DriverWrite(FD_GPIO_BY_INDEX, (const char *) &i, 1, record_pin[i]);
	}
	DriverUpdate(FD_GPIO_BY_INDEX);
}

/*
 * close all device, only run ltc68042
 * */
static int systemEntrySleep(void)
{
	/*step 1*/
	storeAllParameters();

	/*step 2*/
	closeAllIos();
	return 0; //TODO
}

static int systemExitSleep(void)
{
	recoverAllIos();
	return 0;
}

int SystemStateMachine(UINT32 tnow)
{
	static int lastMode = SYSTEM_MODE_SLEEP;
	static int ForceACChargeFlag = 0;
	static UINT32 LastSleepTimems = 0;
	int mode = GetSystemMode();
	switch (mode)
	{
		case SYSTEM_MODE_SLEEP:
			SystemLedFlash(1000);

			// Entry this state
			if (lastMode != SYSTEM_MODE_SLEEP)
			{
				PowerOffHighVoltage();
				LastSleepTimems = tnow;
				if (lastMode != SYSTEM_MODE_DEEPSLEEP)
				{
					RecordTimeOfReposeStart(tnow);
				}
				mprintf("sleep entry!\r\n");
			}

			/*during*/
			{
				TimeOfReposeUpdate(tnow);

				if (TIME_TRHESHOLD_CAL(tnow, LastSleepTimems)
						> gParameters.e_devInfo.sleepCtlTime.time_PowerOffToSleep)
				{
					SetSystemMode(SYSTEM_MODE_DEEPSLEEP);
				}
//				mprintf("sleep doing!\r\n");

				SleepModeDetection();
			}

			/*exit */
			if (GetSystemMode() != SYSTEM_MODE_SLEEP)
			{
				if (GetSystemMode() != SYSTEM_MODE_DEEPSLEEP)
				{
					RecordTimeOfReposeEnd(tnow);
				}
				mprintf("sleep exit!\r\n");
			}
			
			break;

		case SYSTEM_MODE_DC_CHARGE:
			SystemLedFlash(500);
			setDcChargerCondition(CON_CHARGE_RELAY_OP_OK, 1); //set 1  means this condition ok

			if (DC_CHARGER_DISCONNECT())
			{
				shutdownRelayCharge();
//			tellDCChargerStopChargeByCan(REASON_DISCONNECT);// 
				SetSystemMode(SYSTEM_MODE_SLEEP);
			}
			break;

		case SYSTEM_MODE_AC_CHARGE:
			SystemLedFlash(500);
			if ((VEH_CHARGER_selfCheck() == CHECK_PASS)
					&& (Batterystateupdate() == CHECK_PASS))
			{

				if (CheckACChargePWM() == RECEIVE_SUCCESS)
				{
					ForceACChargeFlag = 0;
					if (lastMode != SYSTEM_MODE_AC_CHARGE)
					{
						OpenVEH_Charger_CPC();
						AC_Charge_HighVoltage();
					}
					if(GetAccharge_precharge_state()==STATE_PRECHARGER_OVER)
					{
						 VEH_CHARGER_UpperLimmitCurrent_Update();
						 set_VEH_CHARGER_poweron();
						 ResetAccharge_precharge_state();
					}
					Check_VEH_CHARGER_UpperLimmitCurrent();
					if (Check_VEH_CHARGER_OVER() == STATE_VEH_CHARGER_OVER)
					{
						CloseVEH_Charger_CPC();
						set_VEH_CHARGER_poweroff();
						SetSystemMode(SYSTEM_MODE_SLEEP);
					}
				}
				else if (CheckACChargePWM() == RECEIVE_FAILED)
				{
					if (lastMode != SYSTEM_MODE_AC_CHARGE)
					{
						ForceACChargeFlag = 1;
					}
					else
					{
						ForceACChargeFlag = 0;
					}
					if (ForceACChargeFlag == 1)
					{
						if (GetVCUcmd_ForceCharge() == 0)
						{
							Set_VEH_CHARGER_Force_Current();
						}
						else
						{
							SetSystemMode(SYSTEM_MODE_SLEEP);
						}
					}
					else
					{
						CloseVEH_Charger_CPC();
						set_VEH_CHARGER_poweroff();
						SetSystemMode(SYSTEM_MODE_SLEEP);
					}
				}
			}
			if (AC_CHARGER_DISCONNECT())
			{
				CloseVEH_Charger_CPC();
				set_VEH_CHARGER_poweroff();
				SetSystemMode(SYSTEM_MODE_SLEEP);
			}
			if(CHECK_ACCP_COMMUNICATION_SATE()==RECEIVE_FAILED)
			{
				CloseVEH_Charger_CPC();
				set_VEH_CHARGER_poweroff();
				SetSystemMode(SYSTEM_MODE_SLEEP);
			}
			break;

		case SYSTEM_MODE_DISCHARGE:
			SystemLedFlash(100);
//		if(KEY_TO_ON())
			{
				extern int BMS_selfCheck(void); // main.c
				if (BMS_selfCheck() == ERR_BMS_NONE)
				{
					PowerOnHighVoltage();
				}
				// if bms self check passed, set BMS_SelfCheckPassed bit to 1,and 
				// then send to VCU by can 

				// if VCU check self OK , Power on high voltage.

				// waitForVCUCheckSelfOK();
			}
			if (KEY_TO_ACC()) // ACC activate
			{
//			if( lastMode != SYSTEM_MODE_DISCHARGE)
				// can enable
			}
			else //if( !(gpio_read(IO_STATUS_ON) && gpio_read(IO_STATUS_ACC)) )
			{
				SetSystemMode(SYSTEM_MODE_SLEEP);
			}
			break;

		case SYSTEM_MODE_DEEPSLEEP:
		{
			/*entry*/
			if (lastMode != SYSTEM_MODE_DEEPSLEEP)
			{
				LastSleepTimems = tnow;
				systemEntrySleep();

//				mprintf("deepsleep entry!\r\n");
			}

			/*during*/
			{
//				mprintf("deep sleep doing!\r\n");
				TimeOfReposeUpdate(tnow);
				if (TIME_TRHESHOLD_CAL(tnow, LastSleepTimems)
						> gParameters.e_devInfo.sleepCtlTime.time_SleepToSober)
				{
					SetSystemMode(SYSTEM_MODE_SLEEP);
				}
				SleepModeDetection();
			}

			/*exit*/
			if (GetSystemMode() != SYSTEM_MODE_DEEPSLEEP)
			{
//				mprintf("deep sleep exit!\r\n");
				systemExitSleep();
				if (GetSystemMode() != SYSTEM_MODE_SLEEP)
				{
					RecordTimeOfReposeEnd(tnow);
				}
			}

			break;
		}

		default:
			break;
	}
	if (lastMode != mode)
	{
		lastMode = mode;
		mprintf("state change:%s->%s\r\n", modeToStr(lastMode),
				modeToStr(mode));
	}
	HighVoltageStep();
	return 0;
}

struct states
{
	void (*out)(void);
	void (*in)(void);
	void (*stay)(void);
	void (*trigger)(void);
};

struct states_charge
{
	struct states state;
};

struct states_discharge
{
	struct states state;
};

struct states_sleep
{
	struct states state;
};

struct stateMachine
{
	struct states *state;
	void (*run)(void);
};

