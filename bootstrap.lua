player = rv.Load("player")
print(player.name, player.age, player.game)

result = rv.Save("player", {
  name="Mr Puyo",
  age="30",
  game="still this one",
  uhhh="yes",
  eeehuh = "no",
})

print(result)



