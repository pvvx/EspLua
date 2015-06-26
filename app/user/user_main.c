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

#include "ets_sys.h"
#include "driver/uart.h"
#include "mem.h"

extern uint32 stack_low;

#define SIG_LUA 0
#define TASK_QUEUE_LEN 4
os_event_t *taskQueue;

void task_lua(os_event_t *e){
    char* lua_argv[] = { (char *)"lua", (char *)"-i", NULL };
    test_system_stack();
    NODE_DBG("Task task_lua started.\n");
    switch(e->sig){
        case SIG_LUA:
            NODE_DBG("SIG_LUA received.\n");
            lua_main( 2, lua_argv );
            test_system_stack();
            break;
        default:
            break;
    }
}

void task_init(void){
    taskQueue = (os_event_t *)os_malloc(sizeof(os_event_t) * TASK_QUEUE_LEN);
    system_os_task(task_lua, USER_TASK_PRIO_0, taskQueue, TASK_QUEUE_LEN);
}

extern void spiffs_mount();
// extern void test_spiffs();
// extern int test_romfs();

// extern uint16_t flash_get_sec_num();

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

    
    spiffs_mount();

    // myspiffs_format();
#ifdef DEVELOP_VERSION
    // test_spiffs();
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
    system_os_post(USER_TASK_PRIO_0,SIG_LUA,'s');
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
    // uart_init(BIT_RATE_115200, BIT_RATE_115200);
#endif
	stack_low = 0x5555AAAA;
    system_timer_reinit();
    os_delay_us(1000);

    os_printf("\nSDK version: %s\n", system_get_sdk_version());
#ifdef NODE_DEBUG
     system_print_meminfo();
#endif
    os_printf("Heap size: %u bytes.\n",system_get_free_heap_size());
     eram_init();
     flash_size_init();
    os_printf("Real Flash size: %u bytes.\n", flash_size);
#ifndef NODE_DEBUG
     system_set_os_print(0);
#endif
    system_init_done_cb(nodemcu_init);
}
