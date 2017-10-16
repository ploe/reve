player = rv.Load("player")
print(player.name, player.age, player.game)

result = rv.Save("player", {
  name="Puyo",
  age="30",
  game="this one",
  uhhh="yes",
  eeehuh = "no",
})

print(result)



