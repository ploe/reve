player = rv.Load("player")
print(player.name, player.age / 2, player.game, player.pi, player.sexy, player.terrible)

if player.sexy then print("Yay!") end
if not player.terrible then print("Woo!") end


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

print("Saved?", result)


