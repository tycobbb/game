#ifndef VEC3_H
#define VEC3_H

// -- types --

typedef struct {
  float x;
  float y;
  float z;
} Vec3;

// -- interface --

float Vec3_Dot(Vec3 a, Vec3 b);

float Vec3_Magnitude(Vec3 a);

void Vec3_Scale(Vec3 a, float scale, Vec3* out);

void Vec3_Cross(Vec3 a, Vec3 b, Vec3* out);

void Vec3_Normalize(Vec3 a, Vec3* out);

#endif