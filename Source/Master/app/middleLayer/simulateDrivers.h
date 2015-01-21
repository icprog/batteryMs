/*
 * simulateDrivers.h
 *
 *  Created on: Dec 27, 2014
 *      Author: mgc
 */

#ifndef SIMULATEDRIVERS_H_
#define SIMULATEDRIVERS_H_
#include "public/types.h"

/*
 * simulate adcs
 * */
int SetSimulateAdmvById(UINT8 id, UINT16 admv);
int setSimulateAdEnable( UINT8 enble );
int getSimulateAdState(void);
UINT16 GetSimulateAdmvById(UINT8 id);

/*
 * simulate gpios
 * */
int setSimulateGpioEnable( UINT8 enable );
int getSimulateGpioEnable( void );
int SetSimulateGpioById(UINT8 id, UINT8 state);
int getSimulateGpioById(UINT8 id);

/*
 * simulate temps
 * */
int setSimulateTempEnable( UINT8 enable );
int getSimulateTempEnable( void );
int SetSimulateTempById(UINT8 id, TYPE_TEMP temp);
TYPE_TEMP getSimulateTempById(UINT8 id);

/*
 * simulate voltages
 * */
int setSimulateVolEnable( UINT8 enable );
int getSimulateVolEnable( void );
int SetSimulateVolById(UINT8 id, TYPE_VOLTAGE vol);
TYPE_VOLTAGE getSimulateVolById(UINT8 id);

/*
 * test
 * */
int testSimulatorValue(void);
#endif /* SIMULATEDRIVERS_H_ */



