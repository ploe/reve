# Renderer

Renderer is the module where the type `rv_Renderer` is defined. This module is used for rendering `rv_Quad` to the `OpenGL` context.

An `rv_Quad` is two textured polygons that you want to draw on-screen to represent something in-game.

It is defined in
[rv/Renderer.h](/include/rv/Renderer.h) and implemented in
[Renderer.c](/src/Renderer.c).

## Type

```c
typedef struct {
	rv_Quad *buffer;
	unsigned int index;
	size_t size;
	GLuint vbo;
} rv_Renderer;
```

### rv_Quad \*buffer

An array of quads that we want to the `rv_Renderer` to draw to the OpenGL context.

### unsigned int index

Where in `buffer` to add the the new `rv_Quad`.

### size_t size

The current `size` of `buffer`.

### GLuint vbo

The Vertex Buffer Object associated with this `rv_Renderer` in OpenGL.

## Methods

### rv_Renderer \*rv_RendererNew(size_t size)

Allocate a new `rv_Renderer` with a `buffer` and `size` set to `size`.

```c
r = rv_RendererNew(128);
```

### rv_Renderer \*rv_RendererFree(rv_Renderer \*r);

Frees the `vbo`, `buffer` and `r` and returns `NULL`.

### rv_Bool rv_RendererAdd(rv_Renderer \*r, rv_Quad quad)

Adds an `rv_Quad` to the `rv_Renderer` `r` to draw. Returns true if successful.

```c
rv_Quad quad;
quad.vectors[rv_QPOLY1START] = (rv_Vectors) { -0.5f,  0.5f, 0.0f, 0.0f, 0.0f };
quad.vectors[rv_QPOLY1MID] = (rv_Vectors) { 0.5f,  0.5f, 0.0f,  1.0f, 0.0f };
quad.vectors[rv_QPOLY1END] = (rv_Vectors) { 0.5f, -0.5f, 0.0f, 1.0f, 1.0f };

quad.vectors[rv_QPOLY2START] = quad.vectors[rv_QPOLY1END];
quad.vectors[rv_QPOLY2MID] = (rv_Vectors) { -0.5f, -0.5f, 0.0f, 0.0f, 1.0f};
quad.vectors[rv_QPOLY2END] = quad.vectors[rv_QPOLY1START];

rv_RendererAdd(r, quad);
```

### rv_Bool rv_RendererDraw(rv_Renderer \*r)

Draws the queue of `rv_Quad` in `r`.

```c
rv_RendererDraw(r);
```
