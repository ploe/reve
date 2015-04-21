#include "psilocin.h"

/*	proof of concept Marshaller - returns length of 
	JSON'd string	*/
static char *MarshalFab(ps_Crew *c) {
	return ps_Format("\"%s\":{\"status\":\"%s\"},", 
		c->tag, 
		ps_CrewStatusStr[c->status]
	);	
}

/* 	must remember to free the marshalled char 	*/
/* The Fab Four - MATTHEW, MARK, LUKE and RINGO 
	Dummy Crew members for testing with */
static ps_CrewStatus MATTHEW(ps_Crew *c) {
	c->destroy = c->update = MATTHEW;
	c->tag = "Matthew";
	c->marshal = MarshalFab;
	puts("hello");
	return ps_PAUSE;
}

static ps_CrewStatus MARK(ps_Crew *c) {
	c->destroy = c->update = MARK;
	c->tag = "Mark";
	c->marshal = MarshalFab;
	puts("hey hey");
	return ps_CUT;
}

static ps_CrewStatus LUKE(ps_Crew *c) {
	c->destroy = c->update = LUKE;
	c->tag = "Luke";
	c->marshal = MarshalFab;
	puts("hi");
	return ps_LIVE;
}

static ps_CrewStatus RINGO(ps_Crew *c) {
	c->destroy = c->update = RINGO;
	c->tag = "Ringo";
	c->marshal = MarshalFab;
	puts("HULLO THUR");
	static int i = 0;
	i++;
	if (i > 10) return ps_EXIT;
	else return ps_LIVE;
}

int main() {
	/* All the world's a stage,	*/
	ps_CrewNew(ps_STAGE);
	ps_CrewNew(MARK);
	ps_CrewNew(MATTHEW);
	ps_CrewNew(LUKE);
	ps_CrewNew(RINGO);
	while (ps_CrewRoll()) continue;	
	puts(ps_CrewMarshal());
	ps_CrewPurge();

	return 0;
	/* Sans teeth, sans eyes, sans taste, sans everything. */
}
