#include "vech.h"

// -- implementation --

float VecH_Dot(VecH a, VecH b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

void VecH_FromArray(VecHArray in, VecH* out) {
  out->x = in[0];
  out->y = in[1];
  out->z = in[2];
  out->w = in[3];
}

void VecH_ToArray(VecH vec, VecHArray out) {
  out[0] = vec.x;
  out[1] = vec.y;
  out[2] = vec.z;
  out[3] = vec.w;
}