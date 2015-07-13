/*
 * delay.c
 *
 *  Created on: 09 июля 2015 г.
 *      Author: pvvx
 */
#include "user_config.h"
#include "bios.h"
#include "hw/eagle_soc.h"
#include "hw/esp8266.h"
#include "sdk/osapi.h"
#include "sdk/add_sdk_func.h"

extern void ets_run1(void);
/*
void ICACHE_RAM_ATTR delay_end(uint32 *px)
{
		ets_set_idle_cb(NULL, NULL);
		asm volatile (
				  "addi            a1, a1, 0x10;"
				  "l32i.n          a0, a1, 0;"
				  "l32i.n          a12, a1, 4;"
				  "l32i.n          a13, a1, 8;"
				  "l32i.n          a14, a1, 0x0C;"
				  "l32i.n          a15, a1, 0x10;"
				  "addi            a1, a1, 0x20;"
				  "ret.n;"
		);
}
*/
uint32 tt;
extern volatile bool ets_run_ret;

void ICACHE_RAM_ATTR delay_wait_cb(void *px)
{
	tt = phy_get_mactime();
	ets_run_ret = true;
//	ets_set_idle_cb(delay_end, px);
}

void ICACHE_RAM_ATTR task_delay_us(uint32 us)
{
	if(us < 512) ets_delay_us(us);
	else	{
		ETSTimer delay_timer;
		ets_timer_disarm(&delay_timer);
		ets_timer_setfn(&delay_timer, (ETSTimerFunc *)(delay_wait_cb), NULL);
		ets_timer_arm_new(&delay_timer, us - 128, 0, 0);
		ets_run1();
	}
}

extern void * ets_idle_cb;
void ICACHE_RAM_ATTR run_sdk_tasks(void)
{
	uint32 t = phy_get_mactime();
	if(t-tt > 20480 && ets_idle_cb == NULL) {
		ETSTimer delay_timer;
		ets_timer_disarm(&delay_timer);
		ets_timer_setfn(&delay_timer, (ETSTimerFunc *)(delay_wait_cb), NULL);
		ets_timer_arm_new(&delay_timer, 1024, 0, 0);
		ets_run1();
    	tt=t;
	}
}


