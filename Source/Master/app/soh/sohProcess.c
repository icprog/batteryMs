/*
 * File: ert_main.c
 *
 * Code generated for Simulink model 'SOH'.
 *
 * Model version                  : 1.16
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sun Jan 04 18:40:47 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Freescale->MPC5xx
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 *    3. MISRA-C:2004 guidelines
 * Validation result: Not run
 */

#include <stddef.h>
#include <stdio.h>                     /* This ert_main.c example uses printf/fflush */
#include "soh/SOH.h"                       /* Model's header file */
#include "rtwtypes.h"

#include "public/types.h"
#include "public/public.h"
#include "voltage/voltage.h"
#include "temp/temperature.h"
#include "current/current.h"
#include "driver/timer/timer.h"
#include "driver/eprom/eprom.h"
 static boolean_T RQ_ChaAbrt = false;

  /* '<Root>/RQ_ChaStart' */
 static boolean_T RQ_ChaStart = false;


uint8_T rt_OneStep(void)
{

 
  /* '<Root>/soc_chg_init' */
  static uint8_T soc_chg_init = 0U;

  /* '<Root>/capacity' */
  static real32_T capacity = 90.0F;

  /* '<Root>/current' */
  static int32_T current = 0;

  /* '<Root>/dt' */
  static uint16_T dt = 0U;

  /* '<Root>/soh' */
  static uint8_T soh;

  
   static UINT32 lastTimeMs = 0;
   UINT32 timeMsNow = 0;
   UINT32 timeTh = 0;
  
  
  timeMsNow = getSystemTimeMs();
  timeTh = TIME_TRHESHOLD_CAL(lastTimeMs, timeMsNow);
  lastTimeMs = timeMsNow;
 
  dt=timeTh;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */
  SOH_initialize();
  /* Step the model */
  SOH_custom(RQ_ChaAbrt, RQ_ChaStart, soc_chg_init, capacity, current, dt, &soh);

  /* Get model outputs here */

  //SOH_terminate();
  
  return soh;

}


uint8_T sohProcess(void)

{
	//static uint8_T *soh,*soh1,*soh2,*soh3,*soh4,i;
	
	static i;
	static uint16_T Adr1=1,Adr2=2,Adr3=3,Adr4=4;
	
	uint16_T Addr[4];
	
	uint8_T soh[4];
	uint8_T j,avershoh;
	if (RQ_ChaStart)
	{
	rt_OneStep();
	i++;
	if(i>4)
	i =0;
	 switch(i)
    {
	case 0 :  
	    for(j= 0;j<4;j++)
	    {
		ReadEprom( Addr[j], 1 , &soh[j]);
		
	    avershoh += soh[j];
	    }
	    avershoh = (uint8_T)(avershoh/4);	     
	case 1 : 
		 WriteEprom( Addr[1], 1 , &soh[1]);
	case 2 : 
		WriteEprom( Addr[2], 1 , &soh[2]);
	case 3 : 
		WriteEprom( Addr[3], 1 , &soh[3]);
		
	case 4 : 
		WriteEprom( Addr[4], 1 , &soh[4]);	
    }
    };

}
 
