#ifndef _GPIO_EVENTS_H_
#define _GPIO_EVENTS_H_
/*
 * this module is used for CGpioEvents
 * you should use CGpioEventsAlloc first to alloc a instance, and then you can get a fd to this instance;
 * and then use CGpioEventsInit to init all instance after all gpios are working properly;
 * and then you should put CGpioEventsMonitor in a main loop, and make sure that GetSystemTime function work properly;
 * and now if you want to change gpio status, you can use function CGpioEvents_SetGpio;
 * by mgc 20140826
 * add CGpioEvents_SetGpioAfterTimems 
 * change CGpioEventsMalloc 
 * by mgc 20141112
 * 
 */

#include "public/types.h"
#include "driver/timer/timer.h"

/*config these events*/
#define MAX_GPIO_EVETNS  10

/*types*/
typedef void ( *gpio_set_fun ) ( UINT8 port,UCHAR status );
typedef UINT8 ( *gpio_get_fun ) ( UINT8 port );
typedef struct gpio_events_class
{
    UCHAR *name;
    UINT32 time_last; // the time of the status change
    UINT32 time_change_allowed; //the min time that allowed the real status change
    int fd;  // fd
    UCHAR gpio_status;  
    UCHAR gpio_lookfor_status;
    gpio_set_fun gpio_set;
    gpio_get_fun gpio_get;
    UINT8 ioPort;
} CGpioEvents;

/*interfaces need by this module*/
#define GPIOEVETN_GetSystemTime getSystemTimeS
#define GPIOEVETN_GetSystemTimeMs getSystemTimeMs
#define GPIOEVETN_time_cal( lasttime,timenow) TIME_TRHESHOLD_CAL(timenow,lasttime)
/*
extern UINT32 GetSystemTime(void);
extern UINT32 time_cal(UINT32 lasttime, UINT32 timenow);
 */
/*for external interfaces*/
int CGpioEventsMalloc( CGpioEvents * CGpioEvents, char *name, gpio_get_fun getgpio, gpio_set_fun setgpio, UINT8 ioPort );
void CGpioEvents_SetGpio ( CGpioEvents *pevents, UCHAR status );
void CGpioEvents_SetGpioAfterTimems(CGpioEvents *pevents, UCHAR status, UINT32 timems);
void CGpioEvents_SetGpioImmediately(CGpioEvents *pevents, UCHAR status);
void CGpioEvents_SetGpioByFd( int fd, UCHAR status );
void CGpioEventsMonitor(void ); //must be called by main loop
int GpioMonitor(CGpioEvents *pevents);
int CGpioEventsInit( void ); //init all gpio evetns
int testGpioEvents(void); //test example function
#endif


// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
