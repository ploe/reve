compile = cc -g -O2 -I./hashish -I./murp ./hashish/hashish.o ./murp/murp.o
SDL_FLAGS := `pkg-config sdl2 SDL2_image --cflags --libs`
LUA_FLAGS := `pkg-config lua-5.2 --cflags --libs`

all:
	$(compile) Crew.c Player.c Texture.c Layer.c Stage.c Utils.c main.c -o rv  $(SDL_FLAGS) $(LUA_FLAGS)
