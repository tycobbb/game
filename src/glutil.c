#include "glutil.h"
#include "vecutil.h"

void GlSetVertices(int vertexIndex, Vec3 vector, float vertices[]);

void GlVec3ToVertices(int numVectors, Vec3 vectors[], float vertices[]) {
  for(int i = 0; i < numVectors; i++) {
    GlSetVertices(i, vectors[i], vertices);
  }
}

void GlVecHToVertices(int numVectors, VecH vectors[], float vertices[]) {
  for(int i = 0; i < numVectors; i++) {
    Vec3 vector;
    Vec3FromVecH(vectors[i], &vector);
    GlSetVertices(i, vector, vertices);
  }
}

void GlSetVertices(int vertexIndex, Vec3 vector, float vertices[]) {
  int offset = vertexIndex * 3;
  vertices[offset] = vector.x;
  vertices[offset + 1] = vector.y;
  vertices[offset + 2] = vector.z;
}