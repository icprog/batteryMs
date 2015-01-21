#include "ctl/relay.h"
#include "public/public.h"
#include "voltage/voltage.h"
#include "driver/Gpio/gpioFuncs.h"
#include "driver/Gpio/port_defs.h"
#include "driver/timer/timer.h"



/*
 * relay op control and err check
 * */
struct s_errstate
{
	UINT8 relayErrstatus;
};

static struct relayErrManager
{
	UINT8 errStart;
	UINT32 errStartTimeMs;
};

struct relayStatus
{
	struct s_errstate errs;
	struct relayErrManager relayTimers;
	union
	{
		UINT8 ioByte;
		struct
		{
			UINT8 Control :1;
			UINT8 Feedback :1;
			UINT8 reserved :6;
		} ioBits;
	} IoStatus;
};

static struct s_errstate gRelayErrStatus =
{ 0 };
static const UINT32 timeThresholdRealyOpTime = 1000;
static struct relayStatus hightVolRelays[4];

static int isHighVolRelayOpOk( int lookforStatus, int realStatus)
{
	int ret = 0;
	if( lookforStatus == HIGH_VOL_RELAY_OP_OPEN )
	{
		if( realStatus == HIGH_VOL_RELAY_FEEDBACK_LEVEL_CONNECT )
		ret = 1;
	}
	else if( lookforStatus == HIGH_VOL_RELAY_OP_SHUTDOWN )
	{
		if( realStatus == HIGH_VOL_RELAY_FEEDBACK_LEVEL_DISCONNECT )
		ret = 1;
	}
	return ret;
}
static int relayErrMonitor(void)
{
	UINT8 i = 0;
	UINT32 timeNow = getSystemTimeMs();
	for (i = 0; i < 4; i++)
	{
		if (isHighVolRelayOpOk(hightVolRelays[i].IoStatus.ioBits.Control,hightVolRelays[i].IoStatus.ioBits.Feedback))
		{
			if (hightVolRelays[i].relayTimers.errStart == 1)
			{
				if (hightVolRelays[i].errs.relayErrstatus == 0)
				{
					if (TIME_TRHESHOLD_CAL(
							hightVolRelays[i].relayTimers.errStartTimeMs,
							timeNow) > timeThresholdRealyOpTime)
					{
						if (hightVolRelays[i].IoStatus.ioBits.Control == 0)
						{
							hightVolRelays[i].errs.relayErrstatus =
									ERR_RELAY_SHUTDOWN;
						}
						else
						{
							hightVolRelays[i].errs.relayErrstatus =
									ERR_RELAY_OPEN;
						}
					}
				}
			}
			else
			{
				hightVolRelays[i].relayTimers.errStart = 1;
				hightVolRelays[i].relayTimers.errStartTimeMs =
						getSystemTimeMs();
			}
		}
		else
		{
			hightVolRelays[i].relayTimers.errStart = 0;
			hightVolRelays[i].errs.relayErrstatus = ERR_RELAY_NONE;
		}
	}
}
/*
 IO_CTL_PRECHARGE = 126,
 IO_CTL_DISCHARGE_NEGATIVE = 125,
 IO_CTL_DISCHARGE_POSITIVE = 124,
 IO_CTL_CHARGE = 123,

 IO_STATUS_PRECHARGE = 117,
 IO_STATUS_CHARGE = 116,
 IO_STATUS_DISCHARGE_POSITIVE = 115,
 IO_STATUS_DISCHARGE_NEGATIVE = 114,
 */


int observerRelayStatusUpdate(void)
{
	UINT8 i = 0;
	for (i = 0; i < sizeof(hightVolRelays) / sizeof(struct relayStatus); i++)
	{
		hightVolRelays[i].IoStatus.ioBits.Control = BMS_IO_GET(
				ID_IO_CTL_PRECHARGE + i);
		hightVolRelays[i].IoStatus.ioBits.Feedback = BMS_IO_GET(
				ID_IO_STATUS_PRECHARGE + i);
	}

	if( HIGH_VOL_RELAY_OP_OPEN  == HIGH_VOL_RELAY_FEEDBACK_LEVEL_CONNECT ){
		hightVolRelays[RELAY_ID_PRECHARGE].IoStatus.ioBits.Feedback =  hightVolRelays[RELAY_ID_PRECHARGE].IoStatus.ioBits.Control;
		hightVolRelays[RELAY_ID_CHARGE].IoStatus.ioBits.Feedback = hightVolRelays[RELAY_ID_CHARGE].IoStatus.ioBits.Control;
	}
	else {
		hightVolRelays[RELAY_ID_PRECHARGE].IoStatus.ioBits.Feedback = 1 -hightVolRelays[RELAY_ID_PRECHARGE].IoStatus.ioBits.Control;
		hightVolRelays[RELAY_ID_CHARGE].IoStatus.ioBits.Feedback = 1 -hightVolRelays[RELAY_ID_CHARGE].IoStatus.ioBits.Control;
	}
	relayErrMonitor();
	return 0;
}

static UINT8 getRelayErrById(UINT8 id)
{
	return hightVolRelays[id].errs.relayErrstatus;
}

int BMS_GetRelayStatePositiveFeedbackConnnect(void) // 1:on, 0:off
{
	return gpio_read(IO_STATUS_DISCHARGE_POSITIVE);
}

int BMS_GetRelayStateNegtiveFeedbackConnnect(void)  // 1:on, 0:off
{
	return gpio_read(IO_STATUS_DISCHARGE_NEGATIVE);
}

int setRelayErrStatus(UINT8 err)
{
//	SET_BIT( gRelayErrStatus.relayErrstatus, err, 1);
	gRelayErrStatus.relayErrstatus = (UINT8)mgcSetBit( gRelayErrStatus.relayErrstatus, err);
	return 0;
}

int resetRelayErrStatus(UINT8 err)
{
//	SET_BIT( gRelayErrStatus.relayErrstatus, err, 0);
	gRelayErrStatus.relayErrstatus = (UINT8)mgcResetBit( gRelayErrStatus.relayErrstatus, err);
	return 0;
}

int BMS_isHighRelayErr(void)
{
	UINT8 i = 0;
	for (i = 0; i < 4; i++)
	{
		if (hightVolRelays[i].errs.relayErrstatus != ERR_RELAY_NONE)
		{
			return 1;
		}

	}
	return 0;
}

int BMS_GetErrStatusRelayByType(UINT8 types)
{
	int ret = 0;
	switch (types)
	{
	case ERR_NEGATIVE_OPEN:
		if (getRelayErrById(RELAY_ID_DISCHARGE_N) == ERR_RELAY_OPEN)
			ret = 1;
		break;
	case ERR_NEGATIVE_SHUTDOWN:
		if (getRelayErrById(RELAY_ID_DISCHARGE_N) == ERR_RELAY_SHUTDOWN)
			ret = 1;
		break;
	case ERR_POSITIVE_OPEN:
		if (getRelayErrById(RELAY_ID_DISCHARGE_P) == ERR_RELAY_OPEN)
			ret = 1;
		break;
	case ERR_POSITIVE_SHUTDOWN:
		if (getRelayErrById(RELAY_ID_DISCHARGE_P) == ERR_RELAY_SHUTDOWN)
			ret = 1;
		break;
	case ERR_PRECHARGE_OPEN:
		if (getRelayErrById(RELAY_ID_PRECHARGE) == ERR_RELAY_OPEN)
			ret = 1;
	case ERR_PRECHARGE_SHUTDOWN:
		break;
		if (getRelayErrById(RELAY_ID_PRECHARGE) == ERR_RELAY_SHUTDOWN)
			ret = 1;
		break;
	case ERR_CHARGE_OPEN:
		if (getRelayErrById(RELAY_ID_CHARGE) == ERR_RELAY_OPEN)
			ret = 1;
		break;
	case ERR_CHARGE_SHUTDOWN:
		if (getRelayErrById(RELAY_ID_CHARGE) == ERR_RELAY_SHUTDOWN)
			ret = 1;
		break;

	default:
		break;
	}
	return ret;
//	(void)types;
//	if( types&gRelayErrStatus.relayErrstatus == 0)
//	{
//		return 0;
//	}
//	else
//		return 1;
//	switch( types )
//	{
//		case ERR_NEGATIVE_OPEN:
//			break;
//		case ERR_NEGATIVE_SHUTDOWN:
//			break;
//		case ERR_POSITIVE_OPEN:
//			break;
//		case ERR_POSITIVE_SHUTDOWN:
//			break;
//		case ERR_PRECHARGE_OPEN:
//			break;
//		case ERR_PRECHARGE_SHUTDOWN:
//			break;
//		case ERR_CHARGE_OPEN:
//			break;
//		case ERR_CHARGE_SHUTDOWN:
//			break;
//		default:
//			break;
//	}
//	return 0;//TODO
}  //1:err, 0 OK

