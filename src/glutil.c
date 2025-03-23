#include "glutil.h"
#include "vecutil.h"

void GlSetVertices(int vertexIndex, Vec3 vector, float vertices[]);

void GlSetVertices(int vertexIndex, Vec3 vector, float vertices[]) {
  int offset = vertexIndex * 3;
  vertices[offset] = vector.x;
  vertices[offset + 1] = vector.y;
  vertices[offset + 2] = vector.z;
}