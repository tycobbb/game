#define GL_SILENCE_DEPRECATION

#import <GLFW/glfw3.h>

int main(void) {
    // init glfw
    if (!glfwInit()) {
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

    // while the window is open
    while (!glfwWindowShouldClose(window)) {
        // render
        glClear(GL_COLOR_BUFFER_BIT);

        // swap front & back buffers
        glfwSwapBuffers(window);

        // poll for and process events
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
