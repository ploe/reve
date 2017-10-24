
typedef struct {
	float x, y, w, h, u, v;
	const char *texture;
} rv_Actor;


rv_CrewStatus rv_ActorsUpdate(rv_Crew *c) {
	return rv_LIVE;
}

rv_Quad rv_ActorQuad(rv_Actor *a) {
	rv_Quad quad;

	GLfloat top = a->y, bottom = a->y + a->h, left = a->x, right = a->x + a->w, depth=bottom;

	quad.vectors[rv_QTOPLEFT] = (rv_Vectors) {
		left,  
		top, 
		depth, 
		0.0f, 
		0.0f
	};

	quad.vectors[rv_QTOPRIGHT] = (rv_Vectors) {
		right,
		top,
		depth,
		1.0f,
		0.0f
	};

	quad.vectors[rv_QBOTTOMRIGHT] = (rv_Vectors) {
		right, 
		bottom, 
		depth, 
		1.0f, 
		1.0f
	};

	quad.vectors[rv_POLY2START] = quad.vectors[rv_QBOTTOMRIGHT];

	quad.vectors[rv_QBOTTOMLEFT] = (rv_Vectors) {
		left, 
		bottom,
		depth, 
		0.0f, 
		1.0f
	};

	quad.vectors[rv_POLY2END] = quad.vectors[rv_QTOPLEFT];

	return quad;
}

rv_Actor *rv_ActorNew() {
	rv_Actor *a = calloc(1, sizeof(rv_Actor));
	return a;
}

rv_CrewStatus rv_ACTORS(rv_Crew *c) {
	c->update = rv_ActorsUpdate;
	
	actors = ish_MapNew();

	rv_Actor *myke = rv_ActorNew();
	*myke = (rv_Actor) {0, 0, 100, 100, 1.0f, 1.0f};

	rv_Actor *vix = rv_ActorNew();
	*vix = (rv_Actor) {0, 80, 100, 100, 1.0f, 1.0f};
	
	rv_Quad quad[2]; 
	quad[0] = rv_ActorQuad(myke);
	quad[1] = rv_ActorQuad(vix);

	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad)*2, &quad, GL_STATIC_DRAW);

	return rv_CUT;
}


