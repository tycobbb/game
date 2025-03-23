#include "vecutil.h"

void VecHFromVec3(Vec3 vec, float w, VecH* out) {
  out->x = vec.x;
  out->y = vec.y;
  out->z = vec.z;
  out->w = w;
}

void Vec3FromVecH(VecH vec, Vec3* out) {
  out->x = vec.x / vec.w;
  out->y = vec.y / vec.w;
  out->z = vec.z / vec.w;
}