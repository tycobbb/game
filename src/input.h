#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "vec2.h"

typedef struct {
  bool quit;
  Vec2 cameraTranslate;
} Input;

void InputRead(Input* this, GLFWwindow* window);

#endif