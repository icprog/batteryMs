/*********************************************************
 *
 *
 * File Description
 *
 *   $Revision: 3311 $
 *   $Author: huhai $
 *   $Date: 2014-08-28 10:01:04 +0800 ( 2014) $
 *
 *********************************************************/
#include "public/systemState.h"
#include "voltage/voltage.h"
#include "temp/temperature.h"
#include "current/current.h"
#include "ctl/control.h"
#include "ctl/relay.h"
#include "protocol/vcu_protocol.h"
#include "protocol/cpcu_protocol.h"
//#include "timer/timer.h"
#include "public/public.h"
#include "public/types.h"
#include "public/drivers.h"
#include "protocol/cpcu_protocol.h"
static int FLAG_STATE_CURRENT_CHARGE_OC_LEVEL1=0;
static UINT32 timer=0,time1=0,time2=0;
static UINT16 state_BMSselfCheck;
/*
 * sum vol err change event happen
 * this func may be called by voltage module
 * by mgc 2014-10-30
 * */
/*
 * process for fault of voltage
 * 
 * sum high vol  fault LEVEL1:
 * cell high vol fault LEVEL1:
 * ����2S���������ʱǿ���жϳ��̵�������ʻʱ�����ж������̵��������յ�VCU�ϵ�ָ����ж������̵�����
 * 
 * cell low voltage fault LEVEL1:
 * sum low voltage fault LEVEL1:
 * ����2S��������ʻʱ�����ж������̵��������յ�VCU�ϵ�ָ����ж������̵�����
 * 
 * cell high vol fault LEVEL2:
 * sum high vol fault LEVEL2:
 * ����2S�������������ͨ��CAN�رճ�����
 * 
 * sum high vol fault:
 * cell high vol fualt:
 * ����2S����������������Ƴ�������
 * sum low voltage fault:
 * cell low voltage fault:
 * ����2S���� ����ʾ����
 * 
 * cell low voltage fault LEVEL2:
 * sum low voltage fault LEVEL2:
 * cell voltage difference too bigger LEVEL2:
 * ����2S����
 * 
 * cell voltage differnce too bigger LEVEL1:
 * ����2S�����������ж������̵������յ�VCU�ϵ�ָ����ж������̵�����
 * 
 * */
//sendRequestToVCU,sendRequestToPILS,relayOp  these function provide by other module
static int alarm( UINT32 timems )
{
	ledAlarmOnByTimers(timems);
	return 0;
}

///*
// * cell vol err change event happen
// * this func may be called by voltage module
// * by mgc 2014-10-30
// * */

static int voltageUpdate(void)
{
	int system_state = getSystemState();
	static UINT8 alarmflag = 0;
	if (BMS_GetErrStatusVoltageByType(STATE_VOL_NORMAL) )
	{
		if(alarmflag)
		{
		alarm(0);
		alarmflag = 0;
		}
	}
	else
	{
		alarm(2);
		alarmflag = 1;
	}
	
	if( BMS_GetErrStatusVoltageByType( STATE_CELL_VOL_LOW_LEVEL1) )
	{
		if (system_state == SYSTEM_MODE_DISCHARGE)
	    {
			sendRequestToVCU(VCU_CMD_SHUTDOWN_MAIN_RELAY);//	ͨ��CAN��VCU�����ж������̵�������   TODO
		//		
		}
	}
	if( BMS_GetErrStatusVoltageByType( STATE_CELL_VOL_HIGH) )
	{
		if ( (system_state == SYSTEM_MODE_DC_CHARGE)||(system_state == SYSTEM_MODE_AC_CHARGE) )
		{
			Send_BCL();//			ͨ��CAN����׮�������Ƶ������� TODO
		}//TODO
	}
	if( BMS_GetErrStatusVoltageByType( STATE_CELL_VOL_HIGH_LEVEL1) )
	{
		if ( (system_state == SYSTEM_MODE_DC_CHARGE)||(system_state == SYSTEM_MODE_AC_CHARGE) )
		{
			shutdownRelayCharge();  //shut off the charge relay module
		}
		else if (system_state == SYSTEM_MODE_DISCHARGE)
		{
			sendRequestToVCU(VCU_CMD_SHUTDOWN_MAIN_RELAY);//	ͨ��CAN��VCU�����ж������̵�������   TODO
		}//TODO
	}
	if( BMS_GetErrStatusVoltageByType( STATE_CELL_VOL_HIGH_LEVEL2) )
	{
		if ( (system_state == SYSTEM_MODE_DC_CHARGE)||(system_state == SYSTEM_MODE_AC_CHARGE) )
		{
			Send_BST();//			ͨ��CAN����׮���͹رճ�������   TODO
		}			 
	}
	if( BMS_GetErrStatusVoltageByType( STATE_SUM_VOL_LOW_LEVEL1) )
	{
		sendRequestToVCU(VCU_CMD_SHUTDOWN_MAIN_RELAY);//	ͨ��CAN��VCU�����ж������̵�������   TODO
	}
	if( BMS_GetErrStatusVoltageByType( STATE_SUM_VOL_HIGH) )
	{
		if( (system_state == SYSTEM_MODE_DC_CHARGE)||(system_state == SYSTEM_MODE_AC_CHARGE))
		{
			Send_BCL();//			ͨ��CAN����׮�������Ƶ������� TODO
		}//TODO
	}
	if( BMS_GetErrStatusVoltageByType( STATE_SUM_VOL_HIGH_LEVEL1) )
	{
		if ( (system_state == SYSTEM_MODE_DC_CHARGE)||(system_state == SYSTEM_MODE_AC_CHARGE) )
		{
			shutdownRelayCharge();  //shut off the charge relay module
		}
		else if (system_state == SYSTEM_MODE_DISCHARGE)
	    {
			sendRequestToVCU(VCU_CMD_SHUTDOWN_MAIN_RELAY);//	ͨ��CAN��VCU�����ж������̵�������   TODO
    	}
	}
	if( BMS_GetErrStatusVoltageByType( STATE_SUM_VOL_HIGH_LEVEL2) )
	{
		if ( (system_state == SYSTEM_MODE_DC_CHARGE)||(system_state == SYSTEM_MODE_AC_CHARGE) )
		{
			Send_BST();//			ͨ��CAN����׮���͹رճ�������   TODO
		}
	}
	if( BMS_GetErrStatusVoltageByType( STATE_VOL_CELL_DIFF_BIG_LEVEL1) )
	{
		sendRequestToVCU(VCU_CMD_SHUTDOWN_MAIN_RELAY);//	ͨ��CAN��VCU�����ж������̵�������   TODO
	}	
}

/*
 * temp err change event happen
 * this func may be called by temperature module
 * by mgc 2014-10-30
 * */
static int TempStateUpdate(void)
{
	static int laststate = STATE_TEMP_NORMAL;
	static UINT8 alarmflag = 0;
	int system_state = getSystemState();
	if (BMS_GetErrStatusTemperatureByType(STATE_TEMP_NORMAL) )
	{
		if(alarmflag)
		{
		alarm(0);
		alarmflag = 0;
		}
	}
	else
	{
		alarmflag = 1;
		alarm(2);
	}
	
	if( BMS_GetErrStatusTemperatureByType(STATE_TEMP_LOW_LEVEL1) )
	{
		if ( (system_state == SYSTEM_MODE_DC_CHARGE)||(system_state == SYSTEM_MODE_AC_CHARGE) )
		{
			shutdownRelayCharge();  //shut off the charge relay module
		}
		else if (system_state == SYSTEM_MODE_DISCHARGE)
		{
			sendRequestToVCU(VCU_CMD_SHUTDOWN_MAIN_RELAY);//	ͨ��CAN��VCU�����ж������̵�������   TODO
		}
	}
	if( BMS_GetErrStatusTemperatureByType(STATE_TEMP_LOW_LEVEL2) )
	{
		
		if ( (system_state == SYSTEM_MODE_DC_CHARGE)||(system_state == SYSTEM_MODE_AC_CHARGE) )
		{
			shutdownRelayCharge();  //shut off the charge relay module
		}
	}
	if( BMS_GetErrStatusTemperatureByType(STATE_TEMP_HIGH_LEVEL1) )
	{
		if ( (system_state == SYSTEM_MODE_DC_CHARGE)||(system_state == SYSTEM_MODE_AC_CHARGE) )
		{
			shutdownRelayCharge();  //shut off the charge relay module
		}
		else if (system_state == SYSTEM_MODE_DISCHARGE)
		{
			sendRequestToVCU(VCU_CMD_SHUTDOWN_MAIN_RELAY);//	ͨ��CAN��VCU�����ж������̵�������   TODO
		}		
	}
	if( BMS_GetErrStatusTemperatureByType(STATE_TEMP_HIGH_LEVEL2) )
	{
		if ( (system_state == SYSTEM_MODE_DC_CHARGE)||(system_state == SYSTEM_MODE_AC_CHARGE) )
		{
			Send_BCL();//			ͨ��CAN����׮�������Ƶ�������   TODO
		}		
	}
}
		
/*
 * current err change event happen
 * this func may be called by current module
 * by mgc 2014-10-30
 * */
static int CurrentStateUpdate(void)
{
	static UINT8 alarmflag = 0;
	static int laststate = STATE_CURRENT_NORMAL;
//	int system_state = getSystemState();		
	if(BMS_GetErrStatusCurrentByType(STATE_CURRENT_NORMAL))
	{
		if(alarmflag)
		{
		alarm(0);//TODO		
		alarmflag = 0;
		}
	}
	else
	{
		alarmflag = 1;
		alarm(2);
	}
	
	if( BMS_GetErrStatusCurrentByType( STATE_CURRENT_DISCHARGE_OC_LEVEL1) )
	{
		sendRequestToVCU(VCU_CMD_SHUTDOWN_MAIN_RELAY);//	ͨ��CAN��VCU�����ж������̵�������   TODO
	}	
	if(BMS_GetErrStatusCurrentByType( STATE_CURRENT_CHARGE_OC_LEVEL1))
	{
		if(FLAG_STATE_CURRENT_CHARGE_OC_LEVEL1==0)
		{    
			time1=getSystemTimeMs();
			Send_BST();
			shutdownRelayCharge();
			FLAG_STATE_CURRENT_CHARGE_OC_LEVEL1=1;//		            ͨ��CAN����׮���͹رճ�������    TODO
		}
//		if((FLAG_STATE_CURRENT_CHARGE_OC_LEVEL1==1)&&(TIME_TRHESHOLD_CAL(g_time_ms, time1)>3000))
//		{   
//			shutdownRelayCharge();  //shut off the charge relay module
		FLAG_STATE_CURRENT_CHARGE_OC_LEVEL1=0;
//		}

	}
	if(BMS_GetErrStatusCurrentByType( STATE_CURRENT_CHARGE_OC_LEVEL2))
	{
		Send_BCL();//			ͨ��CAN����׮�������Ƶ�������   TODO
	}
}


/*
 * soc err change event happen
 * this func may be called by soc module
 * by mgc 2014-10-30
 * */
static int SocStateUpdate(void)
{
	static int laststate = 0;
	int system_state = getSystemState();
	if(BMS_GetErrStatusSocByType(STATE_SOC_NORMAL)) //todo  BMS_GetErrStatusSocByType??
	{
		if(laststate)
		{
	   alarm(0);		
	   laststate = 0;
		}
	}
	else
	{
	  alarm(2);
	  laststate = 1;
	}
//	
//	switch (state)
//	{
//	case STATE_SOC_NORMAL:
//		//TODO
//		break;
//	case STATE_SOC_LOW:
//		//todo
//		break;
//	case STATE_SOC_LOW_LEVEL1:
//		//todo
//		break;
//	default:
//		//todo
//		break;
//	}
}

static int BMS_selfCheckUpdate(void)
{
    int date=0,fault_state=0,rv=0;
    rv =ltc6804_voltage_monitor ();
    if ( rv != UCT_SUCCESS )     //��ѹ����쳣
    {
    	alarm(2);
    	MGC_SET_BIT(fault_state, STATE_ERR_BMSRD6804_VOL);
    	sendRequestToVCU(VCU_CMD_SHUTDOWN_MAIN_RELAY);//	ͨ��CAN��VCU�����ж������̵�������   TODO
    }
//    else
//    {
//    	MGC_RESET_BIT(fault_state, STATE_ERR_BMSRD6804_VOL);
//    }
    rv =ltc6804_temp_monitor ();
    if ( rv != UCT_SUCCESS )     //�¶ȼ���쳣
    {
       MGC_SET_BIT(fault_state, STATE_ERR_BMSRD6804_TEMP);
    }
//    else
//    {
//    	MGC_RESET_BIT(fault_state, STATE_ERR_BMSRD6804_TEMP);
//    }
    if(0)                       //��������쳣
    {
    	alarm(2);
        MGC_SET_BIT(fault_state, STATE_ERR_BMSRD_CURRENT);
        sendRequestToVCU(VCU_CMD_SHUTDOWN_MAIN_RELAY);//	ͨ��CAN��VCU�����ж������̵�������   TODO
    }
//    else
//    {
//       	MGC_RESET_BIT(fault_state, STATE_ERR_BMSRD_CURRENT);
//    }
//    
    if(0)                      //BMS�Լ����
    {
    	alarm(2);
    	MGC_SET_BIT(fault_state, STATE_ERR_BMSCHECKSELF);
//    TODO	
    }
//    else
//    {
//       	MGC_RESET_BIT(fault_state, STATE_ERR_BMSCHECKSELF);
//    }
//    
    state_BMSselfCheck=(UINT16)fault_state;

}

int getBMScheckselfState(void)
{
	return state_BMSselfCheck;
}

int BMS_GetErrStatusBMScheckselfByType( UINT8 types)
{
	assert(types<STATE_ERR_TOTAL);
	if( types == 0 )
		return (getBMScheckselfState()==0);
	if( ((1<<types) & getBMScheckselfState()) == 0)
		return 0;
	return 1;
}

/*
 * ���״̬�£����󱨾�������ģ��
 * */
static int ChargeCheckUpdate(void)
{
	int state=0;
	state=charge_type_check();
	if(state==normal)
	{
//		alarm(0);
	}
	else
	{
//		alarm(2);
	}
	if(state==connect_overtime)
	{ 
//         todo
	}
	else if(state==ccs_overtime)
	{
		shutdownRelayCharge();  //shut off the charge relay module
	}
	else if(state==bcl_overtime)
	{
		shutdownRelayCharge();  //shut off the charge relay module
	}
}

int AlarmUpdateVoltage(void){
//	sum_VolStateUpdate();
//	return cell_vol_state_change_update();
	return voltageUpdate();
}	
int AlarmUpdateTemperature(void){
	return TempStateUpdate();
}	
int AlarmUpdateCurrent(void){
	return CurrentStateUpdate();
}	
int AlarmUpdateSoc(void){
	return SocStateUpdate();
}	 
int AlarmUpdateInsulutionResistance(void)
{
	return 0;
};	
int AlarmUpdateRelayModule(void)
{
	return 0;
}
int AlarmUpdateBmsSelfCheck(void)
{
//	return BMS_selfCheckUpdate();
	return 0;
}
int AlarmUpdateCbcuCheck(void)
{
	return ChargeCheckUpdate();
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
