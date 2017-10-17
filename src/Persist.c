#include "reve.h"

static rv_Text ValueText(lua_State *L, int type, int index) {
	rv_Text value = NULL;

	switch (type) {
		case LUA_TNUMBER:
			value = rv_TextNew("%f", lua_tonumber(L, index));
		break;

		case LUA_TBOOLEAN:
			value = rv_TextNew("%s", lua_toboolean(L, index) ? "true" : "false");
		break;

		case LUA_TSTRING:
			value = rv_TextNew("%s", lua_tostring(L, index));
		break;
	}

	return value;
}

static rv_Bool PersistExecCallback(sqlite3 *db, const char *table, rv_Text query, void *probe, SQLiteCallback callback) {
	int rc;
	char *err = NULL;

	rc = sqlite3_exec(db, query, callback, probe, &err);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "%s\n%s\n", query, err);
		rv_TextFree(query);
		rv_Panic(rv_ESQL, "PersistExecCallback failed: killing reve.");
	}

	return rv_YES;
}

#define PersistExec(db, query) PersistExecCallback(db, NULL, query, NULL, NULL)

static char *PERSIST_SCHEMA =
	"CREATE TABLE IF NOT EXISTS %s ("
		"key TEXT PRIMARY KEY, "
		"value TEXT, "
		"type UNSIGNED INT"
	")";

static rv_Bool rv_PersistCreateTable(sqlite3 *db, const char *table) {
	rv_Text schema = rv_TextNew(PERSIST_SCHEMA, table);
	PersistExec(db, schema);
	rv_TextFree(schema);

	return rv_YES;
}

static rv_Bool rv_PersistSavePair(sqlite3 *db, const char *table, const char *key, const char *value, int type) {
	const char *query = "INSERT OR REPLACE INTO '%s' (key, value, type) VALUES(?, ?, ?)";
	rv_Text insert = rv_TextNew(query, table);

	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, insert, -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, value, -1, SQLITE_STATIC);
	sqlite3_bind_int(stmt, 3, type);

	int rc;
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {
		rv_TextFree(insert);
		rv_Panic(rv_ESQL, sqlite3_errmsg(db));
	}

	sqlite3_finalize(stmt);
	rv_TextFree(insert);
	return rv_YES;
}

/* LuaSave(string, table) - Saves the Lua table with the name string */
static int LuaSave(lua_State *L) {
	enum {
		NAME = 1,
		DATA = 2,
		KEY = 3,
		VALUE = 4
	};

	sqlite3 *db = rv_StageGetSQLite();

	if (lua_isstring(L, NAME) && lua_istable(L, DATA)) {
		const char *table = lua_tostring(L, NAME);
		rv_PersistCreateTable(db, table);

		lua_pushnil(L);
		while(lua_next(L, DATA)) {
			const char *key = lua_tostring(L, KEY);
			int type = lua_type(L, VALUE);
			rv_Text value = ValueText(L, type, VALUE);

			rv_PersistSavePair(db, table, key, value, type);
			value = rv_TextFree(value);
			lua_pop(L, 1);
		}

		lua_pushboolean(L, 1);
	}
	else lua_pushboolean(L, 0);

	return 1;
}

static rv_Bool rv_PersistSelectPairs(sqlite3 *db, const char *table, void *probe, SQLiteCallback callback) {
	const char *query = "SELECT key, value, type FROM '%s'";
	rv_Text select = rv_TextNew(query, table);

	PersistExecCallback(db, table, select, probe, callback);

	rv_TextFree(select);
	return rv_YES;
}

int PERSIST_LOAD(void *L, int argc, char **column, char **unused) {
	enum {
		// Lua indices
		DATA = -3,

		// SQL indices
		KEY = 0,
		VALUE = 1,
		TYPE = 2
	};

	const char *key = column[KEY];
	const char *value = column[VALUE];
	int type = atoi(column[TYPE]);

	lua_pushstring(L, key);
	switch (type) {
		case LUA_TNUMBER: {
			lua_pushnumber(L, atof(value));
			break;
		}

		case LUA_TBOOLEAN: {
			rv_Bool b = (strcmp(value, "true") == 0) ? rv_YES : rv_NO;
			lua_pushboolean(L, b);
			break;
		}

		case LUA_TSTRING: {
			lua_pushstring(L, value);
			break;
		}
	}

	lua_settable(L, DATA);

	return 0;
}

static int LuaLoad(lua_State *L) {
	enum {
		NAME = 1
	};

	sqlite3 *db = rv_StageGetSQLite();

	if (lua_isstring(L, NAME)) {
		const char *table = lua_tostring(L, NAME);
		lua_newtable(L);

		rv_PersistSelectPairs(db, table, L, PERSIST_LOAD);
	}

	return 1;
}

rv_Bool rv_PersistLuaBindings() {
	rv_LuaBind (
		"Save", LuaSave,
		"Load", LuaLoad,
		rv_NO, rv_NO
	);

	return rv_YES;
}

static rv_Bool rv_PersistClearTable(sqlite3 *db, const char *table) {
	const char *query = "DROP TABLE IF EXISTS '%s'";
	rv_Text drop = rv_TextNew(query, table);
	PersistExec(db, drop);
	rv_TextFree(drop);
	return rv_YES;
}

static int rv_PERSIST_MARSHAL(void *table, int argc, char **value, char **key) {
	int i;
	for(i=0; i < argc; i++){
		fprintf(stderr, "%s=%s, ", key[i], value[i] ?  : "NULL");
	}

	return 0;
}
