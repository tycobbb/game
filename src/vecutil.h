#ifndef VECUTIL_H
#define VECUTIL_H

#include "vec3.h"
#include "vech.h"

// -- interface --

void VecH_FromVec3(Vec3 vec, float w, VecH* out);

void Vec3_FromVecH(VecH vec, Vec3* out);

#endif