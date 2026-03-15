#ifndef VEC3_H
#define VEC3_H

// -- types --
typedef struct {
    union {
        struct {
            float x;
            float y;
            float z;
        };

        float v[3];
    };
} Vec3;

// -- constants --

const Vec3 Vec3_Up;

// -- interface --

void Vec3_Init(Vec3* this);

float Vec3_Dot(Vec3 a, Vec3 b);

float Vec3_Magnitude(Vec3 a);

void Vec3_Add(Vec3 a, Vec3 b, Vec3* out);

void Vec3_Scale(Vec3 a, float scale, Vec3* out);

void Vec3_Cross(Vec3 a, Vec3 b, Vec3* out);

void Vec3_Normalize(Vec3 a, Vec3* out);

Vec3 Vec3_FromDouble(double v[3]);

#endif