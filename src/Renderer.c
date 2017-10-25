#include "reve.h"

rv_Renderer *rv_RendererNew(size_t size) {
	rv_Renderer *r = calloc(1, sizeof(rv_Renderer));
	if (!r) rv_Panic(-1, "renderer not allocated");
	r->index = 0;
	r->size = size;

	glGenBuffers(1, &r->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
	
	r->buffer = calloc(r->size, sizeof(rv_Quad));
	if (!r->buffer) rv_Panic(-1, "renderer buffer not allocated");
	return r;
}

rv_Bool rv_RendererAdd(rv_Renderer *r, rv_Quad quad) {
	if (r->index >= r->size) {
		r->size *= 2;
		rv_Quad *tmp = realloc(r->buffer, sizeof(rv_Quad) * r->size);
		if (!tmp) rv_Panic(-1, "rv_Quad buffer could not be reallocated");
		r->buffer = tmp;
	}

	r->buffer[r->index] = quad;
	r->index++;

	return rv_YES;
}

static void VectorsPrint(rv_Vectors *v) {
	printf("x: %f, y: %f, z: %f, u: %f, v: %f\n", v->x, v->y, v->z, v->u, v->v);
}

rv_Bool rv_RendererDraw(rv_Renderer *r) {
	glBindBuffer(GL_ARRAY_BUFFER, r->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rv_Quad) * r->index, r->buffer, GL_STATIC_DRAW);

	glClearColor(0.f, 1.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 6 * r->index);
	r->index = 0;

	return rv_YES;
}

rv_Renderer *rv_RendererFree(rv_Renderer *r) {
	if (r) {
		glDeleteBuffers(1, &r->vbo);
		free(r->buffer);
		free(r);
	}

	return NULL;
}
