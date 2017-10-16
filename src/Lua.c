#include "reve.h"

static lua_State *L = NULL;

lua_State *LuaInit() {
	L = luaL_newstate();
	if (!L) rv_Panic(rv_ELUA, "LuaInit failed.");

	luaL_openlibs(L);
	rv_PersistLuaBindings();

	rv_Bool bootstrap = rv_LuaImport(L, "bootstrap");
	if (!bootstrap) {
		lua_close(L);
		rv_Panic(rv_ELUA, "cannot open bootstrap: No such file or directory");
	}

	return L;
}

rv_Bool rv_LuaBind(lua_State *L, const char *key, lua_CFunction func) {
	enum {
		TABLE_GET = -1,
		TABLE_SET = -2,
		NIL = 1,
	};

	lua_getglobal(L, "rv");
	if (lua_isnil(L, TABLE_GET)) {
		lua_pop(L, NIL);
		lua_newtable(L);
	}

	if (lua_istable(L, TABLE_GET)) {
		lua_pushcfunction(L, func);
		lua_setfield(L, TABLE_SET, key);
		lua_setglobal(L, "rv");
	}
	else rv_Panic(rv_ELUA, "namespace 'rv' is already set in Lua state.");

	return rv_YES;
}

rv_Bool rv_LuaImport(lua_State *L, const char *slug) {
	const char *extensions[] = {
		".luac",
		".lua",
		"",
		NULL,
	};

	const char **ext = NULL;
	for (ext = extensions; *ext != NULL; ext++) {
		rv_Text file = rv_TextNew("./%s%s", slug, *ext);
		puts(file);
		if (luaL_loadfile(L, file) || lua_pcall(L, 0, 0, 0)) {
			file = rv_TextFree(file);
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
			continue;
		}
		else {
			file = rv_TextFree(file);
			break;
		}
	}

	return (*ext != NULL) ? rv_YES : rv_NO;
}

lua_State *rv_LuaGet() {
	return L;
}

rv_Bool rv_LuaDestroy() {
	if (L) {
		lua_close(L);
		L = NULL;
		return rv_YES;	
	}

	return rv_NO;
}
