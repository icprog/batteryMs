#ifndef _RELAY_H_
#define _RELAY_H_
#include "public/public.h"
//#include "Gpio/gpio.h"
#include "voltage/voltage.h"
//#include "adc/adc.h"

//#define GetLoadVoltage() (getMvByAdc(ID_ADC_PORT_PRECHARGE)*kprechargeAdcVToHighV/1000)
#define GetLoadVoltage() ((UINT16)(getMvByAdc(ID_ADC_PORT_PRECHARGE)*kprechargeAdcVToHighV))

enum ERR_RELAY_OP
{
	ERR_RELAY_NONE,
	ERR_RELAY_SHUTDOWN,
	ERR_RELAY_OPEN
};

enum RELAY_MANAGER_ID
{
	RELAY_ID_PRECHARGE,
	RELAY_ID_DISCHARGE_N,
	RELAY_ID_DISCHARGE_P,
	RELAY_ID_CHARGE
};

enum ERR_STATE_RELAY_OP
{
	ERR_NEGATIVE_OPEN,
	ERR_NEGATIVE_SHUTDOWN,
	ERR_POSITIVE_OPEN,
	ERR_POSITIVE_SHUTDOWN,
	ERR_PRECHARGE_OPEN,
	ERR_PRECHARGE_SHUTDOWN,
	ERR_CHARGE_OPEN,
	ERR_CHARGE_SHUTDOWN
};

#define RELAY_PORT_2_IOID(port) ()

/*
 * needed by vcu_protocol.h
 * */
int BMS_GetErrStatusRelayByType( UINT8 types); //1:err, 0 OK
int BMS_GetRelayStatePositiveFeedbackConnnect(void); // 1:on, 0:off
int BMS_GetRelayStateNegtiveFeedbackConnnect(void);  // 1:on, 0:off
int BMS_isHighRelayErr(void); //1:has err, 0:no err
int setRelayErrStatus( UINT8 err );
int resetRelayErrStatus(UINT8 err);
/*
 * needed by main.c
 * */
int observerRelayStatusUpdate(void);





#endif



