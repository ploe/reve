#include "reve.h"

rv_Pad rv_Player;

static rv_CrewStatus rv_KEYBOARD(rv_Crew *c) {
	c->tag = "KEYBOARD";
	c->update = rv_KEYBOARD;
	static const Uint8 *key;
	key = SDL_GetKeyboardState(NULL);
	rv_Player.up = key[SDL_SCANCODE_UP];
	rv_Player.down = key[SDL_SCANCODE_DOWN];
	rv_Player.left = key[SDL_SCANCODE_LEFT];
	rv_Player.right = key[SDL_SCANCODE_RIGHT];
	rv_Player.ok = key[SDL_SCANCODE_Z];
	rv_Player.cancel = key[SDL_SCANCODE_X];

	if(key[SDL_SCANCODE_ESCAPE]) return rv_EXIT;

	return rv_LIVE;
}

static rv_CrewStatus teste(rv_Crew *c) {
	c->tag = "testes";
	c->update = teste;

	if (rv_Player.up) puts("key[SDL_SCANCODE_UP]");
	if (rv_Player.down) puts("key[SDL_SCANCODE_DOWN]");
	if (rv_Player.left) puts("key[SDL_SCANCODE_LEFT];");
	if (rv_Player.right) puts("[SDL_SCANCODE_RIGHT];");
	if (rv_Player.ok) puts("key[SDL_SCANCODE_Z];");
	if (rv_Player.cancel) puts("key[SDL_SCANCODE_X];");

	return rv_LIVE;
}

rv_CrewStatus rv_PLAYER(rv_Crew *c) {	
	c->tag = "PLAYER";
	rv_CrewNew(rv_KEYBOARD);
	rv_CrewNew(teste);

	return rv_CUT;
}
