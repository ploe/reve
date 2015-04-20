#include "psilocin.h"

int main() {
	/* all the world's a STAGE...	*/

	ps_Crew *stage = ps_CrewNew(ps_STAGE);
	ps_CrewRollCall();
	ps_Marshaller marshal = stage->marshal;
	puts(marshal(stage));
	while (ps_CrewRoll()) continue;
	ps_CrewPurge();
	ps_CrewRollCall();

	return 0;
}
