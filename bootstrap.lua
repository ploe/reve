player = Load("player")
print(player.name, player.age, player.game)

result = Save("player", {
  name="Miffy",
  age="29",
  game="wuh!",
  uhhh="yes",
  eeehuh = "no",
})

print(result)



