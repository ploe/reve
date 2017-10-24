#ifndef _REVE_SHADERS_H
#define _REVE_SHADERS_H

rv_Bool rv_ShadersInit();
rv_Bool rv_ShadersDestroy();
GLuint rv_ShaderLoad(const char *path, GLenum type);

#endif
