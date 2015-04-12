#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

/* Stage attributes, this is what attr gets set to on the STAGE */
typedef struct {
	SDL_Window *window;
	SDL_Surface *surface;
} ps_Stage;

/* ps_Panic takes an exit code and a prompt, simply drops the user out of
	the app.	*/
void ps_Panic(int i, char *prompt) {
	fprintf (stderr, prompt);
	exit(i);
}

/*	app ExitCodes	*/
typedef int ps_ExitCode;
enum {
	ps_EOK = 0,
	_ps_ERRORS = 900,
	ps_EOSTAGE_INIT,
	_ps_ENDOFERRORS
};

/*	Each Crew update method returns an exit code, this exit code
	dictates whether or not we deallocate the Crew responsible, play
	it again or exit the whole app. */
typedef int ps_CrewStatus;
enum {
	ps_LIVE = 0,
	ps_ENCORE,
	ps_CUT,
	ps_EXIT,
};

/* 	Type for Crew which is a state in the State machine. 
	The Crew are a linked list of actions we need to execute each and
	every frame.	*/ 
typedef struct {
	void *attr;
	void *type, *update, *destroy;
	ps_CrewStatus status;	
	struct Crew *prev, *next;
} Crew;

/*	ps_Updater is the type used to describe methods for the Crew	*/
typedef ps_CrewStatus (*ps_Updater)(Crew *);

/* 	Fetch the appropriate method and call it on the Crew 
	This kind of compacts the calling. */
ps_CrewStatus ps_CrewCall(Crew *c, ps_Updater func) {
	return func(c);
}

/* Destroy function for the STAGE - the return value isn't important */
static ps_CrewStatus DestroyStage(Crew *c) {
	ps_Stage *stage = (ps_Stage *) c->attr;
	SDL_DestroyWindow(stage->window);
	SDL_Quit();

	return ps_CUT;
}

/* Update method for the STAGE - called every frame */
static ps_CrewStatus UpdateStage(Crew *c) {
	return ps_LIVE;
}

/* The init function/type for the STAGE */
ps_CrewStatus ps_STAGE(Crew *c) {
	c->type = ps_STAGE;
	c->destroy = DestroyStage;
	c->update = UpdateStage;

	if (SDL_Init( SDL_INIT_VIDEO ) < 0) ps_Panic(ps_EOSTAGE_INIT, "ps_STAGE: SDL_Init Failed");
	ps_Stage *stage = calloc(1, sizeof(ps_Stage));
	stage->window = SDL_CreateWindow("psilocin", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160, 144, SDL_WINDOW_SHOWN);
	if (!stage->window) ps_Panic(ps_EOSTAGE_INIT, "ps_STAGE: Failed to create STAGE window.");
	stage->surface = SDL_GetWindowSurface(stage->window);

	c->attr = stage;
	return 0;
}


int main() {
	Crew c;
	ps_STAGE(&c);
        SDL_Delay(5000);
	ps_CrewCall(&c, c.destroy);
	return 0;
}
