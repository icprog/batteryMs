/*
 * accp_protocol.c
 *
 *  Created on: Nov 11, 2014
 *      Author: mgc
 */

#include "protocol/accp_protocol.h"
#include "public/types.h"
#include "public/public.h"
#include "public/drivers.h"
#include "protocol/vcu_protocol.h"
#include "ctl/charge.h"
//#include "timer/timer.h"

#define waittime    5000
enum ACCP_CAN_ID
{
	ACCP_CAN_ID_P0 = 0x1840FFE5,
	ACCP_CAN_ID_P1 = 0x1830E5F4,
	ACCP_CAN_ID_P2 = 0x1820E5F4,
	ACCP_CAN_ID_P3 = 0x1810E5F4
};

static UINT32 lastTimems = 0;
static UINT8 Accpsendbuff[8];
static UINT8 Accprecivebuff[8];
static UINT32 first_time = 0;
struct MSG1_T
{
	UINT16 vol;
	TYPE_CURRENT cur;
	UINT8 control;

};

static int AccpCanSendP1(void)
{
//	struct MSG1_T msg1;
//	msg1.vol = ACHATEV;
//	msg1.cur = CHARGI;
//	msg1.control  =  0x80;
	UINT8 i;
	mprintf("sending accp can p1!\r\n");
	Accpsendbuff[0] = (UINT8) (0x00ff & ACHATEV);
	Accpsendbuff[1] = (UINT8) (ACHATEV >> 8);
	Accpsendbuff[2] = (UINT8) (0x00ff & VEH_PRESENT_CHARGER_Current);
	Accpsendbuff[3] = (UINT8) (VEH_PRESENT_CHARGER_Current >> 8);
	Accpsendbuff[4] = ((0x01 & VEH_CHARGER_POWER_STATE) << 7);
	for (i = 5; i < 8; i++)
		Accpsendbuff[i] = 0;

	return 0;
}
static int AccpCanSendP2(void)
{
	UINT16 vol;
	vol = (UINT16) (0x0000ffff & (VALUE_TH_OV_SUM_LEVEL1 / 100));
	Accpsendbuff[0] = (UINT8) (0x00ff & vol);
	Accpsendbuff[1] = (UINT8) (vol >> 8);
	vol = (UINT16) (0x0000ffff & (VALUE_TH_UV_SUM_LEVEL1 / 100));
	Accpsendbuff[2] = (UINT8) (0x00ff & vol);
	Accpsendbuff[3] = (UINT8) (vol >> 8);
	vol = (UINT16) (0x0000ffff & (VALUE_TH_OV_SUM_LEVEL2 / 100));
	Accpsendbuff[4] = (UINT8) (0x00ff & vol);
	Accpsendbuff[5] = (UINT8) (vol >> 8);
	vol = (UINT16) (0x0000ffff & (VALUE_TH_UV_SUM_LEVEL2 / 100));
	Accpsendbuff[6] = (UINT8) (0x00ff & vol);
	Accpsendbuff[7] = (UINT8) (vol >> 8);
	mprintf("sending accp can p2!\r\n");
	return 0;
}

static int AccpCanSendP3(void)
{
	UINT16 cur, i;
	cur = THREATHOLD_VALUE_CAHRGE_OC_LEVEL1 * 10;
	Accpsendbuff[0] = (UINT8) (0x00ff & cur);
	Accpsendbuff[1] = (UINT8) (cur >> 8);
	cur = THREATHOLD_VALUE_CAHRGE_OC_LEVEL2 * 10;
	Accpsendbuff[2] = (UINT8) (0x00ff & cur);
	Accpsendbuff[3] = (UINT8) (cur >> 8);
	for (i = 4; i < 8; i++)
	{
		Accpsendbuff[i] = 0;
	}
	mprintf("sending accp can p3!\r\n");
	return 0;
}

static int AccpCanRxIsr(struct canPack *ppack)
{
	UINT8 i;
	switch (ppack->id)
	{
		case ACCP_CAN_ID_P0:
			for (i = 0; i < 8; i++)
			{
				Accprecivebuff[i] = ppack->data[i];
			}
			ACCP_STATE.outputvol = Accprecivebuff[1];
			ACCP_STATE.outputvol = ((ACCP_STATE.outputvol << 8)
					| (UINT16) Accprecivebuff[0]);
			ACCP_STATE.outputcur = Accprecivebuff[3];
			ACCP_STATE.outputcur = ((ACCP_STATE.outputcur << 8)
					| (UINT16) Accprecivebuff[2]);
			ACCP_STATE.accpstate = ((0x30 && Accprecivebuff[4]) >> 4);
			ACCP_STATE.canconnect = ((0x08 & Accprecivebuff[4]) >> 3);
			ACCP_STATE.inputvolprotect = ((0x04 & Accprecivebuff[4]) >> 2);
			ACCP_STATE.tempprotect = ((0x02 & Accprecivebuff[4]) >> 1);
			ACCP_STATE.hardwareerro = (0x01 & Accprecivebuff[4]);
			ACCP_STATE.Directovervolprotect = ((0x80 & Accprecivebuff[5]) >> 7);
			ACCP_STATE.Directundervolprotect =
					((0x40 & Accprecivebuff[5]) >> 6);
			ACCP_STATE.Directovercurprotect = ((0x20 & Accprecivebuff[5]) >> 5);
			ACCP_STATE.Directovervolwarn = ((0x10 & Accprecivebuff[5]) >> 4);
			ACCP_STATE.Directundervolwarn = ((0x08 & Accprecivebuff[5]) >> 3);
			ACCP_STATE.Directovercurwarn = ((0x04 & Accprecivebuff[5]) >> 2);
			ACCP_STATE.Directshort = ((0x02 & Accprecivebuff[5]) >> 1);
			ACCP_STATE.Directreverse = (0x01 & Accprecivebuff[5]);
			ACCP_STATE.Overvoldataerro = ((0x80 & Accprecivebuff[6]) >> 7);
			ACCP_STATE.Overcurdataerro = ((0x40 & Accprecivebuff[6]) >> 6);
			ACCP_STATE.Undervoldataerro = ((0x40 & Accprecivebuff[6]) >> 5);
//		setRelayControl(ppack->data[0]);
			break;
	}
	return 0;
}

enum accp_msg_erro
{
	accpnatural = 0, waitccsmsg = 1
};

static void overtime_receive()
{
	ACCP_COMMUNICATION_SATE_FLAG == waitccsmsg;
}
static int wait_receive_msg()
{
	if (first_time == 1)
	{
		timer_add(TIMER_ID_CPCU_MSG, overtime_receive, waittime);
	}
	return 0;

}

int ACCP_communication(void)
{

	UINT32 timeNow = getSystemTimeMs();
	static int i = 0;
	if (VEH_CHARGER_POWER_STATE)
	{
		first_time++;
		wait_receive_msg();
	}
	/*means every package send 3*333 ms.*/
	if (TIME_TRHESHOLD_CAL(timeNow, lastTimems) > 333)
	{
		lastTimems = timeNow;
		i++;
		if (i == 1)
		{
			AccpCanSendP1();
		}
		else if (i == 2)
		{
			AccpCanSendP2();
		}
		else
		{
			AccpCanSendP3();
			i = 0;
		}
	}
	return 0;
}
