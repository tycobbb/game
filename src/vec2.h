#ifndef VEC2_H
#define VEC2_H

typedef struct {
  float x;
  float y;
} Vec2;

void Vec2Scale(Vec2 a, float scale, Vec2* out);

#endif