compile = cc -g -O2 -I./libs/hashish -I./libs/murp -I./include ./libs/hashish/hashish.o ./libs/murp/murp.o
src = src/Crew.c src/Player.c src/Texture.c src/Layer.c src/Stage.c src/Utils.c src/main.c src/Persist.c src/Text.c
SDL_FLAGS := `pkg-config sdl2 SDL2_image --cflags --libs`
LUA_FLAGS := `pkg-config lua-5.2 --cflags --libs`
SQLITE_FLAGS := `pkg-config sqlite3 --cflags --libs`

all:
	$(compile) $(src) -o ./rv  $(SDL_FLAGS) $(LUA_FLAGS) $(SQLITE_FLAGS)
