#ifndef _REVE_PERSIST_H
#define _REVE_PERSIST_H

typedef int (*SQLiteCallback)(void*,int,char**,char**);

int rv_PERSIST_MARSHAL(void *arg, int argc, char **argv, char **colName);

rv_Bool rv_PersistCreateTable(sqlite3 *db, const char *table);
rv_Bool rv_PersistSavePair(sqlite3 *db, const char *table, const char *key, const char *value);
rv_Bool rv_PersistClearTable(sqlite3 *db, const char *table);
rv_Bool rv_PersistSelectPairs(sqlite3 *db, const char *table, SQLiteCallback callback);

#endif
