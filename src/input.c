#include "input.h"
#include "GLFW/glfw3.h"

void InputRead(Input* this, GLFWwindow* window) {
    this->quit = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

    Vec2 cameraTranslate = {
        .x = 0.0f,
        .y = 0.0f
    };

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cameraTranslate.x -= 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cameraTranslate.x += 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cameraTranslate.y -= 1.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cameraTranslate.y += 1.0f;
    }

    this->cameraTranslate = cameraTranslate;
}