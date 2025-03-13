#ifndef VEC3_H
#define VEC3_H

typedef struct {
  float x;
  float y;
  float z;
} Vec3;

float Vec3Dot(Vec3 a, Vec3 b);

float Vec3Magnitude(Vec3 a);

void Vec3Scale(Vec3 a, float scale, Vec3* out);

void Vec3Cross(Vec3 a, Vec3 b, Vec3* out);

void Vec3Normalize(Vec3 a, Vec3* out);

#endif