#include <math.h>
#include "vec3.h"

// -- implementation --

float Vec3_Dot(Vec3 a, Vec3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Vec3_Magnitude(Vec3 a) {
  return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

void Vec3_Scale(Vec3 a, float scale, Vec3* out) {
  out->x = a.x * scale;
  out->y = a.y * scale;
  out->z = a.z * scale;
}

void Vec3_Cross(Vec3 a, Vec3 b, Vec3* out) {
  out->x = a.y * b.z - a.z * b.y;
  out->y = a.z * b.x - a.x * b.z;
  out->z = a.x * b.y - a.y * b.x;
}

void Vec3_Normalize(Vec3 a, Vec3* out) {
  float magnitude = Vec3_Magnitude(a);
  Vec3_Scale(a, 1.0f / magnitude, out);
}