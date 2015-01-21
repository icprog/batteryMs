#ifndef _GPIOFUNCS_H_
#define _GPIOFUNCS_H_
#include "driver/Gpio/gpio.h"
#include "driver/Gpio/port_defs.h"
#include "driver/adc/adc.h"
/*
 * by mgc 20141112 
 * */

#define T_AC_VALUE 4000
enum ioTrigerModle
{
	IO_RISING_EDG = 0, IO_FALLING_EDG = 1
};

enum IO_STATUS
{
	IO_STATUS_LOW = 0, IO_STATUS_HIGH = 1
};

int keyToOn(void);
int keyToACC(void);
int chargeInsert(void);
int dischargeInsert(void);

#define KEY_TO_ACC() (BMS_IO_GET(ID_IO_STATUS_ACC)==0) //gpio_read(IO_STATUS_ACC) //keyToACC()
#define KEY_TO_CRG() (BMS_IO_GET(ID_IO_STATUS_ON)==0) //keyToOn()

#define DC_CHARGER_INSERT() (BMS_IO_GET(ID_IO_STATUS_CC2)==1) //(gpio_read(IO_STATUS_CC2) == 0)
#define DC_CHARGER_DISCONNECT() (BMS_IO_GET(ID_IO_STATUS_CC2)==0) //(gpio_read(IO_STATUS_CC2) == 1)
#define AC_CHARGER_INSERT() (getMvByAdc(ADC_PORT_ACCIN) > T_AC_VALUE )
#define AC_CHARGER_DISCONNECT() ( (getMvByAdc(ADC_PORT_ACCIN) <= T_AC_VALUE))

#define FAN_OP(op) BMS_IO_SET(ID_IO_CTL_FANS,(op))
#define HEAT_OP(op) BMS_IO_SET(ID_IO_CTL_HEAT,(op))

#define RelayPrecharge_state()   (gpio_read(IO_STATUS_PRECHARGE))
#define RelayNegative_state()    (gpio_read(IO_STATUS_DISCHARGE_NEGATIVE))
#define RelayPositive_state()    (gpio_read(IO_STATUS_DISCHARGE_POSITIVE))
#define RelayCharge_state()      (gpio_read(IO_STATUS_CHARGE))

int ledAlarmOnByTimers(UINT32 times);

//int getPwmPulsewidth(void); //TODO
int getACCStatus(void); //TODO
int getONStatus(void); //TODO
int getCPStatus(void); //TODO

/*
 * for relay control
 * */
int OpenRelayNegative(void);
int OpenRelayPrecharge(void);
int OpenRelayPositive(void);
int OpenRelayCharge(void);

int ShutdownRelayPrechargeByTimerS(UINT32 times);
int shutdownRelayCharge(void);
int ShutdownRelayNegative(void);
int ShutdownRelayPositive(void);

int OpenVEH_Charger_CPC(void);
int CloseVEH_Charger_CPC(void);

int GetRelayChargeStatus(void);
int testIosForHardware(UINT32 timeMsNow);
/*for cold manager*/
#define HEAT_SET( state ) BMS_IO_SET(ID_IO_CTL_HEAT, state)//	 128
#define FAN1_SET( state ) BMS_IO_SET(ID_IO_CTL_FANS, state)// 	 127
#define FAN1_GET()  BMS_IO_GET(ID_IO_CTL_FANS)
#define HEAT_GET()  BMS_IO_GET(ID_IO_CTL_HEAT)

/*
 * 所有GPIO的控制最终在这里实现
 * */
int gpioFuncsInit(void);
int gpioControl(void);
int BMSIOReadMonitor(void);
int SystemLedFlash(UINT32 timemsInterval);
UINT8 BMS_IO_GET(UINT8 ioId);
void BMS_IO_SET(UINT8 ioId, UINT8 ioStatus);
int getGpioLeverLeverById(UINT8 ioId);
int getGpioLeverChangeStatusById(UINT8 ioId);

/*
 * 硬件测试
 * */
int testIosForHardware(UINT32 timeMsNow);
#endif

