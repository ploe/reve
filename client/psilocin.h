#ifndef _PSILOCIN_MAIN_H
#define _PSILOCIN_MAIN_H

// Their libs
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>


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

//Our libs
#include "Crew.h"
#include "Stage.h"


#endif
