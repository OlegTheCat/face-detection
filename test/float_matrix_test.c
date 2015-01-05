#include "float_matrix_test.h"

#include "minunit.h"
#include "float_matrix.h"

char *testCreateFloatMatrix() {
    FloatMatrix *fm;

    fm = createFloatMatrix(10, 20);

    mu_assert("Wrong matrix width", fm->width == 10);
    mu_assert("Wrong matrix height", fm->height == 20);

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

