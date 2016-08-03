#ifndef OPENGLFUNCS_HPP
#define OPENGLFUNCS_HPP

#include <GL/glew.h>

void createTexImage2D(int width, int height, GLenum iformat, GLenum format, GLenum type, GLuint *tex, void* data);
void createTextureBuffer(int size, GLenum format, GLuint *tex, GLuint *bo, void *data);
void createUniformBuffer(int size, GLuint *bo, void* data);

#endif // OPENGLFUNCS_HPP

