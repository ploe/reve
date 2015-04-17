#include "psilocin.h"

int main() {
	mp_Atomize(ps_GetHTTP("https://api.github.com/users/ploe/repos"), ps_PrintAtom);
//	ps_Crew c;
//	ps_STAGE(&c);
//	ps_CrewNew(ps_STAGE);
//	while (ps_CrewRoll()) continue;
	return 0;
}
