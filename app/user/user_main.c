/******************************************************************************
 * Copyright 2013-2014 Espressif Systems (Wuxi)
 *
 * FileName: user_main.c
 *
 * Description: entry file of user application
 *
 * Modification history:
 *     2014/1/1, v1.0 create this file.
*******************************************************************************/
#include "lua.h"
#include "platform.h"
#include "c_string.h"
#include "c_stdlib.h"
#include "c_stdio.h"

#include "user_interface.h"
#include "user_exceptions.h"

#include "cpu_esp8266.h"

#include "ets_sys.h"
#include "driver/uart.h"
#include "mem.h"
#include "rom2ram.h"

#define SIG_LUA 0
#define SIG_DOJOB 1
#define SIG_TIMER 2
#define SIG_RDLINE 3
#define SIG_FSMNT 4
#define TASK_QUEUE_LEN 8

os_event_t lua_taskQueue[TASK_QUEUE_LEN] DATA_IRAM_ATTR;
os_timer_t readline_timer DATA_IRAM_ATTR;

extern uint32 stack_low;

extern void dojob_cb(void *);
extern void readline(void *);
extern void alarm_timer_common_cb(unsigned b);
extern void spiffs_mount();

void lua_init(void)
{
     //char* lua_argv[] = { (char *)"lua", (char *)"-e", (char *)"print(collectgarbage'count');ttt={};for i=1,100 do table.insert(ttt,i*2 -1);print(i);end for k, v in pairs(ttt) do print('<'..k..' '..v..'>') end print(collectgarbage'count');", NULL };
     //lua_main( 3, lua_argv );
     //char* lua_argv[] = { (char *)"lua", (char *)"-i", NULL };
     //lua_main( 2, lua_argv );
     //char* lua_argv[] = { (char *)"lua", (char *)"-e", (char *)"pwm.setup(0,100,50) pwm.start(0) pwm.stop(0)", NULL };
     //lua_main( 3, lua_argv );
    // NODE_DBG("Flash sec num: 0x%x\n", flash_get_sec_num());
	char* lua_argv[] = { (char *)"lua", (char *)"-i", NULL };
	lua_main( 2, lua_argv );
}


void task_lua(os_event_t *e){
	NODE_DBG("Task_lua(%u, %p)\n", e->sig, e->par);
    switch(e->sig) {
    	case SIG_DOJOB:
    		os_timer_disarm(&readline_timer);
    		dojob_cb((void *)e->par);
    		break;
    	case SIG_RDLINE:
    		readline((void *)e->par);
    		break;
    	case SIG_TIMER:
    		alarm_timer_common_cb((int)e->par);
    		break;
    	case SIG_LUA:
    		lua_init();
    		break;
    	case SIG_FSMNT:
    		spiffs_mount();
//    	    myspiffs_format();
#ifdef DEVELOP_VERSION
    	    test_spiffs();
#endif
    		system_os_post(USER_TASK_PRIO_0, SIG_LUA, 0);
    		break;
    }

}

void set_lua_dojob(void *a)
{
	system_os_post(USER_TASK_PRIO_0, SIG_DOJOB, (uint32)a);
}

void set_lua_timer(int i)
{
	system_os_post(USER_TASK_PRIO_0, SIG_TIMER, i);
}

void ICACHE_RAM_ATTR set_run_lua_readline(void *a)
{
	system_os_post(USER_TASK_PRIO_0, SIG_RDLINE, (uint32)a);
}

void set_lua_readline(void *a)
{
	os_timer_disarm(&readline_timer);
    os_timer_setfn(&readline_timer, (os_timer_func_t *)set_run_lua_readline, a);
	os_timer_arm(&readline_timer, READLINE_INTERVAL, 0);   // no repeat */
}

void user_rf_pre_init(void)
{
	/* volatile */ uint32 * ptr_reg_rtc_ram = (/* volatile */ uint32 *)0x60001000;
	if((ptr_reg_rtc_ram[24] >> 16) > 4) {
		ptr_reg_rtc_ram[24] &= 0xFFFF;
		ptr_reg_rtc_ram[30] = 0;
	}
}

void nodemcu_init(void)
{
	NODE_ERR("\n");
    // Initialize platform first for lua modules.   
    if( platform_init() != PLATFORM_OK )
    {
        // This should never happen
        NODE_DBG("Can not init platform for modules.\n");
        return;
    }
    system_os_task(task_lua, USER_TASK_PRIO_0, lua_taskQueue, TASK_QUEUE_LEN);
    system_os_post(USER_TASK_PRIO_0, SIG_FSMNT, 0);
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
#ifdef DEVELOP_VERSION
    uart_init(BIT_RATE_74880, BIT_RATE_74880);
#else
    uart_init(BIT_RATE_115200,
#ifdef NODE_DEBUG
    		BIT_RATE_921600);
#else
     	 	BIT_RATE_115200);
#endif
    // uart_init(BIT_RATE_74880, BIT_RATE_74880);
#endif
	stack_low = 0x5555AAAA;
    system_timer_reinit();
    iram_buf_init();
    flash_size_init();
#ifdef NODE_DEBUG
     system_print_meminfo();
#endif
    os_printf("\nStart Heap size: %u bytes.\n",system_get_free_heap_size());
    os_printf("Real Flash size: %u bytes.\n", flash_size);
	if(eraminfo.size > 0) os_printf("Found free IRAM: base: %p, size: %d bytes\n", eraminfo.base,  eraminfo.size);
#ifndef NODE_DEBUG
     //system_set_os_print(0);
#endif
    system_init_done_cb(nodemcu_init);
}
