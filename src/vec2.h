#ifndef VEC2_H
#define VEC2_H

// -- types --

typedef struct {
  float x;
  float y;
} Vec2;

// -- interface --

void Vec2_Scale(Vec2 a, float scale, Vec2* out);

#endif