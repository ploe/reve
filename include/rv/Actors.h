#ifndef _REVE_ACTORS_H
#define _REVE_ACTORS_H

typedef struct {
	float x, y, w, h;
	char *texture;
} rv_Actor;


static rv_CrewStatus rv_ACTORS(rv_Crew *c);

#endif
