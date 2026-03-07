#ifndef SHADER_H
#define SHADER_H

#include <OpenGL/gl3.h>

// -- interface --

GLuint Shader_Create(
  GLuint shaderType,
  const char* shaderSource
);

void Shader_Release(
  GLuint shaderId
);

#endif