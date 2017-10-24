#include "reve.h"

GLuint vertexShader;
GLuint fragmentShader;
GLuint shaderProgram;

rv_Bool rv_ShadersInit() {
 	// Create and compile the vertex shader
	vertexShader = rv_ShaderLoad("./shaders/default.vert", GL_VERTEX_SHADER);

	// Create and compile the fragment shader
	fragmentShader = rv_ShaderLoad("./shaders/default.frag", GL_FRAGMENT_SHADER);

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
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(rv_Vectors), rv_VERTEX_OFFSET);

	GLint texcoord = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texcoord);

	glVertexAttribPointer(texcoord, 2, GL_FLOAT, GL_FALSE, sizeof(rv_Vectors), rv_TEXTURE_OFFSET);

	GLint iota = glGetUniformLocation(shaderProgram, "iota");
	glUniform3f(iota, 2.0 / rv_STAGE_WIDTH, 2.0 / rv_STAGE_HEIGHT, 1.0 / rv_STAGE_HEIGHT);

	return rv_YES;
}

rv_Bool rv_ShadersDestroy() {
	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
    	glDeleteShader(vertexShader);

}

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


