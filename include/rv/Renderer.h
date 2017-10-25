#ifndef _RV_VECTORS_H
#define _RV_VECTORS_H

typedef struct {
	GLfloat x, y, z, u, v;
} rv_Vectors;

#define rv_QUAD_VECTORS_MAX 6

typedef struct {
	rv_Vectors vectors[rv_QUAD_VECTORS_MAX];
} rv_Quad;

typedef struct {
	rv_Quad *buffer;
	unsigned int index;
	size_t size;
	GLuint vbo;
} rv_Renderer;

rv_Renderer *rv_RendererNew(size_t size);
rv_Renderer *rv_RendererFree(rv_Renderer *r);
rv_Bool rv_RendererAdd(rv_Renderer *r, rv_Quad quad);
rv_Bool rv_RendererDraw(rv_Renderer *r);

#define rv_QPOLY1START 0
#define rv_QPOLY1MID 1
#define rv_QPOLY1END 2

#define rv_QPOLY2START 3
#define rv_QPOLY2MID 4
#define rv_QPOLY2END 5

#define rv_VERTEX_OFFSET 0
#define rv_TEXTURE_OFFSET ((const void *) (sizeof(GLfloat) * 3))

#endif
