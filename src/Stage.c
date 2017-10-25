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

static rv_Renderer *r;

static rv_CrewStatus StageDestroy(rv_Crew *c) {
	rv_Stage *stage = (rv_Stage *) c->attr;

	rv_RendererFree(r);
	
	rv_ShadersDestroy();
	rv_LuaDestroy();
	sqlite3_close(stage->sqlite);

	SDL_GL_DeleteContext(stage->context);
	SDL_DestroyWindow(stage->window);
	SDL_Quit();

	return rv_CUT;
}

const int rv_FPS = 1000 / 60;
static Uint32 start = 0;

static rv_CrewStatus UpdateStage(rv_Crew *c) {
	rv_Stage *stage = (rv_Stage *) c->attr;

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) return rv_EXIT;
	}

	float i;
	for (i = 0; i < 512; i++) {
		float offset = 1 - (i / 512);
		rv_Quad quad;
		quad.vectors[rv_QPOLY1START] = (rv_Vectors) { -offset, offset, 0.0f, 0.0f, 0.0f };
		quad.vectors[rv_QPOLY1MID] = (rv_Vectors) { offset,  offset, 0.0f,  1.0f, 0.0f };
		quad.vectors[rv_QPOLY1END] = (rv_Vectors) { offset, -offset, 0.0f, 1.0f, 1.0f };

		quad.vectors[rv_QPOLY2START] = quad.vectors[rv_QPOLY1END];
		quad.vectors[rv_QPOLY2MID] = (rv_Vectors) { -offset, -offset, 0.0f, 0.0f, 1.0f};
		quad.vectors[rv_QPOLY2END] = quad.vectors[rv_QPOLY1START];
		rv_RendererAdd(r, quad);
	}

	rv_RendererDraw(r);

	if (rv_FPS > (SDL_GetTicks() - start))
		SDL_Delay(rv_FPS - (SDL_GetTicks() - start));

	SDL_GL_SwapWindow(stage->window);
	start = SDL_GetTicks();

	return rv_LIVE;
}

static rv_Bool WindowInit(rv_Stage *stage) {
	if (SDL_Init( SDL_INIT_VIDEO ) < 0) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: SDL_Init Failed");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	stage->window = SDL_CreateWindow("reve", 0, 0, rv_STAGE_WIDTH, rv_STAGE_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!stage->window)  rv_Panic(rv_EGL,SDL_GetError());

	stage->context = SDL_GL_CreateContext(stage->window);

	if (!stage->context) rv_Panic(rv_EGL,SDL_GetError());

	if (SDL_GL_SetSwapInterval(1) < 0) rv_Panic(rv_EGL,SDL_GetError());

	glewExperimental = GL_TRUE;

	glewInit();
	GLenum error;
	for (error = glGetError(); error != GL_NO_ERROR; error = glGetError()) {
		if (error != GL_INVALID_ENUM) rv_Panic(rv_EGL, "error on glew init");
	}

	return rv_YES;
}

static GLuint vao;

/* The init function/type for the STAGE */
rv_CrewStatus rv_STAGE(rv_Crew *c) {
	c->tag = "STAGE";
	c->type = rv_STAGE;
	c->destroy = StageDestroy;
	c->update = UpdateStage;

	stage = calloc(1, sizeof(rv_Stage));
	if (!stage) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: Could not allocate stage struct.");
	c->attr = stage;

	WindowInit(stage);
	ilInit();

	stage->sqlite = SQLiteInit("./slot1.db");
	rv_LuaInit();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLint err = glGetError(); if (err) rv_Panic(-1, "102");

	r = rv_RendererNew(128);

	rv_ShadersInit();
	err = glGetError(); if (err) rv_Panic(-1, "108");
	rv_Texture *t = rv_TextureNew("./myke.png");
	glBindTexture(GL_TEXTURE_2D, t->texture);

	err = glGetError(); if (err) rv_Panic(-1, "126");
	rv_CrewNew(rv_PLAYER);

	return rv_LIVE;
}
