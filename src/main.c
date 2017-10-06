#include "reve.h"

typedef struct {
	int *map;
	SDL_Surface *surface;
	rv_Bool enabled;
} rv_Layer;

rv_Bool rv_PersistCreateTable(sqlite3 *db, const char *table) {
	char *schema = rv_Format("CREATE TABLE IF NOT EXISTS %s(id PRIMARY KEY NOT NULL, key TEXT, value TEXT)", table);
	int rc;
	char *err = NULL;

	rc = sqlite3_exec(db, schema, NULL, 0, &err);
	if (rc != SQLITE_OK) {
		free(schema);
		rv_Panic(-1, err);
	}

	return rv_YES;
	free(schema);
}

int main(int argc, char *argv[]) {
	/* All the world's a stage,	*/
	rv_CrewNew(rv_STAGE);

	sqlite3 *db = rv_StageGetSQLite();
	rv_PersistCreateTable(db, "myke");

	while (rv_CrewRoll()) continue;

	rv_CrewPurge();
	/* Sans teeth, sans eyes, sans taste, sans everything. */

	return 0;
}
