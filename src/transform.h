#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vech.h"

typedef struct {
  float matrix[4][4];
} Transform;

void TransformMultiply(Transform transform, VecH vec, VecH* out);

void TransformSet(Transform* transform, int rowIndex, int colIndex, float value);

#endif