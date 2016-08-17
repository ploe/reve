compile = cc -g -O2 -I./hashish -I./murp ./hashish/hashish.o ./murp/murp.o
LIBCURL_FLAGS = `curl-config --cflags --libs`
SDL_FLAGS = `pkg-config sdl2 libcurl --cflags --libs`

all:
	$(compile) $(LIBCURL_FLAGS) $(SDL_FLAGS) Crew.c Player.c Stage.c Utils.c main.c -o psi
