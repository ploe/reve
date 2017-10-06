#include "SDL.h"
#include "SDL_opengl.h"
#include "GL/glu.h"
#include <stdio.h>

SDL_Window *window = NULL;
SDL_GLContext *context = NULL;

void panic(const char *msg) {
	fputs(msg, stderr);
	exit(-1);
}

int initGL() {
	GLenum error = GL_NO_ERROR;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	error = glGetError();
	if (error != GL_NO_ERROR) panic(gluErrorString(error));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	error = glGetError();
	if (error != GL_NO_ERROR) panic(gluErrorString(error));

	glClearColor(0.f, 1.f, 0.f, 1.f);

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
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
	glEnd();

	SDL_GL_SwapWindow(window);
}


int main(int argc, char *argv[]) {
	init();

	SDL_Event event;
	while (event.type != SDL_QUIT) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) return 0;
		}

		
		render();
	}

}
