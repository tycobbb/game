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
    for (int i = 0; i < ARRAY_LEN(this->v); i++) {
        this->v[i] = 0;
    }
}

void Transform_InitIdentity(Transform* this) {
    Transform_Init(this);

    for (int i = 0; i < ARRAY_LEN(this->matrix); i++) {
        this->matrix[i][i] = 1;
    }
}

void Transform_InitWithColumns(Transform *this, double cols[12]) {
    Transform_InitIdentity(this);

    this->matrix[0][0] = cols[0];
    this->matrix[1][0] = cols[1];
    this->matrix[2][0] = cols[2];

    this->matrix[0][1] = cols[3];
    this->matrix[1][1] = cols[4];
    this->matrix[2][1] = cols[5];

    this->matrix[0][2] = cols[6];
    this->matrix[1][2] = cols[7];
    this->matrix[2][2] = cols[8];

    this->matrix[0][3] = cols[9];
    this->matrix[1][3] = cols[10];
    this->matrix[2][3] = cols[11];
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
    for (int i = 0; i < ARRAY_LEN(transform.matrix); i++) {
        VecH row;
        Transform_GetRow(transform, i, &row);
        out->v[i] = VecH_Dot(row, vec);
    }
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
    for (int j = 0; j < ARRAY_LEN(a.matrix[0]); j++) {
        out->v[j] = a.matrix[i][j];
    }
}

void Transform_SetRow(Transform* this, int i, VecH row) {
    for (int j = 0; j < ARRAY_LEN(row.v); j++) {
        this->matrix[i][j] = row.v[j];
    }
}

void Transform_GetColumn(Transform a, int j, VecH* out) {
    out->x = a.matrix[0][j];
    out->y = a.matrix[1][j];
    out->z = a.matrix[2][j];
    out->w = a.matrix[3][j];
}

