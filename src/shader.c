#define GL_SILENCE_DEPRECATION

#include <stdio.h>
#include "shader.h"

// -- implementation --

GLuint Shader_Create(GLuint shaderType, const char* shaderSource) {
    GLuint shaderId = glCreateShader(shaderType);
    glShaderSource(shaderId, 1, &shaderSource, NULL);
    glCompileShader(shaderId);

    GLint success;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

    if(success == GL_FALSE) {
        char infoLog[512];
        glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
        printf("ERROR::SHADER::%d::COMPILATION_FAILED\n%s\n", shaderType, infoLog);
    }

    return shaderId;
}

void Shader_Release(GLuint shaderId) {
    glDeleteShader(shaderId);
}