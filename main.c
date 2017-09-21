#include "reve.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

typedef struct {
	int *map;
	SDL_Surface *surface;
	rv_Bool enabled;
} rv_Layer;

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

	/* All the world's a stage,	*/
	rv_CrewNew(rv_STAGE);

	while (rv_CrewRoll()) continue;	
	rv_CrewPurge();
	/* Sans teeth, sans eyes, sans taste, sans everything. */

	return 0;
}
