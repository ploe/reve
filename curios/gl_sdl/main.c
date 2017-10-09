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
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
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


int main(int argc, char *argv[]) {
	init();

	SDL_Event event;
	while (event.type != SDL_QUIT) {
		render();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) return 0;
		}
		
	}

	SDL_GL_DeleteContext(context);
	SDL_Quit();
}
