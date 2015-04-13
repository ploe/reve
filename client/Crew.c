#include "psilocin.h"

/*	Adds a new member of the Crew to the state machine
	If allocation fails we break out of the program. We can assume a Crew
	member is imperative to the flow of the program.
*/
static ps_Crew *top;
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

ps_Bool ps_CrewRoll() {
	ps_Crew *c;
	for (c = top; c != NULL; c = c->next) {
		if (c->status == ps_LIVE) c->status = ps_CrewCall(c, c->update);
		if (c->status == ps_EXIT) return ps_NO;
	}
	return ps_YES;
}

ps_CrewStatus ps_CrewCall(ps_Crew *c, ps_Updater func) {
	return func(c);
}
