#include "reve.h"

GLuint rv_ShaderLoad(const char *path, GLenum type) {
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


