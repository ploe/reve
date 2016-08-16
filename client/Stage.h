#ifndef _PSILOCIN_STAGE_H
#define _PSILOCIN_STAGE_H

/* Stage attributes, this is what attr gets set to on the STAGE */
typedef struct {
	SDL_Window *window;
	SDL_Surface *surface;
	SDL_Renderer *renderer;
} ps_Stage;

ps_CrewStatus ps_STAGE(ps_Crew *c);

#endif
