#include <math.h>
#include "vec3.h"

// -- constants --

const Vec3 Vec3_Up = { .x = 0, .y = 1, .z = 0 };

// -- implementation --

void Vec3_Init(Vec3 *this) {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

float Vec3_Dot(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float Vec3_Magnitude(Vec3 a) {
    return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

void Vec3_Add(Vec3 a, Vec3 b, Vec3* out) {
    out->x = a.x + b.x;
    out->y = a.y + b.y;
    out->z = a.z + b.z;
}

void Vec3_Scale(Vec3 a, float scale, Vec3* out) {
    out->x = a.x * scale;
    out->y = a.y * scale;
    out->z = a.z * scale;
}

void Vec3_Cross(Vec3 a, Vec3 b, Vec3* out) {
    out->x = a.y * b.z - a.z * b.y;
    out->y = a.z * b.x - a.x * b.z;
    out->z = a.x * b.y - a.y * b.x;
}

void Vec3_Normalize(Vec3 a, Vec3* out) {
    float magnitude = Vec3_Magnitude(a);
    Vec3_Scale(a, 1.0f / magnitude, out);
}

Vec3 Vec3_FromDouble(double src[3]) {
    Vec3 dst = {
        .x = src[0],
        .y = src[1],
        .z = src[2]
    };

    return dst;
}