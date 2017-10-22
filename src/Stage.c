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

	SDL_GL_DeleteContext(stage->context);
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

	glClearColor(0.f, 1.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	SDL_GL_SwapWindow(stage->window);

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

typedef struct {
	GLfloat x, y, z, u, v;
} rv_Vectors;

typedef struct {
	rv_Vectors vectors[6];
} rv_Quad;
#define rv_QTOPLEFT 0
#define rv_QTOPRIGHT 1
#define rv_QBOTTOMRIGHT 2

#define rv_POLY2START 3
#define rv_QBOTTOMLEFT 4
#define rv_POLY2END 5

#define rv_VERTEX_OFFSET 0
#define rv_TEXTURE_OFFSET ((const void *) (sizeof(GLfloat) * 3))

typedef struct {
	float x, y, w, h, u, v;
	const char *texture;
} rv_Actor;

static ish_Map *actors = NULL;
static GLuint vbo;

rv_CrewStatus rv_ActorsUpdate(rv_Crew *c) {
	return rv_LIVE;
}

rv_Quad rv_ActorQuad(rv_Actor *a) {
	rv_Quad quad;

	GLfloat top = a->y, bottom = a->y + a->h, left = a->x, right = a->x + a->w, depth=bottom;

	quad.vectors[rv_QTOPLEFT] = (rv_Vectors) {
		left,  
		top, 
		depth, 
		0.0f, 
		0.0f
	};

	quad.vectors[rv_QTOPRIGHT] = (rv_Vectors) {
		right,
		top,
		depth,
		1.0f,
		0.0f
	};

	quad.vectors[rv_QBOTTOMRIGHT] = (rv_Vectors) {
		right, 
		bottom, 
		depth, 
		1.0f, 
		1.0f
	};

	quad.vectors[rv_POLY2START] = quad.vectors[rv_QBOTTOMRIGHT];

	quad.vectors[rv_QBOTTOMLEFT] = (rv_Vectors) {
		left, 
		bottom,
		depth, 
		0.0f, 
		1.0f
	};

	quad.vectors[rv_POLY2END] = quad.vectors[rv_QTOPLEFT];

	return quad;
}

rv_Actor *rv_ActorNew() {
	rv_Actor *a = calloc(1, sizeof(rv_Actor));
	return a;
}

rv_CrewStatus rv_ACTORS(rv_Crew *c) {
	c->update = rv_ActorsUpdate;
	
	actors = ish_MapNew();

	rv_Actor *myke = rv_ActorNew();
	*myke = (rv_Actor) {0, 0, 100, 100, 1.0f, 1.0f};

	rv_Actor *vix = rv_ActorNew();
	*vix = (rv_Actor) {0, 80, 100, 100, 1.0f, 1.0f};
	
	rv_Quad quad[2]; 
	quad[0] = rv_ActorQuad(myke);
	quad[1] = rv_ActorQuad(vix);

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad)*2, &quad, GL_STATIC_DRAW);

	return rv_CUT;
}

/* The init function/type for the STAGE */
rv_CrewStatus rv_STAGE(rv_Crew *c) {
	c->tag = "STAGE";
	c->type = rv_STAGE;
	c->destroy = DestroyStage;
	c->update = UpdateStage;

	stage = calloc(1, sizeof(rv_Stage));
	if (!stage) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: Could not allocate stage struct.");
	c->attr = stage;

	WindowInit(stage);
	ilInit();

	stage->sqlite = SQLiteInit("./slot1.db");
	rv_LuaInit();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	rv_CrewNew(rv_ACTORS);

 	// Create and compile the vertex shader
	GLuint vertexShader = rv_ShaderLoad("./shaders/default.vert", GL_VERTEX_SHADER);

	// Create and compile the fragment shader
	GLuint fragmentShader = rv_ShaderLoad("./shaders/default.frag", GL_FRAGMENT_SHADER);

	// link vertex and fragment shader in to a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// specify layout of vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(rv_Vectors), rv_VERTEX_OFFSET);

	GLint texcoord = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texcoord);

	glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(rv_Vectors), rv_TEXTURE_OFFSET);

	GLint iota = glGetUniformLocation(shaderProgram, "iota");
	glUniform3f(iota, 2.0 / rv_STAGE_WIDTH, 2.0 / rv_STAGE_HEIGHT, 1.0 / rv_STAGE_HEIGHT);

	rv_TextureLoad("./myke.png");

	rv_CrewNew(rv_PLAYER);

	return rv_LIVE;
}
