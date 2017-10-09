#include "reve.h"

static char *PersistSchema() {
	return "CREATE TABLE IF NOT EXISTS %s ("
		"id INT PRIMARY KEY, " 
		"key TEXT, " 
		"value TEXT"
	")";
}

rv_Bool rv_PersistCreateTable(sqlite3 *db, const char *table) {
	char *schema = rv_TextNew(PersistSchema(), table);
	int rc;
	char *err = NULL;
	puts(schema);
	rc = sqlite3_exec(db, schema, NULL, 0, &err);
	if (rc != SQLITE_OK) {
		
		rv_TextFree(schema);
		rv_Panic(-1, err);
	}

	rv_TextFree(schema);
	return rv_YES;
}

rv_Bool rv_PersistSavePair(sqlite3 *db, const char *table, const char *key, const char *value) {
	const char *query = "INSERT INTO %s (key, value) VALUES(\"hello\", \"world\")";
	rv_Text insert = rv_TextNew(query, table);
	puts(insert);
	int rc;
	char *err = NULL;

	rc = sqlite3_exec(db, insert, NULL, 0, &err);
	if (rc != SQLITE_OK) {
		
		rv_TextFree(insert);
		rv_Panic(-1, err);
	}

	rv_TextFree(insert);
	return rv_YES;
}
