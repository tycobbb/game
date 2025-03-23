#include "transform.h"
#include "arrayutil.h"
#include "vec3.h"
#include "vech.h"
#include "vecutil.h"

void TransformInitIdentity(Transform* out);
void TransformGetRow(Transform a, int i, VecH* out);
void TransformGetColumn(Transform a, int j, VecH* out);
void TransformSetRow(Transform* this, int i, VecH row);

void TransformInit(Transform* this) {
    for (int i = 0; i < ARRAY_LEN(this->matrix); i++) {
        for (int j = 0; j < ARRAY_LEN(this->matrix[i]); j++) {
            this->matrix[i][j] = 0;
        }
    }
}

void TransformInitCamera(Transform* this, Vec3 eye, Vec3 gaze, Vec3 up) {
    TransformInit(this);

    Vec3 w;
    Vec3Normalize(gaze, &w);
    Vec3Scale(w, -1.0f, &w);

    Vec3 u;
    Vec3Cross(up, w, &u);
    Vec3Normalize(u, &u);

    Vec3 v;
    Vec3Cross(w, u, &v);

    VecH temp;

    Transform basis;
    TransformInit(&basis);

    VecHFromVec3(u, 0, &temp);
    TransformSetRow(&basis, 0, temp);

    VecHFromVec3(v, 0, &temp);
    TransformSetRow(&basis, 1, temp);

    VecHFromVec3(w, 0, &temp);
    TransformSetRow(&basis, 2, temp);

    basis.matrix[3][3] = 1;

    Transform origin;
    TransformInitIdentity(&origin);

    origin.matrix[0][3] = -eye.x;
    origin.matrix[1][3] = -eye.y;
    origin.matrix[2][3] = -eye.z;

    TransformMultiply(basis, origin, this);
}

void TransformInitIdentity(Transform* this) {
    TransformInit(this);

    for (int i = 0; i < ARRAY_LEN(this->matrix); i++) {
        this->matrix[i][i] = 1;
    }
}

void TransformInitOrthographicProjection(Transform* this, float l, float r, float b, float t, float n, float f) {
    TransformInit(this);

    this->matrix[0][0] = 2.0f / (r - l);
    this->matrix[0][3] = -(r + l) / (r - l);
    this->matrix[1][1] = 2.0f / (t - b);
    this->matrix[1][3] = -(t + b) / (t - b);
    this->matrix[2][2] = 2.0f / (n - f);
    this->matrix[2][3] = -(n + f) / (n - f);
    this->matrix[3][3] = 1.0f;
}

void TransformInitPerspectiveProjection(Transform* this, float l, float r, float b, float t, float n, float f) {
    TransformInit(this);

    this->matrix[0][0] = (2.0f * n) / (r - l);
    this->matrix[0][2] = (l + r) / (l - r);
    this->matrix[1][1] = (2.0f * n) / (t - b);
    this->matrix[1][2] = (b + t) / (b - t);
    this->matrix[2][2] = (f + n) / (n - f);
    this->matrix[2][3] = (2.0f * f * n) / (f - n);
    this->matrix[3][2] = 1.0f;
}

void TransformApply(Transform transform, VecH vec, VecH* out) {
    VecHArray result;

    for (int i = 0; i < ARRAY_LEN(transform.matrix); i++) {
        VecH row;
        TransformGetRow(transform, i, &row);
        result[i] = VecHDot(row, vec);
    }

    VecHFromArray(result, out);
}

void TransformSet(Transform* this, int rowIndex, int colIndex, float value) {
    this->matrix[rowIndex][colIndex] = value;
}

void TransformMultiply(Transform a, Transform b, Transform *out) {
    for (int i = 0; i < ARRAY_LEN(a.matrix); i++) {
        VecH row;
        TransformGetRow(a, i, &row);
        for (int j = 0; j < ARRAY_LEN(a.matrix[0]); j++) {
            VecH column;
            TransformGetColumn(b, j, &column);
            out->matrix[i][j] = VecHDot(row, column);
        }
    }
}

void TransformGetRow(Transform a, int i, VecH* out) {
    VecHFromArray(a.matrix[i], out);
}

void TransformSetRow(Transform* this, int i, VecH row) {
    VecHArray values;
    VecHToArray(row, values);

    for (int j = 0; j < ARRAY_LEN(values); j++) {
        this->matrix[i][j] = values[j];
    }
}

void TransformGetColumn(Transform a, int j, VecH* out) {
    out->x = a.matrix[0][j];
    out->y = a.matrix[1][j];
    out->z = a.matrix[2][j];
    out->w = a.matrix[3][j];
}
