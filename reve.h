#ifndef _REVE_MAIN_H
#define _REVE_MAIN_H

// Their libs
#include <stdio.h>
#include <stdlib.h>

#include <curl/curl.h>
#include <murp.h>

#ifdef __linux
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_image.h>
#endif


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


void rv_Panic(int i, char *prompt);
char *rv_Format(const char *format, ...);
char *rv_GetHTTP(char *url);

mp_Atomizer rv_PrintAtom(mp_Atom atom, void *p);
mp_Atomizer rv_CountElems(mp_Atom atom, void *probe);

//Our libs
#include "Crew.h"
#include "Player.h"
#include "Stage.h"


#endif
