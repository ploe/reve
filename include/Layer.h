#ifndef _REVE_LAYER_H
#define _REVE_LAYER_H
typedef struct {
	SDL_Rect clip;
	rv_Texture *texture;
	unsigned int rc;
} rv_Tile;

rv_Tile *rv_TileNew(char *name, char *texture, SDL_Rect clip);
void rv_TileDraw(rv_Tile *t, rv_Stage *stage);

typedef struct {
	uint8_t w, h;
	SDL_Rect offset;
	rv_Tile **array;
} rv_Layer;


rv_Layer *rv_LayerNew(char *name, uint8_t x, uint8_t y, uint8_t  w, uint8_t h);

#endif
