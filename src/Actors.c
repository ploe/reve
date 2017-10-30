#include "reve.h"

static ish_Map *actors = NULL;
static rv_CrewStatus ActorsDestroy(rv_Crew *c) {
	return rv_CUT;
}

static rv_Quad ActorToQuad(rv_Actor *a) {
	rv_Quad quad;
	float 
		left = a->x, 
		top = a->y, 
		right = a->x + a->w, 
		bottom = a->y + a->h;

	quad.vectors[rv_QPOLY1START] = (rv_Vectors) {left, top, 0.0f, 0.0f, 0.0f };
	quad.vectors[rv_QPOLY1MID] = (rv_Vectors) {right,  top, 0.0f,  1.0f, 0.0f };
	quad.vectors[rv_QPOLY1END] = (rv_Vectors) {right, bottom, 0.0f, 1.0f, 1.0f };

	quad.vectors[rv_QPOLY2START] = quad.vectors[rv_QPOLY1END];
	quad.vectors[rv_QPOLY2MID] = (rv_Vectors) {left, bottom, 0.0f, 0.0f, 1.0f};
	quad.vectors[rv_QPOLY2END] = quad.vectors[rv_QPOLY1START];

	return quad;
}

static int RenderActors(ish_Map *actors, char *key, void *value, void *unused) {
	rv_Actor *a = (rv_Actor *) value;

	rv_Renderer *r = rv_StageGetRenderer();

	rv_RendererAdd(r, rv_ActorToQuad(a));

	return 0;		
}

static rv_CrewStatus ActorsUpdate(rv_Crew *c) {
	ish_MapForPairs(actors, RenderActors);

	return rv_LIVE;
}

static rv_CrewStatus rv_ACTORS(rv_Crew *c) {
	c->update = ActorsUpdate;
	c->destroy = ActorsDestroy;
	c->type = rv_ACTORS;

	actors = ish_MapNew();
	if (!actors) rv_Panic(-1, "actors map not allocated");

	return rv_LIVE;
}
