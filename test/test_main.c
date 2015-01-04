#include <stdio.h>
#include "minunit.h"

int tests_run = 0;

static char *dummy() {
    mu_assert("This should never come up", 1 == 1);
    return 0;
}

static char *all_tests() {
    mu_run_test(dummy);
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
