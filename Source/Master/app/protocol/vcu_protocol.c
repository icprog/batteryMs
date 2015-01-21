/*
 * add by mgc 2014-11-12
 * */
#include "protocol/vcu_protocol.h"
//#include "timer/timer.h"
#include "public/types.h"
#include "public/public.h"
#include "public/drivers.h"
#include "voltage/voltage.h"
#include "temp/temperature.h"
#include "soc/soc.h"
#include "current/currentAbstract.h"
#include "current/current.h"
#include "ctl/relay.h"
#include "ctl/charge.h"
//#include "Gpio/gpioFuncs.h"
//#include "CAN/can.h"
#include "ctl/control.h"

enum vcu_can_id
{
	VCU_CAN_ID_BROADCAST = 0x1802f427, VCU_CAN_ID_B2CB1 = 0x182227f4, // BMS to VCU broadcast package  // 200ms
	VCU_CAN_ID_B2CB2 = 0x182327f4, // BMS to VCU broadcast package  // 200ms
	VCU_CAN_ID_B2CB3 = 0x182427f4, // BMS to VCU broadcast package  // 200ms
	VCU_CAN_ID_B2CB4 = 0x182527f4 // BMS to VCU broadcast package   // 200ms
};

static struct canPack gCp;
static UINT8 gRequestToShutOffPositveRelay = 0;
static UINT8 gVCUPowerOn = CMD_POWER_OFF;
static const int gReversvalue = 0;

int sendRequestToVCU(UINT8 id)
{
	switch (id)
	{
	case VCU_CMD_SHUTDOWN_MAIN_RELAY:
		gRequestToShutOffPositveRelay = 1;
		PowerOffHighVoltage();
		break;
		
	default:
		return -1;
	}
	return 0;
}

static int setRelayControl(UINT8 relaycmd)
{
	//TODO
	(void) relaycmd;
	return 0;
}

 void vcu_can_isr(struct canPack *ppack)
{
	switch (ppack->id)
	{
	case VCU_CAN_ID_BROADCAST:
		if(ppack->data[0]==0xAA)
			gVCUPowerOn = CMD_POWER_ON;
		else
			gVCUPowerOn = CMD_POWER_OFF;
//		setRelayControl(ppack->data[0]);
		break;
	}
}

static UINT8 CanSendBuff[8] =
{ 0 };

/*
 * VCU 开始上电
 * */
static int VCUCanSend(struct canPack *ppack)
{
	//(void)int  ppack;
	int i;
	struct VCU_MASGDATA  msg;
	msg.id = ppack->id;
	msg.datalen = ppack->datalen;
	for(i=0;i<8;i++)
	{
	msg.data[i] = CanSendBuff[i];
	//ppack->data++;
	}
	//msg.data = ppack->data;
	CAN_A_TX(msg);
	//TODO
	return 0;
}
/*
 * 查询VCU是否开启上电。
 * */  
int BMS_GetVcuCmd(void)
{
	return gVCUPowerOn;
}
/*
 *仿真VCU上电指令
 * */
int BMS_SetSimulatorVcuPower(UINT8 onoff)
{
	if(onoff)
		gVCUPowerOn = CMD_POWER_ON;
	else
		gVCUPowerOn = CMD_POWER_OFF;
	return gVCUPowerOn;
}

static int BYTES_ORDER_B2VCAN_16(UINT8 *dst, UINT16 data)
{
	dst[0] = (UINT8)data&0xff;
	dst[1] = (UINT8)(data/256);
	return 0;
}
//static int BYTES_ORDER_B2VCAN_32(UINT8 *dst, UINT32 data)
//{
//	dst[0] = (UINT8)data&0xff;
//	dst[1] = data/256;
//	return 0;
//
//}

int packVCUCanB2VP1Package(void)
{
	UINT8 soc = getSocValue();
	UINT8 soh = getSohValue();
	TYPE_CURRENT currents = 0;
	int systemstate = getSystemState();
	CanSendBuff[0] = soc * 10 / 4; //
	CanSendBuff[1] = soh * 10 / 4;
	/*bus currents*/
	currents = BMS_GetCurrentValueBUS();
	if( currents < -500 )
		currents= -500;
	currents += 500;	
	currents *= 10;	
//	if (systemstate == SYSTEM_MODE_DISCHARGE)
//	{
//		currents = 500 - currents;
//	}
	BYTES_ORDER_B2VCAN_16(CanSendBuff + 2, (UINT16)currents);

	/*max allowed discharge currents*/
	currents = BMS_GetMaxAlowedDisChargeCurrent();
	if(currents < -500)
		currents = -500;
	currents = 500 + currents;
	currents *= 10;
	BYTES_ORDER_B2VCAN_16(CanSendBuff + 4, (UINT16)currents);

	currents = BMS_GetMaxAlowedChargeCurrent();
	if(currents < -500)
		currents = -500;
	currents = 500 + currents;

	currents *= 10;
	BYTES_ORDER_B2VCAN_16(CanSendBuff + 6, (UINT16)currents);

	gCp.id = VCU_CAN_ID_B2CB1;
	gCp.datalen = 8;
	gCp.data = CanSendBuff;
//	VCUCanSend(&gCp);
	return 0;
}

 int packVCUCanB2VP2Package(void)
{
	//TODO
	UINT16 vol = 0;
	UINT32 sumVol = 0;
	UINT8 b = 0;
	/*sum voltage of battery */
	sumVol = BMS_GetVoltageValueBatteryTotal();  //mv
	vol = (UINT16)(sumVol / 100); //0.1v
	BYTES_ORDER_B2VCAN_16(CanSendBuff, vol);

	/*max cell voltage*/
	vol = (UINT16)BMS_GetVoltageValueMax(); //mv
	BYTES_ORDER_B2VCAN_16(CanSendBuff + 2, vol);

	/*min cell voltage*/
	vol = (UINT16)BMS_GetVoltageValueMin(); //mv
	BYTES_ORDER_B2VCAN_16(CanSendBuff + 4, vol);

	/*hardware fault check*/
	b = 0; //B0
	//TODO
   SET_BIT(b, 2, (BMS_GetErrStatusBMScheckselfByType(STATE_ERR_BMSRD6804_TEMP)) );
   SET_BIT(b, 3, (BMS_GetErrStatusBMScheckselfByType(STATE_ERR_BMSRD6804_VOL)) );
//	SET_BIT(b, 1, (BMS_DetectionStateCheckBalance()) );
//	SET_BIT(b, 2, (BMS_DetectionStateCheckTemperature()) );
//	SET_BIT(b, 3, (BMS_DetectionStateCheckVoltage() ) );
//	SET_BIT(b, 4, (BMS_DetectionStateCheckCurrent()) );
//	SET_BIT(b, 1, (BMS_DetectionStateCheckVCULost() ) );
//	SET_BIT(b, 6, (BMS_DetectionStateCheckCanCom()) );
//	SET_BIT(b, 7, (BMS_SelfCheckPass()) );
	CanSendBuff[6] = b;
	
	for(b=0; b<8;b++)
	{
//		SET_BIT(CanSendBuff[7],b,ReadLtc6804stateByID(b)); //TODO
	}
	gCp.id = VCU_CAN_ID_B2CB2;
	gCp.datalen = 8;
	gCp.data = CanSendBuff;
	return 0;
}

int packVCUCanB2VP3Package(void)
{
	//TODO
	int systemstate = getSystemState();
	TYPE_TEMP temp = 0;
	UINT16 loops = 0;
	UINT8 b = 0;
	/*average temperature*/
	temp = BMS_GetTemperatureValueAverage();
	if (temp < -50)
		temp = -50;
	temp += 50;
	CanSendBuff[0] = (UINT8) temp;

	/*byte1 max temperature*/
	temp = BMS_GetTemperaturValueMaxValue();
	if (temp < -50)
		temp = -50;
	temp += 50;
	CanSendBuff[1] = (UINT8) temp;

	/*byte2 min temperature*/
	temp = BMS_GetTemperatureValueMinValue();
	if (temp < -50)
		temp = -50;
	temp += 50;
	CanSendBuff[2] = (UINT8) temp;

	/*byte3 max temperature cell number*/
	b = (UINT8)BMS_GetTemperatureMaxValueSensorId();
	CanSendBuff[3] = b;

	/*byte4 max temperature cell number*/
	b = (UINT8)BMS_GetTemperatureMinValueSensorId();
	CanSendBuff[4] = b;

	/*byte5~6 charge loop times*/
	loops = BMS_GetLoopChargeTimes();
	BYTES_ORDER_B2VCAN_16(CanSendBuff + 5, loops);

	/*byte7*/
	if (systemstate == SYSTEM_MODE_AC_CHARGE
			|| systemstate == SYSTEM_MODE_DC_CHARGE)
	{
		temp = 1;
	}
	else
		temp = 0;

	b = (UINT8)getErrPrecharge();
	b |= (BMS_GetRelayStateNegtiveFeedbackConnnect() << 1);
	b |= (BMS_GetRelayStatePositiveFeedbackConnnect() << 2);
	b |= (temp << 3);
	b |= (HEAT_GET()<<4);
	b |= (FAN1_GET()<<5);
	b |= (BMS_IsChargging() << 6);
	b |= (gRequestToShutOffPositveRelay << 7);
	CanSendBuff[7] = b;

	gCp.id = VCU_CAN_ID_B2CB3;
	gCp.datalen = 8;
	gCp.data = CanSendBuff;
	return 0;
}

//#define SET_BIT(b,n,v) ((b)|=((v)<<(n))) 
int packVCUCanB2VP4Package(void)
{
	//TODO
	UINT8 b = 0;
	/*b0*/
	b = (BMS_GetErrStatusVoltageByType(STATE_CELL_VOL_HIGH_LEVEL2) == 1);
	b |= (BMS_GetErrStatusVoltageByType(STATE_CELL_VOL_LOW_LEVEL2) << 1);
	b |= (BMS_GetErrStatusVoltageByType(STATE_SUM_VOL_HIGH_LEVEL2) << 2);
	b |= (BMS_GetErrStatusSocByType(STATE_SOC_LOW_LEVEL2) << 3);
	b |=
			(BMS_GetErrStatusCurrentByType(STATE_CURRENT_DISCHARGE_OC_LEVEL2)
					<< 4);
	b |= (BMS_GetErrStatusCurrentByType(STATE_CURRENT_FEEDBACK_OC_LEVEL2) << 5);
	b |= (BMS_GetErrStatusTemperatureByType(STATE_TEMP_HIGH_LEVEL2) << 6);
	b |= (BMS_GetErrStatusTemperatureByType(STATE_TEMP_LOW_LEVEL2) << 7);
	CanSendBuff[0] = b;

	/*b1*/
	b = 0;
	SET_BIT(b, 0, BMS_GetErrStatusVoltageByType(STATE_CELL_VOL_HIGH_LEVEL1));
	SET_BIT(b, 1, BMS_GetErrStatusVoltageByType(STATE_CELL_VOL_LOW_LEVEL1));
	SET_BIT(b, 2, BMS_GetErrStatusVoltageByType(STATE_SUM_VOL_HIGH_LEVEL1));
	SET_BIT(b, 3, BMS_GetErrStatusSocByType(STATE_SOC_LOW_LEVEL1));
	SET_BIT(b, 4,
			BMS_GetErrStatusCurrentByType(STATE_CURRENT_DISCHARGE_OC_LEVEL1));
	SET_BIT(b, 5,
			BMS_GetErrStatusCurrentByType(STATE_CURRENT_FEEDBACK_OC_LEVEL1));
	SET_BIT(b, 6, BMS_GetErrStatusTemperatureByType(STATE_TEMP_HIGH_LEVEL1));
	SET_BIT(b, 7, BMS_GetErrStatusTemperatureByType(STATE_TEMP_LOW_LEVEL1));
	CanSendBuff[1] = b;

	/*b2*/
	b = 0;
	SET_BIT(b, 0, BMS_GetErrStatusBMSCheckSelf());
	SET_BIT(b, 1,
			BMS_GetErrStatusVoltageByType(STATE_VOL_CELL_DIFF_BIG_LEVEL2));
	SET_BIT(b, 2, BMS_GetErrStatusVoltageByType(STATE_SUM_VOL_LOW_LEVEL2));
	SET_BIT(b, 3,
			BMS_GetErrStatusTemperatureByType(STATE_TEMP_CELL_DIFF_BIG_LEVEL2));
//	SET_BIT(b, 4, gReversvalue);
//	SET_BIT(b, 5, gReversvalue);
	SET_BIT(b, 6, BMS_GetErrStatusRelayByType(ERR_POSITIVE_SHUTDOWN));
	SET_BIT(b, 7, BMS_GetErrStatusRelayByType(ERR_POSITIVE_OPEN));
	CanSendBuff[2] = b;

	/*b3*/
	b = 0;
	SET_BIT(b, 7, BMS_GetErrStatusRelayByType(ERR_NEGATIVE_OPEN));
	SET_BIT(b, 6, BMS_GetErrStatusRelayByType(ERR_NEGATIVE_SHUTDOWN));
//	SET_BIT(b, 5, gReversvalue);
//	SET_BIT(b, 4, gReversvalue);
	SET_BIT(b, 3,
			BMS_GetErrStatusTemperatureByType(STATE_TEMP_CELL_DIFF_BIG_LEVEL1));
	SET_BIT(b, 2, BMS_GetErrStatusVoltageByType(STATE_SUM_VOL_LOW_LEVEL1));
	SET_BIT(b, 1,
			BMS_GetErrStatusVoltageByType(STATE_VOL_CELL_DIFF_BIG_LEVEL1));
//	SET_BIT(b, 0, gReversvalue);
	CanSendBuff[3] = b;

	/*b4~b5 negative insulation resistor value*/
	CanSendBuff[4] = CanSendBuff[5] = 0;

	/*b4~b5 negative insulation resistor value*/
	CanSendBuff[6] = CanSendBuff[7] = 0;
	gCp.id = VCU_CAN_ID_B2CB4;
	gCp.datalen = 8;
	gCp.data = CanSendBuff;
	return 0;
}
int VCU_communication(void)
{
//	static UINT8 count = 0;
	static UINT32 lastTimems = 0;
	//static UINT32 lastTimems = 0;
//	static UINT32 time1Second = 0;
	//static UINT8 count = 0;
//		UINT32 timeNow = getSystemTimeMs();
//		if (TIME_TRHESHOLD_CAL(timeNow, lastTimems) > 30)
//		{
//			lastTimems = timeNow;
//			count++;
//			if (count == 1)
//			{
//			packVCUCanB2VP1Package();
//			VCUCanSend(&gCp);
//			}
//			else if (count == 2)
//			{
//			packVCUCanB2VP2Package();
//			VCUCanSend(&gCp);
//			}
//			else if (count == 3)
//			{
//			packVCUCanB2VP3Package();
//			VCUCanSend(&gCp);
//			}
//			else if (count == 4)
//			{
//			packVCUCanB2VP4Package();
//			VCUCanSend(&gCp);
//			  // count = 0;
//			}
//	      else if(count == 5)
//			{
//			//packVCUCanB2VP1Package();
//			//VCUCanSend(&gCp);
//			}
//	      else if(count == 6)
//	      {
//	     // packVCUCanB2VP2Package();
//	     // VCUCanSend(&gCp);
//	       //count = 0;
//	      }
//	      else if(count == 7)
//	      {
//	      count = 0;    
//	      }
//		}
//		
	
	UINT32 timeNow = getSystemTimeMs();
	UINT32 t = 0;
	t = TIME_TRHESHOLD_CAL(timeNow, lastTimems);
	if ( t > 200)
	{
		lastTimems = timeNow;
		packVCUCanB2VP1Package();
		VCUCanSend(&gCp);
		packVCUCanB2VP2Package();
		VCUCanSend(&gCp);
		packVCUCanB2VP3Package();
		VCUCanSend(&gCp);
		packVCUCanB2VP4Package();
		VCUCanSend(&gCp);
	}
	return 0;
}

const char *readPackageBuff(void)
{
	return (const char *)CanSendBuff;
}
