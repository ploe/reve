#include "reve.h"

static char *PERSIST_SCHEMA =
	"CREATE TABLE IF NOT EXISTS %s ("
		"id INT PRIMARY KEY, " 
		"key TEXT, " 
		"value TEXT"
	")";

static rv_Bool PersistExec(sqlite3 *db, rv_Text query) {
	int rc;
	char *err = NULL;
	rc = sqlite3_exec(db, query, NULL, 0, &err);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "%s\n", query);

		rv_TextFree(query);
		rv_Panic(-1, err);
	}

	return rv_YES;
}

rv_Bool rv_PersistCreateTable(sqlite3 *db, const char *table) {
	rv_Text schema = rv_TextNew(PERSIST_SCHEMA, table);
	PersistExec(db, schema);
	rv_TextFree(schema);

	return rv_YES;
}

rv_Bool rv_PersistSavePair(sqlite3 *db, const char *table, const char *key, const char *value) {
	const char *query = "INSERT INTO %s (key, value) VALUES(?, ?)";
	rv_Text insert = rv_TextNew(query, table);

	sqlite3_stmt *stmt;
	sqlite3_prepare_v2(db, insert, -1, &stmt, NULL);
	sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC);
	sqlite3_bind_text(stmt, 2, value, -1, SQLITE_STATIC);

	int rc;
	rc = sqlite3_step(stmt);
	if (rc != SQLITE_DONE) {	
		rv_TextFree(insert);
		rv_Panic(-1, sqlite3_errmsg(db));
	}

	rv_TextFree(insert);
	return rv_YES;
}

rv_Bool rv_PersistClearTable(sqlite3 *db, const char *table) {
	const char *query = "DROP TABLE %s";
	rv_Text drop = rv_TextNew(query, table);
	PersistExec(db, drop);
	rv_TextFree(drop);
	return rv_YES;
}
