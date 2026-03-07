#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "vec2.h"

// -- types --

typedef struct {
  bool quit;
  Vec2 cameraTranslate;
} Input;

// -- interface --

void Input_Read(Input* this, GLFWwindow* window);

#endif