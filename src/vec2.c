#include "vec2.h"

// -- implementation --

void Vec2_Scale(Vec2 a, float scale, Vec2* out) {
  out->x = a.x * scale;
  out->y = a.y * scale;
}