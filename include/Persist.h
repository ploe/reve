#ifndef _REVE_PERSIST_H
#define _REVE_PERSIST_H

rv_Bool rv_PersistCreateTable(sqlite3 *db, const char *table);
rv_Bool rv_PersistSavePair(sqlite3 *db, const char *table, const char *key, const char *value);

#endif
