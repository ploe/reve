#include "psilocin.h"

/* Destroy function for the STAGE - the return value isn't important */
static ps_CrewStatus DestroyStage(ps_Crew *c) {
	ps_Stage *stage = (ps_Stage *) c->attr;
	SDL_DestroyWindow(stage->window);
	SDL_Quit();

	return ps_CUT;
}


/*	proof of concept Marshaller - returns length of 
	JSON'd string	*/
static int StageJSON(ps_Crew *c, char *dst, int size) {
	char *format = "\"STAGE\":{\"status\":\"%s\"},";
	return snprintf(dst, size, format, 
		ps_CrewStatusStr[c->status]
	);	
}

/* 	must remember to free the marshalled char 	*/
static char *MarshalStage(ps_Crew *c) {
	int size = 1 + StageJSON(c, NULL, 0);
	char *dst = calloc(size, sizeof(char));
	StageJSON(c, dst, size);

	return dst;
}

/* Update method for the STAGE - called every frame */
static ps_CrewStatus UpdateStage(ps_Crew *c) {
	ps_Stage *stage = (ps_Stage *) c->attr;

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) return ps_EXIT;
	}
	SDL_UpdateWindowSurface(stage->window);

	return ps_LIVE;
}

/* The init function/type for the STAGE */
ps_CrewStatus ps_STAGE(ps_Crew *c) {
	c->tag = "STAGE";
	c->type = ps_STAGE;
	c->destroy = DestroyStage;
	c->update = UpdateStage;
	c->marshal = MarshalStage;

	if (SDL_Init( SDL_INIT_VIDEO ) < 0) ps_Panic(ps_EOSTAGE_INIT, "ps_STAGE: SDL_Init Failed");
	ps_Stage *stage = calloc(1, sizeof(ps_Stage));
	stage->window = SDL_CreateWindow("psilocin", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 160, 144, SDL_WINDOW_SHOWN);
	if (!stage->window) ps_Panic(ps_EOSTAGE_INIT, "ps_STAGE: Failed to create STAGE window.");
	stage->surface = SDL_GetWindowSurface(stage->window);

	c->attr = stage;
	return 0;
}


