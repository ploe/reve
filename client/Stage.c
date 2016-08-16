#include "psilocin.h"

/* Destroy function for the STAGE - the return value isn't important */
static ps_CrewStatus DestroyStage(ps_Crew *c) {
	ps_Stage *stage = (ps_Stage *) c->attr;
	SDL_DestroyRenderer(stage->renderer);
	SDL_DestroyWindow(stage->window);
	SDL_Quit();

	return ps_CUT;
}

/* 	must remember to free the marshalled char 	*/
static char *MarshalStage(ps_Crew *c) {
	return ps_Format("\"%s\":{status:\"%s\"}",
		c->tag,
		ps_CrewStatusStr[c->status]
	);
}

/* Update method for the STAGE - called every frame */
static ps_CrewStatus UpdateStage(ps_Crew *c) {
	ps_Stage *stage = (ps_Stage *) c->attr;

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) return ps_EXIT;
	}

	SDL_RenderClear(stage->renderer);
	SDL_RenderPresent(stage->renderer);


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
	if (!stage) ps_Panic(ps_EOSTAGE_INIT, "ps_STAGE: Could not allocate stage struct.");
	c->attr = stage;

	stage->window = SDL_CreateWindow("psilocin", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 960, 540, SDL_WINDOW_SHOWN);
	if (!stage->window) ps_Panic(ps_EOSTAGE_INIT, "ps_STAGE: Failed to create STAGE window.");

	stage->renderer = SDL_CreateRenderer(stage->window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(stage->renderer, 255, 0, 128, 255);

	ps_CrewNew(ps_PLAYER);

	return 0;
}
