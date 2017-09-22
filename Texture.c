#include "reve.h"

static ish_Map *textures = NULL;

static rv_Texture *TextureGet(ish_Map *textures, char *key, void *value) {
	rv_Texture *texture = (rv_Texture *) value;
	texture->rc++;

	return texture;
}

static rv_Texture *TextureDrop(ish_Map *textures, char *key, void *value) {
	rv_Texture *texture = (rv_Texture *) value;
	texture->rc--;

	if (texture->rc <= 0) {
		free(value);
		return NULL;
	}

	return texture;
}

void *rv_TextureNew(char *src, SDL_Renderer *renderer) {
	if (!textures) textures = ish_MapNew();
	rv_Texture *texture = (rv_Texture *) ish_MapGet(textures, src);
	
	if (!texture) {
		texture = calloc(1, sizeof(rv_Texture));
		if (!texture) rv_Panic(-1, "rv_Texture failed to allocate");
		texture->rc = 1;

		SDL_Surface *surface = IMG_Load(src);
		if (!surface) rv_Panic(-1, IMG_GetError());

		texture->texture = SDL_CreateTextureFromSurface(renderer, surface);
		if (!texture) rv_Panic(-1, SDL_GetError());

		SDL_FreeSurface(surface);

		ish_MapSet(textures, src, texture);
	}

	return texture;
}

