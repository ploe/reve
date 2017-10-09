linux:
	cc main.c -o lel `pkg-config sdl2 glu --cflags --libs`

apple:
	cc main.c -o lel -framework OpenGL `pkg-config glew sdl2 SDL2_image --cflags --libs`
