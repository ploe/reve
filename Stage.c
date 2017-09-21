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

SDL_Texture *chomp = NULL;

/* Update method for the STAGE - called every frame */
static ps_CrewStatus UpdateStage(ps_Crew *c) {
	ps_Stage *stage = (ps_Stage *) c->attr;

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) return ps_EXIT;
	}

	SDL_RenderClear(stage->renderer);
	SDL_Rect r;
	r.w= r.h = 64;
	r.x = r.y = 0;
	static double i = 0;
	SDL_RenderCopyEx(stage->renderer, chomp, NULL, &r, i+=0.01, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(stage->renderer);


	return ps_LIVE;
}

SDL_Texture *ps_LoadTexture(char *src, SDL_Renderer *renderer) {
	SDL_Surface *surface = IMG_Load(src);
	if (!surface) ps_Panic(-1, "unable to load image");

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) ps_Panic(-1, "unable to load image");

	SDL_FreeSurface(surface);

	return texture;
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

	stage->window = SDL_CreateWindow("psilocin", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 360, SDL_WINDOW_SHOWN);
	if (!stage->window) ps_Panic(ps_EOSTAGE_INIT, "ps_STAGE: Failed to create STAGE window.");

	stage->renderer = SDL_CreateRenderer(stage->window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(stage->renderer, 255, 0, 128, 255);

	IMG_Init(IMG_INIT_PNG);
	chomp = ps_LoadTexture("Chomp_Rock-scaled.PNG", stage->renderer);	

	ps_CrewNew(ps_PLAYER);

	return 0;
}
