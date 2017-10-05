#ifndef _REVE_STAGE_H
#define _REVE_STAGE_H

/* Stage attributes, this is what attr gets set to on the STAGE */
typedef struct {
	SDL_Window *window;
	SDL_Surface *surface;
	SDL_Renderer *renderer;
	lua_State *lua;
	sqlite3 *sqlite;
} rv_Stage;

rv_CrewStatus rv_STAGE(rv_Crew *c);

lua_State *rv_StageGetLua();

#endif
