/*
 * c_string.h
 *
 * Definitions for memory and string functions.
 */

#ifndef _C_STRING_H_
#define	_C_STRING_H_
#include "c_stddef.h"
#include "osapi.h"

#ifndef NULL
#define NULL 0
#endif

#define c_memcmp ets_memcmp
#define c_memcpy ets_memcpy
#define c_memset ets_memset

extern const char * rom_strchr(const char * ps, char c);
extern const char * ets_strrchr(const char *string, unsigned char c);
extern const char *ets_strstr(const char *haystack, const char *needle);


#define c_strcat ets_strcat
#define c_strchr rom_strchr
#define c_strrchr ets_strrchr
#define c_strcmp ets_strcmp
#define c_strcpy ets_strcpy
#define c_strlen ets_strlen
#define c_strncmp ets_strncmp
#define c_strncpy ets_strncpy
#define c_strncasecmp ets_strncmp
#define c_strstr ets_strstr
#define c_strncat strncat
#define c_strcspn strcspn
#define c_strpbrk strpbrk
#define c_strcoll strcoll

// const char *c_strstr(const char * __s1, const char * __s2);
// char *c_strncat(char * __restrict /*s1*/, const char * __restrict /*s2*/, size_t n);
// size_t c_strcspn(const char * s1, const char * s2);
// const char *c_strpbrk(const char * /*s1*/, const char * /*s2*/);
// int c_strcoll(const char * /*s1*/, const char * /*s2*/);

#endif /* _C_STRING_H_ */
