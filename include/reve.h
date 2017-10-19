#ifndef _REVE_MAIN_H
#define _REVE_MAIN_H

// Their libs
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "murp.h"
#include "hashish.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <IL/il.h>

#ifdef __linux
#include <SDL2/SDL.h>
#include <SDL_opengl.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#endif

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include <sqlite3.h>

/*	app ExitCodes	*/
typedef int rv_ExitCode;
enum {
	rv_EOK = 0,
	_rv_ERRORS = 999,
	rv_EOALLOC,
	rv_EOCREW_ALLOC,
	rv_EOSTAGE_INIT,
	rv_ELOAD,
	rv_ETEXTURE,
	rv_EGL,
	rv_ELUA,
	rv_ESQL,
	_rv_ENDOFERRORS
};

/* 	Fetch the appropriate method and call it on the Crew
	This kind of compacts the calling. */

typedef int rv_Bool;
enum {
	rv_YES = -1,
	rv_NO
};


void rv_Panic(int i, const char *prompt);

//Our libs
#include "rv/Crew.h"
#include "rv/Player.h"
#include "rv/Stage.h"
#include "rv/Texture.h"
#include "rv/Layer.h"
#include "rv/Persist.h"
#include "rv/Text.h"
#include "rv/Lua.h"
#include "rv/Shaders.h"

#endif
