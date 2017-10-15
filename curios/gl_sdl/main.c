#define GLEW_STATIC
#include <GL/glew.h>
#include <png.h>

#ifdef __linux
#include <SDL2/SDL.h>
#include <SDL_opengl.h>
#elif __APPLE__
#include <SDL.h>
#include <SDL_opengl.h>
#endif

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

SDL_Window *window = NULL;
SDL_GLContext *context = NULL;

void panic(const char *msg) {
	fputs(msg, stderr);
	exit(-1);
}

int ConvertSurfaceToTexImage2D(SDL_Surface *surface) {
	GLint internal;
	GLenum format;
	GLenum type;

	switch (surface->format->format) {
		case SDL_PIXELFORMAT_ARGB8888:
		case SDL_PIXELFORMAT_RGB888:
			internal = GL_RGBA8;
			format = GL_BGRA;
			type = GL_UNSIGNED_INT_8_8_8_8_REV;
		break;

		case SDL_PIXELFORMAT_YV12:
		case SDL_PIXELFORMAT_IYUV:
		case SDL_PIXELFORMAT_NV12:
		case SDL_PIXELFORMAT_NV21:
			internal = GL_LUMINANCE;
			format = GL_LUMINANCE;
			type = GL_UNSIGNED_BYTE;
		break;

		#ifdef __MACOSX__
		case SDL_PIXELFORMAT_UYVY:
			internal = GL_RGB8;
			format = GL_YCBCR_422_APPLE;
			type = GL_UNSIGNED_SHORT_8_8_APPLE;
   		break;
   		#endif

		default:
			return -1;
		break;
  	}

	glTexImage2D(GL_TEXTURE_2D, 0, internal, surface->w, surface->h, 0, format, type, surface->pixels);
	return 0;
}

typedef struct {
	int width, height;
	png_bytep *rows;
} rv_PNG;

static rv_PNG PNGRead(const char *path) {
	FILE *fp = fopen(path, "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) panic("png not loaded");

	png_infop info = png_create_info_struct(png);
	if (!info) panic("png not loaded");

	if(setjmp(png_jmpbuf(png))) panic("png not loaded");

	png_init_io(png, fp);
	png_read_info(png, info);

	rv_PNG out;
	out.width = png_get_image_width(png, info);
	out.height = png_get_image_height(png, info);

	png_byte type = png_get_color_type(png, info);
	png_byte depth  = png_get_bit_depth(png, info);

	if (depth == 16) png_set_strip_16(png);

	if(type == PNG_COLOR_TYPE_PALETTE) png_set_palette_to_rgb(png);

	if(type == PNG_COLOR_TYPE_GRAY && depth < 8) png_set_expand_gray_1_2_4_to_8(png);

	if(png_get_valid(png, info, PNG_INFO_tRNS)) png_set_tRNS_to_alpha(png);

	switch (type) {
		case PNG_COLOR_TYPE_RGB:
		case PNG_COLOR_TYPE_GRAY:
		case PNG_COLOR_TYPE_PALETTE:
    			png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
		break;
	}

	switch (type) {
		case PNG_COLOR_TYPE_GRAY:
		case PNG_COLOR_TYPE_GRAY_ALPHA:
			png_set_gray_to_rgb(png);
		break;
	}

	png_read_update_info(png, info);

	out.rows = calloc(out.height, sizeof(png_bytep));
	int y;
	for (y = 0; y < out.height; y++) out.rows[y] = malloc(png_get_rowbytes(png, info));

	png_read_image(png, out.rows);
//	png_destroy_read_struct(&png, &info, NULL);

	fclose(fp);

	return out;

}

GLuint PNGLoadTexture(const char *path) {
	GLuint texture;
	glGenTextures(1, &texture);
	rv_PNG png = PNGRead(path);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, png.width, png.height, 0, GL_RGB, GL_UNSIGNED_BYTE, png.rows);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	return texture;
}

GLuint loadTexture(const char *filename, int *width, int *height) 
 {
   //header for testing if it is a png
   png_byte header[8];
   
   //open file as binary
   FILE *fp = fopen(filename, "rb");
   if (!fp) {
     panic("TEXTURE_LOAD_ERROR");
   }
   
   //read the header
   fread(header, 1, 8, fp);
   
   //test if png
   int is_png = !png_sig_cmp(header, 0, 8);
   if (!is_png) {
     fclose(fp);
     panic("TEXTURE_LOAD_ERROR");
   }
   
   //create png struct
   png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
       NULL, NULL);
   if (!png_ptr) {
     fclose(fp);
     panic("TEXTURE_LOAD_ERROR");
   }
   
   //create png info struct
   png_infop info_ptr = png_create_info_struct(png_ptr);
   if (!info_ptr) {
     png_destroy_read_struct(&png_ptr, (png_infopp) NULL, (png_infopp) NULL);
     fclose(fp);
     panic("TEXTURE_LOAD_ERROR");
   }
 
   //create png info struct
   png_infop end_info = png_create_info_struct(png_ptr);
   if (!end_info) {
     png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
     fclose(fp);
     panic("TEXTURE_LOAD_ERROR");
   }
 
   //png error stuff, not sure libpng man suggests this.
   if (setjmp(png_jmpbuf(png_ptr))) {
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     panic("TEXTURE_LOAD_ERROR");
   }
 
   //init png reading
   png_init_io(png_ptr, fp);
   
   //let libpng know you already read the first 8 bytes
   png_set_sig_bytes(png_ptr, 8);
 
   // read all the info up to the image data
   png_read_info(png_ptr, info_ptr);
 
   //variables to pass to get info
   int bit_depth, color_type;
   png_uint_32 twidth, theight;
 
   // get info about png
   png_get_IHDR(png_ptr, info_ptr, &twidth, &theight, &bit_depth, &color_type,
       NULL, NULL, NULL);
 
   //update width and height based on png info
   *width = twidth;
   *height = theight;
 
   // Update the png info struct.
   png_read_update_info(png_ptr, info_ptr);
 
   // Row size in bytes.
   int rowbytes = png_get_rowbytes(png_ptr, info_ptr);
 
   // Allocate the image_data as a big block, to be given to opengl
   png_byte *image_data = calloc(rowbytes * *height, sizeof(png_byte));
   if (!image_data) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
     fclose(fp);
     panic("TEXTURE_LOAD_ERROR");
   }
 
   //row_pointers is for pointing to image_data for reading the png with libpng
   png_bytep *row_pointers = calloc(*height, sizeof(png_bytep));
   if (!row_pointers) {
     //clean up memory and close stuff
     png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
//     delete[] image_data;
     fclose(fp);
     panic("TEXTURE_LOAD_ERROR");
   }
   // set the individual row_pointers to point at the correct offsets of image_data
   for (int i = 0; i < *height; ++i)
     row_pointers[*height - 1 - i] = image_data + i * rowbytes;
 
   //read the png into image_data through row_pointers
   png_read_image(png_ptr, row_pointers);
 
   //Now generate the OpenGL texture object
   GLuint texture;
   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);
   glTexImage2D(GL_TEXTURE_2D,0, GL_RGBA, *width, *height, 0,
       GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*) image_data);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
 
   //clean up memory and close stuff
   png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
   fclose(fp);
 
   return texture;
 }

int initGL() {
	glViewport(0.f, 0.f, SCREEN_WIDTH, SCREEN_HEIGHT);

	GLenum error = GL_NO_ERROR;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, 1.0, -1.0);

	error = glGetError();
	if (error != GL_NO_ERROR) panic("OpenGL failed to initialise.");

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	error = glGetError();
	if (error != GL_NO_ERROR) panic("OpenGL failed to initialise.");

	glClearColor(0.f, 1.f, 0.f, 1.f);
	glEnable(GL_TEXTURE_2D);

	return -1;
}

GLuint tex;
int init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) panic(SDL_GetError());

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	window = SDL_CreateWindow("opengl test", 0, 0, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if (!window)  panic(SDL_GetError());

	context = SDL_GL_CreateContext(window);

	if (!context) panic(SDL_GetError());

	if (SDL_GL_SetSwapInterval(1) < 0) panic(SDL_GetError());

	glewExperimental = GL_TRUE;
	glewInit();

	return -1;
}

void render() {
	SDL_GL_SwapWindow(window);
}

const char *FRAGMENT_SRC =
	"#version 330 core \n"
	"in vec3 Color; \n"
	"in vec2 Texcoord \n;"
	"out vec4 outColor; \n"
	"uniform sampler2D tex;"
	"void main() \n"
	"{ \n "
		" outColor = texture(tex, Texcoord) * vec4(Color, 1.0);\n"
	"}";

const char *VERTEX_SRC =
	"#version 330 core \n"
	"in vec2 position; \n"
	"in vec3 color; \n"
	"in vec2 texcoord; \n"
	"out vec3 Color; \n"
	"out vec2 Texcoord; \n"
	"void main() \n"
	"{ \n"
		 "Color = color; \n"
	 	"Texcoord = texcoord; \n"
	 	"gl_Position = vec4(position, 0.0, 1.0); \n"
	"} \n";

GLuint CompileShader(const char *src, GLenum type) {
	GLuint shader = glCreateShader(type);
   	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status) {
		const GLuint LENGTH = 1024;

		char msg[LENGTH];
		glGetShaderInfoLog(shader, LENGTH, NULL, msg);
		fprintf(stderr, "%s\n", src);
		panic(msg);
	}

	return shader;
}

int main(int argc, char *argv[]) {
	init();

	// create vertex array object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// create vertex buffer object
	GLuint vbo;
	glGenBuffers(1, &vbo);

	float vertices[] = {
		// position   red   green blue  tex coords
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    		-0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	// copy data in to it
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

 	// Create and compile the vertex shader
	GLuint vertexShader = CompileShader(VERTEX_SRC, GL_VERTEX_SHADER);

	// Create and compile the fragment shader
	GLuint fragmentShader = CompileShader(FRAGMENT_SRC, GL_FRAGMENT_SHADER);

	// link vertex and fragment shader in to a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// specify layout of vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, (sizeof(float) * 7), 0);


	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 7, (void*) (2 * sizeof(float)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));


//	glGenTextures(1, &tex);

//	glBindTexture(GL_TEXTURE_2D, tex);

//	SDL_Surface *surface = IMG_Load("./myke.png");
//	ConvertSurfaceToTexImage2D(surface);
//	SDL_FreeSurface(surface);

//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


//	GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");

//	GLuint texture = PNGLoadTexture("./myke.png");
	
	int width, height;
	GLuint texture = loadTexture("./myke.png", &width, &height);
	SDL_Event event;
	while (event.type != SDL_QUIT) {

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) return 0;
		}

		glClear(GL_COLOR_BUFFER_BIT);

//		static float alpha = -0.1f;
//		static float fade = 0.01;
//		if ((alpha >= 1.0f) || (alpha <= -1.0f)) fade = -fade;
//		alpha += fade;

//		glUniform3f(uniColor, 0.f, alpha, 0.f);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		render();
	}

	glDeleteTextures(1, &tex);
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
    	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &ebo);
    	glDeleteBuffers(1, &vbo);

    	glDeleteVertexArrays(1, &vao);

	SDL_GL_DeleteContext(context);
	SDL_Quit();
}
