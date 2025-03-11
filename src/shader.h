#ifndef SHADER_H
#define SHADER_H

#include <OpenGL/gl3.h>

GLuint ShaderCreate(
  GLuint shaderType,
  const char* shaderSource
);

void ShaderRelease(
  GLuint shaderId
);

#endif