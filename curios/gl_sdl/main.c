#include "SDL.h"
#include "SDL_opengl.h"
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

SDL_Window *window = NULL;
SDL_GLContext *context = NULL;

void panic(const char *msg) {
	fputs(msg, stderr);
	exit(-1);
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

int init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) panic(SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	window = SDL_CreateWindow("opengl test", 0, 0, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!window)  panic(SDL_GetError());

	context = SDL_GL_CreateContext(window);

	if (!context) panic(SDL_GetError());

	if (SDL_GL_SetSwapInterval(1) < 0) panic(SDL_GetError());

	initGL();

	return -1;
}

void render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
		glVertex2f(0.f, 0.f);
		glVertex2f(100.f, 0.f);
		glVertex2f(100.f, 100.f);
		glVertex2f(0.f, 100.f);
	glEnd();

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

}
