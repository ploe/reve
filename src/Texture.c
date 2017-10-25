#include "reve.h"

static ish_Map *textures = NULL;

static void *TextureGet(ish_Map *textures, char *key, void *value) {
	rv_Texture *t = (rv_Texture *) value;
	t->rc++;

	return t;
}

static void *TextureDrop(ish_Map *textures, char *key, void *value) {
	rv_Texture *t = (rv_Texture *) value;
	t->rc--;

	if (t->rc <= 0) {
		free(value);
		return NULL;
	}

	return t;
}

rv_Texture *rv_TextureNew(const char *path) {
	rv_Texture *t = calloc(1, sizeof(rv_Texture));
	if (!t) rv_Panic(-1, "texture not found\n");

	ILuint img = 0;
	ilGenImages(1, &img);
	ilBindImage(img);

	ilLoadImage(path);

	if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) rv_Panic(rv_ETEXTURE, "image not converted");

	glGenTextures(1, &t->texture);
	glBindTexture(GL_TEXTURE_2D, t->texture);
	t->w = ilGetInteger(IL_IMAGE_WIDTH);
	t->h = ilGetInteger(IL_IMAGE_HEIGHT);
	GLuint *pixels = (GLuint *) ilGetData();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t->w, t->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	ilDeleteImages(1, &img);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	t->rc = 1;
	if (!textures) textures = ish_MapNew();
	ish_MapSet(textures, (char *) path, t);

	return t;
}

void *AtlasSize(ish_Map *textures, char *key, void *value, void *probe) {
	rv_Atlas *atlas = (rv_Atlas *) probe;
	rv_Texture *t = (rv_Texture *) value;

	atlas->w += t->w;
	if (t->h > atlas->h) atlas->h = t->h;

	return 0;
}

rv_Atlas rv_TextureAtlas() {
	rv_Atlas atlas;

	ish_MapProbePairs(textures, AtlasSize, &atlas);
	glGenFramebuffers(1, &atlas.fb);
	glBindFramebuffer(GL_FRAMEBUFFER, atlas.fb);

	glGenTextures(1, &atlas.texture);
	glBindTexture(GL_TEXTURE_2D, atlas.texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, atlas.w, atlas.h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, atlas.texture, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return atlas;
}
