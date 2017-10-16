#include "reve.h"

static rv_Stage *stage = NULL;

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
	rv_LuaDestroy();
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
	rv_LuaInit();

	tile = rv_TileNew("test", "overworld.png", clip, stage);

	rv_CrewNew(rv_PLAYER);

	return 0;
}
