#ifndef _VCU_PROTOCOL_H_
#define _VCU_PROTOCOL_H_
/* vcu_protocol.h
 * the protocol for VCU communication
 * by mgc 2014-11-03
 * */
#include "public/types.h"

enum vcuRequestId
{
	VCU_CMD_SHUTDOWN_MAIN_RELAY = 1
};
struct canPack
{
	UINT32 id;
	UINT8 datalen;
	UINT8 *data;
};

/*
 * need external module interfaces
 * */
extern UINT8 BMS_GetSOCValue(void);  //0~100
extern UINT8 BMS_GetSOHValue(void);	 //0~100

/*currents*/ 

/*voltage*/ 

/*insulation resistance */
extern TYPE_INSRES GetInsulationResValuePositivePole(void);  //ko
extern TYPE_INSRES GetInsulationResValueNegativePole(void); //ko

/*temperature value*/

#define BMS_SelfCheckPass() BMS_selfCheck() //0
//extern int BMS_SelfCheckPass(void);  //1 pass, 0:negative
/*
 * the next 7 functions return value: 0 normal, 1:err
 * */
#define BMS_DetectionStateCheckCurrent() 0
#define BMS_DetectionStateCheckVoltage() 0
#define BMS_DetectionStateCheckTemperature() 0
#define BMS_DetectionStateCheckBalance() 0
#define BMS_DetectionStateCheckCanCom() 0
#define BMS_DetectionStateCheckVCULost() 0
//extern int BMS_DetectionStateCheckCurrent(void);
//extern int BMS_DetectionStateCheckVoltage(void);
//extern int BMS_DetectionStateCheckTemperature(void);
//extern int BMS_DetectionStateCheckBalance(void);
//extern int BMS_DetectionStateCheckCanCom(void);
//extern int BMS_DetectionStateCheckVCULost(void);

#define BMS_GetLoopChargeTimes() 0
//extern UINT16 BMS_GetLoopChargeTimes(void);
//extern int BMS_GetRequestPositiveRelayShutdown(void); // 1 have request, 0: no request

extern int BMS_GetChargerConnectStatus(void);    // 1,connect 0:unconnected
extern int BMS_IsChargging(void);                //1:charging, 0:uncharged



extern int BMS_GetErrStatusPrecharge(void);                 //1 err, 0 OK
extern int BMS_GetErrStatusTemperatureByType( UINT8 types);     //1:err, 0 OK
extern int BMS_GetErrStatusCurrentByType( UINT8 types);         //1:err, 0 OK
extern int BMS_GetErrStatusVoltageByType( UINT8 types);         //1:err, 0 OK
extern int BMS_GetErrStatusSOCByType( UINT8 types);             //1:err, 0 OK
extern int BMS_GetErrStatusRelayByType( UINT8 types); //1:err, 0 OK
extern int BMS_GetErrStatusResInsulationByType( UINT8 types); //1:err, 0 OK
extern int BMS_GetErrStatusBMSCheckSelf(void); //1:err, 0 OK	

/*
 * control interfaces
 * */


/*
 * interfaces for other modules
 * */
int sendRequestToVCU( UINT8 id );
int VCU_communication(void);
int packVCUCanB2VP1Package(void);
int packVCUCanB2VP2Package(void);
int packVCUCanB2VP3Package(void);
int packVCUCanB2VP4Package(void);
const char *readPackageBuff(void);
int BMS_GetVcuCmd(void);
int BMS_SetSimulatorVcuPower(UINT8 onoff);
void vcu_can_isr(struct canPack *ppack);
#endif

