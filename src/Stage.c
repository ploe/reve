#include "reve.h"

static rv_Stage *stage = NULL;

lua_State *LuaInit() {
	lua_State *L = NULL;

	L = luaL_newstate();
	if (!L) rv_Panic(-1, "LuaInit failed.");

	luaL_openlibs(L);
	rv_PersistLuaBindings();

	rv_Bool bootstrap = rv_StageLuaImport(L, "bootstrap");
	if (!bootstrap) {
		lua_close(L);
		rv_Panic(-1, "cannot open bootstrap: No such file or directory");
	}

	return L;
}

rv_Bool rv_StageLuaBind(lua_State *L, const char *key, lua_CFunction func) {
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
	else rv_Panic(-1, "rv namespace is already set to something else in Lua?");

	return rv_YES;
}

rv_Bool rv_StageLuaImport(lua_State *L, const char *slug) {
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

lua_State *rv_StageGetLua() {
	return stage->lua;
}

sqlite3 *SQLiteInit(const char *path) {
	sqlite3 *db;
	int rc;

	rc = sqlite3_open(path, &db);
	if (rc != SQLITE_OK) {
		rv_Panic(-1, sqlite3_errmsg(db));
		sqlite3_close(db);
	}

	return db;
}

sqlite3 *rv_StageGetSQLite() {
	return stage->sqlite;
}

/* Destroy function for the STAGE - the return value isn't important */
static rv_CrewStatus DestroyStage(rv_Crew *c) {
	rv_Stage *stage = (rv_Stage *) c->attr;
	lua_close(stage->lua);
	sqlite3_close(stage->sqlite);

	SDL_DestroyRenderer(stage->renderer);
	SDL_DestroyWindow(stage->window);
	SDL_Quit();

	return rv_CUT;
}

rv_Tile *tile = NULL;

/* Update method for the STAGE - called every frame */
static rv_CrewStatus UpdateStage(rv_Crew *c) {
	rv_Stage *stage = (rv_Stage *) c->attr;

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) return rv_EXIT;
	}

	SDL_RenderClear(stage->renderer);
	rv_TileDraw(tile, stage);
	SDL_RenderPresent(stage->renderer);


	return rv_LIVE;
}

SDL_Texture *rv_LoadTexture(char *src, SDL_Renderer *renderer) {
	SDL_Surface *surface = IMG_Load(src);
	if (!surface) rv_Panic(-1, IMG_GetError());

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) rv_Panic(-1, SDL_GetError());

	SDL_FreeSurface(surface);

	return texture;
}

/* The init function/type for the STAGE */
rv_CrewStatus rv_STAGE(rv_Crew *c) {
	c->tag = "STAGE";
	c->type = rv_STAGE;
	c->destroy = DestroyStage;
	c->update = UpdateStage;

	if (SDL_Init( SDL_INIT_VIDEO ) < 0) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: SDL_Init Failed");

	stage = calloc(1, sizeof(rv_Stage));
	if (!stage) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: Could not allocate stage struct.");
	c->attr = stage;

	stage->window = SDL_CreateWindow("reve", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 360, SDL_WINDOW_SHOWN);
	if (!stage->window) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: Failed to create STAGE window.");

	stage->renderer = SDL_CreateRenderer(stage->window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(stage->renderer, 255, 0, 128, 255);

	IMG_Init(IMG_INIT_PNG);
	SDL_Rect clip = {0, 0, 100, 100};

	stage->sqlite = SQLiteInit("./slot1.db");
	stage->lua = LuaInit();
	
	tile = rv_TileNew("test", "overworld.png", clip, stage);

	rv_CrewNew(rv_PLAYER);

	return 0;
}
