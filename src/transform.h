#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vech.h"
#include "vec3.h"

// -- constants --

#define TRANSFORM_LEN 16

// -- types --

typedef struct Transform {
    union {
        struct {
            float m00, m10, m20, m30;
            float m01, m11, m21, m31;
            float m02, m12, m22, m32;
            float m03, m13, m23, m33;
        };

        float matrix[4][4];
        float v[TRANSFORM_LEN];
    };
} Transform;

// -- interface --

void Transform_Init(Transform* this);

void Transform_InitOrthographicProjection(Transform* transform, float l, float r, float b, float t, float n, float f);

void Transform_InitPerspectiveProjection(Transform* this, float fovY, float aspect, float n, float f);

void Transform_InitCamera(Transform* transform, Vec3 eye, Vec3 gaze, Vec3 up);

void Transform_InitModel(Transform* transform, Vec3 x, Vec3 y, Vec3 z, Vec3 t);

void Transform_InitWithColumns(Transform *this, double cols[12]);

void Transform_Apply(Transform transform, VecH vec, VecH* out);

void Transform_Set(Transform* transform, int rowIndex, int colIndex, float value);

void Transform_Multiply(Transform a, Transform b, Transform* out);

#endif