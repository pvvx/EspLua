/******************************************************************************
 * FileName: ets_run.c
 * Description: Alternate SDK
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/
#include "user_config.h"
#include "bios.h"
#include "hw/esp8266.h"
#include "hw/spi_register.h"


typedef void (* tfunc_tsk)(void *);
typedef struct _sargc_tsk
{
	uint32 x1;
	uint32 x2;
}sargc_tsk;

typedef struct _ss_task{
	tfunc_tsk func;		//+0
	sargc_tsk * argc;	//+4
	uint8 size;			//+8
	uint8 ch_09;		//+9
	uint8 cnts;			//+10
	uint8 cnte;			//+11
	uint32 bitn;		//+12
}ss_task;

// RAM_BIOS:3FFFC6FC вписать в eagle.rom.addr.v6.ld
extern uint8 ets_bit_task_priority;
// RAM_BIOS:3FFFDAB0 вписать в eagle.rom.addr.v6.ld
extern tfunc_tsk ets_idle_cb;
// RAM_BIOS:3FFFDAB4 вписать в eagle.rom.addr.v6.ld
extern void * ets_idle_arg;
// RAM_BIOS:3FFFDAB8 вписать в eagle.rom.addr.v6.ld
extern uint32 ets_bit_count_task;
// RAM_BIOS:3FFFDAC0 вписать в eagle.rom.addr.v6.ld
extern ss_task ets_tab_task[32]; // 512 байт 32x16
/*
typedef struct _ets_idle_cbs{
	tfunc_tsk func;
	void * arg;
}ets_idle_cbs;
*/
volatile uint32 ets_run_ret DATA_IRAM_ATTR;

void ICACHE_RAM_ATTR ets_run(void)
{
	uint8 bnum;
	uint8 bctp = ets_bit_task_priority = 0;
	while(1) {
		while(1) {
			ets_intr_lock();
			bnum = ets_bit_count_task;
			asm volatile ("nsau %0, %1;" :"=r"(bnum) : "r"(bnum)); // Normalization Shift Amount Unsigned
			bnum = 32 - bnum;
			if(bctp < bnum) break;
			if(SPI0_CTRL & SPI_ENABLE_AHB) {
				if(ets_idle_cb != NULL) ets_idle_cb(ets_idle_arg);
				else if(ets_run_ret) {
					ets_run_ret = 0;
					ets_intr_unlock();
					return;
				}
			}
			asm volatile ("waiti 0;"); // Wait for Interrupt
			ets_intr_unlock();
		};
		ss_task * cur_task = &ets_tab_task[bnum-1];
		sargc_tsk * argc = &cur_task->argc[cur_task->cnts++];
		if(cur_task->size == cur_task->cnts) cur_task->cnts = 0;
		if(--cur_task->cnte == 0) {
			ets_bit_count_task &= ~cur_task->bitn;
		}
		ets_bit_task_priority = bnum;
		ets_intr_unlock();
		cur_task->func(argc);
		ets_bit_task_priority = bctp;
	};
}


