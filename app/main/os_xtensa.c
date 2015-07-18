/*
 * FileName: os_xtenca.c
 * Description: Alternate SDK (libwpa.a)
 * (c) pvvx 2015
 */

#include "user_config.h"
#include "bios.h"

/*
char * ICACHE_FLASH_ATTR ets_strdup(char * s)
{
	char * pret = (0);
	if(s != (0)) {
		uint32 len = ets_strlen(s) + 1;
		pret = (char *)pvPortMalloc(len);
		if(pret != (0)) ets_memcpy(pret, s, len);
	}
	return pret;
}
*/

uint32 ICACHE_FLASH_ATTR os_get_time(void)
{
	return 0;
}

extern uint32 phy_get_rand(void); // return *((uint32 *)(&g_phyFuns+0x290)) ^ *((uint32 *)0x3FF20E44);

uint32 ICACHE_FLASH_ATTR r_rand(void)
{
	return phy_get_rand();
}

uint32 ICACHE_FLASH_ATTR os_random(void)
{
	return phy_get_rand();
}

/* os_get_random - Get cryptographically strong pseudo random data :
 * Buffer for pseudo random data :
 * Length of the buffer Returns:
 * 0 on success, -1 on failure  */
int ICACHE_FLASH_ATTR os_get_random(unsigned char *  buf,  size_t  len)
{
	if(buf != NULL) {
		while(len) {
			uint32 z = phy_get_rand();
			do {
				*buf++ = (char)z;
				z >>= 8;
			} while(--len & 3);
		}
	}
	return 0;
}

/*
Name: strrchr
Prototype: char * strrchr (const char *string, int c)
Description:
The function strrchr is like strchr, except that it searches backwards
 from the end of the string string (instead of forwards from the
 front).
For example,
         strrchr ("hello, world", 'l')
              "ld"	*/
const char * ICACHE_FLASH_ATTR ets_strrchr(const char *string, unsigned char c)
{
	char * pret = NULL;
	if(string != NULL) {
		char ch;
		do {
			ch = *string;
			if(ch == c) pret = (char *)string;
			string ++;
		} while(ch != 0);
	}
	return pret;
}

/*
 Name: strcat
Prototype: char * strcat (char *restrict to, const char *restrict from)
Description:
The strcat function is similar to strcpy, except that the characters from from are concatenated or appended to the
 end of to, instead of overwriting it. That is, the first character from from overwrites the null character marking the
 end of to.

An equivalent definition for strcat would be:


         char *
         strcat (char *restrict to, const char *restrict from)
         {
           strcpy (to + strlen (to), from);
           return to;
         }


This function has undefined results if the strings overlap.
 */
char * ICACHE_FLASH_ATTR ets_strcat(char * to, const char * from)
{
	while(*to++ != '\0');
	while((*to++ = *from++) != '\0');
}

// ets_strchr

