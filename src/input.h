#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "spherical.h"

// -- types --

typedef struct Input {
  /// if the game should quit
  bool quit;

  /// the spherical delta for the camera
  Spherical camera;
} Input;

// -- interface --

void Input_Read(Input* this, GLFWwindow* window);

#endif