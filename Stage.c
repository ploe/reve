#include "reve.h"

/* Destroy function for the STAGE - the return value isn't important */
static rv_CrewStatus DestroyStage(rv_Crew *c) {
	rv_Stage *stage = (rv_Stage *) c->attr;
	SDL_DestroyRenderer(stage->renderer);
	SDL_DestroyWindow(stage->window);
	SDL_Quit();

	return rv_CUT;
}

/* 	must remember to free the marshalled char 	*/
static char *MarshalStage(rv_Crew *c) {
	return rv_Format("\"%s\":{status:\"%s\"}",
		c->tag,
		rv_CrewStatusStr[c->status]
	);
}

SDL_Texture *chomp = NULL;

/* Update method for the STAGE - called every frame */
static rv_CrewStatus UpdateStage(rv_Crew *c) {
	rv_Stage *stage = (rv_Stage *) c->attr;

	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) return rv_EXIT;
	}

	SDL_RenderClear(stage->renderer);
	SDL_Rect r;
	r.w= r.h = 64;
	r.x = r.y = 0;
	static double i = 0;
	SDL_RenderCopyEx(stage->renderer, chomp, NULL, &r, i+=0.01, NULL, SDL_FLIP_NONE);
	SDL_RenderPresent(stage->renderer);


	return rv_LIVE;
}

SDL_Texture *rv_LoadTexture(char *src, SDL_Renderer *renderer) {
	SDL_Surface *surface = IMG_Load(src);
	if (!surface) rv_Panic(-1, "unable to load image");

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) rv_Panic(-1, "unable to load image");

	SDL_FreeSurface(surface);

	return texture;
}

/* The init function/type for the STAGE */
rv_CrewStatus rv_STAGE(rv_Crew *c) {
	c->tag = "STAGE";
	c->type = rv_STAGE;
	c->destroy = DestroyStage;
	c->update = UpdateStage;
	c->marshal = MarshalStage;	

	if (SDL_Init( SDL_INIT_VIDEO ) < 0) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: SDL_Init Failed");

	rv_Stage *stage = calloc(1, sizeof(rv_Stage));
	if (!stage) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: Could not allocate stage struct.");
	c->attr = stage;

	stage->window = SDL_CreateWindow("psilocin", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 360, SDL_WINDOW_SHOWN);
	if (!stage->window) rv_Panic(rv_EOSTAGE_INIT, "rv_STAGE: Failed to create STAGE window.");

	stage->renderer = SDL_CreateRenderer(stage->window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(stage->renderer, 255, 0, 128, 255);

	IMG_Init(IMG_INIT_PNG);
	chomp = rv_LoadTexture("Chomp_Rock-scaled.PNG", stage->renderer);	

	rv_CrewNew(rv_PLAYER);

	return 0;
}
