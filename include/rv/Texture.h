#ifndef _REVE_TEXTURE_H
#define _REVE_TEXTURE_H

typedef struct {
	unsigned int rc;
	GLuint texture, x, y, w, h;
} rv_Texture;

typedef struct {
	GLuint fb, texture, w, h;
} rv_Atlas;

rv_Texture *rv_TextureNew(const char *path);
rv_Atlas rv_TextureAtlas();

#endif
