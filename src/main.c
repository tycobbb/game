#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB

#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdio.h>
#include <ufbx.h>
#include "arrayutil.h"
#include "input.h"
#include "mathutil.h"
#include "file.h"
#include "rgb.h"
#include "shader.h"
#include "transform.h"
#include "vec2.h"
#include "vec3.h"
#include "vech.h"
#include "vecutil.h"

bool initialize();
void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height);
GLuint ShaderProgramCreate(GLuint vertexShader, GLuint fragmentShader);

// -- constants --
const float CAMERA_SPEED = 0.01f;

// -- globals --
float windowAspectRatio = -1.0f;

// -- main --
int main(void) {
    // load shaders
    File vertexShaderSource;
    if (!FileLoad(vertexShaderSource, "src/shaders/vert.glsl")) {
        return 1;
    }

    File fragmentShaderSource;
    if (!FileLoad(fragmentShaderSource, "src/shaders/frag.glsl")) {
        return 2;
    }

    // load the scene
    ufbx_load_opts sceneLoadOpts = {0};
    ufbx_error sceneLoadError;

    ufbx_scene* scene = ufbx_load_file("assets/cube.fbx", &sceneLoadOpts, &sceneLoadError);
    if (scene == NULL) {
        printf("ERROR::SCENE::LOAD_FAILED\n%s\n", sceneLoadError.description.data);
        return 3;
    }

    // init glfw
    if (!initialize()) {
        return 4;
    }

    // create window w/ title
    GLFWwindow* window = glfwCreateWindow(
        640,
        480,
        "cube",
        NULL,
        NULL
    );

    if (window == NULL) {
        glfwTerminate();
        return -1;
    }

    // make the window the current context
    glfwMakeContextCurrent(window);

    // update in response to viewport resizing
    glfwSetFramebufferSizeCallback(window, onFrameBufferSizeChanged);

    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    onFrameBufferSizeChanged(window, fbWidth, fbHeight);

    // compile shaders
    GLuint vertexShader = ShaderCreate(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = ShaderCreate(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = ShaderProgramCreate(vertexShader, fragmentShader);

    ShaderRelease(vertexShader);
    ShaderRelease(fragmentShader);

    GLint projMatrixId = glGetUniformLocation(shaderProgram, "projMatrix");
    GLint viewMatrixId = glGetUniformLocation(shaderProgram, "viewMatrix");

    // define camera properties
    Vec3 eye = { .x = 0.0f, .y = 0.0f, .z = 10.0f };
    Vec3 gaze = { .x = 0.0f, .y = 0.0f, .z = -1.0f };
    Vec3 up = { .x = 0.0f, .y = 1.0f, .z = 0.0f };

    // enable depth testing
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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

        // create perspective transform
        Transform perspectiveProjection;
        TransformInitPerspectiveProjection(
            &perspectiveProjection,
            60.0f * DEG2RAD,
            (float)windowAspectRatio,
            -0.1f,
            -1000.0f
        );

        // create camera transform
        Transform camera;
        TransformInitCamera(&camera, eye, gaze, up);

        // TODO: enumerate meshes
        ufbx_mesh* mesh = scene->meshes.data[0];

        // prepare vertex buffer for opengl
        ufbx_vec3_list vertexPositions = mesh->vertices;

        GLfloat vertices[(vertexPositions.count * 2) * 3];

        for(int i = 0; i < vertexPositions.count; i++) {
            int j = i * 6;

            // apply position from mesh
            ufbx_vec3 vertexPos = vertexPositions.data[i];

            vertices[j + 0] = vertexPos.x;
            vertices[j + 1] = vertexPos.y;
            vertices[j + 2] = vertexPos.z;
        }

        ufbx_vertex_vec4 vertexColor = mesh->color_sets.data[0].vertex_color;

        for (int i = 0; i < vertexColor.indices.count; i++) {
            int colorIndex = vertexColor.indices.data[i];
            ufbx_vec4 color = vertexColor.values.data[colorIndex];
            int vertexPosIndex = mesh->vertex_indices.data[i];

            // apply vertex color for mesh
            int j = vertexPosIndex * 6;
            vertices[j + 3] = color.x;
            vertices[j + 4] = color.y;
            vertices[j + 5] = color.z;
        }

        // prepare index buffer for opengl
        ufbx_uint32_list indices = mesh->vertex_indices;

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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.count, indices.data, GL_STATIC_DRAW);

        glBindVertexArray(0);

        // render
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        TransformArray projMatrix;
        TransformToArray(perspectiveProjection , projMatrix);
        glUniformMatrix4fv(projMatrixId, 1, GL_TRUE, projMatrix);

        TransformArray viewMatrix;
        TransformToArray(camera, viewMatrix);
        glUniformMatrix4fv(viewMatrixId, 1, GL_TRUE, viewMatrix);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // swap front & back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    // TODO: do we need some kind of quit function?
    ufbx_free_scene(scene);

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
    windowAspectRatio = (float)width / height;
}