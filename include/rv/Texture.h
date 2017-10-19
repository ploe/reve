#ifndef _REVE_TEXTURE_H
#define _REVE_TEXTURE_H

typedef struct {
	unsigned int rc;
	GLuint texture;
} rv_Texture;

GLuint rv_TextureLoad(const char *path);

#endif
