typedef struct {
	unsigned int rc;
	SDL_Texture *texture;
} rv_Texture;

void *rv_TextureNew(char *src, SDL_Renderer *renderer);
