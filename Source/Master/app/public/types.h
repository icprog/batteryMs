#ifndef _TYPES_H_
#define _TYPES_H_

typedef signed char INT8;
typedef unsigned char UINT8;
typedef signed short INT16;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef signed int INT32;
typedef unsigned long long UINT64;
typedef signed long long INT64;
typedef unsigned char UCHAR;
typedef char byte;
typedef volatile unsigned long long VUINT64;
typedef volatile unsigned int VUINT32;
typedef INT16 CURRENT_TYPE;
typedef INT16 TYPE_TEMP;
typedef INT16 TYPE_CURRENT;
typedef UINT32 TYPE_VOLTAGE;
typedef unsigned short TYPE_INSRES;
typedef unsigned char TYPE_SOC;
typedef double DF64;

enum bits
{
	BIT0 = 0x01,
	BIT1 = 0x02,
	BIT2 = 0x04,
	BIT3 = 0x08,
	BIT4 = 0x10,
	BIT5 = 0x20,
	BIT6 = 0x40,
	BIT7 = 0x80
};

enum Bool
{
	TRUE = 1, FALSE = 0
};

enum enbales
{
	DISABLE = 0,
	ENABLE = 1
};

enum UCT_ERR_CODE
{
	UCT_SUCCESS = 0, UCT_ERR_OPT_FAIL, UCT_ERR_INVALID_PARAMETER
};

enum SYSTEM_CHARGER_STATE
{
	SYSTEM_CHARGER_INSERT, SYSTEM_CHARGER_OUT
};

enum SYSTEM_FAULT_LEVER
{
	SYSTEM_FAULT_NONE, SYSTEM_FAULT_LEVEL1, SYSTEM_FAULT_LEVEL2
};

enum LED_STATE
{
	TURN_OFF, TURN_ON
};

enum TYPE_CHECK
{
	CHECK_PASS = 0,  CHECK_NG= 1
};

enum TYPE_DEVICE_CONNECT
{
	DEV_DISCONNECT = 0, DEV_CONNECTED = 1
};

enum TYPE_REALY_SW
{
	RELAY_OFF = 0, RELAY_ON = 1
};

enum TYPE_ERRO
{
	CHECK_ERR_NONE = 0, CHECK_ERR = 1
};

enum SYSTEM_ACTIVE_MODE
{
	SYSTEM_ACTIVE_MODE_NONE,
	SYSTEM_ACTIVE_MODE_KEY_TO_ACC,
	SYSTEM_ACTIVE_MODE_ACCHARGER_IN,
	SYSTEM_ACTIVE_MODE_DCCHARGER_IN,
	SYSTEM_ACTIVE_MODE_UNKOWN
};

enum SYSTEM_CMD_POWER
{
	CMD_POWER_ON, CMD_POWER_OFF
};

enum CHECK_ACChargePWM
{
	RECEIVE_SUCCESS, RECEIVE_FAILED
};
enum VEH_CHARGER_COURSE
{
	STATE_VEH_CHARGER_OVER, STATE_VEH_CHARGER_CONTINUE
};
enum ACCHARGE_PRECHARGE_STATE
{
	STATE_PRECHARGER_OVER, STATE_PRECHARGER_CONTINUE
};
#endif
