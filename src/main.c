#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include "shader.h"
#include "rgb.h"
#include "vec2.h"
#include "vec3.h"
#include "vech.h"
#include "vecutil.h"
#include "transform.h"
#include "arrayutil.h"
#include "input.h"
#include "file.h"

bool initialize();
void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height);
GLuint ShaderProgramCreate(GLuint vertexShader, GLuint fragmentShader);

// -- constants --
const float CAMERA_SPEED = 0.01f;

// -- main --
int main(void) {
    // load shaders
    File vertexShaderSource;
    if (!FileLoad(vertexShaderSource, "src/shaders/vert.glsl")) {
        return 1;
    }

    File fragmentShaderSource;
    if (!FileLoad(fragmentShaderSource, "src/shaders/frag.glsl")) {
        return 1;
    }

    // init glfw
    if (!initialize()) {
        return -1;
    }

    // create window w/ title
    GLFWwindow* window = glfwCreateWindow(640, 480, "hello world", NULL, NULL);
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
    Vec3 cubeVertices[] = {
        { .x = +0.5f, .y = +0.5f, .z = -2.0f }, // 0 (front top right)
        { .x = +0.5f, .y = -0.5f, .z = -2.0f }, // 1 (front bottom right)
        { .x = -0.5f, .y = -0.5f, .z = -2.0f }, // 2 (front bottom left)
        { .x = -0.5f, .y = +0.5f, .z = -2.0f }, // 3 (front top left)
        { .x = +0.5f, .y = +0.5f, .z = -3.0f }, // 4 (back top right)
        { .x = +0.5f, .y = -0.5f, .z = -3.0f }, // 5 (back bottom right)
        { .x = -0.5f, .y = -0.5f, .z = -3.0f }, // 6 (back bottom left)
        { .x = -0.5f, .y = +0.5f, .z = -3.0f }  // 7 (back top left)
    };

    Rgb cubeColors[] = {
        { .r = 1.0f, .g = 1.0f, .b = 1.0f },
        { .r = 0.0f, .g = 1.0f, .b = 1.0f },
        { .r = 0.0f, .g = 0.0f, .b = 1.0f },
        { .r = 1.0f, .g = 0.0f, .b = 0.0f },
        { .r = 1.0f, .g = 1.0f, .b = 0.0f },
        { .r = 1.0f, .g = 0.0f, .b = 1.0f },
        { .r = 0.0f, .g = 1.0f, .b = 0.0f },
        { .r = 0.0f, .g = 0.0f, .b = 0.0f },
    };

    // create ortho transform
    float l = -2.0f;
    float r = +2.0f;
    float b = -2.0f;
    float t = +2.0f;
    float n = -1.0f;
    float f = -5.0f;

    Transform perpectiveProjection;
    TransformInitPerspectiveProjection(&perpectiveProjection, l, r, b, t, n, f);

    Vec3 eye = {.x = 0.0f, .y = 0.0f, .z = 0.0f};
    Vec3 gaze = {.x = 0.0f, .y = 0.0f, .z = -1.0f};
    Vec3 up = {.x = 0.0f, .y = 1.0f, .z = 0.0f};

    // while the window is open
    Input input;
    while (!glfwWindowShouldClose(window)) {
        InputRead(&input, window);

        // handle input
        if (input.quit) {
            glfwSetWindowShouldClose(window, true);
        }

        // move camera
        Vec2 cameraTranslate;
        Vec2Scale(input.cameraTranslate, CAMERA_SPEED, &cameraTranslate);

        eye.x += cameraTranslate.x;
        eye.y += cameraTranslate.y;

        // create camera transform
        Transform camera;
        TransformInitCamera(&camera, eye, gaze, up);

        // combine world -> canonical view transforms
        Transform transform;
        TransformMultiply(perpectiveProjection, camera, &transform);

        // setup vertices for opengl
        GLfloat vertices[(ARRAY_LEN(cubeVertices) + ARRAY_LEN(cubeColors)) * 3];

        for(int i = 0; i < ARRAY_LEN(cubeVertices); i++) {
            // apply transform to objects
            VecH vertex;
            VecHFromVec3(cubeVertices[i], 1.0f, &vertex);

            VecH transformedVertex;
            TransformApply(transform, vertex, &transformedVertex);

            Vec3 projectedVertex;
            Vec3FromVecH(transformedVertex, &projectedVertex);

            int j = i * 6;
            vertices[j + 0] = projectedVertex.x;
            vertices[j + 1] = projectedVertex.y;
            vertices[j + 2] = projectedVertex.z;

            // apply vertex color
            Rgb vertexColor = cubeColors[i];
            vertices[j + 3] = vertexColor.r;
            vertices[j + 4] = vertexColor.g;
            vertices[j + 5] = vertexColor.b;
        }

        // setup buffers
        GLuint indices[] = {
            // front face
            0, 1, 3,
            1, 2, 3,
            // top face
            3, 4, 0,
            3, 7, 4,
            // back face
            4, 5, 7,
            7, 5, 6,
            // bottom face
            1, 5, 6,
            1, 6, 2,
            // left face
            3, 2, 7,
            2, 6, 7,
            // right face
            0, 4, 1,
            1, 4, 5
        };

        // create buffers
        GLuint vao;
        glGenVertexArrays(1, &vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);

        GLuint ebo;
        glGenBuffers(1, &ebo);

        // bind vertex array
        glBindVertexArray(vao);

        // bind vertex data
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        // bind element data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray(0);

        // render
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, ARRAY_LEN(indices), GL_UNSIGNED_INT, 0);
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