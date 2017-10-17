#define GLEW_STATIC
#include <GL/glew.h>
#include <IL/il.h>
#include <IL/ilu.h>

#ifdef __linux
#include <SDL2/SDL.h>
#include <SDL_opengl.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_opengl.h>
#endif

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

SDL_Window *window = NULL;
SDL_GLContext *context = NULL;

void panic(const char *msg) {
	fputs(msg, stderr);
	exit(-1);
}

GLuint TextureLoad(const char *path) {
	ILuint img = 0;
	ilGenImages(1, &img);
	ilBindImage(img);

	ilLoadImage(path);

	if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) panic("image not converted");

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	GLuint width = ilGetInteger(IL_IMAGE_WIDTH), height = ilGetInteger(IL_IMAGE_HEIGHT);
	GLuint *pixels = (GLuint *) ilGetData();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return texture;

	return 0;
}

int initGL() {
	glViewport(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT);

	GLenum error = GL_NO_ERROR;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);

	error = glGetError();
	if (error != GL_NO_ERROR) panic("OpenGL failed to initialise.");

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	error = glGetError();
	if (error != GL_NO_ERROR) panic("OpenGL failed to initialise.");

	glClearColor(0.f, 1.f, 0.f, 1.f);
	glEnable(GL_TEXTURE_2D);

	return -1;
}

GLuint tex;
int init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) panic(SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	window = SDL_CreateWindow("opengl test", 0, 0, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!window)  panic(SDL_GetError());

	context = SDL_GL_CreateContext(window);

	if (!context) panic(SDL_GetError());

	if (SDL_GL_SetSwapInterval(1) < 0) panic(SDL_GetError());

	glewExperimental = GL_TRUE;

	glewInit();
	GLenum error;
	for (error = glGetError(); error != GL_NO_ERROR; error = glGetError()) {
		if (error != GL_INVALID_ENUM) panic(gluErrorString(error));
	}

	return -1;
}

void render() {
	SDL_GL_SwapWindow(window);
}

const char *FRAGMENT_SRC =
	"#version 330 core \n"
	"in vec3 Color; \n"
	"in vec2 Texcoord \n;"
	"out vec4 outColor; \n"
	"uniform sampler2D tex;"
	"void main() \n"
	"{ \n "
		" outColor = texture(tex, Texcoord) * vec4(Color, 1.0);\n"
	"}";

const char *VERTEX_SRC =
	"#version 330 core \n"
	"in vec2 position; \n"
	"in vec3 color; \n"
	"in vec2 texcoord; \n"
	"out vec3 Color; \n"
	"out vec2 Texcoord; \n"
	"void main() \n"
	"{ \n"
		 "Color = color; \n"
	 	"Texcoord = texcoord; \n"
	 	"gl_Position = vec4(position, 0.0, 1.0); \n"
	"} \n";

GLuint CompileShader(const char *src, GLenum type) {
	GLuint shader = glCreateShader(type);
   	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status) {
		const GLuint LENGTH = 1024;

		char msg[LENGTH];
		glGetShaderInfoLog(shader, LENGTH, NULL, msg);
		fprintf(stderr, "%s\n", src);
		panic(msg);
	}

	return shader;
}

int main(int argc, char *argv[]) {
	init();
	ilInit();
	iluInit();

	// create vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// create vertex buffer object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	float vertices[] = {
		// position   red   green blue  tex coords
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
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
	GLuint vertexShader = CompileShader(VERTEX_SRC, GL_VERTEX_SHADER);

	// Create and compile the fragment shader
	GLuint fragmentShader = CompileShader(FRAGMENT_SRC, GL_FRAGMENT_SHADER);

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
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 7), 0);


	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*) (2 * sizeof(float)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

	GLuint texture = TextureLoad("./myke.png");
	SDL_Event event;
	while (event.type != SDL_QUIT) {
		vertices[0] += 0.01f;
		vertices[7] += 0.01f;
		vertices[14] += 0.01f;
		vertices[21] += 0.01f;

//		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) return 0;
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glClear(GL_COLOR_BUFFER_BIT);

//		glUniform3f(uniColor, 0.f, alpha, 0.f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		render();
	}

	glDeleteTextures(1, &tex);
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
    	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &ebo);
    	glDeleteBuffers(1, &vbo);

    	glDeleteVertexArrays(1, &vao);

	SDL_GL_DeleteContext(context);
	SDL_Quit();
}
