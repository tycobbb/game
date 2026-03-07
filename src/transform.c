#include "transform.h"
#include "arrayutil.h"
#include "vec3.h"
#include "vech.h"
#include "vecutil.h"
#include <math.h>

// -- interface --

void Transform_InitIdentity(Transform* out);
void Transform_GetRow(Transform a, int i, VecH* out);
void Transform_SetRow(Transform* this, int i, VecH row);
void Transform_GetColumn(Transform a, int j, VecH* out);

// -- implementation --

void Transform_Init(Transform* this) {
    for (int i = 0; i < ARRAY_LEN(this->matrix); i++) {
        for (int j = 0; j < ARRAY_LEN(this->matrix[i]); j++) {
            this->matrix[i][j] = 0;
        }
    }
}

void Transform_InitCamera(Transform* this, Vec3 eye, Vec3 gaze, Vec3 up) {
    Transform_Init(this);

    Vec3 w;
    Vec3_Normalize(gaze, &w);
    Vec3_Scale(w, -1.0f, &w);

    Vec3 u;
    Vec3_Cross(up, w, &u);
    Vec3_Normalize(u, &u);

    Vec3 v;
    Vec3_Cross(w, u, &v);

    VecH temp;

    Transform basis;
    Transform_Init(&basis);

    VecH_FromVec3(u, 0, &temp);
    Transform_SetRow(&basis, 0, temp);

    VecH_FromVec3(v, 0, &temp);
    Transform_SetRow(&basis, 1, temp);

    VecH_FromVec3(w, 0, &temp);
    Transform_SetRow(&basis, 2, temp);

    basis.matrix[3][3] = 1;

    Transform origin;
    Transform_InitIdentity(&origin);

    origin.matrix[0][3] = -eye.x;
    origin.matrix[1][3] = -eye.y;
    origin.matrix[2][3] = -eye.z;

    Transform_Multiply(basis, origin, this);
}

void Transform_InitIdentity(Transform* this) {
    Transform_Init(this);

    for (int i = 0; i < ARRAY_LEN(this->matrix); i++) {
        this->matrix[i][i] = 1;
    }
}

void Transform_InitOrthographicProjection(Transform* this, float l, float r, float b, float t, float n, float f) {
    Transform_Init(this);

    this->matrix[0][0] = 2.0f / (r - l);
    this->matrix[0][3] = -(r + l) / (r - l);
    this->matrix[1][1] = 2.0f / (t - b);
    this->matrix[1][3] = -(t + b) / (t - b);
    this->matrix[2][2] = 2.0f / (n - f);
    this->matrix[2][3] = -(n + f) / (n - f);
    this->matrix[3][3] = 1.0f;
}

void Transform_InitPerspectiveProjection(Transform* this, float fovY, float aspect, float n, float f) {
    Transform_Init(this);

    float fovY_2 = fovY / 2;
    float scaleY = cos(fovY_2) / sin(fovY_2);

    this->matrix[0][0] = scaleY / aspect;
    this->matrix[1][1] = scaleY;
    this->matrix[2][2] = (f + n) / (n - f);
    this->matrix[2][3] = (2.0f * f * n) / (f - n);
    this->matrix[3][2] = -1.0f;
}

void Transform_Apply(Transform transform, VecH vec, VecH* out) {
    VecHArray result;

    for (int i = 0; i < ARRAY_LEN(transform.matrix); i++) {
        VecH row;
        Transform_GetRow(transform, i, &row);
        result[i] = VecH_Dot(row, vec);
    }

    VecH_FromArray(result, out);
}

void Transform_Set(Transform* this, int rowIndex, int colIndex, float value) {
    this->matrix[rowIndex][colIndex] = value;
}

void Transform_Multiply(Transform a, Transform b, Transform *out) {
    for (int i = 0; i < ARRAY_LEN(a.matrix); i++) {
        VecH row;
        Transform_GetRow(a, i, &row);
        for (int j = 0; j < ARRAY_LEN(a.matrix[0]); j++) {
            VecH column;
            Transform_GetColumn(b, j, &column);
            out->matrix[i][j] = VecH_Dot(row, column);
        }
    }
}

void Transform_GetRow(Transform a, int i, VecH* out) {
    VecH_FromArray(a.matrix[i], out);
}

void Transform_SetRow(Transform* this, int i, VecH row) {
    VecHArray values;
    VecH_ToArray(row, values);

    for (int j = 0; j < ARRAY_LEN(values); j++) {
        this->matrix[i][j] = values[j];
    }
}

void Transform_GetColumn(Transform a, int j, VecH* out) {
    out->x = a.matrix[0][j];
    out->y = a.matrix[1][j];
    out->z = a.matrix[2][j];
    out->w = a.matrix[3][j];
}

void Transform_ToArray(Transform this, TransformArray out) {
    for (int row = 0; row < ARRAY_LEN(this.matrix); row++) {
        for (int column = 0; column < ARRAY_LEN(this.matrix[0]); column++) {
            out[row * 4 + column] = this.matrix[row][column];
        }
    }
}
