#ifndef _REVE_PERSIST_H
#define _REVE_PERSIST_H

typedef int (*SQLiteCallback)(void*,int,char**,char**);

rv_Bool rv_PersistLuaBindings();

#endif
