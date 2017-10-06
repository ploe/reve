#include "reve.h"

static rv_Stage *stage = NULL;

lua_State *LuaInit() {
	lua_State *L = NULL;

	L = luaL_newstate();
	luaL_openlibs(L);

	const char *bootfiles[] = {
		"main",
		"main.luac",
		"main.lua",
		NULL,
	};

	const char **file = NULL;
	for (file = bootfiles; file != NULL; file++) {
		if (luaL_loadfile(L, *file) || lua_pcall(L, 0, 0, 0)) {
			lua_pop(L, 1);
			continue;
		}
		break;
	}

	if (file == NULL) {
		lua_close(stage->lua);
		rv_Panic(-1, "cannot open [main, main.luac, main.lua]: No such file or directory");
	}

	return L;
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

/* 	must remember to free the marshalled char 	*/
static char *MarshalStage(rv_Crew *c) {
	return rv_Format("\"%s\":{status:\"%s\"}",
		c->tag,
		rv_CrewStatusStr[c->status]
	);
}

rv_Texture *chomp = NULL;
rv_Tile *tile = NULL;

/* Update method for the STAGE - called every frame */
static rv_CrewStatus UpdateStage(rv_Crew *c) {
	rv_Stage *stage = (rv_Stage *) c->attr;

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) return rv_EXIT;
	}

	SDL_RenderClear(stage->renderer);
	SDL_Rect r;
	r.w= r.h = 64;
	r.x = r.y = 0;
	static double i = 0;
	SDL_RenderCopyEx(stage->renderer, chomp->texture, NULL, &r, i+=0.01, NULL, SDL_FLIP_NONE);
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
	c->marshal = MarshalStage;	

	if (SDL_Init( SDL_INIT_VIDEO ) < 0) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: SDL_Init Failed");

	stage = calloc(1, sizeof(rv_Stage));
	if (!stage) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: Could not allocate stage struct.");
	c->attr = stage;

	stage->window = SDL_CreateWindow("reve", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 360, SDL_WINDOW_SHOWN);
	if (!stage->window) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: Failed to create STAGE window.");

	stage->renderer = SDL_CreateRenderer(stage->window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(stage->renderer, 255, 0, 128, 255);

	IMG_Init(IMG_INIT_PNG);
	chomp = rv_TextureNew("./Chomp_Rock-scaled.PNG", stage->renderer);
	SDL_Rect clip = {0, 0, 100, 100};

	stage->lua = LuaInit();
	stage->sqlite = SQLiteInit("./slot1.db");

	tile = rv_TileNew("test", "overworld.png", clip, stage);

	rv_CrewNew(rv_PLAYER);

	return 0;
}
