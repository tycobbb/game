#include "vertex.h"
#include "vec3.h"

// -- implementation --

void Vertex_Init(Vertex *this) {
    this->meshIndex = 0;
    Vec3_Init(&this->pos);
    Vec3_Init(&this->normal);
    Vec3_Init(&this->color);
}