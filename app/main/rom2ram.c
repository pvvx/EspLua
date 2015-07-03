/*
 * FileName: rom2ram.c
 * Description: Alternate SDK (main.a)
 * (c) pvvx 2015
 */

#include "user_config.h"

void __attribute__((optimize("Os"))) ICACHE_RAM_ATTR copy_s4d1(unsigned char * pd, void * ps, unsigned int len)
{
	union {
		unsigned char uc[4];
		unsigned int ud;
	}tmp;
	unsigned int *p = (unsigned int *)((unsigned int)ps & (~3));

	unsigned int xlen = (unsigned int)ps & 3;
	if(xlen) {
		tmp.ud = *p++;
		while (len)  {
			len--;
			*pd++ = tmp.uc[xlen++];
			if(xlen & 4) break;
		}
	}
	xlen = len >> 2;
	while(xlen) {
		tmp.ud = *p++;
		*pd++ = tmp.uc[0];
		*pd++ = tmp.uc[1];
		*pd++ = tmp.uc[2];
		*pd++ = tmp.uc[3];
		xlen--;
	}
	if(len & 3) {
		tmp.ud = *p;
		pd[0] = tmp.uc[0];
		if(len & 2) {
			pd[1] = tmp.uc[1];
			if(len & 1) {
				pd[2] = tmp.uc[2];
			}
		}
	}
}


void __attribute__((optimize("Os"))) ICACHE_RAM_ATTR copy_s1d4(void * pd, unsigned char * ps, unsigned int len)
{
	union {
		unsigned char uc[4];
		unsigned int ud;
	}tmp;
	unsigned int *p = (unsigned int *)(((unsigned int)pd) & (~3));
	unsigned int xlen = (unsigned int)pd & 3;
	if(xlen) {
		tmp.ud = *p;
		while (len)  {
			len--;
			tmp.uc[xlen++] = *ps++;
			if(xlen & 4) break;
		}
		*p++ = tmp.ud;
	}
	xlen = len >> 2;
	while(xlen) {
		tmp.uc[0] = *ps++;
		tmp.uc[1] = *ps++;
		tmp.uc[2] = *ps++;
		tmp.uc[3] = *ps++;
		*p++ = tmp.ud;
		xlen--;
	}
	if(len & 3) {
		tmp.ud = *p;
		tmp.uc[0] = ps[0];
		if(len & 2) {
			tmp.uc[1] = ps[1];
			if(len & 1) {
				tmp.uc[2] = ps[2];
			}
		}
		*p = tmp.ud;
	}
}

char __attribute__((optimize("Os"))) ICACHE_RAM_ATTR get_rom_chr(const char *ps)
{
	return (*((unsigned int *)((unsigned int)ps & (~3))))>>(((unsigned int)ps & 3) << 3);
}
/*
unsigned int __attribute__((optimize("Os"))) ICACHE_RAM_ATTR rom_strlen(void * ps)
{
	union {
		unsigned char uc[4];
		unsigned int ud;
	}tmp;
	if(ps == 0) return (0);
	unsigned int len = 0;
	unsigned int *p = (unsigned int *)((unsigned int)ps & (~3));
	unsigned int xlen = (unsigned int)ps & 3;
	while(1) {
		tmp.ud = *p;
		do {
			if(tmp.uc[xlen++] == 0) return len;
			len++;
		} while((xlen & 4) == 0);
		xlen = 0;
		p++;
	}
}

const char *__attribute__((optimize("Os"))) ICACHE_RAM_ATTR rom_strchr(const char * ps, char c)
{
	union {
		unsigned char uc[4];
		unsigned int ud;
	}tmp;
	if(ps == 0) return (0);
	unsigned int *p = (unsigned int *)((unsigned int)ps & (~3));
	unsigned int xlen = (unsigned int)ps & 3;
	while(1) {
		tmp.ud = *p;
		do {
			if(tmp.uc[xlen] == c) return (const char *)((unsigned int)p + xlen);
			else if(tmp.uc[xlen] == 0) return (0);
			xlen++;
		} while((xlen & 4) == 0);
		xlen = 0;
		p++;
	}
}


char * __attribute__((optimize("Os"))) ICACHE_RAM_ATTR rom_strcpy(char * pd_, void * ps, unsigned int maxlen)
{
	union {
		unsigned char uc[4];
		unsigned int ud;
	}tmp;
	char * pd = pd_;
	unsigned int *p = (unsigned int *)((unsigned int)ps & (~3));
	*pd = 0;
	if(ps == (0)) return pd_;
	char c;

	unsigned int xlen = (unsigned int)ps & 3;
	if(xlen) {
		tmp.ud = *p++;
		while (maxlen)  {
			maxlen--;
			c = *pd++ = tmp.uc[xlen++];
			if(c == 0) return pd_;
			if(xlen & 4) break;
		}
	}
	xlen = maxlen >> 2;
	while(xlen) {
		tmp.ud = *p++;
		c = *pd++ = tmp.uc[0];
		if(c == 0) return pd_;
		c = *pd++ = tmp.uc[1];
		if(c == 0) return pd_;
		c = *pd++ = tmp.uc[2];
		if(c == 0) return pd_;
		c = *pd++ = tmp.uc[3];
		if(c == 0) return pd_;
		xlen--;
	}
	if(maxlen & 3) {
		tmp.ud = *p;
		c = pd[0] = tmp.uc[0];
		if(c == 0) return pd_;
		if(maxlen & 2) {
			c = pd[1] = tmp.uc[1];
			if(c == 0) return pd_;
			if(maxlen & 1) {
				c = pd[2] = tmp.uc[2];
				if(c == 0) return pd_;
			}
		}
	}
	return pd_;
}

int __attribute__((optimize("Os"))) ICACHE_RAM_ATTR rom_memcmp( void * ps, const char * pd_, unsigned int len)
{
	union {
		unsigned char uc[4];
		unsigned int ud;
	}tmp;
	char * pd = (void *) pd_;
	unsigned int *p = (unsigned int *)((unsigned int)ps & (~3));
	char c;

	unsigned int xlen = (unsigned int)ps & 3;
	if(xlen) {
		tmp.ud = *p++;
		while (len)  {
			len--;
			if(*pd++ != tmp.uc[xlen++]) return -1;
			if(xlen & 4) break;
		}
	}
	xlen = len >> 2;
	while(xlen) {
		tmp.ud = *p++;
		if(*pd++ != tmp.uc[0]) return -1;
		if(*pd++ != tmp.uc[1]) return -1;
		if(*pd++ != tmp.uc[2]) return -1;
		if(*pd++ != tmp.uc[3]) return -1;
		xlen--;
	}
	if(len & 3) {
		tmp.ud = *p;
		if(pd[0] != tmp.uc[0]) return -1;
		if(len & 2) {
			if(pd[1] != tmp.uc[1]) return -1;
			if(len & 1) {
				if(pd[2] != tmp.uc[2]) return -1;
			}
		}
	}
	return 0;
}
*/