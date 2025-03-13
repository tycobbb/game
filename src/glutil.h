#ifndef GLUTIL_H
#define GLUTIL_H

#include "vec3.h"
#include "vech.h"

void GlVec3ToVertices(int numVectors, Vec3 vectors[], float vertices[]);

void GlVecHToVertices(int numVectors, VecH vectors[], float vertices[]);

#endif