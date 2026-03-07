#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vech.h"
#include "vec3.h"

// -- types --

typedef struct {
  float matrix[4][4];
} Transform;

typedef float TransformArray[16];

// -- interface --

void Transform_Init(Transform* this);

void Transform_InitOrthographicProjection(Transform* transform, float l, float r, float b, float t, float n, float f);

void Transform_InitPerspectiveProjection(Transform* this, float fovY, float aspect, float n, float f);

void Transform_InitCamera(Transform* transform, Vec3 eye, Vec3 gaze, Vec3 up);

void Transform_Apply(Transform transform, VecH vec, VecH* out);

void Transform_Set(Transform* transform, int rowIndex, int colIndex, float value);

void Transform_Multiply(Transform a, Transform b, Transform* out);

void Transform_ToArray(Transform this, TransformArray out);

#endif