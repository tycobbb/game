#ifndef VERTEX_H
#define VERTEX_H

#include <OpenGL/OpenGL.h>
#include "vec3.h"

// -- types --

typedef struct Vertex {
    GLuint meshIndex;
    Vec3 pos;
    Vec3 normal;
    Vec3 color;
} Vertex;

// -- interface --

void Vertex_Init(Vertex* this);

#endif