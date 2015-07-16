#include "c_stdio.h"
#include "platform.h"
#include "spiffs.h"

spiffs fs;

extern char _flash_used_end[];

#define LOG_PAGE_SIZE       256
  
static u8_t spiffs_work_buf[LOG_PAGE_SIZE*2];
static u8_t spiffs_fds[32*4];
static u8_t spiffs_cache[(LOG_PAGE_SIZE+32)*4];

static s32_t my_spiffs_read(u32_t addr, u32_t size, u8_t *dst) {
	flash_safe_read(addr, (void *)dst, size);
	return SPIFFS_OK;
}

static s32_t my_spiffs_write(u32_t addr, u32_t size, u8_t *src) {
	flash_safe_write(addr, src, size);
	return SPIFFS_OK;
}


static s32_t my_spiffs_erase(u32_t addr, u32_t size) {
  u32_t sect_first = addr >> 12;
  u32_t sect_last = (addr + size - 1) >> 12;
	while( sect_first <= sect_last ) {
		if(sect_first >= FLASH_SEC_NUM && (sect_first & 15) == 0 && (sect_first + 15) <= sect_last) {
			NODE_DBG("fs.erase_blk: 0x%x\n", sect_first);
			if(flash_safe_erase_block(sect_first>>4) != 0) return SPIFFS_ERR_INTERNAL;
			sect_first += 1<<4;
		}
		else {
			NODE_DBG("fs.erase_sec: 0x%x\n", sect_first);
			if(flash_safe_erase_sector(sect_first) != 0 && flash_safe_erase_sector(sect_first) != 0 )
				return SPIFFS_ERR_INTERNAL;
			sect_first++;
		}
		run_sdk_tasks();
	}
	return SPIFFS_OK;
} 

void myspiffs_check_callback(spiffs_check_type type, spiffs_check_report report, u32_t arg1, u32_t arg2){
  // if(SPIFFS_CHECK_PROGRESS == report) return;
  // NODE_ERR("type: %d, report: %d, arg1: %d, arg2: %d\n", type, report, arg1, arg2);
}

/*******************
The W25Q32BV array is organized into 16,384 programmable pages of 256-bytes each. Up to 256 bytes can be programmed at a time. 
Pages can be erased in groups of 16 (4KB sector erase), groups of 128 (32KB block erase), groups of 256 (64KB block erase) or 
the entire chip (chip erase). The W25Q32BV has 1,024 erasable sectors and 64 erasable blocks respectively. 
The small 4KB sectors allow for greater flexibility in applications that require data and parameter storage. 

********************/

void spiffs_mount() {
  spiffs_config cfg;
	if(flash_size <= INTERNAL_FLASH_SIZE) {
		cfg.phys_addr = ((uint32_t)_flash_used_end + SPI_FLASH_SEC_SIZE - INTERNAL_FLASH_START_ADDRESS) & ( ~(SPI_FLASH_SEC_SIZE - 1));
		cfg.phys_size = (SYS_PARAM_SEC_START * SPI_FLASH_SEC_SIZE) - cfg.phys_addr;
	}
	else {
		cfg.phys_addr = INTERNAL_FLASH_SIZE;
		cfg.phys_size = flash_size - INTERNAL_FLASH_SIZE;
	}
	cfg.phys_erase_block = SPI_FLASH_SEC_SIZE;
	cfg.log_block_size = SPI_FLASH_SEC_SIZE;
	cfg.log_page_size = LOG_PAGE_SIZE;
	cfg.hal_read_f = my_spiffs_read;
	cfg.hal_write_f = my_spiffs_write;
	cfg.hal_erase_f = my_spiffs_erase;

	NODE_DBG("fs.start: 0x%08x, size: %u\n",cfg.phys_addr,cfg.phys_size);
	int res = SPIFFS_mount(&fs,
			&cfg,
			spiffs_work_buf,
			spiffs_fds,
			sizeof(spiffs_fds),
			spiffs_cache,
			sizeof(spiffs_cache),
			// myspiffs_check_callback);
			0);
	NODE_DBG("mount res: %d\n", res);
}

// FS formatting function
// Returns 1 if OK, 0 for error
int myspiffs_format( void )
{
  SPIFFS_unmount(&fs);
  u32_t start, size;
	if(flash_size <= INTERNAL_FLASH_SIZE) {
		start = ((uint32_t)_flash_used_end + SPI_FLASH_SEC_SIZE - INTERNAL_FLASH_START_ADDRESS) & ( ~(SPI_FLASH_SEC_SIZE - 1));
		size = (SYS_PARAM_SEC_START*SPI_FLASH_SEC_SIZE) - start;
	}
	else {
		start = INTERNAL_FLASH_SIZE;
		size = flash_size - INTERNAL_FLASH_SIZE;
	}
	NODE_DBG("addr_first: 0x%08x, size: %u\n", start, size);
	if (my_spiffs_erase(start, size) != SPIFFS_OK) return 0;
	spiffs_mount();
	return 1;
}

int myspiffs_check( void )
{
  // ets_wdt_disable();
  // int res = (int)SPIFFS_check(&fs);
  // ets_wdt_enable();
  // return res;
}

int myspiffs_open(const char *name, int flags){
  return (int)SPIFFS_open(&fs, (char *)name, (spiffs_flags)flags, 0);
}

int myspiffs_close( int fd ){
  SPIFFS_close(&fs, (spiffs_file)fd);
  return 0;
}

size_t myspiffs_write( int fd, const void* ptr, size_t len ){
#if 0
  if(fd==c_stdout || fd==c_stderr){
    uart0_tx_buffer((u8_t*)ptr, len);
    return len;
  }
#endif
  int res = SPIFFS_write(&fs, (spiffs_file)fd, (void *)ptr, len);
  if (res < 0) {
    NODE_DBG("write errno %i\n", SPIFFS_errno(&fs));
    return 0;
  }
  return res;
}
size_t myspiffs_read( int fd, void* ptr, size_t len){
  int res = SPIFFS_read(&fs, (spiffs_file)fd, ptr, len);
  if (res < 0) {
    NODE_DBG("read errno %i\n", SPIFFS_errno(&fs));
    return 0;
  }
  return res;
}
int myspiffs_lseek( int fd, int off, int whence ){
  return SPIFFS_lseek(&fs, (spiffs_file)fd, off, whence);
}
int myspiffs_eof( int fd ){
  return SPIFFS_eof(&fs, (spiffs_file)fd);
}
int myspiffs_tell( int fd ){
  return SPIFFS_tell(&fs, (spiffs_file)fd);
}
int myspiffs_getc( int fd ){
  unsigned char c = 0xFF;
  int res;
  if(!myspiffs_eof(fd)){
    res = SPIFFS_read(&fs, (spiffs_file)fd, &c, 1);
    if (res != 1) {
      NODE_DBG("getc errno %i\n", SPIFFS_errno(&fs));
      return (int)EOF;
    } else {
      return (int)c;
    }
  }
  return (int)EOF;
}
int myspiffs_ungetc( int c, int fd ){
  return SPIFFS_lseek(&fs, (spiffs_file)fd, -1, SEEK_CUR);
}
int myspiffs_flush( int fd ){
  return SPIFFS_fflush(&fs, (spiffs_file)fd);
}
int myspiffs_error( int fd ){
  return SPIFFS_errno(&fs);
}
void myspiffs_clearerr( int fd ){
  SPIFFS_clearerr(&fs);
}
int myspiffs_rename( const char *old, const char *newname ){
  return SPIFFS_rename(&fs, (char *)old, (char *)newname);
}
size_t myspiffs_size( int fd ){
  return SPIFFS_size(&fs, (spiffs_file)fd);
}

#ifdef DEVELOP_VERSION
void test_spiffs() {
  char buf[20];
  
  // Surely, I've mounted spiffs before entering here
  // myspiffs_format();
  
  spiffs_file fd = SPIFFS_open(&fs, "init.lua", SPIFFS_CREAT | SPIFFS_TRUNC | SPIFFS_RDWR, 0);
  if (SPIFFS_write(&fs, fd, (u8_t *)"print(node.heap());\n", 20) < 0) NODE_DBG("errno %i\n", SPIFFS_errno(&fs));
  SPIFFS_close(&fs, fd); 
  fd = SPIFFS_open(&fs, "init.lua", SPIFFS_RDWR, 0);
  if (SPIFFS_read(&fs, fd, (u8_t *)buf, 20) < 0) NODE_DBG("errno %i\n", SPIFFS_errno(&fs));
  SPIFFS_close(&fs, fd);

  NODE_DBG("%s", buf);
}
#endif
