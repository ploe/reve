#include "reve.h"

/*	Adds a new member of the Crew to the state machine
	If allocation fails we break out of the program. We can assume a Crew
	member is imperative to the flow of the program.
*/
static char *MarshalDefault(rv_Crew *c);

static rv_Crew *top = NULL;
rv_Crew *rv_CrewNew(rv_Updater type) {
	rv_Crew *c = calloc(1, sizeof(rv_Crew));
	if (c) {
		if(top) top->prev = c;
		c->next = top;
		top = c;

		if(type) {
			c->status = type(c);
			c->type = type;
		}

		c->marshal = MarshalDefault;
	}
	else rv_Panic(rv_EOCREW_ALLOC, "rv_CrewNew: calloc failed to return anything. Out of memory?");
	return c;
}

/* delete the Crew member	from the stack*/
void rv_CrewFree(rv_Crew *c) {
	rv_Crew *prev = c->prev, *next = c->next;
	if (c->next) c->next->prev = c->prev;

	if (c->prev) c->prev->next = c->next;
	else top = c->next;

	if (c->destroy) rv_CrewCall(c, c->destroy);
	free(c);
}

/* delete all CUT members from the stack */
static rv_Bool rv_CrewCutFree() {
	rv_Crew *c;
	for (c = top; c != NULL; c = c->next) {
		if (c->status == rv_CUT) {
			rv_Crew *next = c->next;
			rv_CrewFree(c);

			if(next && next->prev) c = next->prev;
			else return rv_NO;
		}
	}
	return rv_YES;
}

void rv_CrewTidy() {
	while (!rv_CrewCutFree()) continue;
}

/* deletes **ALL** Crew members from the stack */
void rv_CrewPurge() {
	rv_Crew *c;
	for (c = top; c != NULL; c = top) {
		rv_Crew *next = c->next;
		rv_CrewFree(c);	
	}
}

/* execute every LIVE Crewmember */
rv_Bool rv_CrewRoll() {
	rv_Crew *c;
	for (c = top; c != NULL; c = c->next) {
		if ((c->status == rv_LIVE) && c->update) c->status = rv_CrewCall(c, c->update);
		if (c->status == rv_EXIT) return rv_NO;
	}
	return rv_YES;
}

const char *rv_CrewStatusStr[] = {
		"LIVE",
		"ENCORE",
		"PAUSE",
		"CUT",
		"EXIT"
};

/* 	shows the Crew members currently in the stack in stderr
 	now outputs JSON so we can do more with it!	*/
void rv_CrewRollCall() {
	rv_Crew *c;
	int i = 0;
	
	if (top != NULL) {
		fputs("[", stderr);
		for (c = top; c != NULL; c = c->next) {
			fprintf(stderr, "{\"run\":\"%d\",\"type\":\"%p\",\"tag\":\"%s\",\"status\":\"%s\"},", i++, c->type, c->tag, rv_CrewStatusStr[c->status]);
		}
		fputs("]\n", stderr);
	}
	else fputs("rv_CrewRollCall: Pretty vacant...\n", stderr);
}

/*	sticks the whole Crew in a JSON structure
	this string will need freeing!	*/
char *rv_CrewMarshal() {
	rv_Crew *c;
	char *dst = calloc(1, sizeof(char));
	for (c = top; c != NULL; c = c->next) {
		rv_Marshaller marshal = c->marshal;
		if (marshal) {
			char *str = marshal(c);
			char *tmp = rv_Format("%s%s", dst, str);

			free(dst);
			dst = tmp;
			free(str);
		}
	}

	char *tmp = rv_Format("{%s}", dst);
	free(dst);

	return tmp;
}

rv_CrewStatus rv_CrewCall(rv_Crew *c, rv_Updater func) {
	return func(c);
}

/*	proof of concept Marshaller - returns length of 
	JSON'd string	*/
static char *MarshalDefault(rv_Crew *c) {
	return rv_Format("\"%s\":{\"status\":\"%s\"},", 
		c->tag, 
		rv_CrewStatusStr[c->status]
	);	
}

/* 	must remember to free the marshalled char 	*/
/* The Fab Four - MATTHEW, MARK, LUKE and RINGO 
	Dummy Crew members for testing with */
static rv_CrewStatus MATTHEW(rv_Crew *c) {
	c->destroy = c->update = MATTHEW;
	c->tag = "Matthew";
	c->marshal = MarshalDefault;
	puts("hello");
	return rv_PAUSE;
}

static rv_CrewStatus MARK(rv_Crew *c) {
	c->destroy = c->update = MARK;
	c->tag = "Mark";
	c->marshal = MarshalDefault;
	puts("hey hey");
	return rv_CUT;
}

static rv_CrewStatus LUKE(rv_Crew *c) {
	c->destroy = c->update = LUKE;
	c->tag = "Luke";
	c->marshal = MarshalDefault;
	puts("hi");
	return rv_LIVE;
}

static rv_CrewStatus RINGO(rv_Crew *c) {
	c->destroy = c->update = RINGO;
	c->tag = "Ringo";
	c->marshal = MarshalDefault;
	puts("HULLO THUR");
	static int i = 0;
	i++;
	if (i > 10) return rv_EXIT;
	else return rv_LIVE;
}


