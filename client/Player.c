#include "psilocin.h"

ps_Pad ps_Player;

static ps_CrewStatus ps_KEYBOARD(ps_Crew *c) {
	c->tag = "KEYBOARD";
	c->update = ps_KEYBOARD;
	static const Uint8 *key;
	key = SDL_GetKeyboardState(NULL);

	ps_Player.up = key[SDL_SCANCODE_UP];
	ps_Player.down = key[SDL_SCANCODE_DOWN];
	ps_Player.left = key[SDL_SCANCODE_LEFT];
	ps_Player.right = key[SDL_SCANCODE_RIGHT];
	ps_Player.ok = key[SDL_SCANCODE_Z];
	ps_Player.cancel = key[SDL_SCANCODE_X];

	if(key[SDL_SCANCODE_ESCAPE]) return ps_EXIT;

	return ps_LIVE;
}

static ps_CrewStatus teste(ps_Crew *c) {
	c->tag = "testes";
	c->update = teste;

	if (ps_Player.up) puts("key[SDL_SCANCODE_UP]");
	if (ps_Player.down) puts("key[SDL_SCANCODE_DOWN]");
	if (ps_Player.left) puts("key[SDL_SCANCODE_LEFT];");
	if (ps_Player.right) puts("[SDL_SCANCODE_RIGHT];");
	if (ps_Player.ok) puts("key[SDL_SCANCODE_Z];");
	if (ps_Player.cancel) puts("key[SDL_SCANCODE_X];");

	return ps_LIVE;
}

ps_CrewStatus ps_PLAYER(ps_Crew *c) {	
	c->tag = "PLAYER";
	ps_CrewNew(ps_KEYBOARD);
	ps_CrewNew(teste);

	return ps_CUT;
}


