#ifndef VECH_H
#define VECH_H

typedef struct {
  float x;
  float y;
  float z;
  float w;
} VecH;

typedef float VecHArray[4];

float VecHDot(VecH a, VecH b);

void VecHFromArray(VecHArray in, VecH* out);

void VecHToArray(VecH vec, VecHArray out);

#endif