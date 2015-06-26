/*
** $Id: ldebug.h,v 2.3.1.1 2007/12/27 13:02:25 roberto Exp $
** Auxiliary functions from Debug Interface module
** See Copyright Notice in lua.h
*/

#ifndef ldebug_h
#define ldebug_h


#include "lstate.h"


#define pcRel(pc, p)	(cast(int, (pc) - (p)->code) - 1)

#define getline(f,pc)	(((f)->lineinfo) ? (f)->lineinfo[pc] : 0)

#define resethookcount(L)	(L->hookcount = L->basehookcount)


LUAI_FUNC void luaG_typeerror (lua_State *L, const TValue *o,
                                             const char *opname);
LUAI_FUNC void luaG_concaterror (lua_State *L, StkId p1, StkId p2);
LUAI_FUNC void luaG_aritherror (lua_State *L, const TValue *p1,
                                              const TValue *p2);
LUAI_FUNC int luaG_ordererror (lua_State *L, const TValue *p1,
                                             const TValue *p2);

LUAI_FUNC void luaG_runerror_ (lua_State *L, const char *fmt, ...);
#define luaG_runerror(a, fmt, ...) do {	\
	static const char flash_str[] ICACHE_RODATA_ATTR = fmt;	\
	luaG_runerror_(a, flash_str, ##__VA_ARGS__);	\
	} while(0)

LUAI_FUNC void luaG_errormsg (lua_State *L);
LUAI_FUNC int luaG_checkcode (const Proto *pt);
LUAI_FUNC int luaG_checkopenop (Instruction i);

#endif
