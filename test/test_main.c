#include <stdio.h>
#include "minunit.h"

#include "pgm_test.h"
#include "pointer_array_test.h"
#include "data_set_test.h"

int tests_run = 0;

static char *dummy() {
    mu_assert("This should never come up", 1 == 1);
    return 0;
}

static char *all_tests() {
    mu_run_test(dummy);
    mu_run_test(testReadImage);
    mu_run_test(testCreateImage);
    mu_run_test(testSaveImage);
    mu_run_test(testSubImage);
    mu_run_test(testCreatePointerArray);
    mu_run_test(testAddToPointerArray);
    mu_run_test(testReadImageList);
    mu_run_test(testSubSampleImage);

    return 0;
}

int main() {
    char *result = all_tests();
    if (result != 0) {
	fprintf(stderr, "%s\n", result);
    } else {
	printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
