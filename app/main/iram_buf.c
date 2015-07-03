/*
 * FileName: iram_buf.c
 * Description: Alternate SDK (main.a)
 * (c) pvvx 2015
 */
#include "user_config.h"
#include "bios.h"
#include "sdk/osapi.h"
#include "sdk/add_sdk_func.h"
#include "hw/esp8266.h"
#include "rom2ram.h"
#include "iram_buf.h"

#ifndef DEBUGSOO
#define DEBUGSOO 2
#endif

#ifndef ICACHE_RODATA_ATTR
#define ICACHE_RODATA_ATTR __attribute__((aligned(4))) __attribute__((section(".irom.text")))
#endif

#if DEBUGSOO > 0
const char *txtSPIFlashInterface[] = {"QIO","QOUT","DIO","DOUT"};
const char *txtFSize[] = {"512K", "256K", "1M", "2M", "4M"};
const char *txtSFreq[] = {"40MHz","26MHz","20MHz","80MHz"};
#endif


#define flash_read spi_flash_read

ERAMInfo eraminfo;

bool  get_eram_size(ERAMInfo *einfo) {
	struct SPIFlashHeader x;
	int i = 1;
	uint32 faddr = 0x000;
	einfo->base = NULL;
	einfo->size = 0;
	uint32 iramsize = 32768 + ((((DPORT_BASE[9]>>3)&3)==3)? 0 : 16384);
	if (flash_read(faddr, (uint32 *)&x, 8) != 0)
		return false;
	if(x.head.number_segs <= 1) {
		faddr = 0xC0;
		if (flash_read(faddr, (uint32 *)&x, 8) != 0)
			return false;
	}
	faddr += 8;
#if DEBUGSOO > 1
	os_printf("Flash Header:\n");
#endif
	if (x.head.id != 0xE9) {
#if DEBUGSOO > 1
		os_printf(" Bad Header!\n");
#endif
		return false;
	} else {
#if DEBUGSOO > 1
/*		uint32 speed = x.head.hsz.spi_freg;
		if(speed > 2) {
			if(speed == 15) speed = 3;
			else speed = 0;
		} */
		os_printf(
				" Number of segments: %u\n SPI Flash Interface: %s\n SPI CLK: %s\n Flash size: %s\n Entry point: %p\n",
				x.head.number_segs, txtSPIFlashInterface[x.head.spi_interface & 3], txtSFreq[x.head.hsz.spi_freg & 3],
				txtFSize[x.head.hsz.flash_size&3], x.entry_point);
#endif
		while (x.head.number_segs) {
			if (flash_read(faddr, (uint32 *)&x.seg, 8) != 0) {
#if DEBUGSOO > 1
				os_printf("flash read error!");
#endif
				return false;
			};
			if ((x.seg.memory_offset >= IRAM1_BASE)
					&& (x.seg.memory_offset < (IRAM1_BASE + iramsize))) {
				if (((x.seg.memory_offset + x.seg.segment_size) > (uint32)einfo->base)
						&& ((x.seg.memory_offset + x.seg.segment_size)
								< (IRAM1_BASE + iramsize))) {
					einfo->size = iramsize - x.seg.segment_size;
					einfo->base = (uint32 *)(x.seg.memory_offset + x.seg.segment_size);
				};
			};
#if DEBUGSOO > 1
			os_printf(" Segment %u: offset: %p, size: %u\n", i, x.seg.memory_offset,
					x.seg.segment_size);
#endif
			x.head.number_segs--;
			i++;
			faddr += x.seg.segment_size + 8;
		};
	};
	if((eraminfo.base)&&(eraminfo.size > MIN_GET_IRAM)) return true;
	return false;
}

void  eram_init(void) {
	if(get_eram_size(&eraminfo)) {
#if DEBUGSOO > 0
		os_printf("Found free IRAM: base:%p, size:%u bytes\n", eraminfo.base,  eraminfo.size);
#endif
		os_memset(eraminfo.base, 0, eraminfo.size);
	}
}

//extern void copy_s4d1(uint8 * pd, void * ps, uint32 len);

bool  __attribute__((optimize("Os"))) ICACHE_RAM_ATTR eRamRead(uint32 addr, uint8 *pd, uint32 len)
{
	if (addr + len > eraminfo.size) return false;
	copy_s4d1(pd, (void *)((uint32)eraminfo.base + addr), len);
	return true;
}

//extern void copy_s1d4(void * pd, uint8 * ps, uint32 len);

bool  __attribute__((optimize("Os"))) ICACHE_RAM_ATTR eRamWrite(uint32 addr, uint8 *ps, uint32 len)
{
	if (addr + len > eraminfo.size) return false;
	copy_s1d4((void *)((uint32)eraminfo.base + addr), ps, len);
	return true;
}

