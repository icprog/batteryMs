/*
 * gpio.c
 * gpio driver
 * for mpc5634
 * by mgc 20141023
 * */

#include "MPC5634M.h"
#include "driver/Gpio/gpio.h"
#include "driver/pll/pll.h"
#define uchar unsigned char
#define uint unsigned int

///************************************************************/
///*                       初始化I/O口                        */
///************************************************************/
//void init_io_input(void) {
//	uint i;
//	//EMIOS0，2，4，8对应PCR179，181，183，187作为按键输入
//	SIU.PCR[179].R = 0x0103;             //EMIOS0,INPUT
//	SIU.PCR[181].R = 0x0103;             //EMIOS2,INPUT
//	SIU.PCR[183].R = 0x0103;             //EMIOS4,INPUT
//	SIU.PCR[187].R = 0x0103;             //EMIOS8,INPUT
//	//EMIOS9，10，11，12对应PCR188，189，190，191作为LED输出
//	for (i = 188; i < 192; i++) {
//		SIU.PCR[i].R = 0x0203;
//	}
//}
//
///************************************************************/
///*                       初始化LED灯                        */
///************************************************************/
//void init_io_output(void) {
//	//EMIOS9，10，11，12对应PCR188，189，190，191
//	SIU.PCR[188].R = 0x0203;            //EMIOS9,OUTPUT
//	SIU.PCR[189].R = 0x0203;             //EMIOS10,OUTPUT
//	SIU.PCR[190].R = 0x0203;             //EMIOS11,OUTPUT
//	SIU.PCR[191].R = 0x0203;             //EMIOS12,OUTPUT
//}

void gpio_init(UINT8 gpioport, enum GPIO_TYPE xtype)
{
	assert( gpioport>=GPIO_PORT_MIN && gpioport<= GPIO_PORT_MAX);
	if (xtype == GPIO_INPUT)
		SIU .PCR[gpioport].R = 0x0103;
	else if (xtype == GPIO_OUTPUT)
		SIU .PCR[gpioport].R = 0x0203;
	else if( xtype == GPIO_INANDOUT)
		SIU .PCR[gpioport].R = 0x0303;
}

void gpio_set(UINT8 gpioport, UINT8 xstate)
{
	assert( gpioport>=GPIO_PORT_MIN && gpioport<=GPIO_PORT_MAX);
	if (xstate == GPIO_HIGH)
		SIU .GPDO[gpioport].R = 1;
	else if (xstate == GPIO_LOW)
		SIU .GPDO[gpioport].R = 0;
}

UINT8 gpio_read(UINT8 gpioport)
{
	assert( gpioport>=GPIO_PORT_MIN && gpioport<=GPIO_PORT_MAX);
	return SIU .GPDI[gpioport].R;
}

/************************************************************/
/*                       延时程序                        */
/************************************************************/
static void gpio_delay(uint ms)
{
	uint i, j;
	for (i = 0; i < ms; i++)
		for (j = 0; j < 8000; j++)
		{
			;
		}
}

static const UINT8 key_port[] =
{ 179, 181, 183, 187 };
static const UINT8 led_port[] =
{ 188, 189, 190, 191 };
test_gpio_init(void)
{
	UINT8 i = 0;
	Config_PLL();
	for (i = 0; i < 4; i++)
	{
		gpio_init(led_port[i], GPIO_OUTPUT);
		gpio_init(key_port[i], GPIO_INPUT);
	}
}

int test_gpio(void)
{
	static k = 0, j = 0, flag = 0;

//	while (1)
	{
		for (k = 0; k < 3; k++)
		{
			if (gpio_read(key_port[k]) == 1)
			{
				gpio_set(led_port[k], GPIO_HIGH);
			}
			else
			{
				gpio_set(led_port[k], GPIO_LOW);
			}
		}
		j++;
		if (j % 10 == 0)
		{
			if (flag)
				gpio_set(191, GPIO_HIGH);
			else
				gpio_set(191, GPIO_LOW);
			flag = 1 - flag;
		}
		gpio_delay(10);
	}
	return 0;
}

