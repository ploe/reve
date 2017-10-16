# Persist

Persist is the module used for saving and loading data. It is defined in
[rv/Persist.h](/include/rv/Persist.h) and implemented in
[Persist.c](/src/Persist.c).

It provides Lua bindings for saving and loading, and is backed by a `SQLite3`
database. The Lua interface is not aware that it's backed by `SQLite3`
[as the functions save and load the data as Lua tables](/docs/Lua API/Persist.md).

## Schema

The schema for each saved table is a list of `key-value pairs` with an extra
`column` for `type`. It is defined as below.

```c
static char *PERSIST_SCHEMA =
	"CREATE TABLE IF NOT EXISTS %s ("
		"key TEXT PRIMARY KEY, "
		"value TEXT, "
		"type UNSIGNED INT"
	")";
```

Each `pair` in the `table` that gets passed to
[rv.Save](/docs/Lua%20API/Persist.md#rvsavename-table) is pushed as a new row to
 the table `%s`.

 `%s` is the `name` passed to [rv.Save](/docs/Lua%20API/Persist.md#rvsavename-table).

 `type` is used during marshalling/unmarshalling and directly relates to the
 return value from `lua_type`. The only types that get marshalled are
 `LUA_TNUMBER`, `LUA_TBOOLEAN` and `LUA_TSTRING`.

## Functions

### rv_Bool rv_PersistLuaBindings()

Called by `rv_LuaInit`, it binds the Lua functions `rv.Save` and `rv.Load` to
the `rv` namespace.

### static int LuaSave(lua_State \*L)

Where the Lua `function` `rv.Save` is implemented. The signature of this
function is what Lua expects for its `cfunction` type. The behaviour of this
function is documented [here](/docs/Lua%20API/Persist.md#rvsavename-table).

### static int LuaLoad(lua_State \*L)

Where the Lua `function` `rv.Load` is implemented. The signature of this
function is what Lua expects for its `cfunction` type. The behaviour of this
function is documented [here](/docs/Lua%20API/Persist.md#rvloadname).
