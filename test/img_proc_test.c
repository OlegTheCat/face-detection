#include "img_proc_test.h"

#include <stdio.h>

#include "minunit.h"
#include "img_proc.h"
#include "utils.h"

/* Returns 3x3 matrix with values
1 2 3
4 5 6
7 8 9
*/
static FloatMatrix *getFloatMatrix() {
    FloatMatrix *fm;

    fm = createFloatMatrix(3, 3);

    setMatVal(fm, 0, 0, 1);
    setMatVal(fm, 0, 1, 2);
    setMatVal(fm, 0, 2, 3);
    setMatVal(fm, 1, 0, 4);
    setMatVal(fm, 1, 1, 5);
    setMatVal(fm, 1, 2, 6);
    setMatVal(fm, 2, 0, 7);
    setMatVal(fm, 2, 1, 8);
    setMatVal(fm, 2, 2, 9);

    return fm;
}

const char *testComputeII() {
    FloatMatrix *fm, *ii, *ii2;

    fm = getFloatMatrix();
    ii = computeII(fm, 0);

    mu_assert("Wrong height of ii", ii->rows == fm->rows + 1);
    mu_assert("Wrong width of ii", ii->cols == fm->cols + 1);
    mu_assert("Wrong value of ii at pos [0, 0]", matVal(ii, 0, 0) == 0);
    mu_assert("Wrong value of ii at pos [0, 2]", matVal(ii, 0, 2) == 0);
    mu_assert("Wrong value of ii at pos [2, 2]", matVal(ii, 2, 2) == 12);
    mu_assert("Wrong value of ii at pos [3, 2]", matVal(ii, 3, 2) == 27);

    ii2 = computeII(fm, 1);
    mu_assert("Wrong height of ii2", ii2->rows == fm->rows + 1);
    mu_assert("Wrong width of ii2", ii2->cols == fm->cols + 1);
    mu_assert("Wrong value of ii2 at pos [0, 0]", matVal(ii2, 0, 0) == 0);
    mu_assert("Wrong value of ii2 at pos [0, 2]", matVal(ii2, 0, 2) == 0);
    mu_assert("Wrong value of ii2 at pos [2, 2]",
	      matVal(ii2, 2, 2) == 1 + 2 * 2 + 4 * 4 + 5 * 5);
    mu_assert("Wrong value of ii2 at pos [3, 2]",
	      matVal(ii2, 3, 2) == 1 + 2 * 2 + 4 * 4 + 5 * 5 + 7 * 7 + 8 * 8);

    deleteFloatMatrix(fm);
    deleteFloatMatrix(ii);
    deleteFloatMatrix(ii2);
    return 0;
}

const char *testStdDev() {
    FloatMatrix *fm;

    fm = getFloatMatrix();

    /* printf("stddev = %f\n", stdDev(fm)); */
    mu_assert("Wrong stddev", floatEqualEps(stdDev(fm), 2.581989f, 0.000001f));

    deleteFloatMatrix(fm);
    return 0;
}

const char *testMean() {
    FloatMatrix *fm;

    fm = getFloatMatrix();

    mu_assert("Wrong mean value", mean(fm) == 5);

    deleteFloatMatrix(fm);
    return 0;
}

const char *testConvertToNormalized() {
    FloatMatrix *fm;

    fm = getFloatMatrix();

    // Just check for memory errors and leaks
    convertToNormalized(fm);

    deleteFloatMatrix(fm);
    return 0;
}
