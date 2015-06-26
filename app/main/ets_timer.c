/******************************************************************************
 * FileName: ets_timer.c
 * Description: Alternate SDK (libmain.a)
 * Author: PV`
 * (c) PV` 2015
 * ver 0.0.0 (b1)
*******************************************************************************/
#include "user_config.h"
#include "bios.h"
#include "hw/eagle_soc.h"
#include "hw/esp8266.h"

extern bool timer2_ms_flag;

#define MIN_US_TIMER 0

// us timer
// 0xFFFFFFFF/(80000000 >> 4) = 858.993459
// (80000000 >> 4)/(1000000>>2) = 20
// (80000000 >> 4)/1000000 = 5
// ms timer
// 0xFFFFFFFF/(80000000 >> 8) = 13743.895344
// (80000000 >> 8)/(1000>>2) = 1250
// (80000000 >> 8)/1000 = 312.5
#define XS_TO_RTC_TIMER_TICKS(t, prescaler, period)	\
     (((t) > (0xFFFFFFFF/(APB_CLK_FREQ >> prescaler))) ?	\
      (((t) >> 2) * ((APB_CLK_FREQ >> prescaler)/(period>>2)) + ((t) & 0x3) * ((APB_CLK_FREQ >> prescaler)/period))  :	\
      (((t) * (APB_CLK_FREQ >> prescaler)) / period))

void ICACHE_RAM_ATTR ets_timer_arm_new(ETSTimer *ptimer, uint32_t us_ms, int repeat_flag, int isMstimer)
{
//	ets_intr_lock();
	if(ptimer->timer_next != (ETSTimer *)0xffffffff) ets_timer_disarm(ptimer);
	if(us_ms) {
		if(timer2_ms_flag == 0) { // us_timer
			if(isMstimer) us_ms *= 1000;
#if ((APB_CLK_FREQ>>4)%1000000)
			us_ms = XS_TO_RTC_TIMER_TICKS(us_ms, 4, 1000000);
#else
			us_ms *= (APB_CLK_FREQ>>4)/1000000;
#endif
		}
		else { // ms_timer
#if ((APB_CLK_FREQ>>8)%1000)
			us_ms = XS_TO_RTC_TIMER_TICKS(us_ms, 8, 1000);
#else
			us_ms *= (APB_CLK_FREQ>>8)/1000;
#endif
		}
	}
#if MIN_US_TIMER != 0
	if(us_ms < MIN_US_TIMER) us_ms = MIN_US_TIMER;
#endif
	if(repeat_flag) ptimer->timer_period = us_ms;
	MEMW();
	timer_insert(TIMER1_COUNT + us_ms, ptimer);
//	ets_intr_unlock();
}

/*
// ------------------------------------------------------
// system_timer_reinit
//  :16  :256
void system_timer_reinit(void)
{
	timer2_ms_flag = 1;
	TIMER1_CTRL = 0x84;
}
*/
