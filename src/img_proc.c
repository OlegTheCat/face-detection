#include "img_proc.h"

#include <math.h>

#include "utils.h"

FloatMatrix *computeII(FloatMatrix *fm, int squared) {
    FloatMatrix *ii;
    int x, y;
    float p1, p2, p3, p4;

    ii = createFloatMatrix(fm->cols + 1, fm->rows + 1);

    for (y = 1; y < fm->rows + 1; y++) {
	for (x = 1; x < fm->cols + 1; x++) {

	    p4 = matVal(fm, y - 1, x - 1);
	    p3 = matVal(ii, y, x - 1);
	    p2 = matVal(ii, y - 1, x);
	    p1 = matVal(ii, y - 1, x - 1);

	    setMatVal(ii, y, x, (squared ? sqrf(p4) : p4) - p1 + p3 + p2);
	}
    }

    return ii;
}

float stdDev(FloatMatrix *fm) {
    float sqr_sum, variance;
    int i, j;

    sqr_sum = 0;
    for (i = 0; i < fm->rows; i++) {
	for (j = 0; j < fm->cols; j++) {
	    sqr_sum += sqrf(matVal(fm, i, j));
	}
    }

    variance = sqr_sum / (fm->cols * fm->rows) - sqrf(mean(fm));

    return sqrtf(fabsf(variance));
}

float mean(FloatMatrix *fm) {
    return matSum(fm) / (fm->cols * fm->rows);
}

// This also can be done using II
void convertToNormalized(FloatMatrix *fm) {
    float std_dev;
    int i, j;

    std_dev = stdDev(fm);

    if (std_dev != 0) {
	for (i = 0; i < fm->rows; i++) {
	    for (j = 0; j < fm->cols; j++) {
		setMatVal(fm, i, j, matVal(fm, i, j) / std_dev);
	    }
	}
    }
}
