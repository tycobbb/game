#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include "glutil.h"
#include "shader.h"
#include "vec3.h"
#include "vech.h"
#include "vecutil.h"
#include "transform.h"
#include "arrayutil.h"

bool initialize();
void readInput(GLFWwindow* window);
void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height);
GLuint ShaderProgramCreate(GLuint vertexShader, GLuint fragmentShader);

// -- constants --
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 fragColor; \n"
    "void main() {\n"
    "    fragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);\n"
    "}\0";

// -- main --
int main(void) {
    // init glfw
    if (!initialize()) {
        return -1;
    }

    // create window w/ title
    GLFWwindow* window = glfwCreateWindow(
        640, 480, "hello world", NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    // make the window's context current
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, onFrameBufferSizeChanged);

    // compile shaders
    GLuint vertexShader = ShaderCreate(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = ShaderCreate(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = ShaderProgramCreate(vertexShader, fragmentShader);

    ShaderRelease(vertexShader);
    ShaderRelease(fragmentShader);

    // the world
    Vec3 quad[] = {
        { .x = +0.5f, .y = +0.5f, .z = -2.0f },
        { .x = +0.5f, .y = -0.5f, .z = -2.0f },
        { .x = -0.5f, .y = -0.5f, .z = -2.0f },
        { .x = -0.5f, .y = +0.5f, .z = -2.0f }
    };

    // apply ortho transform
    float l = -2.0f;
    float r = +2.0f;
    float b = -2.0f;
    float t = +2.0f;
    float n = -1.0f;
    float f = -3.0f;

    Transform orthoViewTransform;
    orthoViewTransform.matrix[0][0] = 2.0f / (r - l); // 1
    orthoViewTransform.matrix[0][3] = -(r + l) / (r - l); // 0
    orthoViewTransform.matrix[1][1] = 2.0f / (t - b); // 1
    orthoViewTransform.matrix[1][3] = -(t + b) / (t - b); // 0
    orthoViewTransform.matrix[2][2] = 2.0f / (n - f); // 1
    orthoViewTransform.matrix[2][3] = -(n + f) / (n - f); // 2
    orthoViewTransform.matrix[3][3] = 1.0f; // 1
    // expected: {+0.5, +0.5, +0f}

    for(int i = 0; i < ARRAY_LEN(quad); i++) {
        VecH vertex;
        VecHFromVec3(quad[i], 1.0f, &vertex);

        VecH transformedVertex;
        TransformMultiply(orthoViewTransform, vertex, &transformedVertex);

        Vec3FromVecH(transformedVertex, quad + i);
    }

    // while the window is open
    while (!glfwWindowShouldClose(window)) {
        // input
        readInput(window);

        // setup buffers
        float vertices[ARRAY_LEN(quad) * 3];
        GlVec3ToVertices(ARRAY_LEN(quad), quad, vertices);

        GLuint indices[] = {
            0, 1, 3, // top right triangle
            1, 2, 3  // bottom left triangle
        };

        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);

        // render
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // swap front & back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

bool initialize() {
    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    return true;
}

void readInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

GLuint ShaderProgramCreate(GLuint vertexShader, GLuint fragmentShader) {
    GLuint programId = glCreateProgram();

    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    glLinkProgram(programId);

    GLint success;
    glGetProgramiv(programId, GL_LINK_STATUS, &success);
    if(success == GL_FALSE) {
        char infoLog[512];
        glGetProgramInfoLog(programId, 512, NULL, infoLog);
        printf("ERROR::PROGRAM::LINK_FAILED\n%s\n", infoLog);
    }

    return programId;
}

void ShaderProgramRelease(GLuint programId) {
}

// -- events --
void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}