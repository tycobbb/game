#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vech.h"
#include "vec3.h"

typedef struct {
  float matrix[4][4];
} Transform;

void TransformInit(Transform* this);

void TransformInitOrthographicProjection(Transform* transform, float l, float r, float b, float t, float n, float f);

void TransformInitPerspectiveProjection(Transform* transform, float l, float r, float b, float t, float n, float f);

void TransformInitCamera(Transform* transform, Vec3 eye, Vec3 gaze, Vec3 up);

void TransformApply(Transform transform, VecH vec, VecH* out);

void TransformSet(Transform* transform, int rowIndex, int colIndex, float value);

void TransformMultiply(Transform a, Transform b, Transform* out);

#endif