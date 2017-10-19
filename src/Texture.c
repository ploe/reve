#include "reve.h"

static ish_Map *textures = NULL;

static void *TextureGet(ish_Map *textures, char *key, void *value) {
	rv_Texture *texture = (rv_Texture *) value;
	texture->rc++;

	return texture;
}

static void *TextureDrop(ish_Map *textures, char *key, void *value) {
	rv_Texture *texture = (rv_Texture *) value;
	texture->rc--;

	if (texture->rc <= 0) {
		free(value);
		return NULL;
	}

	return texture;
}

GLuint rv_TextureLoad(const char *path) {
	ILuint img = 0;
	ilGenImages(1, &img);
	ilBindImage(img);

	ilLoadImage(path);

	if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) rv_Panic(rv_ETEXTURE, "image not converted");

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	GLuint width = ilGetInteger(IL_IMAGE_WIDTH), height = ilGetInteger(IL_IMAGE_HEIGHT);
	GLuint *pixels = (GLuint *) ilGetData();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	return texture;
}
