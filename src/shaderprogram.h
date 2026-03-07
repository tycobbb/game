#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <OpenGL/gl3.h>

// -- interface --

GLuint ShaderProgram_Create(GLuint vertexShader, GLuint fragmentShader);

void ShaderProgram_Release(GLuint programId);

#endif