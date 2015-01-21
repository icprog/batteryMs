
#include "public/types.h"
#include "driver/Gpio/gpio_events.h"
#include "driver/timer/timer.h"
#include "driver/Gpio/gpio.h"
/*
 * 
 * GPIO manager class
 * by mgc 20141112
 * */
/*defination*/
#define CGpioEvenst_REGISTER( name, gpiosetfun, gpiogetfun, ioPort ) \
    { name,\
        0,0,0,0,0,\
        gpiosetfun,\
        gpiogetfun,\
        ioPort\
    }
/*private type*/
struct CGpioEventsFd
{
	CGpioEvents *pevents;
	UCHAR dirtyflag;
};

/*private value*/
static struct CGpioEventsFd g_gpio_events[MAX_GPIO_EVETNS];
static UCHAR valide_fds = 0;

/*
 * malloc a object
 * if return zero means failed
 * else return the point to the class
 */
int CGpioEventsMalloc(CGpioEvents *pevents, char *name, gpio_get_fun getgpio,
		gpio_set_fun setgpio, UINT8 ioPort)
{
	UCHAR i = 0;

	for (i = 0; i < MAX_GPIO_EVETNS; i++)
	{
		if (g_gpio_events[i].dirtyflag == 0) // unused
		{
			// memset ( &g_gpio_events[i], 0, sizeof ( struct CGpioEventsFd ) );
			g_gpio_events[i].dirtyflag = 1;
			pevents->fd = i; // set fd
			pevents->gpio_get = getgpio;
			pevents->gpio_set = setgpio;
			pevents->time_change_allowed = 0;
			pevents->ioPort = ioPort;
			pevents->name = (UINT8 *)name;

			g_gpio_events[i].pevents = pevents;
			valide_fds++;
			return i;
		}
	}

	return -1;
}

/*
 * CGpioEventsInit
 * init all CGpioEvents(g_gpio_events)
 * shuld be called by initclass
 * by mgc 20140826
 */
int CGpioEventsInit(void) //init all gpio evetns
{
	UCHAR i = 0;

	for (i = 0; i < MAX_GPIO_EVETNS; i++)
	{
		if (g_gpio_events[i].dirtyflag != 0) // used
		{
			if (g_gpio_events[i].pevents->gpio_get)
			{
				g_gpio_events[i].pevents->gpio_status =
						g_gpio_events[i].pevents->gpio_get(g_gpio_events[i].pevents->ioPort);
			}
		}
	}
	return 0;
}

static int isValidFd(int fd)
{
	if (fd >= 0 && fd < valide_fds)
	{
		if (g_gpio_events[fd].dirtyflag == 1)
			return 1;
	}

	return 0;
}

void CGpioEvents_SetGpioByFd(int fd, UCHAR status)
{
	if (isValidFd(fd))
		CGpioEvents_SetGpio(g_gpio_events[fd].pevents, status);
}
/*
 * param:
 * 	status: 0 low, 1 high
 * */
void CGpioEvents_SetGpio(CGpioEvents *pevents, UCHAR status)
{
	if (pevents->gpio_lookfor_status != status)
	{
		pevents->time_last = GPIOEVETN_GetSystemTimeMs();
		pevents->time_change_allowed = 0;
		pevents->gpio_lookfor_status = status;
	}
}
/*
 * this will change the io state immediately
 * */
void CGpioEvents_SetGpioImmediately(CGpioEvents *pevents, UCHAR status)
{
	if (pevents->gpio_lookfor_status != status)
	{
		pevents->time_last = GPIOEVETN_GetSystemTimeMs();
		pevents->time_change_allowed = 0;
		pevents->gpio_lookfor_status = status;
		GpioMonitor(pevents);
	}
}

/*
 * times: the time that gpio will change after this function called (second) 
 * */
void CGpioEvents_SetGpioAfterTimems(CGpioEvents *pevents, UCHAR status, UINT32 timems)
{
	if (pevents->gpio_lookfor_status != status)
	{
		pevents->time_last = GPIOEVETN_GetSystemTimeMs();
		pevents->time_change_allowed = timems;
		pevents->gpio_lookfor_status = status;
	}
}

int GpioMonitor(CGpioEvents *pevents)
{
	UINT32 time = 0;

//	if (pevents->gpio_get)
//	{
//		pevents->gpio_status = pevents->gpio_get(pevents->ioPort);
//	}

	if (pevents->gpio_status != pevents->gpio_lookfor_status)
	{
		if (pevents->time_change_allowed == 0) // if open  do it immidiately
		{
			pevents->gpio_status = pevents->gpio_lookfor_status;
		}
		else // delay some time
		{
			time = GPIOEVETN_GetSystemTimeMs();
			time = GPIOEVETN_time_cal ( pevents->time_last, time );

			if (time > pevents->time_change_allowed)
			{
				mprintf("%s time reach! %d->%d\r\n", pevents->name,
						pevents->gpio_status, pevents->gpio_lookfor_status);
				pevents->gpio_status = pevents->gpio_lookfor_status;
			}
		}
	}

	// make sure the gpio_status is same as real gpio status
	if (pevents->gpio_set)
	{
//		return 
				pevents->gpio_set(pevents->ioPort,pevents->gpio_status);
	}

	return 0;
}
/*
 * this func must be called by main loop
 * by mgc 20140826
 */
void CGpioEventsMonitor(void)
{
	UCHAR i = 0;
	int ret = 0;

	for (i = 0; i < valide_fds; i++)
	{
		if (g_gpio_events[i].dirtyflag != 0) // used
		{
			ret = GpioMonitor(g_gpio_events[i].pevents);
			(void) ret;
//          ret = ret; // avoid alarm
		}
	}
}

#include "driver/Gpio/port_defs.h"
#include "driver/Gpio/gpio.h"
 CGpioEvents gpio_charge;
 CGpioEvents gpio_discharge;
int testGpioEvents(void)
{
	int i = 0;UINT8 flag = 0;
	CGpioEventsMalloc(&gpio_charge, "charge_gpio", gpio_read,
			gpio_set, IO_CTL_CHARGE);
	CGpioEventsMalloc(&gpio_discharge, "discharge_gpio", gpio_read,
			gpio_set, IO_CTL_DISCHARGE_NEGATIVE);
	
	
	CGpioEvents_SetGpio(&gpio_charge, 1);  //this IO will change immediately after CGpioEventsMonitor() called
	CGpioEvents_SetGpioAfterTimems(&gpio_discharge, 1, 5000);  // after 5 seconds, this IO will change
	while (1)
	{
		CGpioEventsMonitor();
	}
	return 0;
}

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 

