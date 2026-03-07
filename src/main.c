#include <assert.h>
#include <stddef.h>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB

#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>
#include <stdbool.h>
#include <stdio.h>
#include <ufbx.h>
#include "arrayutil.h"
#include "input.h"
#include "file.h"
#include "mathutil.h"
#include "shader.h"
#include "shaderprogram.h"
#include "transform.h"
#include "vec2.h"
#include "vec3.h"

// -- interface --

bool initialize();
void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height);

// -- constants --

const float CAMERA_SPEED = 0.01f;

// -- globals --

float windowAspectRatio = -1.0f;

// -- main --

int main(void) {
    // load shaders
    File vertexShaderSource;
    if (!File_Load(vertexShaderSource, "src/shaders/vert.glsl")) {
        return 1;
    }

    File fragmentShaderSource;
    if (!File_Load(fragmentShaderSource, "src/shaders/frag.glsl")) {
        return 2;
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

    // TODO: this could all be encapsulated in ShaderProgram_Create
    // compile shaders
    GLuint vertexShader = Shader_Create(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = Shader_Create(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint shaderProgram = ShaderProgram_Create(vertexShader, fragmentShader);

    Shader_Release(vertexShader);
    Shader_Release(fragmentShader);

    GLint projMatrixId = glGetUniformLocation(shaderProgram, "projMatrix");
    GLint viewMatrixId = glGetUniformLocation(shaderProgram, "viewMatrix");

    // load scene
    ufbx_load_opts sceneLoadOpts = {0};
    ufbx_error sceneLoadError;

    ufbx_scene* scene = ufbx_load_file("assets/scene.fbx", &sceneLoadOpts, &sceneLoadError);
    if (scene == NULL) {
        printf("ERROR::SCENE::LOAD_FAILED\n%s\n", sceneLoadError.description.data);
        return 3;
    }

    ufbx_mesh_list meshes = scene->meshes;

    size_t numVertices = 0;
    size_t numIndices = 0;
    for(int meshIndex = 0; meshIndex < meshes.count; meshIndex++) {
        ufbx_mesh* mesh = scene->meshes.data[meshIndex];
        numVertices += mesh->num_vertices;
        numIndices += mesh->num_indices;
    }

    // each vertex is comprised of one position (xyz) and one color (rgb)
    const size_t stride = 6;

    GLfloat vertices[numVertices * stride];
    GLuint indices[numIndices];

    int currVerticesIndex = 0;
    int currIndicesIndex = 0;

    // TODO: add logger w/ levels
    printf("scene\n---\nmeshes: %zu\nvertices: %zu\nindices: %zu\n", meshes.count, numVertices, numIndices);

    for (int meshIndex = 0; meshIndex < meshes.count; meshIndex++) {
        ufbx_mesh* mesh = scene->meshes.data[meshIndex];

        // prepare model matrix for opengl
        ufbx_node_list instances = mesh->instances;
        assert(instances.count == 1);

        ufbx_node* instance = instances.data[0];
        ufbx_matrix objectToWorld = instance->node_to_parent;

        Transform modelMatrix;
        Transform_InitWithColumns(&modelMatrix, objectToWorld.v);

        // prepare vertex buffer for opengl
        ufbx_vec3_list vertexPositions = mesh->vertices;
        assert(vertexPositions.count == mesh->num_vertices);

        for (int i = 0; i < vertexPositions.count; i++) {
            int j = (i + currVerticesIndex) * stride;

            // apply position from mesh
            ufbx_vec3 vertexPos = vertexPositions.data[i];

            vertices[j + 0] = vertexPos.x;
            vertices[j + 1] = vertexPos.y;
            vertices[j + 2] = vertexPos.z;

            // AAA: figure out how to pass the model matrix correctly into the shader
            VecH v0 = {
                .x = vertexPos.x,
                .y = vertexPos.y,
                .z = vertexPos.z,
                .w = 1,
            };

            VecH v1;
            Transform_Apply(modelMatrix, v0, &v1);

            vertices[j + 0] = v1.x;
            vertices[j + 1] = v1.y;
            vertices[j + 2] = v1.z;
        }

        ufbx_vertex_vec4 vertexColor = mesh->color_sets.data[0].vertex_color;
        for (int i = 0; i < vertexColor.indices.count; i++) {
            int colorIndex = vertexColor.indices.data[i];
            ufbx_vec4 color = vertexColor.values.data[colorIndex];
            int vertexPosIndex = mesh->vertex_indices.data[i];

            // apply vertex color for mesh
            int j = (vertexPosIndex + currVerticesIndex) * stride;
            vertices[j + 3] = color.x;
            vertices[j + 4] = color.y;
            vertices[j + 5] = color.z;
        }

        // prepare index buffer for opengl
        ufbx_uint32_list vertexIndices = mesh->vertex_indices;
        assert(vertexIndices.count == mesh->num_indices);

        for(int i = 0; i < vertexIndices.count; i++) {
            indices[i + currIndicesIndex] = vertexIndices.data[i] + currVerticesIndex;
        }

        currVerticesIndex += mesh->num_vertices;
        currIndicesIndex += mesh->num_indices;
    }

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // bind element data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

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
        Input_Read(&input, window);

        // handle input
        if (input.quit) {
            glfwSetWindowShouldClose(window, true);
        }

        // move camera
        Vec2 cameraTranslate;
        Vec2_Scale(input.cameraTranslate, CAMERA_SPEED, &cameraTranslate);

        eye.x += cameraTranslate.x;
        eye.y += cameraTranslate.y;

        // create perspective transform
        Transform perspectiveProjection;
        Transform_InitPerspectiveProjection(
            &perspectiveProjection,
            60.0f * DEG2RAD,
            (float)windowAspectRatio,
            -0.1f,
            -1000.0f
        );

        // create camera transform
        Transform camera;
        Transform_InitCamera(&camera, eye, gaze, up);

        // clear the screen
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // select the shaders
        glUseProgram(shaderProgram);

        // bind uniforms
        glUniformMatrix4fv(projMatrixId, 1, GL_TRUE, perspectiveProjection.v);
        glUniformMatrix4fv(viewMatrixId, 1, GL_TRUE, camera.v);

        // bind vertex data
        glBindVertexArray(vao);

        // render
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
        //glDrawElementsBaseVertex(GL_TRIANGLES, indices.count, GL_UNSIGNED_INT, 0, 0);
        glBindVertexArray(0);

        // swap front & back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    // TODO: do we need some kind of quit function?

    // free resources
    ufbx_free_scene(scene);
    ShaderProgram_Release(shaderProgram);

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

// -- events --

void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    windowAspectRatio = (float)width / height;
}