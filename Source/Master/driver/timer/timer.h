#ifndef _TIMER_H_
#define _TIMER_H_

enum timer_state
{
	RUNNING, STOP
};

typedef void (*type_isr_pit)(void);
/************************************************************/
static const int MAX_SOFT_TIMERS = 6;
void initPIT_RTI(void);
UINT32 getSystemTimeMs(void);
UINT32 getSystemTimeS(void);
int test_timer_RTI(void);
int test_timer_pit_init(void);
int test_timer_pit(void);
int timer_add(int id, type_isr_pit isr_func, UINT32 timems_interval);
int timer_stop(int id);
int timer_continue(int id);
int timer_getState(int id);
int timer_reset(int id);

void init_PIT(UINT8 id);

void enableIrq(void);
void disableIrq(void);

void delay_us(UINT32 us);
void delay_ms(UINT32 ms);
void delay_s(UINT32 s);

static UINT32
TIME_TRHESHOLD_CAL(UINT32 tnow, UINT32 tlast)
{

  return (
      (tnow) >= (tlast) ? ((tnow) - (tlast)) : (0xFFFFFFFF - (tnow) + (tlast)));
}

int test_timer_RIT_init(void);
void init_PIT_RTI(void);

enum timer_ids
{
	TIMER_ID_CPCU_MSG = 0,
	TIMER_ID_WAKE_UP = 2,
	TIMER_ID_STOP_ALARM_LED = 4,
	TIMER_ID_SHUTDOWN_RELAY_PRECAHRGE = 5
};
#endif
