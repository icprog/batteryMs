/*********************************************************
 *
 * charge.c(UCT)
 *
 * File Description
 *
 *   $Revision: 1813 $
 *   $Author: huhai $
 *   $Date: 2014-06-12 11:11:58 +0800 $
 *   implement all of the function that described by <<ZS008....20140728-V0.6.doc>>
 *   by mgc 20140809
 *********************************************************/
#include "public/public.h"
#include "public/drivers.h"
#include "public/types.h"
#include "public/systemState.h"
#include "ctl/charge.h"
#include "ctl/relay.h"
#include "current/current.h"
#include "voltage/voltage.h"
#include "protocol/vcu_protocol.h"
#include "protocol/accp_protocol.h"
#include "driver/intc/intc.h"
//#include "gpio/gpio.h"
//#include "Gpio/gpioFuncs.h"
//#include "timer/timer.h"
/*
 * types
 * */

enum chargeerrs
{
	CHARGE_ERR_NONE,
	CHARGE_ERR_PRECHARGE,
	CAHRGE_ERR_DISCHARGE,
	CHARGE_ERR_CHARGE
};

struct s_charge_errs
{
	UINT8 precharge;
	UINT8 charge;
	UINT8 discharge;
};

enum POWER_ON_STATE
{
	STATE_WAIT_VCU_CMD,
	STATE_OPEN_N_RELAY,
	STATE_OPEN_PRECHARGE_RELAY,
	STATE_WAIT_PRECHARGE_OVER,
	STATE_OPEN_P_RELAY,
	STATE_SHUTDOWN_PRECHARGE_RELAY,
	STATE_POWERON_OVER,
	STATE_PRECHARGE_ERR
};

enum ACCP_STATE
{
	STATE_READY,
	STATE_CHARGING,
	STATE_FAULT,
	STATE_NULL
};

enum ACCP_LINK_STATE
{
	POWER_STATE_ZERO,
	POWER_STATE_16A,
	POWER_STATE_32,
	POWER_STATE_UNKNOWN
};



static UINT16 ACCP_POWER_VALUE_ZERO=0;
static UINT16 ACCP_POWER_VALUE_LOW=16;
static UINT16 ACCP_POWER_VALUE_HIGH=32;

static UINT16 CC_POWER_VALUE_ZERO=0;
static UINT16 CC_POWER_VALUE_LOW=16;
static UINT16 CC_POWER_VALUE_HIGH=32;

/*
 *  variable
 * */
static struct s_charge_errs gChargeErrs = {0,0,0};
static const UINT32 timeThresholdPrechargeMaxAllowed = 2; //2s
static const UINT32 MAX_ALLOWED_REFLECT_TIME = 5000; //5s
static volatile enum POWER_ON_STATE state = STATE_WAIT_VCU_CMD;
static int gPowerOffCmd = 0;
static int gDcChargeReady = 1;

static int AcChargePrechargeOverFlag = 0;   //1:precharge over  0:precharge continue



static int 	setErrPrecharge(void)
{
	gChargeErrs.precharge = 1;
	return 0;
}

int getErrPrecharge(void)
{
	return gChargeErrs.precharge;
}
int GetAccharge_precharge_state(void)
{
	if(AcChargePrechargeOverFlag==1)
	   return STATE_VEH_CHARGER_OVER;
	else
	   return STATE_VEH_CHARGER_CONTINUE;
}
void SetAccharge_precharge_state(void)
{
	AcChargePrechargeOverFlag=1;
}
void ResetAccharge_precharge_state(void)
{
	AcChargePrechargeOverFlag=0;
}
int PowerOnHighVoltage(void)
{
	if( state == STATE_WAIT_VCU_CMD )
	{ 
		if (BMS_GetVcuCmd() == CMD_POWER_ON )//(BMS_GetVcuCmd() == CMD_POWER_ON )  模拟测试加入
			state = STATE_OPEN_N_RELAY;
	}
	return 0;
}

int AC_Charge_HighVoltage(void)
{
	if( state == STATE_WAIT_VCU_CMD )
	{ 
			state = STATE_OPEN_N_RELAY;
	}
	return 0;
}

static int PowerOnHighVoltageStep(void)
{
	TYPE_VOLTAGE v_load = GetLoadVoltage();
	int systemState = getSystemState();
	int ret  = 0;
#if 1	  //for test by mgc 20141204
	if (state>= STATE_OPEN_N_RELAY && state <= STATE_POWERON_OVER && BMS_isHighRelayErr())
	{
		state = STATE_PRECHARGE_ERR;
	}
#endif
	
	switch (state)
	{

	case STATE_OPEN_N_RELAY:
		ret = OpenRelayNegative();
		if (ret == 0)
			state = STATE_OPEN_PRECHARGE_RELAY;
		break;

	case STATE_OPEN_PRECHARGE_RELAY:
		ret = OpenRelayPrecharge();
		if (ret == 0)
			state = STATE_WAIT_PRECHARGE_OVER;
		break;

	case STATE_WAIT_PRECHARGE_OVER:
		v_load = GetLoadVoltage();
		if (v_load > (BMS_GetVoltageValueBatteryTotal() * 85 / 100))
		{
			state = STATE_OPEN_P_RELAY;
		}
		break;

	case STATE_OPEN_P_RELAY:
		ret = OpenRelayPositive();
		if (ret == 0)
		{
			state = STATE_SHUTDOWN_PRECHARGE_RELAY;
			if( systemState == SYSTEM_MODE_AC_CHARGE)
			{
				SetAccharge_precharge_state();
			}
		}
		break;

	case STATE_SHUTDOWN_PRECHARGE_RELAY:
		ShutdownRelayPrechargeByTimerS(timeThresholdPrechargeMaxAllowed);
		state = STATE_POWERON_OVER;
		break;

	case STATE_POWERON_OVER:
		//DO NOTHING
		break;

	case STATE_PRECHARGE_ERR:
		setErrPrecharge();
		mprintf("precharge err!\r\n");
		state = STATE_WAIT_VCU_CMD;
		break;

	default:
		break;

	}
	return state;
}

int PowerOffHighVoltage(void)
{
	return gPowerOffCmd = 1;
}

static int BMS_NeedPowerOff(void)
{
	return gPowerOffCmd;
}

/*
 * if get VCU command "relay command = 1", then do this
 * or err LEVEL 1 happen, and 5S past.
 * */
static int PowerOffHV(void)
{
	if(state!= STATE_WAIT_VCU_CMD)
	{
		ShutdownRelayNegative();
		ShutdownRelayPositive();
		ShutdownRelayPrechargeByTimerS(0);
		state = STATE_WAIT_VCU_CMD;
	}
	gPowerOffCmd = 0;
	return 0;
}


/*
 * if return 0, power off already
 * if 1, is doing this or not start
 * */
static int PowerOffHighVoltageStep(void)
{
	static UINT32 LastTimeMs = 0;
	UINT32 tNow = getSystemTimeMs();
	int systemState = getSystemState();
	UINT32 th = 0;
	int vcuCmd = BMS_GetVcuCmd();
	
	/*VCU操作时 只可能处于放电模式下*/
	if( systemState == SYSTEM_MODE_DISCHARGE)
	{
		if( vcuCmd == CMD_POWER_OFF )  /*vcu 关闭继电器*/
		{   
			PowerOffHV();
		}
	}
	
	/*如果出现一级异常， 发命令给VCU要求关闭，但VCU没有响应*/
	if( BMS_NeedPowerOff() == 1 ) 
	{
		th = TIME_TRHESHOLD_CAL(LastTimeMs, tNow);
		if( th> MAX_ALLOWED_REFLECT_TIME)
		{
			PowerOffHV();
		}
	}
	else
	{
		LastTimeMs = tNow;
	}
	return gPowerOffCmd;
}


/*
 * this will be called by main loop
 * */
int HighVoltageStep(void)
{

	/*处理上电事项*/
	PowerOnHighVoltageStep();
	
	/*处理下电事项*/
	PowerOffHighVoltageStep();
	
	return 0;
}

static int PowerOnChargeRelay(void)
{
	return OpenRelayCharge();
}


int BMS_GetChargerConnectStatus(void)    // 1,connect 0:unconnected
{
	if (DC_CHARGER_INSERT() || AC_CHARGER_INSERT())
		return 1;
	return 0;
}

int BMS_IsChargging(void)             //1:charging, 0:uncharged
{

	if (BMS_GetCurrentValueBUS() > 0)
		return 1;
	else
		return 0;
}

int BMS_GetErrStatusPrecharge(void)
{
	return getErrPrecharge();
}


int StartChargeByUart(void)
{
	// open relay charge
	OpenRelayCharge();
	return 0;
}

int StopChargeByUart(void)
{
	// close relay charge
	shutdownRelayCharge();
	return 0;
}

int setDcChargerCondition(UINT8 conditions, UINT8 state)
{
	if(state == 1)
		MGC_RESET_BIT( gDcChargeReady,conditions);
	else
		MGC_SET_BIT( gDcChargeReady,conditions);			
	return 0;
}

int isDcChargerReadyOk(void)
{
	return gDcChargeReady;
}
enum vehicle_coupler_state
{
	STATE_NORMAL,
	STATE_ERR,
	STATE_ERR_A,
	STATE_ERR_B
};
//int ACchargeFaultmonitor(void)
//{
//	static UINT32 STATE_B_LastTimeMs = 0;
//	UINT32 tNow = getSystemTimeMs();
//	UINT32 th = 0;
//	if(GetCCconnectState()==STATE_ERR_B)
//	{
//		th = TIME_TRHESHOLD_CAL(STATE_B_LastTimeMs, tNow);
//		if( th> MAX_ALLOWED_S3CUTOFF_TIME)
//		{
//			return STATE_ERR;		
//    	}
//		else
//		{
//			return STATE_NORMAL;
//		}
//	}
//	else
//	{
//		STATE_B_LastTimeMs = tNow;
//	}
//
//	if(GetCCconnectState()==STATE_ERR_A)
//	{
//		return STATE_ERR;
//	}
//	return STATE_NORMAL; 
//}
int CHECK_ACCP_COMMUNICATION_SATE(void)
{
	if(ACCP_COMMUNICATION_SATE_FLAG==RECEIVE_FAILED)
		return 1;
	else
		return 0;
}
int set_VEH_CHARGER_poweron(void)
{
	VEH_CHARGER_POWER_STATE = CMD_POWER_ON;
}

int set_VEH_CHARGER_poweroff(void)
{
	VEH_CHARGER_POWER_STATE = CMD_POWER_OFF;
	PowerOffHV();
}
UINT16 GetVehChargerCurrentByCan(void)
{
	return ACCP_STATE.outputcur;
}
int VEH_CHARGER_selfCheck(void)
{
	if((ACCP_STATE.accpstate == STATE_READY)||(ACCP_STATE.accpstate == STATE_CHARGING))
	    return CHECK_PASS;
	else if(ACCP_STATE.accpstate ==STATE_FAULT)
	    return CHECK_NG;
}
int Batterystateupdate(void)
{
	return 0;//todo
}
int CheckACChargePWM(void)
{
	if(getPwm()==POWER_STATE_ZERO)   
		return CHECK_NG;
	else 
		return CHECK_PASS;
}

UINT16 GetPowerSupplyCurrentByPWM(void)
{
	UINT16 PWM_STATE;
	PWM_STATE=getPwm();
	if(PWM_STATE == POWER_STATE_ZERO)
		return ACCP_POWER_VALUE_ZERO;
	else if(PWM_STATE == POWER_STATE_16A)
		return ACCP_POWER_VALUE_LOW;
	else if(PWM_STATE == POWER_STATE_32)
		return ACCP_POWER_VALUE_HIGH;
	else
		return ACCP_POWER_VALUE_ZERO;
}

UINT16 GetCableRateCurrentByCC(void)
{
	UINT16 CC_STATE;
	CC_STATE = getCC_state();
	if(CC_STATE == POWER_STATE_ZERO)
		return CC_POWER_VALUE_ZERO;
	else if(CC_STATE == POWER_STATE_16A)
		return CC_POWER_VALUE_LOW;
	else if(CC_STATE == POWER_STATE_32)
		return CC_POWER_VALUE_HIGH;
	else
		return CC_POWER_VALUE_ZERO;
			
//	UINT16 CableCapacity_MV,CableCapacity_R;
//	CableCapacity_MV = getMvByAdc(ADC_PORT_ACCIN);
//	CableCapacity_R = Conversion_CableCapacityMVtoR(CableCapacity_MV);
//	if(CableCapacity_R)
//		return CableCapacity_A1;
//	else if(CableCapacity_R)
//		return CableCapacity_A2;
}

int GetVCUcmd_ForceCharge(void)
{
	return 0; // todo
}
int GetVCUcmd_STOPACcharge(void)
{
    return 0;//	todo
}
void SendVEH_CHARGER_UpperLimmitCurrentByCan(UINT16 data_current)
{
	VEH_PRESENT_CHARGER_Current = data_current;
}
int WaitACchargeOver(void)
{

	if( (BMS_GetVoltageValueBatteryTotal()>=VALUE_TH_OV_SUM)||(BMS_GetVoltageValueMin()>=VALUE_TH_OV_CELL))
	{
		return STATE_VEH_CHARGER_OVER;
	}
	else
		return STATE_VEH_CHARGER_CONTINUE;
}

int GetCCconnectState(void)
{
//	todo
}
int Check_VEH_CHARGER_UpperLimmitCurrent(void)
{
	static UINT32 CheckPWM_LastTimeMs = 0;
	static UINT32 LAST_VALUE = 0;
	UINT16 NOW_VALUE;
	UINT32 tNow = getSystemTimeMs();
	UINT32 th = 0;
	th = TIME_TRHESHOLD_CAL(CheckPWM_LastTimeMs, tNow);
	if( th> MAX_ALLOWED_CHECKPWM_TIME)
	{
		NOW_VALUE = GetPowerSupplyCurrentByPWM();
		if(NOW_VALUE!=LAST_VALUE)
		{
			VEH_CHARGER_UpperLimmitCurrent_Update();
			LAST_VALUE = NOW_VALUE;
		}
    }
	else
	{
		CheckPWM_LastTimeMs = tNow;
	}

}
void Set_VEH_CHARGER_Force_Current(void)
{
	SendVEH_CHARGER_UpperLimmitCurrentByCan(VEH_FORCE_CHARGER_Current); 
}

UINT16 Exportleast(UINT16 dataA,UINT16 dataB,UINT16 dataC)
{
	UINT16 data;
	data = dataA<dataB?dataA:dataB;
	data = data<dataC?data:dataC;
	return data;
}

void VEH_CHARGER_UpperLimmitCurrent_Update(void)
{
	UINT16 PowerSupply_Current,CableRate_Current,VehCharger_Current,VEH_CHARGER_UpperLimmitCurrent;
	PowerSupply_Current = GetPowerSupplyCurrentByPWM();
	CableRate_Current = GetCableRateCurrentByCC();
	VehCharger_Current = GetVehChargerCurrentByCan();
	VEH_CHARGER_UpperLimmitCurrent = Exportleast(PowerSupply_Current,CableRate_Current,VehCharger_Current);
	SendVEH_CHARGER_UpperLimmitCurrentByCan(VEH_CHARGER_UpperLimmitCurrent);
}



int Check_VEH_CHARGER_OVER(void)
{
	if(WaitACchargeOver()==STATE_VEH_CHARGER_OVER)
		return STATE_VEH_CHARGER_OVER;
	else if(GetVCUcmd_STOPACcharge()==STATE_VEH_CHARGER_OVER)
		return STATE_VEH_CHARGER_OVER;
//	if(ACchargeFaultmonitor()!=STATE_NORMAL)
//	    return STATE_VEH_CHARGER_OVER;
	else
	    return STATE_VEH_CHARGER_CONTINUE;
}


