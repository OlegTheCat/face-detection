#include "img_proc_test.h"

#include "minunit.h"
#include "img_proc.h"

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

char *testComputeII() {
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

char *testStdDev() {
    return 0;
}

char *testMean() {
    return 0;
}

char *testConvertToNormalized() {
    return 0;
}
