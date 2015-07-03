/*
 * rom2ram.h
 * Author: pvvx
 */

#ifndef _INCLUDE_ROM2RAM_H_
#define _INCLUDE_ROM2RAM_H_

#define USE_ROM_STRINGS

extern void copy_s4d1(unsigned char * pd, void * ps, unsigned int len);
extern void copy_s1d4(void * pd, unsigned char * ps, unsigned int len);
extern const char get_rom_chr(const char *s);
/*
extern unsigned int rom_strlen(const char *s);
extern const char * rom_strchr(const char *s, char c);
extern const char * rom_strcpy(char * pd_, void * ps, unsigned int maxlen);
extern int rom_memcmp( void * ps, const char * pd_, unsigned int len);
*/

#endif /* _INCLUDE_ROM2RAM_H_ */
