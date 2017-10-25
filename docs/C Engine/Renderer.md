# Renderer

Renderer is the module where the type `rv_Renderer` is the module used for rendering `rv_Quad` to the `OpenGL` context.

It is defined in
[rv/Renderer.h](/include/rv/Renderer.h) and implemented in
[Renderer.c](/src/Renderer.c).

## Methods

### rv_Renderer \*rv_RendererNew(size_t size)

Allocate a new `rv_Renderer` with a `buffer` and `size` set to `size`.

```c
r = rv_RendererNew(128);
```

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
