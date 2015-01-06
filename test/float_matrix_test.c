#include "float_matrix_test.h"

#include "minunit.h"
#include "float_matrix.h"

char *testCreateFloatMatrix() {
    FloatMatrix *fm;

    fm = createFloatMatrix(10, 20);

    mu_assert("Wrong matrix cols", fm->rows == 10);
    mu_assert("Wrong matrix rows", fm->cols == 20);

    deleteFloatMatrix(fm);
    return 0;
}

char *testSetGetValues() {
    FloatMatrix *fm;

    fm = createFloatMatrix(10, 20);

    mu_assert("Non-zero value at pos [5, 5]",
	      matVal(fm, 5, 5) == 0);
    mu_assert("Non-zero at pos [6, 3]",
	      matVal(fm, 6, 3) == 0);

    setMatVal(fm, 5, 5, 123);
    setMatVal(fm, 6, 3, 321);

    mu_assert("Wrong value at pos [5, 5]",
	      matVal(fm, 5, 5) == 123);
    mu_assert("Wrong value at pos [6, 3]",
	      matVal(fm, 6, 3) == 321);

    deleteFloatMatrix(fm);
    return 0;
}

char *testMatSum() {
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

    mu_assert("Wrong mat sum", matSum(fm) == 45);

    deleteFloatMatrix(fm);
    return 0;
}

