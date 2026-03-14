#include "mathutil.h"
#include "spherical.h"

// -- implementation --

void Spherical_Scale(Spherical* this, float scale) {
    this->radius *= scale;
    this->azimuth *= scale;
    this->zenith *= scale;
}

void Spherical_Add(Spherical a, Spherical b, Spherical* out) {
    out->radius = a.radius + b.radius;
    out->azimuth = a.azimuth + b.azimuth;
    out->zenith = a.zenith + b.zenith;
}

Vec3 Spherical_ToCartesian(Spherical* this) {
    float r = (this->radius);
    float a = (this->azimuth);
    float z = (this->zenith + PI_2);

    float rSinZ = r * sin(z);
    float rCosZ = r * cos(z);

    Vec3 point = {
        .x = rSinZ * cos(a),
        .y = rCosZ,
        .z = rSinZ * sin(a),
    };

    return point;
}