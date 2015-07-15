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

#include "ets_sys.h"
#include "driver/uart.h"
#include "mem.h"
#include "rom2ram.h"

extern uint32 stack_low;

#define SIG_LUA 0
#define SIG_DOJOB 1
#define SIG_TIMERS 2
#define SIG_TIMERE (2+6)
#define SIG_FSMNT 100
#define TASK_QUEUE_LEN 8

//os_event_t *taskQueue DATA_IRAM_ATTR;
os_event_t lua_taskQueue[4] DATA_IRAM_ATTR;

extern void dojob_cb(void *);
extern void alarm_timer_common_cb(void * a, unsigned b);
extern void spiffs_mount();

void task_lua(os_event_t *e){
    char* lua_argv[] = { (char *)"lua", (char *)"-i", NULL };
    NODE_DBG("Task task_lua started.\n");
    if(e->sig == SIG_DOJOB) {
    	dojob_cb((void *)e->par);
    }
    else if(e->sig >= SIG_TIMERS && e->sig <= SIG_TIMERE) {
    	alarm_timer_common_cb((void *)e->par, e->sig - SIG_TIMERS);
    }
    else if(e->sig == SIG_LUA) {
        NODE_DBG("SIG_LUA received.\n");
        lua_main( 2, lua_argv );
    }
    else if(e->sig == SIG_FSMNT) {
        spiffs_mount();
    	system_os_post(USER_TASK_PRIO_0, SIG_LUA, 's');
    }

}
void task_init(void){
//    taskQueue = (os_event_t *)os_malloc(sizeof(os_event_t) * TASK_QUEUE_LEN);
    system_os_task(task_lua, USER_TASK_PRIO_0, lua_taskQueue, TASK_QUEUE_LEN);
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
    
    // spiffs_mount();

    // myspiffs_format();
#ifdef DEVELOP_VERSION
    test_spiffs();
#endif
    // endpoint_setup();

    // char* lua_argv[] = { (char *)"lua", (char *)"-e", (char *)"print(collectgarbage'count');ttt={};for i=1,100 do table.insert(ttt,i*2 -1);print(i);end for k, v in pairs(ttt) do print('<'..k..' '..v..'>') end print(collectgarbage'count');", NULL };
    // lua_main( 3, lua_argv );
    // char* lua_argv[] = { (char *)"lua", (char *)"-i", NULL };
    // lua_main( 2, lua_argv );
    // char* lua_argv[] = { (char *)"lua", (char *)"-e", (char *)"pwm.setup(0,100,50) pwm.start(0) pwm.stop(0)", NULL };
    // lua_main( 3, lua_argv );
    // NODE_DBG("Flash sec num: 0x%x\n", flash_get_sec_num());
    task_init();
    system_os_post(USER_TASK_PRIO_0, SIG_FSMNT, 0);
}

void set_lua_dojob(void *a)
{
	system_os_post(USER_TASK_PRIO_0, SIG_DOJOB, (uint32)a);
}

void set_lua_timer(void *a, int i)
{
	system_os_post(USER_TASK_PRIO_0, SIG_TIMERS+i, (uint32)a);
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
    uart_init(BIT_RATE_9600, BIT_RATE_9600);
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
