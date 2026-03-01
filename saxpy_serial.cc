
#include <stddef.h>

// single-precision A*X + Y
void saxpy_serial(float a, float *x, float *y, float *result, size_t n) {
  for (int i = 0; i < n; ++i)
    result[i] = a * x[i] + y[i];
}
