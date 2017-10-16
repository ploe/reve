# Persist

Persist is the module used for Saving and Loading data.

It is currently backed by `SQLite3` in engine, however you do not
need to be aware of the schema or have to write any queries. You simply
drop in your Lua `table` and the data gets automatically marshaled to the
database.

It only marshals `numbers`, `booleans` and `strings`. The other types in Lua
represent more sophisticated constructs that are defined in the engine itself.

## rv.Save(name, table)

Returns a `boolean` `true` if saving was successful and a `boolean` `false` if
not.

`rv.Save` is a function used to store a Lua `table` between sessions. What this
means is that you close the app and reopen it, you can reload it.

Below is an example of how to use it:

```lua
result = rv.Save("player", {
  name="Mr Puyo",
  age=30,
  game="still this one",
  uhhh="yes",
  eeehuh = "no",
  sexy=true,
  terrible=false,
  pi=3.14
})
```

## rv.Load(name)

Returns the `table` saved with `name` if one is found. Otherwise the engine
panics and exits.

Here's an example of how to use it:

```lua
player = rv.Load("player")
print(player.name, player.age / 2, player.game, player.pi, player.sexy, player.terrible)

if player.sexy then print("Yay!") end
if not player.terrible then print("Woo!") end
```
