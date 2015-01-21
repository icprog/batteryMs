/*************************************************************************
 by mgc 2014-10-23
 timer.c
 for mpc5634
 at BM06
 *************************************************************************/
#include "MPC5634M.h"
#include "driver/pll/pll.h"
#include "driver/Gpio/gpio.h"
#include "driver/timer/timer.h"
#include "IntcInterrupts.h"   
#include "public/public.h"

#define S_2_H(s)  ((s)/3600)
#define S_2_M(s)  ((s)%3600/60)
#define S_2_S(s)  ((s)%60)

UINT8 Sys_Flag = 0;
static VUINT32 g_time_ms = 0, g_time_100ms = 0, g_time_s = 0; /*ϵͳʱ�Ӽ�������������RTI�жϷ���������ۼ�*/

void
RTI_ISR(void);
/************************************************************/
/*                        ʹ���ж�                          */
/************************************************************/
void enableIrq(void)
{
	INTC .CPR.B.PRI = 0; /* MPC563x: Lower INTC's current priority */
	asm(" wrteei 1");
	/* Enable external interrupts */
}

void disableIrq(void)
{
	asm(" wrteei 0");
}

/************************************************************/
/*                        ʵʱ�жϳ�ʼ������                                                                            */
/************************************************************/
void initPIT_RTI(void)
{
//  PIT.MCR.R = 0X00000001;             //RTIʱ��ʹ�ܣ�PIT��ʱ��ʱ��ʹ�ܣ�����ģʽֹͣ��ʱ
	PIT .PITMCR.R = 0X00000001; 		   //RTIʱ��ʹ�ܣ�PIT��ʱ��ʱ��ʹ�ܣ�����ģʽֹͣ��ʱ
	PIT .RTI.LDVAL.R = 8000;             //ʵʱ�ж�����Ϊ1ms
	PIT .RTI.TCTRL.R = 0X00000003;       //ʵʱ�ж�ʹ�ܣ���ʱ��ʹ��
	INTC .PSR[305].R = 1;                //ʵʱ�ж����ȼ�
	INTC_InstallINTCInterruptHandler(RTI_ISR, 305, 1);  //�����жϺ�

}

UINT32 getSystemTimeMs(void)
{
	return g_time_ms;
}

UINT32 getSystemTimeS(void)
{
	return g_time_s;
}

void delay_us(UINT32 us)
{
	UINT32 j = 0, i = 0;
	for (j = us; j; j--)
		for (i = 0xff; i; i--)
			;
}

void delay_ms(UINT32 ms)
{
	UINT32 lms = getSystemTimeMs();
	while (TIME_TRHESHOLD_CAL(g_time_ms, lms) < ms)
		;
}

void delay_s(UINT32 s)
{
	UINT32 ls = getSystemTimeS();
	UINT32 t = 0;
	while (t < s)
		t = TIME_TRHESHOLD_CAL(g_time_s, ls);
}

//void printf_sys_time(void)
//{
//  mprintf("systime[%lds]", g_time_s);
//  mprintf("%ldh:", S_2_H(g_time_s));
//  mprintf("%ldm:", S_2_M(g_time_s));
//  mprintf("%lds\r\n", S_2_S(g_time_s));
//}

/************************************************************/
/*                           ������                         */
/************************************************************/

#define LED_PORT 191
int test_timer_RIT_init(void)
{
	Config_PLL();
	INTC_InitINTCInterrupts();
	initPIT_RTI();
	gpio_init(LED_PORT, GPIO_OUTPUT);
	enableIrq();
}

int test_timer_RTI(void)
{
	static UINT8 Sys_Flag = 0;
//  while (1)
	{
		if (getSystemTimeMs() % 500 == 0)
		{
			Sys_Flag = 1 - Sys_Flag;
		}

		if (Sys_Flag)
		{
			gpio_set(LED_PORT, GPIO_HIGH);        	//����Ӧ�Ĺܽ�����ߵ�ƽ
		}
		else
		{
			gpio_set(LED_PORT, GPIO_LOW);        	//����Ӧ�Ĺܽ�����ߵ�ƽ
		}
	}
	return 0;
}

void RTI_ISR(void)
{
	g_time_ms++;
	if (g_time_ms % 100 == 0)
	{
		g_time_100ms++;
		if (g_time_100ms % 10 == 0)
		{
			g_time_s++;
		}
	}
	PIT .RTI.TFLG.B.TIF = 1; /*��RTI�ж� */
}

/******************the next is pit ***************************/
/**/
void
isr_pit0(void);
void
isr_pit1(void);
void
isr_pit2(void);
void
isr_pit3(void);
/*types*/

typedef struct pit_soft_s
{
	type_isr_pit func;
	UINT32 time_interval;
	UINT32 counter;
	UINT8 state;
};

/*varilbles*/
static type_isr_pit isr_pits[4] =
{
	isr_pit0,
	isr_pit1,
	isr_pit2,
	isr_pit3 };

//static const int MAX_SOFT_TIMERS = 6;

static struct pit_soft_s pits_soft[MAX_SOFT_TIMERS];

/*functions*/
void init_PIT_RTI(void)
{
	UINT8 i = 0;
	PIT .PITMCR.R = 0X00000001;             //RTIʱ��ʹ�ܣ�PIT��ʱ��ʱ��ʹ�ܣ�����ģʽֹͣ��ʱ
	for (i = 0; i < 4; i++)
	{
		PIT .TIMER[i].LDVAL.R = 64000;       //�����ж϶�ʱ��ͨ��1�ж�����Ϊ1ms��
		PIT .TIMER[i].TCTRL.R = 0X00000003;  //�����ж϶�ʱ��ͨ��1�ж�ʹ�ܣ���ʱ��ʹ��
		//INTC.PSR[301+i].R = 1;                //�����ж϶�ʱ��ͨ��1���ȼ�
		INTC_InstallINTCInterruptHandler(isr_pits[i], (UINT16) (301 + i), 2); //�����жϺ�
//		INTC_InstallINTCInterruptHandler(RTI_ISR, 302, 1);  //�����жϺ�
	}
}

void init_PIT(UINT8 id)
{
	assert( id>=0 && id<= 3);
	PIT .PITMCR.R = 0X00000001;             //RTIʱ��ʹ�ܣ�PIT��ʱ��ʱ��ʹ�ܣ�����ģʽֹͣ��ʱ
	PIT .TIMER[id].LDVAL.R = 64000;       //�����ж϶�ʱ��ͨ��1�ж�����Ϊ1ms��
    PIT .TIMER[id].TCTRL.R = 0X00000003;  //�����ж϶�ʱ��ͨ��1�ж�ʹ�ܣ���ʱ��ʹ��
			//INTC.PSR[301+i].R = 1;                //�����ж϶�ʱ��ͨ��1���ȼ�
   INTC_InstallINTCInterruptHandler(isr_pits[id], (UINT16) (301 + id), 2); //�����жϺ�
}

void isr_pit0(void)
{
	static UINT32 time_ms = 0;
	time_ms++;
	if (pits_soft[0].state == RUNNING)
	{
		if (time_ms % pits_soft[0].time_interval == 0)
		{
			if (pits_soft[0].func)
				pits_soft[0].func();
		}
	}
	PIT .TIMER[0].TFLG.B.TIF = 1; //��ʱ���жϱ�־λ��λ
}

void isr_pit1(void)
{
	static UINT32 time_ms = 0;
	time_ms++;
	if (pits_soft[1].state == RUNNING)
	{
		if (time_ms % pits_soft[1].time_interval == 0)
		{
			if (pits_soft[1].func)
				pits_soft[1].func();
		}
	}
	PIT .TIMER[1].TFLG.B.TIF = 1; //��ʱ���жϱ�־λ��λ
}

void isr_pit2(void)
{
	static UINT32 time_ms = 0;
	time_ms++;
	if (pits_soft[2].state == RUNNING)
	{
		if (time_ms % pits_soft[2].time_interval == 0)
		{
			if (pits_soft[2].func)
				pits_soft[2].func();
		}
	}
	PIT .TIMER[2].TFLG.B.TIF = 1; //��ʱ���жϱ�־λ��λ
}

void isr_pit3(void)
{
	static UINT32 time_ms = 0;
	UINT8 i = 0;
	time_ms++;

	for (i = 3; i < 6; i++)
	{
		if (pits_soft[i].state == RUNNING)
		{
			if (  pits_soft[i].counter-- == 0)
			{
				pits_soft[i].counter = pits_soft[i].time_interval;
				if (pits_soft[i].func)
					pits_soft[i].func();
			}
		}
	}
	PIT .TIMER[3].TFLG.B.TIF = 1; //��ʱ���жϱ�־λ��λ
}

/*
 *
 */
int timer_add(int id, type_isr_pit isr_func, UINT32 timems_interval)
{
	assert(id<MAX_SOFT_TIMERS);
	pits_soft[id].func = isr_func;
	pits_soft[id].time_interval = timems_interval;
	pits_soft[id].state = RUNNING;
	pits_soft[id].counter = timems_interval;
	return 0;
}

int timer_reset(int id)
{	
	assert(id<MAX_SOFT_TIMERS);
	pits_soft[id].state = STOP;
	pits_soft[id].counter = pits_soft[id].time_interval;
	pits_soft[id].state = RUNNING;
	return 0;
}

int timer_stop(int id)
{
	assert(id<MAX_SOFT_TIMERS);
	pits_soft[id].state = STOP;
	return 0;
}

int timer_continue(int id)
{
	assert(id<MAX_SOFT_TIMERS);
	pits_soft[id].state = RUNNING;
	return 0;
}

int timer_getState(int id)
{
	assert(id<MAX_SOFT_TIMERS);
	return pits_soft[id].state ;
}

/*
 * 
 * the next is the test program for timer pit
 * by mgc 2014-10-24
 * */
const UINT8 key_port[] =
{
	179,
	181,
	183,
	187 };
const UINT8 led_port[] =
{
	188,
	189,
	190,
	191 };

static void timer0_isr(void)
{
	static UINT8 flag = 0;
	flag = 1 - flag;
	if (flag)
		gpio_set(led_port[0], GPIO_LOW);
	else
		gpio_set(led_port[0], GPIO_HIGH);
}
static void timer1_isr(void)
{
	static UINT8 flag = 0;
	flag = 1 - flag;
	if (flag)
		gpio_set(led_port[1], GPIO_LOW);
	else
		gpio_set(led_port[1], GPIO_HIGH);
}
static void timer2_isr(void)
{
	static UINT8 flag = 0;
	flag = 1 - flag;
	if (flag)
		gpio_set(led_port[2], GPIO_LOW);
	else
		gpio_set(led_port[2], GPIO_HIGH);
}
static void timer3_isr(void)
{
	static UINT8 flag = 0;
	flag = 1 - flag;
	if (flag)
		gpio_set(led_port[3], GPIO_LOW);
	else
		gpio_set(led_port[3], GPIO_HIGH);
}

static type_isr_pit test_timer_isrs[] =
{
	timer0_isr,
	timer1_isr,
	timer2_isr,
	timer3_isr };

int test_timer_pit_init(void)
{
	INTC_InitINTCInterrupts();
//	Config_PLL();
	init_PIT_RTI();
	enableIrq();
}

int test_timer_pit(void)
{
	int ret = 0;
	/*make sure that these timers be added only once if in a loop*/
	static int i = 0;

	for (i = 0; i < sizeof(test_timer_isrs) / sizeof(type_isr_pit); i++)
	{
		gpio_init(led_port[i], GPIO_OUTPUT);
		gpio_init(key_port[i], GPIO_INPUT);
		timer_add(i, test_timer_isrs[i], (UINT32) ((i + 1) * 100));
	}

//	while (1)
//		;

}

