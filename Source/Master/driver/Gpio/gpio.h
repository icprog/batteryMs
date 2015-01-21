#ifndef _GPIO_H_
#define _GPIO_H_

#include "public/public.h"
#include "driver/Gpio/port_defs.h"
//
//typedef unsigned char UINT8;
//typedef unsigned short UINT16;
//typedef unsigned int UINT32;
//typedef unsigned long long UINT64;
enum GPIO_TYPE
{
	GPIO_INPUT,
	GPIO_OUTPUT,
	GPIO_INANDOUT
};
enum GPIO_STATE
{
	GPIO_HIGH = 1 ,
	GPIO_LOW = 0
};

#define GPIO_PORT_MAX 232
#define GPIO_PORT_MIN 83
//
#define LED1_PORT 191
#define LED2_PORT 191
#define LED3_PORT 191
#define LED4_PORT 191


void gpio_init(UINT8 gpioport, enum GPIO_TYPE xtype) ;
void gpio_set(UINT8 gpioport, UINT8 xstate) ;
UINT8 gpio_read(UINT8 gpioport);
int test_gpio_init(void);
int test_gpio(void);

#endif
