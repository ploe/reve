# Lua

Lua is where the `L` the `lua_State` for the engine lives. It is defined in
[rv/Lua.h](/include/rv/Lua.h) and implemented in [Lua.c](/src/Lua.c).

Lua is going to be used across the app for various configuration tasks, from
defining elements of gameplay (Scenes, Actors, Saving, Loading etc.),
parsing the command line arguments using the `bootstrap` mechanism, and even
elements of context creation. These are still works in progress but this
documentation will be amended to elucidate on these functionalities once they
are complete.

## Functions

## lua_State \*rv_LuaInit()

Creates the global `lua_State` `L` which is a static variable in [Lua.c](/src/Lua.c).  

## rv_Bool rv_LuaImport(const char \*file)

Loads `file` and runs it on the `lua_State` `L`. If loading suceeds it returns a
`rv_Boolean` `rv_YES` and if it fails it returns `rv_NO`.

You do not need to specify the file extension. It looks for the bareword and the
extensions `.lua` and `.luac`.

### lua_State \*rv_LuaGet()

Returns the `lua_State` `L`.

### rv_Bool rv_LuaBind(const char \*key, lua_CFunction func, ...);

Binds the Lua `functions` to the `rv` namespace. What does this mean? Well all
the fuctions that the engine implements are in a `global` `table` called `rv` in
the `lua_State`.

This function is variadic and used like so:

```c
rv_LuaBind (
	"Save", LuaSave,
	"Load", LuaLoad,
	rv_NO, rv_NO
);
```

`key` is the the name in the `rv` namespace we want to address `func` as. The
function will keep parsing these pairs until it hits an empty `key` or `func`.

The two `rv_NO` in the above example represent an empty `key` and `func`. The
behaviour for only one of these entries being set it undefined.

### rv_Bool rv_LuaDestroy();

Calls `lua_close` on the `lua_State` `L`.
