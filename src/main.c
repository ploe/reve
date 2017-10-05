#include "reve.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "sqlite3.h"

typedef struct {
	int *map;
	SDL_Surface *surface;
	rv_Bool enabled;
} rv_Layer;

void SQLLoad() {
	sqlite3 *db;
	char *zErrMsg = NULL;
	int rc;

	rc = sqlite3_open("slot1.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}

	rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS mmmmm(id PRIMARY KEY NOT NULL, key TEXT, value TEXT)", NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}

	sqlite3_close(db);
}

int main(int argc, char *argv[]) {
	lua_State *L = NULL;
	L = luaL_newstate();
	luaL_openlibs(L);

	int i;
	for (i = 1; i < argc; i++) {
		if (luaL_loadfile(L, argv[i]) || lua_pcall(L, 0, 0, 0)) {
			fprintf(stderr, "%s\n", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}
	lua_close(L);

	SQLLoad();

	/* All the world's a stage,	*/
	rv_CrewNew(rv_STAGE);

	while (rv_CrewRoll()) continue;	
	rv_CrewPurge();
	/* Sans teeth, sans eyes, sans taste, sans everything. */

	return 0;
}
