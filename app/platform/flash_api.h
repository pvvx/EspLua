#ifndef __FLASH_API_H__
#define __FLASH_API_H__

#include "user_config.h"
#include "os_type.h"
#include "spi_flash.h"


#define flash_safe_get_size_byte() flash_size
#define flash_safe_get_sec_num() (flash_size >> 12)

#ifdef USE_PVV_SPIFLASH

#define flash_safe_read spi_flash_read
#define flash_safe_erase_sector spi_flash_erase_sector
#define flash_safe_erase_block spi_flash_erase_block

#else

SpiFlashOpResult flash_safe_read(uint32 src_addr, void *des_addr, uint32 size);
SpiFlashOpResult flash_safe_erase_sector(uint16 sec);
SpiFlashOpResult flash_safe_erase_block(uint16 blk);

#endif

SpiFlashOpResult flash_safe_write(uint32 des_addr, uint8 *src_addr, uint32 size);

void flash_size_init(void);

uint8_t byte_of_aligned_array(const uint8_t* aligned_array, uint32_t index);

extern uint32_t flash_size;

#endif // __FLASH_API_H__
