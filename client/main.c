#include "psilocin.h"

ps_CrewStatus MATTHEW(ps_Crew *c) {
	c->destroy = c->update = MATTHEW;
	c->tag = "Matthew";
	puts("hello");
	return ps_PAUSE;
}

ps_CrewStatus MARK(ps_Crew *c) {
	c->destroy = c->update = MARK;
	c->tag = "Mark";
	puts("hey hey");
	return ps_CUT;
}

ps_CrewStatus LUKE(ps_Crew *c) {
	c->destroy = c->update = LUKE;
	c->tag = "Luke";
	puts("hi");
	return ps_CUT;
}

ps_CrewStatus RINGO(ps_Crew *c) {
	c->destroy = c->update = RINGO;
	c->tag = "Ringo";
	puts("HULLO THUR");
	static int i = 0;
	i++;
	if (i > 10) return ps_EXIT;
	else return ps_LIVE;
}

int main() {
//	mp_Atomize(ps_GetHTTP("https://api.github.com/users/ploe/repos"), ps_PrintAtom);
//	ps_Crew c;
//	ps_STAGE(&c);
	//ps_CrewNew(ps_STAGE);
	ps_CrewNew(MATTHEW);
	ps_CrewNew(MARK);
	ps_CrewNew(LUKE);
	ps_CrewNew(RINGO);
	ps_CrewNew(LUKE);
	
	while (ps_CrewRoll()) continue;
	ps_CrewRollCall();

	ps_CrewTidy();
	ps_CrewRollCall();
	ps_CrewPurge();
	ps_CrewRollCall();
	return 0;
}
