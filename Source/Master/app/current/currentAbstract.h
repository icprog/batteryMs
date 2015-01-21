#ifndef _CURRENT_ABSTRACT_H_
#define _CURRENT_ABSTRACT_H_
/*
 * current module
 * by mgc 20141105
 * */

#include "public/types.h"
//#include "adc/adc.h"
#include "public/systemState.h"
//#include "Gpio/port_Defs.h"s
#include "public/public.h"


/*
 * types
 * */

enum type_current_state
{
	STATE_CURRENT_NORMAL,
	STATE_CURRENT_DISCHARGE_OC_LEVEL1,
	STATE_CURRENT_DISCHARGE_OC_LEVEL2,
	STATE_CURRENT_CHARGE_OC_LEVEL1,
	STATE_CURRENT_CHARGE_OC_LEVEL2,
	STATE_CURRENT_FEEDBACK_OC_LEVEL1,
	STATE_CURRENT_FEEDBACK_OC_LEVEL2,
	STATE_CURRENT_TOTAL
};

enum current_id
{
    CURRENT_ID_DISCHARGE = 0,
    CURRENT_ID_CHARGE,
    CURRENT_ID_BUS,
    CURRENT_ID_FEEDBACK,
    TOTAL_CURRENTS
};

enum current_direction
{
	CURRENT_DIR_N = 0,
	CURRENT_DIR_P = 1
};

enum current_adchannel
{
  ADC_DISCHARGE_CURRENT = 0,
  ADC_CHARGE_CURRENT ,
  TOTAL_CURRENT_ADCS
};

/*
 * macros
 * */
//TODO
#define ADC_CHANNEL_CHARGE ADC_PORT_CURRENT_CHARGE
#define ADC_CHANNEL_DISCHARGE ADC_PORT_CURRENT_DISCHARGE




/* 
 * external fuction interfaces
 * */
#define CURRENT_ADC_READ(i) getMvByAdc(i)// Adc_ReadResultMvByChannel(i)
//extern int GetSystemState(void);


/*private interfaces*/
//TYPE_CURRENT getCurrentById(int id);
TYPE_CURRENT BMS_GetCurrentValueFeedback(void);
TYPE_CURRENT BMS_GetChargeCurrentValue(void);
TYPE_CURRENT BMS_GetCurrentValueBUS(void);
TYPE_CURRENT BMS_GetDischargeCurrentValue(void);

int observerCurrentUpdate(void); // called by adc update
int BMS_GetErrStatusCurrentByType( UINT8 types );
int getCurrentDirection(void);
int testCurrents(void);

extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableFeedback(void);  //TODO
TYPE_CURRENT set_simulator_current( TYPE_CURRENT charge_current, TYPE_CURRENT discharge_current ); //TODO
extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableDischarge(void);  //TODO
extern TYPE_CURRENT BMS_GetCurrentValueMaxAllowableCharge(void);   //TODO
#endif

