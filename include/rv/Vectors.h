#ifndef _RV_VECTORS_H
#define _RV_VECTORS_H

typedef struct {
	GLfloat x, y, z, u, v;
} rv_Vectors;

typedef struct {
	rv_Vectors vectors[6];
} rv_Quad;
#define rv_QTOPLEFT 0
#define rv_QTOPRIGHT 1
#define rv_QBOTTOMRIGHT 2

#define rv_POLY2START 3
#define rv_QBOTTOMLEFT 4
#define rv_POLY2END 5

#define rv_VERTEX_OFFSET 0
#define rv_TEXTURE_OFFSET ((const void *) (sizeof(GLfloat) * 3))

#endif
