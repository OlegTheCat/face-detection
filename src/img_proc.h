#pragma once

#include "float_matrix.h"

FloatMatrix *computeII(FloatMatrix *fm, int squared);
float stdDev(FloatMatrix *fm);
float mean(FloatMatrix *fm);
void convertToNormalized(FloatMatrix *fm);
