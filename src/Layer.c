#include "reve.h"

static ish_Map *tiles;

static void *TileGet(ish_Map *tiles, char *key, void *value) {
	rv_Tile *t = (rv_Tile *) value;
	t->rc++;

	return t;
}

static void *TileDrop(ish_Map *tiles, char *key, void *value) {
	rv_Tile *t = (rv_Tile *) value;

	t->rc--;
	if (t->rc <= 0) {
		free(value);
		return NULL;
	}

	return t;
}

rv_Tile *rv_TileNew(char *name, char *texture, SDL_Rect clip, rv_Stage *stage) {
	if (!tiles) tiles = ish_MapNew();

	rv_Tile *t = ish_MapGet(tiles, name);
	if (!t) {
		t = calloc(1, sizeof(rv_Tile));
		if (!t) return NULL;

		t->rc = 1;
//		t->texture = rv_TextureNew(texture, stage->renderer);
		t->clip = clip;

		ish_MapSetWithAllocators(tiles, name, t, TileGet, TileDrop);
	}

	return t;
}

void rv_TileDraw(rv_Tile *t, rv_Stage *stage) {
	SDL_RenderCopy(stage->renderer, t->texture->texture, &t->clip, &t->clip);
}
