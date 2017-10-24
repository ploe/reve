# Text

`Text` is the module where the type `rv_Text` is defined.

The module `Text` is described in the header [rv/Text.h](/include/rv/Text.h) and
implemented in [Text.c](/src/Text.c)

It is a `typedef` of a `char *` and the methods are to simplify the
dynamic allocation and destruction of them.

The impetus to wrapping this up was to make it clear which `char *` are
dynamically allocated, and therefore need freeing and which aren't.

`const char *` is used when this is ambiguous or hardcoded.

## Methods

### rv_Text rv_TextNew(const char \*format, ...)

`rv_TextNew` dynamically allocates a new `rv_Text` object and returns it if
successful. It provides an interface like the C library function `printf`.

Here is an example from [Persist.c](/src/Persist.c) that illustrates how to use
it:

```c
rv_Text value = NULL;

switch (type) {
  case LUA_TNUMBER:
    value = rv_TextNew("%f", lua_tonumber(L, index));
  break;

  case LUA_TBOOLEAN:
    value = rv_TextNew("%s", lua_toboolean(L, index) ? "true" : "false");
  break;

  case LUA_TSTRING:
    value = rv_TextNew("%s", lua_tostring(L, index));
  break;
}
```

### rv_Text rv_TextFromFile(const char \*path)

Create a new rv_Text from the contents of the file at `path`.

### rv_Text rv_TextFree(rv_Text s)

Frees `s` and returns `NULL`.

```c
value = rv_TextFree(value);
```
