#ifndef __CPU_ESP8266_H__
#define __CPU_ESP8266_H__

#include "os_type.h"
#include "spi_flash.h"
#include "pin_map.h"
#include "user_config.h"
//#include "flash_api.h"
// Number of resources (0 if not available/not implemented)

#define NUM_GPIO              GPIO_PIN_NUM
#define NUM_SPI               2
#define NUM_UART              1
#define NUM_PWM               GPIO_PIN_NUM
#define NUM_ADC               1
#define NUM_CAN               0
#define NUM_I2C               1
#define NUM_OW                GPIO_PIN_NUM
#define NUM_TMR               7

#define FLASH_SEC_NUM 	0x80
#define SYS_PARAM_SEC_NUM 4
#define SYS_PARAM_SEC_START (FLASH_SEC_NUM - SYS_PARAM_SEC_NUM)


#define INTERNAL_FLASH_SECTOR_SIZE      SPI_FLASH_SEC_SIZE
#define INTERNAL_FLASH_WRITE_UNIT_SIZE  4
#define INTERNAL_FLASH_READ_UNIT_SIZE	4

#define INTERNAL_FLASH_SIZE             (FLASH_SEC_NUM*SPI_FLASH_SEC_SIZE)
#define INTERNAL_FLASH_START_ADDRESS    0x40200000

// SpiFlashOpResult spi_flash_erase_sector(uint16 sec);
// SpiFlashOpResult spi_flash_write(uint32 des_addr, uint32 *src_addr, uint32 size);
// SpiFlashOpResult spi_flash_read(uint32 src_addr, uint32 *des_addr, uint32 size);
/*
#if defined(FLASH_SAFE_API)
#define flash_write flash_safe_write
#define flash_erase flash_safe_erase_sector
#define flash_read flash_safe_read
#else
#define flash_write spi_flash_write
#define flash_erase spi_flash_erase_sector
#define flash_read spi_flash_read
#endif // defined(FLASH_SAFE_API)
*/
#endif // #ifndef __CPU_ESP8266_H__
