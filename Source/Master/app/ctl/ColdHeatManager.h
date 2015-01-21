/*
 * ColdHeatManager.h
 *
 *  Created on: Nov 13, 2014
 *      Author: mgc
 */

#ifndef COLDHEATMANAGER_H_
#define COLDHEATMANAGER_H_

#include "temp/temperature.h"
#include "public/public.h"
//#include "Gpio/gpio.h"
//#include "Gpio/port_defs.h"
//#include "Gpio/gpioFuncs.h"

int ColdHeatControler(void);

#define TEMP_OPEN_FAN()  FAN_OP(1)
#define TEMP_CLOSE_FAN() FAN_OP(0)
#define TEMP_OPEN_HEAT() HEAT_OP(1)
#define TEMP_CLOSE_HEAT() HEAT_OP(0)

#endif /* COLDHEATMANAGER_H_ */
