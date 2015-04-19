#include "psilocin.h"

/*	Adds a new member of the Crew to the state machine
	If allocation fails we break out of the program. We can assume a Crew
	member is imperative to the flow of the program.
*/
static ps_Crew *top = NULL;
ps_Crew *ps_CrewNew(ps_Updater type) {
	ps_Crew *c = calloc(1, sizeof(ps_Crew));
	if (c) {
		if(top) top->prev = c;
		c->next = top;
		top = c;

		if(type) {
			type(c);
			c->type = type;
		}
	}
	else ps_Panic(ps_EOCREW_ALLOC, "ps_CrewNew: calloc failed to return anything. Out of memory?");

	return c;
}

/* delete the Crew member	from the stack*/
void ps_CrewFree(ps_Crew *c) {
	ps_Crew *prev = c->prev, *next = c->next;
	if (c->next) c->next->prev = c->prev;
	if (c->prev) c->prev->next = c->next;
	if (c->destroy) ps_CrewCall(c, c->destroy);
	free(c);
}

/* delete all CUT members from the stack */
void ps_CrewTidy() {
	ps_Crew *c;
	for (c = top; c != NULL; c = c->next) {
		if (c->status == ps_CUT) {
			ps_Crew *next = c->next;
			ps_CrewFree(c);

			if(next) c = next->prev;
			else break;
		}
	}
}

/* deletes **ALL** Crew members from the stack */
void ps_CrewPurge() {
	ps_Crew *c;
    for (c = top; c != NULL; c = c->next) {
		ps_Crew *next = c->next;
		ps_CrewFree(c);

		if(next) c = next->prev;
        else break;
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

ps_CrewStatus ps_CrewCall(ps_Crew *c, ps_Updater func) {
	return func(c);
}
