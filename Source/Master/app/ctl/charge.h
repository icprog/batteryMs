#ifndef _CHARGE_H_
#define _CHARGE_H_

/*****************************************includs******************************************/

#include "public/public.h"
#include "voltage/voltage.h"
#include "current/current.h"


/*****************************************types******************************************/

#define CableCapacity_A1  32
#define CableCapacity_A2  16
static const UINT32 MAX_ALLOWED_S3CUTOFF_TIME = 300;
static const UINT32 MAX_ALLOWED_CHECKPWM_TIME = 5000;
static const UINT32 VEH_FORCE_CHARGER_Current = 13; //13A
static  UINT32 VEH_PRESENT_CHARGER_Current ;
static  UINT8 VEH_CHARGER_POWER_STATE ;
static  UINT8  ACCP_COMMUNICATION_SATE_FLAG=0;  // 0:communication successful; 1:communication failed
enum DC_CHARGER_READY_CONDITION
{
	CON_CHARGE_RELAY_OP_OK = 0,
	CON_TOTAL
};
int BMS_GetChargerConnectStatus(void);    // 1,connect 0:unconnected
int BMS_IsChargging(void);                //1:charging, 0:uncharged

//int BMS_SelfCheckPass(void);  //1 pass, 0:negative

int SetSystemActiveMode(UINT8 mode);
UINT8 GetSystemActiveMode( void );
int SleepModeDetection(void);

int PowerOnHighVoltage(void);
int PowerOffHighVoltage(void);
int HighVoltageStep(void);
int AC_Charge_HighVoltage(void);

int getErrPrecharge(void);

int StartDcCharge(void);
int StartAcCharge(void);
int StartDischarge(void);

int StartChargeByUart(void);
int StopChargeByUart(void);

int setDcChargerCondition(UINT8 conditions, UINT8 state);
int isDcChargerReadyOk(void);

int set_VEH_CHARGER_poweron(void);
int set_VEH_CHARGER_poweroff(void);
UINT16 GetVehChargerCurrentByCan(void);
int VEH_CHARGER_selfCheck(void);
int Batterystateupdate(void);
int Check_VEH_CHARGER_UpperLimmitCurrent(void);
void Set_VEH_CHARGER_Force_Current(void);
void VEH_CHARGER_UpperLimmitCurrent_Update(void);
int Check_VEH_CHARGER_OVER(void);
int Conversion_CableCapacityMVtoR(UINT16 data_mv);
int CheckACChargePWM(void);
void SendVEH_CHARGER_UpperLimmitCurrentByCan(UINT16 data_current);
int GetVCUcmd_ForceCharge(void);
int GetVCUcmd_STOPACcharge(void);
int GetCCconnectState(void);
int WaitACchargeOver(void);
UINT16 GetPowerSupplyCurrentByPWM(void);
UINT16 GetCableRateCurrentByCC(void);
UINT16 Exportleast(UINT16 dataA,UINT16 dataB,UINT16 dataC);

int CHECK_ACCP_COMMUNICATION_SATE(void);
int GetAccharge_precharge_state(void);
void SetAccharge_precharge_state(void);
void ResetAccharge_precharge_state(void);


int BMS_GetErrStatusPrecharge();
//int PowerOnChargeRelay(void);
//int StartChargeByUart(void);
//int StopChargeByUart(void);
//int HighVoltageStep(void);

#endif

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
