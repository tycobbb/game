#ifndef SPHERICAL_H
#define SPHERICAL_H

#include "vec3.h"

// -- types --

typedef struct Spherical {
    float radius;
    float azimuth;
    float zenith;
} Spherical;

// -- interface --

void Spherical_Scale(Spherical* this, float scale);

void Spherical_Add(Spherical a, Spherical b, Spherical* out);

Vec3 Spherical_ToCartesian(Spherical* this);

#endif