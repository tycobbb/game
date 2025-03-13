#ifndef VEC3_H
#define VEC3_H

typedef struct {
  float x;
  float y;
  float z;
} Vec3;

float Vec3Dot(Vec3 a, Vec3 b);

#endif