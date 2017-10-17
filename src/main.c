#include "reve.h"

int main(int argc, char *argv[]) {
	/* All the world's a stage,	*/
	rv_CrewNew(rv_STAGE);

	sqlite3 *db = rv_StageGetSQLite();

	while (rv_CrewRoll()) continue;

	rv_CrewPurge();
	/* Sans teeth, sans eyes, sans taste, sans everything. */

	return 0;
}
