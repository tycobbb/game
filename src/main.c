#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_GLCOREARB

#import <GLFW/glfw3.h>
#import <stdbool.h>
#import <stdio.h>

bool initialize();
void readInput(GLFWwindow* window);
void onFrameBufferSizeChanged(GLFWwindow* window, int width, int height);
GLuint ShaderCreate(GLuint shaderType, const char* shaderSource);
void ShaderRelease(GLuint shaderId);
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

    // while the window is open
    while (!glfwWindowShouldClose(window)) {
        // input
        readInput(window);

        // setup buffers
        float vertices[] = {
            -0.5f, -0.5f, +0.0f,
            +0.5f, -0.5f, +0.0f,
            +0.0f, +0.5f, +0.0f
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

        glBindVertexArray(0);

        // render
        glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
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