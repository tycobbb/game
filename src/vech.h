#ifndef VECH_H
#define VECH_H

// -- types --

typedef struct {
  float x;
  float y;
  float z;
  float w;
} VecH;

typedef float VecHArray[4];

// -- interface --

float VecH_Dot(VecH a, VecH b);

void VecH_FromArray(VecHArray in, VecH* out);

void VecH_ToArray(VecH vec, VecHArray out);

#endif