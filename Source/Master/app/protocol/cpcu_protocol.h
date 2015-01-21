/*
 * this module should implement the protocol of the communication between
 * CPCU(charge point communication unit) and BMS(battery manage system).
 * which is described by <<GBT 27930>>
 * by mgc 20141106
 * */
#ifndef _CPCU_PROTOCOL_H_
#define _CPCU_PROTOCOL_H_
#include "public/types.h"
#include "voltage/voltage.h"
#include "temp/temperature.h"
#include "soc/soc.h"

/*
 * parameter configure state
 * 
 * */
// TODO

enum  Alarm
{
	  normal,
      connect_overtime,
      ccs_overtime,
      bcl_overtime
};
static UINT8 ERRO_INFO,alarmtype = normal;
extern UINT8 GetBMSProtocolVersion(void);// TODO       //   01
extern UINT8 GetBatteryType(void);// TODO                  //   03  
extern UINT16 GetBatteryChargeTimes(void);// TODO   
extern UINT16 GetBatteryMaxAllowedChargeVoltage(void);// TODO      //电池允许最大充电电压
extern UINT16 GetBatteryMaxAllowedChargeCurrent(void);// TODO     //电池允许最大充电电流
extern TYPE_CURRENT BMS_GetChargeCurrentValue(void);
extern int isDcChargerReadyOk(void);
/*
 * ready to charge
 * */
extern int BMS_ReadyToChargeStatus(void);  //TODO before charge , what should do.
static int alarm( UINT32 timems );
/*
 * state:
 * phase charging
 * the next functions are needed at this state.
 * */

/*BCL*/
UINT16 B2P_CPCU_VoltageNeedGet(void);
INT16 B2P_CPCU_CurrentNeedGet(void);
UINT8 B2P_CPCU_ChargeMode(void)  ;
/*BCS*/
extern UINT32 BMS_GetVoltageValueBatteryTotal(void);  //mv
extern TYPE_VOLTAGE BMS_GetVoltageValueMax(void);   //MV




 UINT16 GetChargeVoltage(void);  //TODO        // mv
 UINT16 GetRestChargingTime(void); //TODO     //  min
//extern int BMS_GetVolValueMaxCellId(void);
/*BSM*/
extern int BMS_GetTemperatureMaxValueSensorId(void);
extern int BMS_GetTemperatureMinValueSensorId(void);
extern TYPE_TEMP BMS_GetTemperaturValueMaxValue(void);
extern TYPE_TEMP BMS_GetTemperatureValueMinValue(void);

int BMS_CPCU_GetVechleVoltageState(void); //TODO 
int BMS_CPCU_GetCellVoltageState(void); //TODO
int BMS_CPCU_GetChargeCurrentState(void); //TODO
int BMS_CPCU_GetInsulationState(void); //TODO
int BMS_CPCU_GetOutConnectorConnectStatus(void); //TODO
extern int GetChargeFuncionAllowedStatus(void);  //TODO

/*BMV*/
extern TYPE_VOLTAGE getVoltageValueByCellIndex(UINT8 index);

/*BMT*/
extern TYPE_TEMP BMS_GetTemperatureValueById(int id);

/*BSP*/
//none

/*BST*/
struct stopChargeReason
{
	UINT8 stopReason;
	UINT16 FaultReason;
	UINT8 errReason;
};
int StopCharge(struct stopChargeReason *reason);

/*CST*/
//TODO
int DCCP_communication(void);

/*
 * 
 * the next state is the end of charging state.
 * */
/*BSD*/
extern UINT8 GetSoc(void);
extern TYPE_VOLTAGE BMS_GetVoltageValueMaxCell(void); //mv
extern TYPE_VOLTAGE BMS_GetVoltageValueMinCell(void); //mv
extern TYPE_TEMP BMS_GetTemperaturValueMaxValue(void);
extern TYPE_TEMP BMS_GetTemperatureValueMinValue(void);

/*CSD*/

/*
 * errs 
 * */
/*BEM*/ 
 //TODO
 

 #define MSG_SINFVOLT  0x01D1F3   //单体电压
 //MSG_SINFVOLT =  119283,   //单体电压
 #define MSG_SINFTEMP 0x3DD1F3   //单体温度
 #define MSG_CARCONT1 0x01D0F4   //整车监控1
 #define MSG_CARCONT2 0x02D0F4   //整车监控2

#define MSG_CRM 0x0001F556   //充电机辨识报文
#define MSG_BRM 0x000256F4 //BMS和车辆辨识报文
#define MSG_BCP 0x000656F4 //动力蓄电池充电参数
#define MSG_CTS 0x0007F556   //充电机发送时间同步信息
#define MSG_CML 0x0008F556   //充电机最大输出能力
#define MSG_BRO 0x000956F4 //电池充电准备就绪状态
#define MSG_CRO 0x000AF556   //充电机输出准备就绪状态
#define MSG_BCL 0x001056F4 //电池充电需求
#define MSG_BCS 0x001156F4 //电池充电总状态
#define MSG_CCS 0x0012F556   //充电机充电状态 
#define MSG_BSM 0x001356F4 //动力蓄电池状态信息
#define MSG_BMV 0x001556F4 //单体动力蓄电池电压
#define MSG_BMT 0x001656F4 //动力蓄电池温度
#define MSG_BSP 0x001756F4 //动力蓄电池预留
#define MSG_BST 0x001956F4 //BMS中止充电
#define MSG_CST 0x001AF556   //充电机中止充电
#define MSG_BSD 0x001C56F4 //BMS统计数据
#define MSG_CSD 0x001DF556  //充电机统计数据
#define MSG_BEM 0x001E56F5 //BMS错误报文
#define MSG_CEM 0x001FF556 //充电机错误报文
#define UP PTIH_PTIH0
#define DOWN PTIH_PTIH1
#define UP_dir DDRH_DDRH0
#define DOWN_dir DDRH_DDRH1

#define BARALLNUM   24
#define TEMPALLNUM   10


struct T_SMSGDATA     //发送报文的结构体
{
    UINT8 id[4];
    UINT8 data[8];
    UINT8 len;
    UINT8 prty;
};

struct T_RMSGDATA     //发送报文的结构体
{
    UINT8 id[4];
    UINT8 data[8];
};

struct T_ID         //消息id
{
    UINT8 cPF; //报文ID
    UINT8 cPS; //报文发送的目标地址
    UINT8 cSA; //报文发送的源地址
    UINT8 ptr;
};

struct T_SINGV    //单体电压
{
    UINT16 iV1;
    UINT16 iV2;
    UINT16 iV3;
    UINT16 iV4;
};

struct T_SINGT   //单体温度：
{
    UINT8 iT1;
    UINT8 iT2;
    UINT8 iT3;
    UINT8 iT4;
    UINT8 iT5;
    UINT8 iT6;
    UINT8 iT7;
    UINT8 iT8;
};

struct T_CarCt1  //整车监控1
{
    UINT8 iNum; //编号
    UINT16 iBusV;
    UINT16 iI;
    UINT8 cSoc;
    UINT8 iVHT;
    UINT8 cStFlg;
    UINT8 cResPower;
};

struct T_CarCt2  //整车监控2
{
    UINT8 iNum; //编号
    UINT16 iHV;
    UINT16 iLV;
    UINT8 cEHT;
    UINT8 cTTrage;
    UINT16 iStaFlag2;
};

struct T_BMSCarIdt  //BMS和车辆辨识报文
{
    UINT8 cpBmsVer[3]; //BMS通信版本号
    UINT8 cBatType; //电池类型
    UINT16  iRatingI; //额定总量
    UINT16  iRatingV; //额定电压
};

struct T_BCP1    //动力蓄电池充电参数报文1
{
    UINT8 iNum; //编号
    UINT16 iSChgV; //单体动力蓄电池最高允许充电电压
    INT16 iChgI;//最高允许充电电流
    UINT16 iAPower;//动力蓄电池标称总能力
};

struct T_BCP2  //动力蓄电池充电参数报文2
{
    UINT8 iNum; //编号
    UINT16 iAChargeV;//最高允许充电总电压
    UINT8 cAlT;//最高允许温度
    UINT16 iElcState; //整车动力蓄电池荷电状态
    UINT16 iAV; //整车动力蓄电池总电压
};

struct T_BRO  //BMS充电准备就绪报文
{
    UINT8 cChgRdState; //BMS充电准备就绪报文，0x00 未做好充电准备，0xAA完成充电准备
};

struct T_BCL //电池充电需求报文
{
    UINT16  iVReq; //电压要求
    INT16  iIReq; //电流需求
    UINT8 cChgType; //充电模式
};

struct T_BCS1 //电池充电总状态报文
{
    UINT8  iNum;
    UINT16  iVMer; //充电电压测量值
    INT16  iIMer; //充电电流测量值
    INT16  iSgVNum; //单体动力蓄电池电压及其组号
};

struct T_BCS2 //电池充电总状态报文
{
    UINT16  iNum;
    UINT8  cSOC; //当前荷电状态SOC（%）
    UINT16  iRmTime; //估算剩余充电时间
};

struct T_BSM  //发送动力蓄电池状态信息报文
{
    UINT8 cHSgVNum; //最高单体动力蓄电池电压所在编号
    UINT8 cHBarT; //最高动力蓄电池温度
    UINT8 cHTNum; //最高温度检测点编号
    UINT8 cLBarT; //最低动力蓄电池温度
    UINT8 cLTNum; //最低蓄电池温度检测点编号
    UINT8 cBarState1;
    UINT8 cBarState2;
};

struct T_BMV //单体动力蓄电池电压
{
    UINT16 iSgV1; //#1单体动力蓄电池电压
    UINT16 iSgV2; //#2单体动力蓄电池电压
    UINT16 iSgV3; //#3单体动力蓄电池电压
    UINT16 iSgV4; //#4单体动力蓄电池电压
};

struct T_BMT //单体动力蓄电池温度
{
    UINT8 iSgT1; //#1单体动力蓄电池温度
    UINT8 iSgT2; //#2单体动力蓄电池温度
    UINT8 iSgT3; //#3单体动力蓄电池温度
    UINT8 iSgT4; //#4单体动力蓄电池温度
    UINT8 iSgT5; //#1单体动力蓄电池温度
    UINT8 iSgT6; //#2单体动力蓄电池温度
    UINT8 iSgT7; //#3单体动力蓄电池温度
    UINT8 iSgT8; //#4单体动力蓄电池温度
};

struct T_BSP  //动力蓄电池预留
{
    UINT8 cObi1; //最高单体动力蓄电池电压所在编号
    UINT8 cObi2; //最高动力蓄电池温度
    UINT8 cObi3; //最高温度检测点编号
    UINT8 cObi4; //最低动力蓄电池温度
    UINT8 cObi5; //最低蓄电池温度检测点编号
    UINT8 cObi6;
    UINT8 cObi7;
    UINT8 cObi8;
};

struct T_BST  //中止充电报文
{
    UINT8 cReson; //BMS中止充电原因
    UINT16  iFault; //BMS中止充电故障原因
    UINT8 cErr;  //BMS中止充电错误原因
};

struct T_BSD  //BMS统计数据报文
{
    UINT8 cSOC; //中止荷电状态
    UINT16  iLSgV;//动力蓄电池单体最低电压
    UINT16  iHSgV;//动力蓄电池单体最高电压
    UINT8 cLSgT;//动力蓄电池最低温度
    UINT8 cHSgT;//动力蓄电池最高温度
};

struct T_CRM //充电机辨识报文
{
    UINT8 cRsult; //辨识结果，0x00 不能辨识，0xAA 能辨识
    UINT8 cChargeNum;//充电机编号
};

struct T_CTS //充电机发送时间同步信息
{
    UINT8 cTime[7]; //年月日时分秒
};

struct T_CML  //充电机最大输出能力
{
    UINT16 iHOtV;  //最高输出电压
    UINT16 iLOtV;  //最低输出电压
    UINT16 iMaxOtI; //最大输出电流
};

struct T_CRO  //充电机输出准备就绪
{
    UINT8 iReady; //充电机是否充电准备好，0x00充电机未完成准备，0xAA充电机完成充电准备
};

struct T_CCS //充电机充电状态报文
{
    UINT16  iOtV; //电压输出值
    UINT16  iOtI; //电流输出值
    UINT16  iAcTime; //累计充电时间
};

struct T_CST //充电机中止充电报文
{
    UINT8 cReson; //充电机中止充电原因
    UINT16  iFault; //充电机中止充电故障原因
    UINT8 cErr;  //充电机中止充电错误原因

};

struct T_CSD  //充电机统计数据报文
{
    UINT16 iAcTime; //累计充电时间
    UINT16 iOtPower; //输出能量
    UINT8 cChargeNum;//充电机编号
};

struct T_BEM   //BSM错误报文
{
	UINT8 CRM1 :2;                
	UINT8 CRM2 :2; 
	UINT8  :4;
	UINT8 CTS_CML :2;
	UINT8 CRO : 2;
	UINT8  :4;
	UINT8 CCS : 2; 
	UINT8 CST : 2;
	UINT8  :4;
	UINT8 CSD : 2;
	UINT8 :6;
};

struct T_CEM
{  
	UINT8 BRM :2;                
	UINT8  :6;
	UINT8 BCP :2;
	UINT8 BRO : 2;
	UINT8  :4;
	UINT8 BCS : 2; 
	UINT8 BCL: 2;
	UINT8 BST: 2;
	UINT8  :2;
	UINT8 BSD : 2;
	UINT8 :6;
	 
};
struct T_VINFOR
{
    UINT16  iVolt;  //电压
    UINT8   iNum;   //电压编号
};

struct T_TINFOR
{
    UINT8  iTemp;  //温度
    UINT8   iNum;   //电压编号
};


UINT8 MSCAN1RecvMsg();
void send_erro(UINT8 type);
//UINT8 MSCANGetMsg( struct T_RCANMSG *msg );
//UINT8 UnpackCANMsg( struct T_RCANMSG msg, void* rmsg );
UINT8 MSCAN0GetMsg( struct T_ID* msgid, UINT8* msgdata );
UINT8 MSCAN1GetMsg( struct T_ID* msgid, char* msgdata );
void cpyid(struct T_ID* id,UINT32 MsgId);

void SelectUnpackFun ( struct T_ID id, struct T_RMSGDATA msg, void* vdata );
int UnpackSignV ( struct T_RMSGDATA msg, void* vdata );
//将id转化到发送结构体中
UINT8 PackMsgid ( struct T_SMSGDATA* msg, struct T_ID msgid );
//将消息内容结构体转化到id中
UINT8 UnpackMsgid ( struct T_RMSGDATA msg, struct T_ID* msgid );
//对外接口
void INIT_CAN ( void ); //初始化can
void SendStopCharge(); //发送停止充电命令

//extern void CAN_A_TX(struct T_SMSGDATA msg);
UINT8 SendCanMsg( struct T_ID id, UINT8* msgdata );
void RecvCanMsg ( void );
void MsgCrmDeal ( struct T_CRM* tCRM);
void MsgCmlDeal ( struct  T_CML* tCML );
void MsgCroDeal ( struct T_CRO* tCRO );
void MsgCcsDeal ( );
void MsgCstDeal ( struct T_CST* tCST );
struct T_VINFOR GetUltimateV ( int iType );
struct T_TINFOR GetUltimateT ( int iType );
void RecvCanMsg();
void Send_BMS();
void Send_BCP();
void Send_BRO();
void Send_BCL(void);
void Send_BCS(void);
void Send_BSM();
void Send_BMV();
void Send_BMT();
void Charge_check();
void Send_BSD();
void Send_BEM();
//外部需要调用
void Send_BST();
UINT8 charge_type_check(void);
//测试函数
void test_SINGV();
void test_SINGT();
void test();
void  DeserveCEM();
UINT8 get_creson();
UINT8 get_stopcharge_err();
UINT16 get_stopcharge_fault();
int BMS_GetErrStatusVoltageByType( UINT8 types);  
#define GET_CHARGE_ON()   0xaa //170   
//BMS是否充电准备好 返回一个字节  0x00 BMS未做好充电准备 0xAA BMS完成充电准备
#define GET_CHARGE_OFF()   0x00 //170   

#define  GET_CHARGE_MODE()   1  
//充电模式 返回一个字节   0x01 恒压充电   0x02恒流充电                                                                 
//#define GET_CHARGE_TIME()   600  //累计充电时间

#define GET_LEFT_CHAGRE_TIME()  60 
//估算剩余充电时间   返回两个字节   数据范围：0min~600min



 UINT8 GET_BARSTATE1(void) ;  
//  电池状态1    返回一个字节 
//   0-1  单体电压状态(0x00 正常 0x01 过高  0x10过低)
//   2-3 SOC状态(0x00正常 0x01过高 0x10过低)
//   4-5 电流状态(0x00正常  0x01过流  0x10 不可信  )
//   6-7 温度状态 (0x00 正常 0x01 过高  0x10过低)
 

 
 
 UINT8 GET_BARSTATE2(void)   ; 
//电池状态2   返回一个字节  
//0-1 动力出电池绝缘状态（0x00正常  0x01过流  0x10 不可信 ）
//2-3  动力蓄电池输出连接器连接状态（0x00正常  0x01过流  0x10 不可信 ）
//4-5  充电允许（00禁止，01允许）

 
 
 
#define GET_SOC_STATE()  soc_state_get() //1     
//是否达到所需的SOC目标值，0：未达到，1达到，2不可信状态

#define GET_CHIEFVOLT_STATE()  vol_state_get()// 1     
//是否达到总电压设定值，0：未达到，1达到，2不可信状态

#define GET_SIGNVOLT_STATE()  vol_state_get() // 1    
//是否达到单体电压设定值，0：未达到，1达到，2不可信状态


#define GET_STOPCHARGE_FAULT() 2   
//BMS中止充电故障原因  返回2字节  
//第1~2位：绝缘故障(00正常，01故障，10不可信转改)
//第3~4位：输出连接器过温故障(00正常，01故障，10不可信转改)
//第5~6位：BMS原件、输出连接器过温(00正常，01故障，10不可信转改)
//第7~8位：充电连接器故障(00正常，01故障，10不可信转改)
//第9~10位：电池组温度过高(00正常，01故障，10不可信转改)
//第11~12位：其他故障(00正常，01故障，10不可信转改)


 
#define GET_STOPCHARGE_ERR()  ((vol_state_get()<<2)|current_state_get())// 2   
//BMS中止充电错误原因  返回1字节  
//第1~2位：电流过大  （00电流正常，01电流超出需求之，10不可信状态)
//第3~4位：电压异常（00：正常，01电压异常，10不可信状态）

#define crmFunc()   1   
//充电机辨识报文  输入参数const struct T_CRM tCRM，返回执行结果，0正确，非0错误

#define ctsFunc( timespec )    1 
//充电机发送时间同步信息    输入参数const struct T_CTS tCTS，返回执行结果，0正确，非0错误

#define cmlFunc( tCML )   1 
//充电机最大输出能力      输入参数const struct T_CML tCML，返回执行结果，0正确，非0错误

#define croFunc()   1 
//充电机输出准备就绪状态   输入参蔯onst struct T_CRO tCRO祷刂葱薪峁?正确，非0错误
//收到此命令后，BMS开始充电


#define ccsFunc()  1//充电机充电状态          输入参数const struct T_CCS tCCS，返回执行结果，0正确，非0错误

/******************************************************
struct T_CST //充电机中止充电报文
{
    UINT8 cReson; //充电机中止充电原因
    UINT16  iFault; //充电机中止充电故障原因
    UINT8 cErr;  //充电机中止充电错误原因

};
****************************************************/
#define cstFunc()  1//充电机中止充电原因    输入参数const struct T_CST tCST，返回执行结果，0正确，非0错误

/*****************************************************
struct T_CSD  //充电机统计数据报文
{
    UINT16 iAcTime; //累计充电时间
    UINT16 iOtPower; //输出能量
    UINT8 cChargeNum;//充电机编号
};
*****************************************************/
#define csdFunc()  1  //充电机统计数据      输入参数const struct T_CSD tCSD，返回执行结果，0正确，非0错误


#endif

