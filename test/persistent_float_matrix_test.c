#include "persistent_float_matrix_test.h"

#include <stdlib.h>

#include "minunit.h"
#include "persistent_float_matrix.h"

char *testCreatePfm() {
    PersistentFloatMatrix *pfm;

    system("rm -f test_storage.storage*");
    pfm = createPfm("test_storage.storage", 2, 2);

    mu_assert("Error while creating pfm", pfm != NULL);

    deletePfm(pfm);
    system("rm -f test_storage.storage*");
    return 0;
}

char *testGetPfmCol() {
    return 0;
}

