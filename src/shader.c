#define GL_SILENCE_DEPRECATION

#include <stdio.h>
#include "shader.h"

GLuint ShaderCreate(GLuint shaderType, const char* shaderSource) {
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

void ShaderRelease(GLuint shaderId) {
    glDeleteShader(shaderId);
}