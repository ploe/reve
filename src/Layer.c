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

rv_Tile *rv_TileNew(char *name, char *texture, SDL_Rect clip) {
	if (!tiles) tiles = ish_MapNew();

	rv_Tile *t = ish_MapGet(tiles, name);
	if (!t) {
		t = calloc(1, sizeof(rv_Tile));
		if (!t) return NULL;

		t->rc = 1;
		t->texture = rv_TextureNew(texture, rv_StageGetRenderer());
		t->clip = clip;

		ish_MapSetWithAllocators(tiles, name, t, TileGet, TileDrop);
	}

	return t;
}

static ish_Map *layers = NULL;

void rv_TileDraw(rv_Tile *t, SDL_Rect offset, rv_Stage *stage) {
	SDL_RenderCopy(stage->renderer, t->texture->texture, &t->clip, &offset);
}

unsigned int rv_LayerLength(rv_Layer *l) {
	return l->w * l->h;
}

rv_Layer *rv_LayerNew(char *name, uint8_t  w, uint8_t h) {
	rv_Layer *l = calloc(1, sizeof(rv_Layer));
	if (l) {
		l->w = w;
		l->h = h;
		SDL_Rect offset = {0, 0, 16, 16};
		l->offset = offset;

		l->array = calloc(w * h, sizeof(*rv_Tile));
		if (!l->array) {
			free(l);
			return NULL;
		}
	}

	return l;
}

void rv_LayerDraw(rv_Layer *l) {
	unsigned int i;
	for (i = 0; i < rv_LayerLength(l), i++) {
		rv_TileDraw(l->array[i], l, stage);
	}
}
