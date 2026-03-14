#include "input.h"
#include "GLFW/glfw3.h"
#include "spherical.h"

// -- constants --

/// the slow camera rotation multipler
const float CAMERA_ROTATION_SLOW = 1.0f;

/// the fast camera rotation multipler
const float CAMERA_ROTATION_FAST = 2.0f;

// -- implementation --

void Input_Read(Input* this, GLFWwindow* window) {
    this->quit = glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS;

    // find the camera delta
    float cameraSpeed = CAMERA_ROTATION_SLOW;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        cameraSpeed = CAMERA_ROTATION_FAST;
    }

    // rotate the camera azimuth / zenith
    Spherical camera = {
        .azimuth = 0.0f,
        .zenith = 0.0f,
    };

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.azimuth -= cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.azimuth += cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.zenith -= cameraSpeed;
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.zenith += cameraSpeed;
    }

    this->camera = camera;
}