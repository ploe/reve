#define GLEW_STATIC
#include <GL/glew.h>

#ifdef __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_opengl.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
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

int ConvertSurfaceToTexImage2D(SDL_Surface *surface) {
	GLint internal;
	GLenum format;
	GLenum type;

	switch (surface->format->format) {
		case SDL_PIXELFORMAT_ARGB8888:
		case SDL_PIXELFORMAT_RGB888:
			internal = GL_RGBA8;
			format = GL_BGRA;
			type = GL_UNSIGNED_INT_8_8_8_8_REV;
		break;

		case SDL_PIXELFORMAT_YV12:
		case SDL_PIXELFORMAT_IYUV:
		case SDL_PIXELFORMAT_NV12:
		case SDL_PIXELFORMAT_NV21:
			internal = GL_LUMINANCE;
			format = GL_LUMINANCE;
			type = GL_UNSIGNED_BYTE;
		break;

		#ifdef __MACOSX__
		case SDL_PIXELFORMAT_UYVY:
			internal = GL_RGB8;
			format = GL_YCBCR_422_APPLE;
			type = GL_UNSIGNED_SHORT_8_8_APPLE;
   		break;
   		#endif

		default:
			return -1;
		break;
  	}

	glTexImage2D(GL_TEXTURE_2D, 0, internal, surface->w, surface->h, 0, format, type, surface->pixels);
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

SDL_Surface *surface = NULL;
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
//	initGL();


//	surface = IMG_Load("./myke.png");
//	glGenTextures(1, &tex);
//	glBindTexture(GL_TEXTURE_2D, tex);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


//	ConvertSurfaceToTexImage2D(surface);
	return -1;
}

void render() {
//	glClear(GL_COLOR_BUFFER_BIT);
//	glBegin(GL_QUADS);
//		glTexCoord2f( 0.f, 0.f ); glVertex2f(0.f, 0.f);
//		glTexCoord2f( 1.f, 0.f ); glVertex2f(400.f, 0.f);
//		glTexCoord2f( 1.f, 1.f ); glVertex2f(400.f, 400.f);
//		glTexCoord2f( 0.f, 1.f ); glVertex2f(0.f, 400.f);
//	glEnd();

	SDL_GL_SwapWindow(window);
}

const char *VERTEX_SRC =
	"#version 150 core \n"
	"in vec2 position; \n"
	"void main() \n"
	"{ \n"
		"gl_Position = vec4(position, 0.0, 1.0); \n"
	"}";

const char *FRAGMENT_SRC =
	"#version 150 core \n"
	"uniform vec3 triangleColor; \n"
	"out vec4 outColor; \n"
	"void main() { \n"
		"outColor = vec4(triangleColor, 1.0); \n"
	"}";

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

	// create vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// create vertex buffer object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	float vertices[] = {
		0.0f,  0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};

	// copy data in to it
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(posAttrib);

	GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");

	SDL_Event event;
	while (event.type != SDL_QUIT) {

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) return 0;
		}
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		static float alpha = -0.1f;
		static float fade = 0.01;
		if ((alpha >= 1.0f) || (alpha <= -1.0f)) fade = -fade;
		alpha += fade;

		glUniform3f(uniColor, 0.f, alpha, 0.f);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		render();
	}

	SDL_GL_DeleteContext(context);
	SDL_Quit();
}
