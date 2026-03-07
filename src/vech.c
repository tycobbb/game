#include "vech.h"

// -- implementation --

float VecH_Dot(VecH a, VecH b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}