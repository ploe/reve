#include "psilocin.h"

int main() {
	/* all the world's a STAGE...	*/

	ps_CrewNew(ps_STAGE);
	ps_CrewRollCall();
	while (ps_CrewRoll()) continue;
	ps_CrewPurge();
	ps_CrewRollCall();

	return 0;
}
