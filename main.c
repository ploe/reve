#include "reve.h"

typedef struct {
	int *map;
	SDL_Surface *surface;
	rv_Bool enabled;
} rv_Layer;

int main() {

	/* All the world's a stage,	*/
	rv_CrewNew(rv_STAGE);
//	char *str = rv_CrewMarshal();
//	puts(str);
//	mp_Atomize(str, rv_PrintAtom);
//	free(str);
	while (rv_CrewRoll()) continue;	
	rv_CrewPurge();
	/* Sans teeth, sans eyes, sans taste, sans everything. */

	return 0;
}
