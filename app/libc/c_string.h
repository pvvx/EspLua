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
#define c_memmove ets_memmove
#define c_memset ets_memset
#define c_putc ets_putc
#define c_str2macaddr ets_str2macaddr
#define c_strcat strcat
#define c_strchr strchr
#define c_strcmp ets_strcmp
#define c_strcpy ets_strcpy
#define c_strlen ets_strlen
#define c_strncmp ets_strncmp
#define c_strncpy ets_strncpy
//#define c_strstr ets_strstr
#define c_strncasecmp c_strncmp

#define c_strstr  strstr
#define c_strncat strncat
#define c_strcspn strcspn
#define c_strpbrk strpbrk
#define c_strcoll strcoll
#define c_strrchr strrchr

// const char *c_strstr(const char * __s1, const char * __s2);
// char *c_strncat(char * __restrict /*s1*/, const char * __restrict /*s2*/, size_t n);
// size_t c_strcspn(const char * s1, const char * s2);
// const char *c_strpbrk(const char * /*s1*/, const char * /*s2*/);
// int c_strcoll(const char * /*s1*/, const char * /*s2*/);

#endif /* _C_STRING_H_ */
