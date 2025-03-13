#include <math.h>
#include "vec3.h"

float Vec3Dot(Vec3 a, Vec3 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Vec3Magnitude(Vec3 a) {
  return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

void Vec3Scale(Vec3 a, float scale, Vec3* out) {
  out->x = a.x * scale;
  out->y = a.y * scale;
  out->z = a.z * scale;
}

void Vec3Cross(Vec3 a, Vec3 b, Vec3* out) {
  out->x = a.y * b.z - a.z * b.y;
  out->y = a.z * b.x - a.x * b.z;
  out->z = a.x * b.y - a.y * b.x;
}

void Vec3Normalize(Vec3 a, Vec3* out) {
  float magnitude = Vec3Magnitude(a);
  Vec3Scale(a, 1.0f / magnitude, out);
}