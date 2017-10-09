#include "reve.h"

typedef struct {
	int *map;
	SDL_Surface *surface;
	rv_Bool enabled;
} rv_Layer;

int main(int argc, char *argv[]) {
	/* All the world's a stage,	*/
	rv_CrewNew(rv_STAGE);

	sqlite3 *db = rv_StageGetSQLite();
	rv_PersistCreateTable(db, "myke");
	rv_PersistSavePair(db, "myke", "key", "value");

	while (rv_CrewRoll()) continue;

	rv_CrewPurge();
	/* Sans teeth, sans eyes, sans taste, sans everything. */

	return 0;
}
