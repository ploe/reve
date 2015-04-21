#include "psilocin.h"

/*	Adds a new member of the Crew to the state machine
	If allocation fails we break out of the program. We can assume a Crew
	member is imperative to the flow of the program.
*/
static char *MarshalDefault(ps_Crew *c);

static ps_Crew *top = NULL;
ps_Crew *ps_CrewNew(ps_Updater type) {
	ps_Crew *c = calloc(1, sizeof(ps_Crew));
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
	else ps_Panic(ps_EOCREW_ALLOC, "ps_CrewNew: calloc failed to return anything. Out of memory?");
	return c;
}

/* delete the Crew member	from the stack*/
void ps_CrewFree(ps_Crew *c) {
	ps_Crew *prev = c->prev, *next = c->next;
	if (c->next) c->next->prev = c->prev;

	if (c->prev) c->prev->next = c->next;
	else top = c->next;

	if (c->destroy) ps_CrewCall(c, c->destroy);
	free(c);
}

/* delete all CUT members from the stack */
static ps_Bool ps_CrewCutFree() {
	ps_Crew *c;
	for (c = top; c != NULL; c = c->next) {
		if (c->status == ps_CUT) {
			ps_Crew *next = c->next;
			ps_CrewFree(c);

			if(next && next->prev) c = next->prev;
			else return ps_NO;
		}
	}
	return ps_YES;
}

ps_Crew ps_CrewTidy() {
	while (!ps_CrewCutFree()) continue;
}

/* deletes **ALL** Crew members from the stack */
void ps_CrewPurge() {
	ps_Crew *c;
	for (c = top; c != NULL; c = top) {
		ps_Crew *next = c->next;
		ps_CrewFree(c);	
	}
}

/* execute every LIVE Crewmember */
ps_Bool ps_CrewRoll() {
	ps_Crew *c;
	for (c = top; c != NULL; c = c->next) {
		if ((c->status == ps_LIVE) && c->update) c->status = ps_CrewCall(c, c->update);
		if (c->status == ps_EXIT) return ps_NO;
	}
	return ps_YES;
}

const char *ps_CrewStatusStr[] = {
		"LIVE",
		"ENCORE",
		"PAUSE",
		"CUT",
		"EXIT"
};

/* 	shows the Crew members currently in the stack in stderr
 	now outputs JSON so we can do more with it!	*/
void ps_CrewRollCall() {
	ps_Crew *c;
	int i = 0;
	
	if (top != NULL) {
		fputs("[", stderr);
		for (c = top; c != NULL; c = c->next) {
			fprintf(stderr, "{\"run\":\"%d\",\"type\":\"%p\",\"tag\":\"%s\",\"status\":\"%s\"},", i++, c->type, c->tag, ps_CrewStatusStr[c->status]);
		}
		fputs("]\n", stderr);
	}
	else fputs("ps_CrewRollCall: Pretty vacant...\n", stderr);
}

/*	sticks the whole Crew in a JSON structure
	this string will need freeing!	*/
char *ps_CrewMarshal() {
	ps_Crew *c;
	char *dst = calloc(1, sizeof(char));
	for (c = top; c != NULL; c = c->next) {
		ps_Marshaller marshal = c->marshal;
		if (marshal) {
			char *str = marshal(c);
			char *tmp = ps_Format("%s%s", dst, str);

			free(dst);
			dst = tmp;
			free(str);
		}
	}

	char *tmp = ps_Format("{%s}", dst);
	free(dst);

	return tmp;
}

ps_CrewStatus ps_CrewCall(ps_Crew *c, ps_Updater func) {
	return func(c);
}

/*	proof of concept Marshaller - returns length of 
	JSON'd string	*/
static char *MarshalDefault(ps_Crew *c) {
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
	c->marshal = MarshalDefault;
	puts("hello");
	return ps_PAUSE;
}

static ps_CrewStatus MARK(ps_Crew *c) {
	c->destroy = c->update = MARK;
	c->tag = "Mark";
	c->marshal = MarshalDefault;
	puts("hey hey");
	return ps_CUT;
}

static ps_CrewStatus LUKE(ps_Crew *c) {
	c->destroy = c->update = LUKE;
	c->tag = "Luke";
	c->marshal = MarshalDefault;
	puts("hi");
	return ps_LIVE;
}

static ps_CrewStatus RINGO(ps_Crew *c) {
	c->destroy = c->update = RINGO;
	c->tag = "Ringo";
	c->marshal = MarshalDefault;
	puts("HULLO THUR");
	static int i = 0;
	i++;
	if (i > 10) return ps_EXIT;
	else return ps_LIVE;
}


