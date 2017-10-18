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
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(stage->window);

	return rv_LIVE;
}

static rv_Bool WindowInit(rv_Stage *stage) {
	if (SDL_Init( SDL_INIT_VIDEO ) < 0) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: SDL_Init Failed");

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	stage->window = SDL_CreateWindow("reve", 0, 0, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!stage->window)  rv_Panic(rv_EGL,SDL_GetError());

	stage->context = SDL_GL_CreateContext(stage->window);

	if (!stage->context) rv_Panic(rv_EGL,SDL_GetError());

	if (SDL_GL_SetSwapInterval(1) < 0) rv_Panic(rv_EGL,SDL_GetError());

	glewExperimental = GL_TRUE;

	glewInit();
	GLenum error;
	for (error = glGetError(); error != GL_NO_ERROR; error = glGetError()) {
		if (error != GL_INVALID_ENUM) rv_Panic(rv_EGL,gluErrorString(error));
	}

	return rv_YES;
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

	// create vertex buffer object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	float vertices[] = {
		-0.5f,  0.5f,
     		0.5f,  0.5f,
     		0.5f, -0.5f,
    		-0.5f, -0.5f
	};

	// copy data in to it
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

 	// Create and compile the vertex shader

	// Create and compile the fragment shader
	GLuint fragmentShader = rv_ShaderLoad("./shaders/default.frag", GL_FRAGMENT_SHADER);

	GLuint vertexShader = rv_ShaderLoad("./shaders/default.vert", GL_VERTEX_SHADER);
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
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 2), 0);

	return 0;
}
