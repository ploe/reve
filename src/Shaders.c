#include "reve.h"

GLuint rv_ShaderLoad(const char *path, GLenum type) {
	rv_Text src = rv_TextFromFile(path);

	GLuint shader = glCreateShader(type);
   	glShaderSource(shader, 1, (const GLchar **) &src, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status) {
		const GLuint LENGTH = 1024;

		char msg[LENGTH];
		glGetShaderInfoLog(shader, LENGTH, NULL, msg);
		fprintf(stderr, "%s\n", src);
		rv_Panic(rv_EGL, msg);
	}

	src = rv_TextFree(src);
	return shader;
}


