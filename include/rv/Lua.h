#ifndef _REVE_LUA_H
#define _REVE_LUA_H

lua_State *LuaInit();
rv_Bool rv_LuaImport(const char *file);
lua_State *rv_LuaGet();
rv_Bool rv_LuaBind(const char *key, lua_CFunction func, ...);
rv_Bool rv_LuaDestroy();

#endif
