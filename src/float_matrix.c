#include "float_matrix.h"

#include <stdlib.h>
#include <string.h>

FloatMatrix *createFloatMatrix(int cols, int rows) {
    FloatMatrix *fm;

    fm = malloc(sizeof(FloatMatrix));
    fm->data = malloc(cols * rows * sizeof(float));
    memset(fm->data, 0, cols * rows * sizeof(float));

    fm->cols = cols;
    fm->rows = rows;

    return fm;
}

float matVal(const FloatMatrix *fm, int row, int col) {
    return fm->data[row * fm->cols + col];
}

void setMatVal(FloatMatrix *fm, int row, int col, float val) {
    fm->data[row * fm->cols + col] = val;
}

float matSum(FloatMatrix *fm) {
    float sum;
    int i;
    sum = 0;

    for (i = 0; i < fm->cols * fm->rows; i++) {
	sum += fm->data[i];
    }

    return sum;
}

void deleteFloatMatrix(FloatMatrix *fm) {
    if (fm != NULL) {
	if (fm->data != NULL) free(fm->data);
	free(fm);
    }
}

