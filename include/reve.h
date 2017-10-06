#ifndef _REVE_MAIN_H
#define _REVE_MAIN_H

// Their libs
#include <stdio.h>
#include <stdlib.h>

#include "murp.h"
#include "hashish.h"

#ifdef __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "sqlite3.h"

/*	app ExitCodes	*/
typedef int rv_ExitCode;
enum {
	rv_EOK = 0,
	_rv_ERRORS = 900,
	rv_EOALLOC,
	rv_EOCREW_ALLOC,
	rv_EOSTAGE_INIT,
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
char *rv_Format(const char *format, ...);

//Our libs
#include "Crew.h"
#include "Player.h"
#include "Stage.h"
#include "Texture.h"
#include "Layer.h"
#include "Persist.h"

#endif
