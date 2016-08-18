compile = cc -g -O2 -I./hashish -I./murp ./hashish/hashish.o ./murp/murp.o
LIBCURL_FLAGS := `curl-config --cflags --libs`
SDL_FLAGS := `pkg-config sdl2 SDL2_image libcurl --cflags --libs`

all:
	$(compile) Crew.c Player.c Stage.c Utils.c main.c -o psi $(LIBCURL_FLAGS) $(SDL_FLAGS)
