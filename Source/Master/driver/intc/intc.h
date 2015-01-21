/*
 * intc.h
 *
 *  Created on: Nov 11, 2014
 *      Author: Administrator
 */

#ifndef INTC_H_
#define INTC_H_

//#include "public/public.h"


void start_PWM_MONITOR(void);

void stop_PWM_MONITOR(void);

UINT16 getCC_state();

UINT8 SYS_sleep(UINT32 sleeptime);

UINT8  SYS_wakeupstate();

void CP_test();

//void TIME_wakeup();


//static void INTC_wakeup();

UINT16 getPwm();

void sleeptest();
#endif /* INTC_H_ */
