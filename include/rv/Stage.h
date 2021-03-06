#ifndef _REVE_STAGE_H
#define _REVE_STAGE_H

/* Stage attributes, this is what attr gets set to on the STAGE */
typedef struct {
	SDL_Window *window;
	SDL_GLContext *context;
	SDL_Surface *surface;
	SDL_Renderer *renderer;
	sqlite3 *sqlite;
} rv_Stage;

rv_CrewStatus rv_STAGE(rv_Crew *c);
sqlite3 *rv_StageGetSQLite();

#endif
