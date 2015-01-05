#include "float_matrix.h"

#include <stdlib.h>
#include <string.h>

FloatMatrix *createFloatMatrix(int width, int height) {
    FloatMatrix *fm;

    fm = malloc(sizeof(FloatMatrix));
    fm->data = malloc(width * height * sizeof(float));
    memset(fm->data, 0, width * height * sizeof(float));

    fm->width = width;
    fm->height = height;

    return fm;
}

float matVal(const FloatMatrix *fm, int row, int col) {
    return fm->data[row * fm->width + col];
}

void setMatVal(FloatMatrix *fm, int row, int col, float val) {
    fm->data[row * fm->width + col] = val;
}

float matSum(FloatMatrix *fm) {
    float sum;
    int i;
    sum = 0;

    for (i = 0; i < fm->width * fm->height; i++) {
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

