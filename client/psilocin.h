#ifndef _PSILOCIN_MAIN_H
#define _PSILOCIN_MAIN_H

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
#endif


/*	app ExitCodes	*/
typedef int ps_ExitCode;
enum {
	ps_EOK = 0,
	_ps_ERRORS = 900,
	ps_EOALLOC,
	ps_EOCREW_ALLOC,
	ps_EOSTAGE_INIT,
	_ps_ENDOFERRORS
};

/* 	Fetch the appropriate method and call it on the Crew 
	This kind of compacts the calling. */

typedef int ps_Bool;
enum {
	ps_YES = -1,
	ps_NO
};


void ps_Panic(int i, char *prompt);
char *ps_Format(const char *format, ...);
char *ps_GetHTTP(char *url);

mp_Atomizer ps_PrintAtom(mp_Atom atom, void *p);
mp_Atomizer ps_CountElems(mp_Atom atom, void *probe);

//Our libs
#include "Crew.h"
#include "Player.h"
#include "Stage.h"


#endif
