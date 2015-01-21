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
extern UINT16 GetBatteryMaxAllowedChargeVoltage(void);// TODO      //�������������ѹ
extern UINT16 GetBatteryMaxAllowedChargeCurrent(void);// TODO     //���������������
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
 

 #define MSG_SINFVOLT  0x01D1F3   //�����ѹ
 //MSG_SINFVOLT =  119283,   //�����ѹ
 #define MSG_SINFTEMP 0x3DD1F3   //�����¶�
 #define MSG_CARCONT1 0x01D0F4   //�������1
 #define MSG_CARCONT2 0x02D0F4   //�������2

#define MSG_CRM 0x0001F556   //������ʶ����
#define MSG_BRM 0x000256F4 //BMS�ͳ�����ʶ����
#define MSG_BCP 0x000656F4 //�������س�����
#define MSG_CTS 0x0007F556   //��������ʱ��ͬ����Ϣ
#define MSG_CML 0x0008F556   //��������������
#define MSG_BRO 0x000956F4 //��س��׼������״̬
#define MSG_CRO 0x000AF556   //�������׼������״̬
#define MSG_BCL 0x001056F4 //��س������
#define MSG_BCS 0x001156F4 //��س����״̬
#define MSG_CCS 0x0012F556   //�������״̬ 
#define MSG_BSM 0x001356F4 //��������״̬��Ϣ
#define MSG_BMV 0x001556F4 //���嶯�����ص�ѹ
#define MSG_BMT 0x001656F4 //���������¶�
#define MSG_BSP 0x001756F4 //��������Ԥ��
#define MSG_BST 0x001956F4 //BMS��ֹ���
#define MSG_CST 0x001AF556   //������ֹ���
#define MSG_BSD 0x001C56F4 //BMSͳ������
#define MSG_CSD 0x001DF556  //����ͳ������
#define MSG_BEM 0x001E56F5 //BMS������
#define MSG_CEM 0x001FF556 //����������
#define UP PTIH_PTIH0
#define DOWN PTIH_PTIH1
#define UP_dir DDRH_DDRH0
#define DOWN_dir DDRH_DDRH1

#define BARALLNUM   24
#define TEMPALLNUM   10


struct T_SMSGDATA     //���ͱ��ĵĽṹ��
{
    UINT8 id[4];
    UINT8 data[8];
    UINT8 len;
    UINT8 prty;
};

struct T_RMSGDATA     //���ͱ��ĵĽṹ��
{
    UINT8 id[4];
    UINT8 data[8];
};

struct T_ID         //��Ϣid
{
    UINT8 cPF; //����ID
    UINT8 cPS; //���ķ��͵�Ŀ���ַ
    UINT8 cSA; //���ķ��͵�Դ��ַ
    UINT8 ptr;
};

struct T_SINGV    //�����ѹ
{
    UINT16 iV1;
    UINT16 iV2;
    UINT16 iV3;
    UINT16 iV4;
};

struct T_SINGT   //�����¶ȣ�
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

struct T_CarCt1  //�������1
{
    UINT8 iNum; //���
    UINT16 iBusV;
    UINT16 iI;
    UINT8 cSoc;
    UINT8 iVHT;
    UINT8 cStFlg;
    UINT8 cResPower;
};

struct T_CarCt2  //�������2
{
    UINT8 iNum; //���
    UINT16 iHV;
    UINT16 iLV;
    UINT8 cEHT;
    UINT8 cTTrage;
    UINT16 iStaFlag2;
};

struct T_BMSCarIdt  //BMS�ͳ�����ʶ����
{
    UINT8 cpBmsVer[3]; //BMSͨ�Ű汾��
    UINT8 cBatType; //�������
    UINT16  iRatingI; //�����
    UINT16  iRatingV; //���ѹ
};

struct T_BCP1    //�������س���������1
{
    UINT8 iNum; //���
    UINT16 iSChgV; //���嶯����������������ѹ
    INT16 iChgI;//������������
    UINT16 iAPower;//�������ر��������
};

struct T_BCP2  //�������س���������2
{
    UINT8 iNum; //���
    UINT16 iAChargeV;//����������ܵ�ѹ
    UINT8 cAlT;//��������¶�
    UINT16 iElcState; //�����������غɵ�״̬
    UINT16 iAV; //�������������ܵ�ѹ
};

struct T_BRO  //BMS���׼����������
{
    UINT8 cChgRdState; //BMS���׼���������ģ�0x00 δ���ó��׼����0xAA��ɳ��׼��
};

struct T_BCL //��س��������
{
    UINT16  iVReq; //��ѹҪ��
    INT16  iIReq; //��������
    UINT8 cChgType; //���ģʽ
};

struct T_BCS1 //��س����״̬����
{
    UINT8  iNum;
    UINT16  iVMer; //����ѹ����ֵ
    INT16  iIMer; //����������ֵ
    INT16  iSgVNum; //���嶯�����ص�ѹ�������
};

struct T_BCS2 //��س����״̬����
{
    UINT16  iNum;
    UINT8  cSOC; //��ǰ�ɵ�״̬SOC��%��
    UINT16  iRmTime; //����ʣ����ʱ��
};

struct T_BSM  //���Ͷ�������״̬��Ϣ����
{
    UINT8 cHSgVNum; //��ߵ��嶯�����ص�ѹ���ڱ��
    UINT8 cHBarT; //��߶��������¶�
    UINT8 cHTNum; //����¶ȼ�����
    UINT8 cLBarT; //��Ͷ��������¶�
    UINT8 cLTNum; //��������¶ȼ�����
    UINT8 cBarState1;
    UINT8 cBarState2;
};

struct T_BMV //���嶯�����ص�ѹ
{
    UINT16 iSgV1; //#1���嶯�����ص�ѹ
    UINT16 iSgV2; //#2���嶯�����ص�ѹ
    UINT16 iSgV3; //#3���嶯�����ص�ѹ
    UINT16 iSgV4; //#4���嶯�����ص�ѹ
};

struct T_BMT //���嶯�������¶�
{
    UINT8 iSgT1; //#1���嶯�������¶�
    UINT8 iSgT2; //#2���嶯�������¶�
    UINT8 iSgT3; //#3���嶯�������¶�
    UINT8 iSgT4; //#4���嶯�������¶�
    UINT8 iSgT5; //#1���嶯�������¶�
    UINT8 iSgT6; //#2���嶯�������¶�
    UINT8 iSgT7; //#3���嶯�������¶�
    UINT8 iSgT8; //#4���嶯�������¶�
};

struct T_BSP  //��������Ԥ��
{
    UINT8 cObi1; //��ߵ��嶯�����ص�ѹ���ڱ��
    UINT8 cObi2; //��߶��������¶�
    UINT8 cObi3; //����¶ȼ�����
    UINT8 cObi4; //��Ͷ��������¶�
    UINT8 cObi5; //��������¶ȼ�����
    UINT8 cObi6;
    UINT8 cObi7;
    UINT8 cObi8;
};

struct T_BST  //��ֹ��籨��
{
    UINT8 cReson; //BMS��ֹ���ԭ��
    UINT16  iFault; //BMS��ֹ������ԭ��
    UINT8 cErr;  //BMS��ֹ������ԭ��
};

struct T_BSD  //BMSͳ�����ݱ���
{
    UINT8 cSOC; //��ֹ�ɵ�״̬
    UINT16  iLSgV;//�������ص�����͵�ѹ
    UINT16  iHSgV;//�������ص�����ߵ�ѹ
    UINT8 cLSgT;//������������¶�
    UINT8 cHSgT;//������������¶�
};

struct T_CRM //������ʶ����
{
    UINT8 cRsult; //��ʶ�����0x00 ���ܱ�ʶ��0xAA �ܱ�ʶ
    UINT8 cChargeNum;//�������
};

struct T_CTS //��������ʱ��ͬ����Ϣ
{
    UINT8 cTime[7]; //������ʱ����
};

struct T_CML  //��������������
{
    UINT16 iHOtV;  //��������ѹ
    UINT16 iLOtV;  //��������ѹ
    UINT16 iMaxOtI; //����������
};

struct T_CRO  //�������׼������
{
    UINT8 iReady; //�����Ƿ���׼���ã�0x00����δ���׼����0xAA������ɳ��׼��
};

struct T_CCS //�������״̬����
{
    UINT16  iOtV; //��ѹ���ֵ
    UINT16  iOtI; //�������ֵ
    UINT16  iAcTime; //�ۼƳ��ʱ��
};

struct T_CST //������ֹ��籨��
{
    UINT8 cReson; //������ֹ���ԭ��
    UINT16  iFault; //������ֹ������ԭ��
    UINT8 cErr;  //������ֹ������ԭ��

};

struct T_CSD  //����ͳ�����ݱ���
{
    UINT16 iAcTime; //�ۼƳ��ʱ��
    UINT16 iOtPower; //�������
    UINT8 cChargeNum;//�������
};

struct T_BEM   //BSM������
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
    UINT16  iVolt;  //��ѹ
    UINT8   iNum;   //��ѹ���
};

struct T_TINFOR
{
    UINT8  iTemp;  //�¶�
    UINT8   iNum;   //��ѹ���
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
//��idת�������ͽṹ����
UINT8 PackMsgid ( struct T_SMSGDATA* msg, struct T_ID msgid );
//����Ϣ���ݽṹ��ת����id��
UINT8 UnpackMsgid ( struct T_RMSGDATA msg, struct T_ID* msgid );
//����ӿ�
void INIT_CAN ( void ); //��ʼ��can
void SendStopCharge(); //����ֹͣ�������

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
//�ⲿ��Ҫ����
void Send_BST();
UINT8 charge_type_check(void);
//���Ժ���
void test_SINGV();
void test_SINGT();
void test();
void  DeserveCEM();
UINT8 get_creson();
UINT8 get_stopcharge_err();
UINT16 get_stopcharge_fault();
int BMS_GetErrStatusVoltageByType( UINT8 types);  
#define GET_CHARGE_ON()   0xaa //170   
//BMS�Ƿ���׼���� ����һ���ֽ�  0x00 BMSδ���ó��׼�� 0xAA BMS��ɳ��׼��
#define GET_CHARGE_OFF()   0x00 //170   

#define  GET_CHARGE_MODE()   1  
//���ģʽ ����һ���ֽ�   0x01 ��ѹ���   0x02�������                                                                 
//#define GET_CHARGE_TIME()   600  //�ۼƳ��ʱ��

#define GET_LEFT_CHAGRE_TIME()  60 
//����ʣ����ʱ��   ���������ֽ�   ���ݷ�Χ��0min~600min



 UINT8 GET_BARSTATE1(void) ;  
//  ���״̬1    ����һ���ֽ� 
//   0-1  �����ѹ״̬(0x00 ���� 0x01 ����  0x10����)
//   2-3 SOC״̬(0x00���� 0x01���� 0x10����)
//   4-5 ����״̬(0x00����  0x01����  0x10 ������  )
//   6-7 �¶�״̬ (0x00 ���� 0x01 ����  0x10����)
 

 
 
 UINT8 GET_BARSTATE2(void)   ; 
//���״̬2   ����һ���ֽ�  
//0-1 ��������ؾ�Ե״̬��0x00����  0x01����  0x10 ������ ��
//2-3  ���������������������״̬��0x00����  0x01����  0x10 ������ ��
//4-5  �������00��ֹ��01����

 
 
 
#define GET_SOC_STATE()  soc_state_get() //1     
//�Ƿ�ﵽ�����SOCĿ��ֵ��0��δ�ﵽ��1�ﵽ��2������״̬

#define GET_CHIEFVOLT_STATE()  vol_state_get()// 1     
//�Ƿ�ﵽ�ܵ�ѹ�趨ֵ��0��δ�ﵽ��1�ﵽ��2������״̬

#define GET_SIGNVOLT_STATE()  vol_state_get() // 1    
//�Ƿ�ﵽ�����ѹ�趨ֵ��0��δ�ﵽ��1�ﵽ��2������״̬


#define GET_STOPCHARGE_FAULT() 2   
//BMS��ֹ������ԭ��  ����2�ֽ�  
//��1~2λ����Ե����(00������01���ϣ�10������ת��)
//��3~4λ��������������¹���(00������01���ϣ�10������ת��)
//��5~6λ��BMSԭ�����������������(00������01���ϣ�10������ת��)
//��7~8λ���������������(00������01���ϣ�10������ת��)
//��9~10λ��������¶ȹ���(00������01���ϣ�10������ת��)
//��11~12λ����������(00������01���ϣ�10������ת��)


 
#define GET_STOPCHARGE_ERR()  ((vol_state_get()<<2)|current_state_get())// 2   
//BMS��ֹ������ԭ��  ����1�ֽ�  
//��1~2λ����������  ��00����������01������������֮��10������״̬)
//��3~4λ����ѹ�쳣��00��������01��ѹ�쳣��10������״̬��

#define crmFunc()   1   
//������ʶ����  �������const struct T_CRM tCRM������ִ�н����0��ȷ����0����

#define ctsFunc( timespec )    1 
//��������ʱ��ͬ����Ϣ    �������const struct T_CTS tCTS������ִ�н����0��ȷ����0����

#define cmlFunc( tCML )   1 
//��������������      �������const struct T_CML tCML������ִ�н����0��ȷ����0����

#define croFunc()   1 
//�������׼������״̬   ������const struct T_CRO tCRO�������ִ�н���?��ȷ����0����
//�յ��������BMS��ʼ���


#define ccsFunc()  1//�������״̬          �������const struct T_CCS tCCS������ִ�н����0��ȷ����0����

/******************************************************
struct T_CST //������ֹ��籨��
{
    UINT8 cReson; //������ֹ���ԭ��
    UINT16  iFault; //������ֹ������ԭ��
    UINT8 cErr;  //������ֹ������ԭ��

};
****************************************************/
#define cstFunc()  1//������ֹ���ԭ��    �������const struct T_CST tCST������ִ�н����0��ȷ����0����

/*****************************************************
struct T_CSD  //����ͳ�����ݱ���
{
    UINT16 iAcTime; //�ۼƳ��ʱ��
    UINT16 iOtPower; //�������
    UINT8 cChargeNum;//�������
};
*****************************************************/
#define csdFunc()  1  //����ͳ������      �������const struct T_CSD tCSD������ִ�н����0��ȷ����0����


#endif

