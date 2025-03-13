#include "vec2.h"

void Vec2Scale(Vec2 a, float scale, Vec2* out) {
  out->x = a.x * scale;
  out->y = a.y * scale;
}