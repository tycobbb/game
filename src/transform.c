#include "transform.h"
#include "arrayutil.h"

void TransformMultiply(Transform transform, VecH vec, VecH* out) {
  VecHArray result;

  for (int i = 0; i < ARRAY_LEN(transform.matrix); i++) {
    VecH row;
    VecHFromArray(transform.matrix[i], &row);
    result[i] = VecHDot(row, vec);
  }

  VecHFromArray(result, out);
}

void TransformSet(Transform* transform, int rowIndex, int colIndex, float value) {
  transform->matrix[rowIndex][colIndex] = value;
}