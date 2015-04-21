#include "psilocin.h"

int main() {

	/* All the world's a stage,	*/
	ps_CrewNew(ps_STAGE);
//	char *str = ps_CrewMarshal();
//	puts(str);
//	mp_Atomize(str, ps_PrintAtom);
//	free(str);
	while (ps_CrewRoll()) continue;	
	ps_CrewPurge();
	/* Sans teeth, sans eyes, sans taste, sans everything. */

	return 0;
}
