#ifndef _INSULATION_H_
#define _INSULATION_H_
#include "public/types.h"
#include "public/drivers.h"
//#define CONTROL_S0(state)  // state = 0  open, state = 1 close
//#define CONTROL_S1(state) //                                                  
#define read_res_vp() getMvByAdc(ADC_PORT_INS_P)
#define read_res_vn() getMvByAdc(ADC_PORT_INS_N)
#define OV_TO_100OUV( ov )
#define OV( ov ) (ov)
#define CONTROL_S0  IO_CTL_INS_CTRL_V_POSITIVE
#define CONTROL_S1  IO_CTL_INS_CTRL_V_NEGATIVE
#define	set_gpio_value(port, value) gpio_set(port, value)
/*for external interfaces*/
UINT16 get_insulation_res_p ( void );
UINT16 get_insulation_res_n ( void );
extern void set_CONTROL_S0(UINT8 state)  ;
extern void set_CONTROL_S1(UINT8 state)  ;
int insulation_resistance_calculate ( void ); // if < 0 err else ok

enum type_insulation_state
{
	STATE_INSULATION_ERR_LEVER1,
	STATE_INSULATION_ERR_LEVER2
};


/*
 * needed by vcu_protocol.h
 * */
 int BMS_GetErrStatusResInsulationByType( UINT8 types); //1:err, 0 OK
 TYPE_INSRES GetInsulationResValuePositivePole(void);  //ko
 TYPE_INSRES GetInsulationResValueNegativePole(void); //ko

#endif



// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
