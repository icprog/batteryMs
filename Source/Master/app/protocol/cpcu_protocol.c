#include "public/types.h"
#include "public/public.h"
#include "voltage/voltage.h"
#include "temp/temperature.h"
//#include "soc/soc.h"
#include "string.h"
#include "public/drivers.h"
#include "current/current.h"
#include "protocol/cpcu_protocol.h"
//#include "ctl/control.h"
#define UP PTIH_PTIH0
#define DOWN PTIH_PTIH1
#define UP_dir DDRH_DDRH0
#define DOWN_dir DDRH_DDRH1
#define over_time 20
#define BARALLNUM   24
#define TEMPALLNUM   10
#define TEMP_PY 50      //温度偏移
//#define FALSE 0
//#define TRUE 1

enum CPCU_STATE
{
	ST_LINK = 0, START_ON = 1, PA_CONFIG = 2, Get_Charge = 3, Charge_Over = 4
};
enum Wait_msg
{
	BMS_R_CRM0,
	BMS_R_CRM1,
	BMS_R_CTS,
	BMS_R_CML,
	BMS_R_CRO,
	BMS_R_CCS,
	BMS_R_CST,
	BMS_R_CST1,
	BMS_R_CSD

};

enum ERRO_Inf
{
	BMS_R_CRM0_OVERTIME = 0,
	BMS_R_CRM1_OVERTIME,
	BMS_R_CTS_OVERTIME,
	BMS_R_CML_OVERTIME,
	BMS_R_CRO_OVERTIME,
	BMS_R_CCS_OVERTIME,
	BMS_R_CST_OVERTIME,
	BMS_R_CSD_OVERTIME,
	BMS_R_CEM
};

UINT8 flag;
UINT8 BMSCRM0_flag = 1, BMSCRM1_flag = 1, BMSCTS_flag = 1, BMSCML_flag = 1,
		BMSCRO_flag = 1, BMSCST1_flag = 1;
UINT8 BMSCRM0_waitflag = 1, BMSCRM1_waitflag = 1, BMSCTS_waitflag = 1,
		BMSCML_waitflag = 1, BMSCRO_waitflag = 1, BMSCST1_waitflag = 1;
UINT8 BMSCCS_waitflag = 1, BMSCSD_waitflag = 1, BMSCST_waitflag = 1;
UINT8 BMSCCS_flag = 1, BMSCSD_flag = 1, BMSCST_flag = 1;
INT8 lastTime_flag = 0;
INT8 CPCU_state = 0;
INT8 WAIT_MSG = BMS_R_CRM0;
UINT32 lid;
UINT8 Put_flag = 0, recive_cem = 0, deserve_cem = 0;
static UINT8 msgdata[8];
struct T_CEM tCEM[100];
struct T_BEM tBEM;
//BOOl  g_bHreat = TRUE; //TRUE表示第一次心跳，FALSE表示不是第一次心跳

UINT8 g_bHreat = TRUE; //TRUE表示第一次心跳，FALSE表示不是第一次心跳
/*************************************************************/
/*                      获取充电装的消息内容             */
/*************************************************************/

/*************************************************************/
//函数名称：UnpackMsgid
//函数功能：将msg中的id转换到msgid中
//输入参数：msg，接收到的消息内容，msgid，要转换为的消息id
//输出参数：TURE表示数据添加成功，FALSE表示数据添加失败
/*************************************************************/
UINT8 UnpackMsgid(struct T_RMSGDATA msg, struct T_ID* msgid)  //BOOl
{
//    msgid->cSA = (msg.id[3]>>1)|((msg.id[2]<<7)&0x80);
//    msgid->cPS = (msg.id[2]>>1)|((msg.id[1]<<7)&0x80);
//    msgid->cPF = ((msg.id[1]>>1)&0x03)|((msg.id[1]>>3)&0x1C)|((msg.id[0]<<5));   
	msgid->cSA = msg.id[3];
	msgid->cPS = msg.id[2];
	msgid->cPF = (msg.id[1] & 0x03) | ((msg.id[1] & 0xf0) >> 2)
			| ((msg.id[0] & 0x03) << 6);
	// msgid->ptr = ((msg.id[0]&0x70)>>4);

	return TRUE;
}

/*************************************************************/
//函数名称：PackMsgid
//函数功能：将msgid中的数据添加到msg中
//输入参数：msg，要发送的数据的结构体，msgid，要发送的命令的id
//输出参数：TURE表示数据添加成功，FALSE表示数据添加失败
/*************************************************************/
UINT8 PackMsgid(struct T_SMSGDATA* msg, struct T_ID msgid)   //BOOL
{
//    msg->id[2] = msgid.cPS<<1|((msgid.cSA&0x80)>>7);
//    msg->id[1] = 0xE7&(((msgid.cPS&0x80)>>7)|((msgid.cPF<<1)&0x06)|((msgid.cPF<<3)&0xE0));
//    msg->id[0] = (msgid.cPF>>5)|0x00; 
//    msg->id[3] = 0xFE&(msgid.cSA<<1);
//    msg->id[0] = 0xf3&((msgid.ptr&0x07)<<4)|((msgid.cPF&0xc0)>>6);      //8 sof, 7_5 ptr ,4_r 3_dp,2_1 pf8_7
	msg->id[0] = 0xf3 & ((msgid.cPF & 0xc0) >> 6);
	msg->id[1] = ((msgid.cPF & 0x3c) << 2) | (msgid.cPF & 0x03) | 0x04; //8-5, pf6_3, 4_sr 3_ide, 2_1 pf2_1
	msg->id[2] = 0xff & msgid.cPS;                         //  ps
	msg->id[3] = 0xFF & msgid.cSA;                        //sa
	return TRUE;
}

UINT16 B2P_CPCU_VoltageNeedGet(void)
{
	//UINT16 needpower,needVoltage;
	UINT16 needVoltage;

	needVoltage = (VALUE_TH_OV_CELL / 100) * MAX_CELLS;
}
// mv 
INT16 B2P_CPCU_CurrentNeedGet(void)
{
	INT16 Need_Current;
	Need_Current = BMS_GetMaxAlowedChargeCurrent();
	return Need_Current;
}    // a   -400
UINT8 B2P_CPCU_ChargeMode(void)         //01 恒压 ，  02 恒流
{
	UINT8 changemode;
	if (1)
		changemode = 0x02;
	else
		changemode = 0x01;
	return changemode;
}
UINT16 GetRestChargingTime()
{
	UINT16 power, resttime, chargecurrent;
	power = (UINT16) (AWEIGHT * (BMS_GetSOCValue()));
	power = AWEIGHT - power;
	chargecurrent = (UINT16) (BMS_GetChargeCurrentValue() / 1000);
	if (chargecurrent)
		resttime = power / chargecurrent;
	else
		resttime = 0;
	return resttime;
}

UINT8 GET_BARSTATE1(void)
{
	UINT8 state1, cellV, socstate, currentstate, tempstate;
	if (BMS_GetErrStatusVoltageByType(STATE_VOL_NORMAL))
	{
		cellV = 0x03 & 0x00;
	}
	else if (BMS_GetErrStatusVoltageByType(STATE_CELL_VOL_LOW))
	{
		cellV = 0x03 & 0x10;
	}
	else   //( BMS_GetErrStatusVoltageByType(STATE_CELL_VOL_HIGH))
	{
		cellV = 0x03 & 0x01;
	}
	if (BMS_GetErrStatusSocByType(STATE_SOC_NORMAL))
	{
		socstate = 0x03 & 0x00;
	}
	else if (BMS_GetErrStatusSocByType(STATE_SOC_LOW))
	{
		socstate = 0x03 & 0x10;
	}
	else
	{
		socstate = 0x03 & 0x01;
	}
	if (BMS_GetErrStatusCurrentByType(STATE_CURRENT_NORMAL))
	{
		currentstate = 0x03 & 0x00;
	}
	else if (BMS_GetErrStatusCurrentByType(STATE_CURRENT_CHARGE_OC_LEVEL2))
	{
		currentstate = 0x03 & 0x01;
	}
	else
	{
		currentstate = 0x03 & 0x10;
	}
	if (BMS_GetErrStatusTemperatureByType(STATE_TEMP_NORMAL))
	{
		tempstate = 0x03 & 0x00;
	}
	else if (BMS_GetErrStatusTemperatureByType(STATE_TEMP_LOW))
	{
		tempstate = 0x03 & 0x10;
	}
	else
	{
		tempstate = 0x03 & 0x01;
	}
	state1 = cellV
			& 0x03 + ((socstate & 0x03) << 2) + ((currentstate & 0x03) << 4)
					+ ((tempstate & 0x03) << 6);
	return state1;
}

UINT8 get_creson()
{
	UINT8 reson, soc, chiefv, cellv;
	if (0 < BMS_GetSOCValue() < 100)
	{
		soc = 0x03 & 0x00;
	}
	else if (BMS_GetSOCValue() == 100)
	{
		soc = 0x03 & 0x01;
	}
	else
	{
		soc = 0x03 & 0x10;
	}
	if ((BMS_GetVoltageValueBatteryTotal() / 100) < ACHATEV)
	{
		chiefv = 0x03 & 0x00;
	}
	else if ((BMS_GetVoltageValueBatteryTotal() / 100) == ACHATEV)
	{
		chiefv = 0x03 & 0x01;
	}
	else
	{
		chiefv = 0x03 & 0x10;
	}
	if ((BMS_GetVoltageValueMin() / 10) < SCHGV)
	{
		cellv = 0x03 & 0x00;
	}
	else if ((BMS_GetVoltageValueMin() / 10) == SCHGV)
	{
		cellv = 0x03 & 0x01;
	}
	else
	{
		cellv = 0x03 & 0x10;
	}

	reson = soc & 0x03 + ((chiefv & 0x03) << 2) + ((cellv & 0x03) << 4);
	return reson;
}

UINT8 GET_BARSTATE2(void)
{
	UINT8 state2, rstate, cstate, charge;
	//rstate = 0x03&(BMS_GetErrStatusResInsulationByType());             ///++++
	rstate = 0x03 & 0x00;
	cstate = 0x03 & 0x00;                                                ///++++
	charge = 0x03 & ((UINT8) isDcChargerReadyOk());
	state2 = rstate & 0x03 + ((cstate & 0x03) << 2) + ((charge & 0x03) << 4);
	return state2;
}

UINT16 get_stopcharge_fault()
{
	UINT16 fault, R_fault, trans_fault, BMS_falut, charge_fault, temp_high,
			other_fault;
	R_fault = 0x0000;
	trans_fault = 0x0000;
	BMS_falut = 0x0000;
	if (BMS_IO_GET(ID_IO_STATUS_CC2) == 0)
		charge_fault = 0x0000;
	else
		charge_fault = 0x0001;
	if (BMS_GetErrStatusTemperatureByType(STATE_TEMP_HIGH))
		temp_high = 0x0001;
	else if (BMS_GetErrStatusTemperatureByType(STATE_TEMP_NORMAL))
		temp_high = 0x0000;
	else
		temp_high = 0x0010;
	other_fault = 0x0000;
	fault = R_fault
			& 0x0003 + ((trans_fault & 0x0003) << 2)
					+ ((BMS_falut & 0x0003) << 4)
					+ ((charge_fault & 0x0003) << 6)
					+ ((temp_high & 0x0003) << 8)
					+ ((other_fault & 0x0003) << 10);
	fault = 0xf000 | fault;
	return fault;
}

UINT8 get_stopcharge_err()
{
	UINT8 err, vol, curr;
	if (BMS_GetErrStatusCurrentByType(STATE_CURRENT_CHARGE_OC_LEVEL1))
		curr = 0x01;
	else
		curr = 0x00;

	if (BMS_GetErrStatusVoltageByType(STATE_CELL_VOL_HIGH_LEVEL2))
		vol = 0x01;
	else
		vol = 0x00;
	err = 0x03 & curr + ((0x03 & vol) << 2);
	return err;
}
/*************************************************************/
/*                       将msgid中的数据添加到msg中           */
/*************************************************************/

void SelectUnpackFun(struct T_ID id, struct T_RMSGDATA msg, void* vdata)
{
	lid = ((UINT32) id.cPF << 16) | ((UINT32) id.cPS << 8) | (id.cSA);

	switch (lid)
	{
		case 0x01D1F3:   //单体电压
			UnpackMsgid(msg, vdata);
			break;
		default:
			break;
	}
}

/*************************************************************/
//函数名称：SendCanMsg?
//函数功能：发送CAN消息
//输入参数：id，表示要发送的命令id，msgdata，要发送的命令内容
//输出参数：TURE表示数据发送成功，FALSE表示数据发送失败
/*************************************************************/
UINT8 SendCanMsg(struct T_ID id, UINT8* msgdata)   //bool
{
	struct T_SMSGDATA msg;
	lid = ((unsigned long) id.cPF << 16) | ((unsigned long) id.cPS << 8)
			| (id.cSA);
	(void) PackMsgid(&msg, id); //将id转到msg结构体中
	switch (lid)
	{
		case MSG_SINFVOLT:   //单体电压
			msg.len = 8;
			msg.prty = 6;
			//if(PackSignV(&msg,msgdata)))
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			break;
		case MSG_SINFTEMP: //单体温度
			msg.len = 8;
			msg.prty = 6;
			// if(PackSignT(&msg,*((struct T_SINGT*)msgdata)))
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			break;
		case MSG_CARCONT1: //整车监控1
			msg.len = 8;
			msg.prty = 6;
			//if(PackCarCt1(&msg,*((struct T_CarCt1*)msgdata)))
			memcpy(msg.data, msgdata, msg.len);
			//   MSCAN1SendMsg(msg);
			break;
		case MSG_CARCONT2: //整车监控2
			msg.len = 8;
			msg.prty = 6;
			//if(PackCarCt2(&msg,*((struct T_CarCt2*)msgdata)))
			memcpy(msg.data, msgdata, msg.len);
			//   MSCAN1SendMsg(msg);
			break;
		case MSG_BRM: //BMS和车辆辨识报文
			msg.len = 8;
			msg.prty = 6;
			//if(PackBMSCarIdt(&msg,*((struct T_BMSCarIdt*)msgdata))==1)
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			break;
		case MSG_BCP: //动力蓄电池充电参数
			msg.prty = 6;
			if (msgdata[0] == 0)
			{
				msg.len = 7;
			}
			else if (msgdata[0] == 1)
			{
				msg.len = 8;
			}
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			break;
		case MSG_BRO: //电池充电准备就绪状态
			msg.len = 1;
			msg.prty = 4;
			//if(PackBRO(&msg,*((struct T_BRO*)msgdata)))
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			break;
		case MSG_BCL: //电池充电需求
			msg.len = 5;
			msg.prty = 6;
			//if(PackBCL(&msg,*((struct T_BCL*)msgdata)))
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			break;
		case MSG_BCS: //电池充电总状态
			msg.len = 6;
			if (msgdata[0] == 0)
			{
				msg.len = 7;
			}
			else if (msgdata[0] == 1)
			{
				msg.len = 4;
			}
			msg.prty = 6;
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			break;
		case MSG_BSM: //动力蓄电池状态信息
			msg.len = 7;
			msg.prty = 6;
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			break;
		case MSG_BMV: //单体动力蓄电池电压
			msg.len = 8;
			msg.prty = 6;
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			break;
		case MSG_BMT: //动力蓄电池温度
			msg.len = 8;
			msg.prty = 6;
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			break;
		case MSG_BSP: //动力蓄电池预留
			msg.len = 5;
			msg.prty = 6;
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			break;
		case MSG_BST: //BMS中止充电
			msg.len = 4;
			msg.prty = 4;
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			WAIT_MSG = BMS_R_CST;
			break;
		case MSG_BSD: //BMS统计数据
			msg.len = 7;
			msg.prty = 6;
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
			break;
		case MSG_BEM:
			msg.len = 4;
			msg.prty = 2;
			memcpy(msg.data, msgdata, msg.len);
			CAN_C_TX(msg);
		default:
			break;
	}

	return TRUE;
}

/*************************************************************/
//函数名称：GetCanMsg
//函数功能：处理CAN消息
//输入参数：
//输出参数：
/*************************************************************/
void RecvCanMsg(void)
{
	struct T_ID msgid;
	struct T_CRM tCRM;
	struct T_CTS tCTS;
	struct T_CML tCML;
	struct T_CRO tCRO;
	struct T_CCS tCCS;
	struct T_CST tCST;
	struct T_CSD tCSD;

	/*  if(MSCAN0GetMsg(&msgid,(void*)msgdata)) 
	 {
	 lid = ((unsigned long)msgid.cPF<<16)|((unsigned long)msgid.cPS<<8)|(msgid.cSA);
	 switch(lid) 
	 {
	 default:
	 break;
	 }
	 } */
	if (MSCAN0GetMsg(&msgid, msgdata))
	{
		//char ** ptmp = (void**)&msgdata;
		//  UINT8 ap;
		lid = ((UINT32) msgid.cPF << 16) | ((UINT32) msgid.cPS << 8)
				| (msgid.cSA);
		switch (lid)
		{
			case MSG_CRM:   //充电机辨识报文
				// BMSCRM0_flag = 1;
				//memcpy(&tCRM,msgdata,2);
				CPCU_state = START_ON;
				tCRM.cRsult = msgdata[0];
				tCRM.cChargeNum = msgdata[1];
				MsgCrmDeal(&tCRM);
				break;

			case MSG_CTS:   //充电机发送时间同步信息
				BMSCTS_flag = 0;
				memcpy(&tCTS, msgdata, 7);
				WAIT_MSG = BMS_R_CML;
				break;

			case MSG_CML:   //充电机最大输出能力
				//memcpy(tCML,msgdata,6);
				BMSCML_flag = 0;
				tCML.iHOtV = ((0x00ff & (UINT16) msgdata[0]) << 8)
						| (UINT16) msgdata[1];
				tCML.iLOtV = ((0x00ff & (UINT16) msgdata[2]) << 8)
						| (UINT16) msgdata[3];
				tCML.iMaxOtI = ((0x00ff & (UINT16) msgdata[4]) << 8)
						| (UINT16) msgdata[5];
				//to do          
				MsgCmlDeal(&tCML);
				break;

			case MSG_CRO:   //充电机输出准备就绪状态
				CPCU_state = Get_Charge;
				BMSCRO_flag = 0;
				// ap = msgdata[0];
				tCRO.iReady = msgdata[0];
				//memcpy(&(tCRO->iReady),msgdata,1);
				MsgCroDeal(&tCRO);
				break;

			case MSG_CCS:   //充电机充电状态
				BMSCCS_flag = 0;
				tCCS.iOtV = ((0x00ff & (UINT16) msgdata[0]) << 8)
						| (UINT16) msgdata[1];
				tCCS.iOtI = ((0x00ff & (UINT16) msgdata[2]) << 8)
						| (UINT16) msgdata[3];
				tCCS.iAcTime = msgdata[4];
				MsgCcsDeal(&tCCS);
				break;

			case MSG_CST:   //充电机中止充电
				if (!Put_flag)
				{
					BMSCST_flag = 0;
				}
				else
				{
					BMSCST1_flag = 0;
				}
				//memcpy(&tCST,msgdata,4); 
				tCST.cReson = msgdata[0];
				tCST.iFault = ((0x00ff & (UINT16) msgdata[1]) << 8)
						| (UINT16) msgdata[2];
				tCST.cErr = msgdata[1];
				MsgCstDeal(&tCST);
				break;

			case MSG_CSD:  //充电机统计数据
				//memcpy(&tCSD,msgdata,4); 
				BMSCSD_flag = 0;
				tCSD.iAcTime = ((0x00ff & (UINT16) msgdata[0]) << 8)
						| (UINT16) msgdata[1];
				tCSD.iOtPower = ((0x00ff & (UINT16) msgdata[2]) << 8)
						| (UINT16) msgdata[3];
				tCSD.cChargeNum = msgdata[4];
				break;

			case MSG_CEM:

				if (recive_cem >= 100)
					recive_cem = 0;
				tCEM[recive_cem].BRM = 0x03 & msgdata[0];
				tCEM[recive_cem].BCP = 0x03 & msgdata[1];
				tCEM[recive_cem].BRO = 0x03 & (msgdata[1] >> 2);
				tCEM[recive_cem].BCS = 0x03 & msgdata[2];
				tCEM[recive_cem].BCL = 0x03 & (msgdata[2] >> 2);
				tCEM[recive_cem].BST = 0x03 & (msgdata[2] >> 4);
				tCEM[recive_cem].BSD = 0x03 & (msgdata[3]);
				recive_cem++;
				send_erro(BMS_R_CEM);
				break;
			default:
				break;
		}
	}
}

void Send_BMS()
{
	struct T_BMSCarIdt tBMSCarIdt;
	struct T_ID id;
	//int iNum = 0,i;
	UINT8 cTempData[8];
	tBMSCarIdt.cpBmsVer[0] = 0;
	tBMSCarIdt.cpBmsVer[1] = 1;
	tBMSCarIdt.cpBmsVer[2] = 0;
	tBMSCarIdt.cBatType = 3;
	//tBMSCarIdt.cBatType =  GetBatteryType();     ==+
	tBMSCarIdt.iRatingI = AWEIGHT;           // +++
	tBMSCarIdt.iRatingV = VRATING;            //  +++

	cpyid(&id, (UINT32) MSG_BRM);
	memcpy(cTempData, &tBMSCarIdt, sizeof(tBMSCarIdt));
	//发送BMS和车辆辨识报文
	SendCanMsg(id, cTempData);
//      for(i=0;i<8;i++)
//      {
//      mprintf("cTEMPDATA :%dms state:%d\r\n",id,cTempData[i]);
//      }
	WAIT_MSG = BMS_R_CRM1;
	// BMSCRM0_flag = 0;
}

void Send_BCP()
{
	struct T_ID id;
	struct T_BCP1 tBCP1;
	struct T_BCP2 tBCP2;
	//int iNum = 0,i;
	UINT8 cTempData[8];
	tBCP1.iNum = 0;
	tBCP1.iSChgV = SCHGV;      //0.1v  
	tBCP1.iChgI = CHARGI - 4000;           //400A 偏移
	//tBCP1.iChgI = GetBatteryMaxAllowedChargeCurrent();
	tBCP1.iAPower = IPOWER;         // ++ 
	tBCP2.iNum = 1;
	// tBCP2.iAChargeV = GetBatteryMaxAllowedChargeVoltage();
	tBCP2.iAChargeV = ACHATEV;
	tBCP2.cAlT = ALTEMP;                //++
	tBCP2.iElcState = BMS_GetSOCValue();
	tBCP2.iAV = GetUltimateV(2).iVolt * 1000;
	cpyid(&id, (UINT32) MSG_BCP);
	//动力蓄电池充电参数
	memcpy(cTempData, &tBCP1, sizeof(tBCP1));
	SendCanMsg(id, cTempData);
	memcpy(cTempData, &tBCP2, sizeof(tBCP2));
	SendCanMsg(id, cTempData);
	WAIT_MSG = BMS_R_CTS;

}
//充电机辨识报文处理
void MsgCrmDeal(struct T_CRM* tCRM)
{

	struct T_CRM a = *tCRM;
	//UINT16 vol[24];

	if (tCRM->cRsult == 0x00) //如果收到的报文是0x00，则发送T_BMSCarIdt，直到收到0xAA
	{
		BMSCRM0_flag = 0;
		Send_BMS();
	}
	else if ((tCRM->cRsult == 0xAA) && (g_bHreat == TRUE)) //第一次心跳成功发送 T_BCP1
	{
		BMSCRM1_flag = 0;
		CPCU_state = PA_CONFIG;
		Send_BCP();
	}
}

void Send_BRO()
{
	struct T_BRO tBRO;
	struct T_ID id;
	UINT8 cTempData[8];
	if (isDcChargerReadyOk())
	{
		tBRO.cChgRdState = GET_CHARGE_ON();
		WAIT_MSG = BMS_R_CRO;
	}
	else
		tBRO.cChgRdState = GET_CHARGE_OFF();
	cpyid(&id, (UINT32) MSG_BRO);
	memcpy(cTempData, &tBRO, sizeof(tBRO));
	//电池充电准备就绪状    
	SendCanMsg(id, cTempData);
}

//充电机最大输出能力
void MsgCmlDeal(struct T_CML* tCML)
{
	(void)tCML;
	Send_BRO();
}

void Send_BCL(void)
{
	struct T_ID id;
	struct T_BCL tBCL;
	UINT8 cTempData[8];
	tBCL.iVReq = B2P_CPCU_VoltageNeedGet();
	tBCL.iIReq = B2P_CPCU_CurrentNeedGet();
	tBCL.cChgType = B2P_CPCU_ChargeMode();

	cpyid(&id, (UINT32) MSG_BCL);
	////电池充电需求
	memcpy(cTempData, &tBCL, sizeof(tBCL));
	SendCanMsg(id, cTempData);
}

void Send_BCS(void)
{
	struct T_ID id;
	struct T_BCS1 tBCS1;
	struct T_BCS2 tBCS2;
	UINT8 cTempData[8];
	tBCS1.iNum = 0;
	tBCS1.iVMer = BMS_GetVoltageValueBatteryTotal();
	tBCS1.iIMer = BMS_GetChargeCurrentValue();
	tBCS1.iSgVNum = (INT16) (BMS_GetVolValueMaxCellId());
	cpyid(&id, (UINT32) MSG_BCS);
	memcpy(cTempData, &tBCS1, sizeof(tBCS1));
	////电池充电总状态
	SendCanMsg(id, cTempData);

	tBCS2.iNum = 1;
	tBCS2.cSOC = BMS_GetSOCValue();
	tBCS2.iRmTime = GetRestChargingTime();
	cpyid(&id, (UINT32) MSG_BCS);
	memcpy(cTempData, &tBCS2, sizeof(tBCS2));
	////电池充电总状态
	SendCanMsg(id, cTempData);
	WAIT_MSG = BMS_R_CCS;
}
//充电机输出准备就绪状态
void MsgCroDeal(struct T_CRO* tCRO)
{

	if (tCRO->iReady == 0x00)  //如果充电机没有完成充电准备，则发送BMS完成充电准备
	{
		Send_BRO();
	}
	else if (tCRO->iReady == 0xAA) //如果收到充电机完成充电准备则进入充电阶段报文
	{
		Send_BCL();
		Send_BCS();
	}
}

void Send_BSM()
{
	struct T_BSM tBSM;
	struct T_ID id;
	UINT8 cTempData[8];
	tBSM.cHSgVNum = GetUltimateV(1).iNum;
	tBSM.cHBarT = (UINT8) BMS_GetTemperaturValueMaxValue() + TEMP_PY;
	tBSM.cHTNum = (UINT8) BMS_GetTemperatureMaxValueSensorId();
	tBSM.cLBarT = (UINT8) BMS_GetTemperatureValueMinValue() + TEMP_PY;
	tBSM.cLTNum = (UINT8) BMS_GetTemperatureMinValueSensorId();
	tBSM.cBarState1 = GET_BARSTATE1();
	tBSM.cBarState2 = GET_BARSTATE2();
	cpyid(&id, (UINT32) MSG_BSM);
	memcpy(cTempData, &tBSM, sizeof(tBSM));
	SendCanMsg(id, cTempData);
}

void Send_BMV()
{
	struct T_BMV tBMV;
	struct T_ID id;
	UINT8 cTempData[8];
	tBMV.iSgV1 = (UINT16) (getVoltageValueByCellIndex(0) / 10);         //0.01 v
	tBMV.iSgV2 = (UINT16) (getVoltageValueByCellIndex(1) / 10);
	tBMV.iSgV3 = (UINT16) (getVoltageValueByCellIndex(2) / 10);
	tBMV.iSgV4 = (UINT16) (getVoltageValueByCellIndex(3) / 10);
	cpyid(&id, (UINT32) MSG_BMV);
	memcpy(cTempData, &tBMV, sizeof(tBMV));
	////单体动力蓄电池电压
	SendCanMsg(id, cTempData);
}

void Send_BMT()
{
	struct T_BMT tBMT;
	struct T_ID id;
	UINT8 cTempData[8];
	tBMT.iSgT1 = (UINT8) BMS_GetTemperatureValueById(0) + TEMP_PY;     //-50  偏移
	tBMT.iSgT2 = (UINT8) BMS_GetTemperatureValueById(1) + TEMP_PY;
	tBMT.iSgT3 = (UINT8) BMS_GetTemperatureValueById(2) + TEMP_PY;
	tBMT.iSgT4 = (UINT8) BMS_GetTemperatureValueById(3) + TEMP_PY;
	tBMT.iSgT5 = (UINT8) BMS_GetTemperatureValueById(0) + TEMP_PY;
	tBMT.iSgT6 = (UINT8) BMS_GetTemperatureValueById(1) + TEMP_PY;
	tBMT.iSgT7 = (UINT8) BMS_GetTemperatureValueById(2) + TEMP_PY;
	tBMT.iSgT8 = (UINT8) BMS_GetTemperatureValueById(3) + TEMP_PY;
	cpyid(&id, (UINT32) MSG_BMT);
	memcpy(cTempData, &tBMT, sizeof(tBMT));
	////动力蓄电池温度
	SendCanMsg(id, cTempData);
}
void Send_BEM()
{
	struct T_ID id;
	UINT8 cTempData[8];
	cpyid(&id, (UINT32) MSG_BEM);
	memcpy(cTempData, &tBEM, sizeof(tBEM));
	////动力蓄电池温度
	SendCanMsg(id, cTempData);
}
//充电机充电状态
void MsgCcsDeal()
{
	////电池状态信息报文
	Send_BSM();
	Send_BMV();
	Send_BMT();
}
//充电机中止充电

/*************************************************************/
//函数名称：SendStopCharge
//函数功能：对外接口：发送停止充电命令
//输入参数：
//输出参数：
/*************************************************************/
void Send_BST()
{
	struct T_BST tBST;
	struct T_ID id;
	UINT8 cTempData[8];
	// UINT32 a = MSG_BST;
	//tBST.cReson =  GET_SOC_STATE()&0x03 +  ((GET_CHIEFVOLT_STATE()&0x03)<<2) +  ((GET_SIGNVOLT_STATE()&0x03)<<4);
	tBST.cReson = get_creson();
	tBST.iFault = get_stopcharge_fault();
	tBST.cErr = get_stopcharge_err();
	cpyid(&id, (UINT32) MSG_BST);
	memcpy(cTempData, &tBST, sizeof(tBST));
	SendCanMsg(id, cTempData);
}

void Charge_check()
{
	UINT32 Voltage_min, Voltage_total;
	Voltage_min = BMS_GetVoltageValueMin() / 10 + 100;
	Voltage_total = BMS_GetVoltageValueBatteryTotal() / 100 + 10;
	if ((Voltage_min >= SCHGV) | (Voltage_total >= ACHATEV))
	{
		Send_BST();
		Put_flag = 1;
	}
}

void Send_BSD()
{
	struct T_BSD tBSD;
	struct T_ID id;
	UINT8 cTempData[8];
	tBSD.cSOC = BMS_GetSOCValue();
	tBSD.iLSgV = GetUltimateV(0).iVolt;
	tBSD.iHSgV = GetUltimateV(1).iVolt;
	tBSD.cLSgT = GetUltimateT(0).iTemp;
	tBSD.cHSgT = GetUltimateT(1).iTemp;
	cpyid(&id, (UINT32) MSG_BSD);
	memcpy(cTempData, &tBSD, sizeof(tBSD));
	////BMS统计数据
	SendCanMsg(id, cTempData);
	WAIT_MSG = BMS_R_CSD;
}

void MsgCstDeal(struct T_CST* tCST)
{
	(void) tCST;
	if (!Put_flag)
	{
		Send_BST();
		WAIT_MSG = BMS_R_CST1;
		//BMSCST1_flag = 1;
	}
	else
	{

		Send_BSD();
		WAIT_MSG = BMS_R_CSD;
	}
}

//获取电压的极限值，0表示最小值，1表示最大值，2表示总和
struct T_VINFOR GetUltimateV(int iType)
{
	struct T_VINFOR tVlotInfor;
	static UINT16 vol[BARALLNUM];
	UINT8 i, iNum = 0;
	UINT16 iVMin = 0, iVMax = 0, iChiefV = 0;
	UINT8 iVMinNum, iVMaxNum;
	// get_voltage(vol);
	for (i = 0; i < BARALLNUM; i++)
	{
		vol[i] = (UINT16)getVoltageValueByCellIndex(i);
	}
	for (iNum = 0; iNum < BARALLNUM; iNum++)
	{
		if (iVMin > vol[iNum])
		{
			iVMin = vol[iNum];
			iVMinNum = iNum;
		}
		if (iVMax < vol[iNum])
		{
			iVMax = vol[iNum];
			iVMaxNum = iNum;
		}
		iChiefV += vol[iNum];
	}
	if (iType == 0)
	{
		tVlotInfor.iVolt = iVMin;
		tVlotInfor.iNum = iVMinNum;
	}
	else if (iType == 1)
	{
		tVlotInfor.iVolt = iVMax;
		tVlotInfor.iNum = iVMaxNum;
	}
	else
	{
		tVlotInfor.iVolt = iChiefV;
		tVlotInfor.iNum = 0;
	}
	return tVlotInfor;
}

//获取温度的极限值，0表示最小值，1表示最大值
struct T_TINFOR GetUltimateT(int iType)
{
	struct T_TINFOR tTempInfor;
	UINT8 temp[TEMPALLNUM];
	UINT8 iTMin = 0, iTMax = 0;
	UINT8 iTMinNum, iTMaxNum;
	UINT8 i, iNum = 0;
	// get_temperature(temp);
	for (i = 0; i < TEMPALLNUM; i++)
	{
		temp[i] = (UINT8) BMS_GetTemperatureValueById(i);
	}
	for (iNum = 0; iNum < TEMPALLNUM; iNum++)
	{
		if (iTMin > temp[iNum])
		{
			iTMin = temp[iNum];
			iTMinNum = iNum;
		}
		if (iTMax < temp[iNum])
		{
			iTMax = temp[iNum];
			iTMaxNum = iNum;
		}
	}
	if (iType == 0)
	{
		tTempInfor.iTemp = iTMin;
		tTempInfor.iNum = iTMinNum;
	}
	else if (iType == 1)
	{
		tTempInfor.iTemp = iTMax;
		tTempInfor.iNum = iTMaxNum;
	}
	return tTempInfor;
}

void cpyid(struct T_ID* id, UINT32 MsgId)
{
	id->cPF = (UINT8) (MsgId >> 16);
	id->cPS = (UINT8) (MsgId >> 8);
	id->cSA = (UINT8) (0x000000ff & MsgId);
	// id->ptr = (UINT8)(MsgId>>24);
}

void DeserveCEM()
{
	if (deserve_cem >= 100)
		deserve_cem = 0;
	if (tCEM[deserve_cem].BRM == 0x01)
		Send_BMS();
	else if (tCEM[deserve_cem].BCP == 0x01)
		Send_BMS();
	else if (tCEM[deserve_cem].BRO == 0x01)
		Send_BMS();
	else if (tCEM[deserve_cem].BCL == 0x01)
		alarmtype = bcl_overtime;
	else if (tCEM[deserve_cem].BCS == 0x01)
		Send_BMS();
	else if (tCEM[deserve_cem].BST == 0x01)
		Send_BMS();
	deserve_cem++;
}
void send_erro(UINT8 type)
{

	//struct TCEM tCEM;
	//ERRO_INFO = type;
	//while(1);
	switch (type)
	{
		case BMS_R_CRM0_OVERTIME:
			tBEM.CRM1 = 0x01;
			Send_BEM();
			alarmtype = connect_overtime;
			// alarm(2);
			break;
		case BMS_R_CRM1_OVERTIME:
			tBEM.CRM2 = 0x01;
			Send_BEM();
			alarmtype = connect_overtime;
			// alarm(2);
			break;
		case BMS_R_CTS_OVERTIME:
			tBEM.CTS_CML = 0x01;
			Send_BEM();
			Send_BMS();
			break;
		case BMS_R_CML_OVERTIME:
			tBEM.CTS_CML = 0x01;
			Send_BEM();
			Send_BMS();
			break;
		case BMS_R_CRO_OVERTIME:
			tBEM.CRO = 0x01;
			Send_BEM();
			Send_BMS();
			break;
		case BMS_R_CCS_OVERTIME:
			tBEM.CCS = 0x01;
			alarmtype = ccs_overtime;
			Send_BEM();
			// Send_BST();
			break;
		case BMS_R_CST_OVERTIME:
			tBEM.CST = 0x01;
			Send_BEM();
			Send_BMS();
			break;
		case BMS_R_CSD_OVERTIME:
			tBEM.CSD = 0x01;
			Send_BEM();
			Send_BMS();
			break;
		case BMS_R_CEM:
			DeserveCEM();
			break;
	}

}

enum flag_check
{
	BMS_R_CRM0_flag = 0,
	BMS_R_CRM1_flag,
	BMS_R_CTS_flag,
	BMS_R_CML_flag,
	BMS_R_CRO_flag,
	BMS_R_CCS_flag,
	BMS_R_CST_flag,
	BMS_R_CST1_flag,
	BMS_R_CSD_flag
};

static type_isr_pit Outtimer_check()
{

	switch (flag)
	{
		case BMS_R_CRM0_flag:
			BMSCRM0_waitflag = 1;
			if (!BMSCRM0_flag)
			{
				//BMSCRM0_waitflag = 1;
				break;
			}
			else
			{
				BMSCRM0_flag++;
				if (BMSCRM0_flag >= over_time)
				{
					BMSCRM0_flag = 1;
					send_erro(BMS_R_CRM0_OVERTIME);
				}

				break;
			}
		case BMS_R_CRM1_flag:
			BMSCRM1_waitflag = 1;
			if (!BMSCRM1_flag)
			{
				BMSCRM1_flag = 1;
				break;
			}
			else
			{
				BMSCRM1_flag++;
				if (BMSCRM1_flag >= over_time)
				{
					BMSCRM1_flag = 1;
					send_erro(BMS_R_CRM1_OVERTIME);
				}
				else
				{
					Send_BMS();
				}
				break;
			}

		case BMS_R_CTS_flag:
			BMSCTS_waitflag = 1;
			if (!BMSCTS_flag)
			{
				BMSCTS_flag = 1;
				break;
			}
			else
			{
				BMSCRM1_flag++;
				if (BMSCRM1_flag >= over_time)
				{
					BMSCRM1_flag = 1;
					send_erro(BMS_R_CTS_OVERTIME);
				}
				else
				{
					Send_BCP();
				}
				break;
			}
		case BMS_R_CML_flag:
			BMSCML_waitflag = 1;
			if (!BMSCML_flag)
			{
				BMSCML_flag = 1;
				break;
			}
			else
			{
				BMSCML_flag = 1;
				send_erro(BMS_R_CML_OVERTIME);
				break;
			}
		case BMS_R_CRO_flag:
			BMSCRO_waitflag = 1;
			if (!BMSCRO_flag)
			{
				BMSCRO_flag = 1;
				break;
			}
			else
			{
				BMSCRO_flag++;
				if (BMSCRO_flag >= over_time)
				{
					BMSCRO_flag = 1;
					send_erro(BMS_R_CRO_OVERTIME);
				}
				else
				{
					Send_BRO();
				}
				break;
			}
		case BMS_R_CCS_flag:
			BMSCCS_waitflag = 1;
			if (!BMSCCS_flag)
			{
				BMSCCS_flag = 1;
				break;
			}
			else
			{
				BMSCCS_flag = 1;
				send_erro(BMS_R_CCS_OVERTIME);
				break;
			}
		case BMS_R_CST_flag:
			BMSCST_waitflag = 1;
			if (!BMSCST_flag)
			{
				BMSCSD_flag = 1;
				break;
			}
			else
			{
				MsgCcsDeal();
				break;
			}
		case BMS_R_CST1_flag:
			BMSCST1_waitflag = 1;
			if (!BMSCST1_flag)
			{
				BMSCSD_flag = 1;
				break;
			}
			else
			{
				//BMSCST1_flag = 1;
				send_erro(BMS_R_CST_OVERTIME);
				break;
			}
		case BMS_R_CSD_flag:
			BMSCSD_waitflag = 1;
			if (!BMSCSD_flag)
			{
				BMSCSD_flag = 1;
				break;
			}
			else
			{
				BMSCSD_flag++;
				if (BMSCSD_flag >= over_time)
				{
					BMSCSD_flag = 1;
					send_erro(BMS_R_CSD_OVERTIME);
				}
				else
				{
					Send_BSD();
				}
				break;
			}
	}
}

int DCCP_communication(void)
{
	//UINT8 flag;
	//INT8 CPCU_state = 0;
	//UINT8 lastTime_flag = 1;	
	//RecvCanMsg();	
	//Send_BCP();
	switch (WAIT_MSG)
	{
		case BMS_R_CRM0:
			flag = BMS_R_CRM0_flag;
			// timer_add(0, Outtimer_check(), 250);
			break;

		case BMS_R_CRM1:
			flag = BMS_R_CRM1_flag;
			if (BMSCRM1_waitflag)
			{
				BMSCRM1_waitflag = 0;
				timer_add(TIMER_ID_CPCU_MSG, Outtimer_check(), 250);
			}
			break;

		case BMS_R_CTS:
			flag = BMS_R_CTS_flag;
			if (BMSCTS_waitflag)
			{
				BMSCTS_waitflag = 0;
				timer_add(TIMER_ID_CPCU_MSG, Outtimer_check(), 500);
			}
			break;
		case BMS_R_CML:
			flag = BMS_R_CML_flag;
			if (BMSCML_waitflag)
			{
				BMSCML_waitflag = 0;
				timer_add(TIMER_ID_CPCU_MSG, Outtimer_check(), 250);
			}
			break;
		case BMS_R_CRO:
			flag = BMS_R_CRO_flag;
			if (BMSCRO_waitflag)
			{
				BMSCRO_waitflag = 0;
				timer_add(TIMER_ID_CPCU_MSG, Outtimer_check(), 250);
			}
			break;
		case BMS_R_CCS:
			flag = BMS_R_CCS_flag;
			if (BMSCCS_waitflag)
			{
				BMSCCS_waitflag = 0;
				timer_add(TIMER_ID_CPCU_MSG, Outtimer_check(), 100);
			}
			break;
		case BMS_R_CST:
			flag = BMS_R_CST_flag;
			if (BMSCST_waitflag)
			{
				BMSCST_waitflag = 0;
				timer_add(TIMER_ID_CPCU_MSG, Outtimer_check(), 250);
			}
			break;
		case BMS_R_CST1:
			flag = BMS_R_CST1_flag;
			if (BMSCST1_waitflag)
			{
				BMSCST1_waitflag = 0;
				timer_add(TIMER_ID_CPCU_MSG, Outtimer_check(), 250);
			}
			break;
		case BMS_R_CSD:
			flag = BMS_R_CSD_flag;
			if (BMSCSD_waitflag)
			{
				BMSCSD_waitflag = 0;
				timer_add(TIMER_ID_CPCU_MSG, Outtimer_check(), 250);
			}
			break;
	}
	return 0;
}

UINT8 charge_type_check(void)
{
	return alarmtype;
}
static const UINT32 c_message_id[] =
{
	MSG_CRM,
	MSG_CTS,
	MSG_CML,
	MSG_CRO,
	MSG_CCS,
	MSG_CST,
	MSG_CSD,
	MSG_CEM };
void test()
{
	UINT8 i;
	static struct T_ID t_id[8];
	static struct T_SMSGDATA msg[8];
	for (i = 0; i < 8; i++)
	{
		cpyid(&t_id[i], c_message_id[i]);
		//lid = ((unsigned long)id.cPF<<16)|((unsigned long)id.cPS<<8)|(id.cSA);
		(void) PackMsgid(&msg[i], t_id[i]); //将id转到msg结构体中

	}
}
