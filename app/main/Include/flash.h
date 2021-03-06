/******************************************************************************
 * FileName: spi_flash.h
 * Description: FLASH
 * Alternate SDK
 * Author: PV`
 * (c) PV` 2015
*******************************************************************************/

#ifndef SPI_FLASH_H
#define SPI_FLASH_H

#include "bios/spiflash.h"

//#define USE_MAX_IRAM 48k

#define SPI_FLASH_SEC_SIZE      4096

uint32 spi_flash_get_id(void);
SpiFlashOpResult spi_flash_read_status(uint32_t * sta);
SpiFlashOpResult spi_flash_write_status(uint32_t sta);
SpiFlashOpResult spi_flash_erase_sector(uint16 sec);
SpiFlashOpResult spi_flash_write(uint32 faddr, uint32 *src_addr, uint32 size);
SpiFlashOpResult spi_flash_read(uint32 faddr, void *des, uint32 size);
SpiFlashOpResult spi_flash_erase_block(uint32 blk);

uint32 spi_flash_real_size(void);

#ifdef USE_OVERLAP_MODE

typedef SpiFlashOpResult (* user_spi_flash_read)(
		SpiFlashChip *spi,
		uint32 src_addr,
		uint32 *des_addr,
        uint32 size);

extern user_spi_flash_read flash_read;

void spi_flash_set_read_func(user_spi_flash_read read);

#endif

#define USE_MSPI_FLASH_O 1 // включена "песочница" для SDK в 512 килобайт flash. Включить доступ к flash до 16Мбайт включительно

#define MASK_ADDR_FLASH_ICACHE_DATA	0xfffff

#define spi_flash_read_byte(faddr, dest) spi_flash_read(faddr, dest, 1);

#ifdef USE_MSPI_FLASH_O

#define open_16m() 			flashchip->chip_size = FLASH_MAX_SIZE
#define close_16m() 		flashchip->chip_size = FLASH_MIN_SIZE

#else

#define open_16m()
#define close_16m()

#endif

#endif
