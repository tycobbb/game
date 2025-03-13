#ifndef VECUTIL_H
#define VECUTIL_H

#include "vec3.h"
#include "vech.h"

void VecHFromVec3(Vec3 vec, float w, VecH* out);

void Vec3FromVecH(VecH vec, Vec3* out);

#endif