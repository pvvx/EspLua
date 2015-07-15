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

uint32 tt DATA_IRAM_ATTR;
extern volatile uint32 ets_run_ret DATA_IRAM_ATTR;

void ICACHE_RAM_ATTR delay_wait_cb(void *px)
{
	tt = phy_get_mactime();
	ets_run_ret = 1;
}

void ICACHE_RAM_ATTR task_delay_us(uint32 us)
{
	if(us < 512) ets_delay_us(us);
	else	{
		ETSTimer delay_timer;
		ets_timer_disarm(&delay_timer);
		ets_timer_setfn(&delay_timer, (ETSTimerFunc *)(delay_wait_cb), NULL);
		ets_timer_arm_new(&delay_timer, us - 128, 0, 0);
		ets_run();
	}
}

void ICACHE_RAM_ATTR run_sdk_tasks(void)
{
	uint32 t = phy_get_mactime();
	if(t-tt > 20480) {
		ETSTimer delay_timer;
		ets_timer_disarm(&delay_timer);
		ets_timer_setfn(&delay_timer, (ETSTimerFunc *)(delay_wait_cb), NULL);
		ets_timer_arm_new(&delay_timer, 1024, 0, 0);
		ets_run();
    	tt=t;
	}
}


