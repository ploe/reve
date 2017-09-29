#ifndef _REVE_LAYER_H
#define _REVE_LAYER_H
typedef struct {
	SDL_Rect clip;
	rv_Texture *texture;
	unsigned int rc;
} rv_Tile;

rv_Tile *rv_TileNew(char *name, char *texture, SDL_Rect clip, rv_Stage *stage);
void rv_TileDraw(rv_Tile *t, rv_Stage *stage);
#endif
