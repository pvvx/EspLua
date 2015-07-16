#ifndef __USER_CONFIG_H__
#define __USER_CONFIG_H__

#ifndef USE_US_TIMER
	#define USE_US_TIMER
#endif

#define SDK_VERSION 1129   // 1119 = 1.1.2 release
#define DEBUGSOO 1
#define USE_PVV_SPIFLASH 1
#define USE_MAX_IRAM 48k

// #define DEVELOP_VERSION
//#define NODE_DEBUG

#define USE_OPTIMIZE_PRINTF

#ifdef DEVELOP_VERSION
#define NODE_DEBUG
#define COAP_DEBUG
#endif	/* DEVELOP_VERSION */

#define NODE_ERROR

#ifdef NODE_DEBUG
#define NODE_DBG os_printf
#define NODE_DBG_ os_printf_plus
#else
#define NODE_DBG
#define NODE_DBG_
#endif	/* NODE_DEBUG */

#ifdef NODE_ERROR
#define NODE_ERR c_printf
#define NODE_ERR_ c_printf_
#else
#define NODE_ERR
#endif	/* NODE_ERROR */

#ifndef ICACHE_RAM_ATTR
#define ICACHE_RAM_ATTR __attribute__((section(".text")))
#endif

#ifndef ICACHE_FLASH_ATTR
#define ICACHE_FLASH_ATTR __attribute__((section(".text")))
#endif

#define ICACHE_STORE_ATTR

#ifndef DATA_IRAM_ATTR
#define DATA_IRAM_ATTR __attribute__((aligned(4), section(".iram.data")))
#endif

//#define ICACHE_STR_LUA_ATTR __attribute__((aligned(4), section("irom.text")))

#define CLIENT_SSL_ENABLE
#define GPIO_INTERRUPT_ENABLE
#define SHA2_ENABLE
// #define BUILD_WOFS		1
#define BUILD_SPIFFS	1

// #define LUA_NUMBER_INTEGRAL

#define USE_ROM_UART_FUNCS

#define LUA_OPTRAM
#ifdef LUA_OPTRAM
#define LUA_OPTIMIZE_MEMORY			2
#else
#define LUA_OPTIMIZE_MEMORY         0
#endif	/* LUA_OPTRAM */

#define READLINE_INTERVAL	80

#ifdef DEVKIT_VERSION_0_9
#define KEY_SHORT_MS	200
#define KEY_LONG_MS		3000
#define KEY_SHORT_COUNT (KEY_SHORT_MS / READLINE_INTERVAL)
#define KEY_LONG_COUNT (KEY_LONG_MS / READLINE_INTERVAL)

#define LED_HIGH_COUNT_DEFAULT 10
#define LED_LOW_COUNT_DEFAULT 0
#endif

#endif	/* __USER_CONFIG_H__ */
