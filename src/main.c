#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB

#include <assert.h>
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <ufbx.h>
#include "input.h"
#include "file.h"
#include "mathutil.h"
#include "shader.h"
#include "shaderprogram.h"
#include "spherical.h"
#include "transform.h"
#include "vec3.h"
#include "vertex.h"

// -- interface --

bool initialize();
void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height);

// -- constants --

/// the base camera speed in degrees
const float CAMERA_SPEED = 3.0f;

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
    GLint modelMatricesId = glGetUniformLocation(shaderProgram, "modelMatrices");

    // load scene
    ufbx_load_opts sceneLoadOpts = {0};
    ufbx_error sceneLoadError;

    ufbx_scene* scene = ufbx_load_file("assets/scene.fbx", &sceneLoadOpts, &sceneLoadError);
    if (scene == NULL) {
        printf("ERROR::SCENE::LOAD_FAILED\n%s\n", sceneLoadError.description.data);
        return 3;
    }

    ufbx_mesh_list meshes = scene->meshes;

    // calculate the total number of vertices and indices
    size_t numVertices = 0;
    size_t numElements = 0;

    for(int meshIndex = 0; meshIndex < meshes.count; meshIndex++) {
        ufbx_mesh* mesh = scene->meshes.data[meshIndex];
        numVertices += mesh->num_vertices;
        numElements += mesh->num_indices;
    }

    // TODO: add logger w/ levels
    printf("\nscene:\n- meshes:   %zu\n- vertices: %zu\n- elements: %zu\n", meshes.count, numVertices, numElements);

    // allocate opengl buffer data
    Vertex vertices[numElements];
    GLuint elements[numElements];
    GLfloat modelMatrices[meshes.count * TRANSFORM_LEN];

    for (int i = 0; i < numVertices; i++) {
        Vertex_Init(&vertices[i]);
    }

    // prepare opengl data
    int currVerticesIndex = 0;
    int currIndicesIndex = 0;

    for (int meshIndex = 0; meshIndex < meshes.count; meshIndex++) {
        ufbx_mesh* mesh = scene->meshes.data[meshIndex];

        printf("\nmesh %d:\n- vertices: %zu\n- elements: %zu\n- normals:  %zu\n- colors:   %zu\n", meshIndex, mesh->num_vertices, mesh->num_indices, mesh->vertex_normal.indices.count, mesh->vertex_color.indices.count);

        // prepare model matrix buffer for opengl
        ufbx_node_list instances = mesh->instances;
        assert(instances.count == 1);

        ufbx_node* instance = instances.data[0];
        ufbx_matrix objectToWorld = instance->node_to_parent;

        Transform modelMatrix;
        Transform_InitWithColumns(&modelMatrix, objectToWorld.v);
        for(int modelMatrixIndex = 0; modelMatrixIndex < 16; modelMatrixIndex++) {
            modelMatrices[meshIndex * 16 + modelMatrixIndex] = modelMatrix.v[modelMatrixIndex];
        }

        // prepare vertex buffer for opengl
        for (int i = 0; i < mesh->num_indices; i++) {
            int j = (i + currVerticesIndex);
            Vertex* vertex = &vertices[j];

            // add the mesh index
            vertex->meshIndex = meshIndex;

            // apply position from mesh
            ufbx_vertex_vec3 positions = mesh->vertex_position;
            ufbx_vec3 pos = positions.values.data[positions.indices.data[i]];
            vertex->pos = Vec3_FromDouble(pos.v);

            // get the vertex normal
            ufbx_vertex_vec3 normals = mesh->vertex_normal;
            ufbx_vec3 normal = normals.values.data[normals.indices.data[i]];
            vertex->normal = Vec3_FromDouble(normal.v);

            // get the vertex color
            ufbx_vertex_vec4 colors = mesh->vertex_color;
            ufbx_vec4 color = colors.values.data[colors.indices.data[i]];
            vertex->color = Vec3_FromDouble(color.v);
        }

        // prepare element buffer for opengl
        for(int i = 0; i < mesh->num_indices; i++) {
            elements[i + currIndicesIndex] = i + currVerticesIndex;
        }

        currVerticesIndex += mesh->num_vertices;
        currIndicesIndex += mesh->num_indices;
    }

    // free scene
    ufbx_free_scene(scene);

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

    // the number of 4-byte data elements in the vertex
    const size_t stride = sizeof(Vertex) / 4;

    // mesh index attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    // position attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(1 * sizeof(GLuint)));

    // normal attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(1 * sizeof(GLuint) + 3 * sizeof(GLfloat)));

    // color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(1 * sizeof(GLuint) + 6 * sizeof(GLfloat)));

    // bind element data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glBindVertexArray(0);

    // define camera properties
    Vec3 center = { .x = 0.5f, .y = 2.0f, .z = 0.0f };
    Vec3 gaze = { .x = 0.0f, .y = 0.0f, .z = -1.0f };
    Vec3 up = { .x = 0.0f, .y = 1.0f, .z = 0.0f };

    // the initial camera spherical position on the sphere
    Spherical cameraSpherePos = {
        .radius = 10.0f,
        .azimuth = PI_2,
    };

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

        // scale the camera input speed
        Spherical cameraSphereDelta = input.camera;
        Spherical_Scale(&cameraSphereDelta, CAMERA_SPEED * DEG2RAD);

        // move the camera
        Spherical_Add(cameraSpherePos, cameraSphereDelta, &cameraSpherePos);

        // calculate the camera eye position & look direction
        Vec3 cameraPos = Spherical_ToCartesian(&cameraSpherePos);

        Vec3 gaze;
        Vec3_Scale(cameraPos, -1.0f, &gaze);
        Vec3_Normalize(gaze, &gaze);

        Vec3 eye = cameraPos;
        Vec3_Add(eye, center, &eye);

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
        glUniformMatrix4fv(modelMatricesId, meshes.count, GL_TRUE, modelMatrices);

        // bind vertex data
        glBindVertexArray(vao);

        // render
        glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_INT, 0);
        //glDrawElementsBaseVertex(GL_TRIANGLES, indices.count, GL_UNSIGNED_INT, 0, 0);

        // release render settings
        glUseProgram(0);
        glBindVertexArray(0);

        // swap front & back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    // free resources
    ShaderProgram_Release(shaderProgram);

    // quit
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