#include "reve.h"


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
