#include "persistent_float_matrix_test.h"

#include <stdlib.h>

#include "minunit.h"
#include "persistent_float_matrix.h"
#include "utils.h"

const char *testCreatePfm() {
    PersistentFloatMatrix *pfm;
    int rows, cols;

    rows = 100;
    cols = 200;

    system("rm -f test_storage.storage*");
    pfm = createPfm("test_storage.storage", rows, cols);

    mu_assert("Error while creating pfm", pfm != NULL);
    mu_assert("Wrong rows num", pfm->rows == rows);
    mu_assert("Wrong cols num", pfm->cols == cols);

    deletePfm(pfm);
    system("rm -f test_storage.storage*");
    return 0;
}

const char *testGetStorePfmCol() {
    PersistentFloatMatrix *pfm;
    float col[10];
    int i;

    system("rm -f test_storage2.storage*");

    pfm = createPfm("test_storage2.storage", 10, 20);

    for (i = 0; i < 10; i++) {
	col[i] = (float)i;
    }

    storePfmCol(pfm, col, 5);
    storePfmCol(pfm, col, 10);

    getPfmCol(pfm, col, 0);
    for (i = 0; i < 10; i++) {
	mu_assert("Non-zero value in col", floatEqual(col[i], 0));
    }

    getPfmCol(pfm, col, 5);
    for (i = 0; i < 10; i++) {
	mu_assert("Wrong value in col #5", floatEqual(col[i], i));
    }

    getPfmCol(pfm, col, 10);
    for (i = 0; i < 10; i++) {
	mu_assert("Wrong value in col #10", floatEqual(col[i], i));
    }

    system("rm -f test_storage2.storage");
    deletePfm(pfm);
    return 0;
}

const char *testRemovePfmRow() {
    PersistentFloatMatrix *pfm;
    float col[10];
    int i;

    system("rm -f test_storage3.storage*");

    pfm = createPfm("test_storage3.storage", 10, 20);

    for (i = 0; i < 10; i++) {
	col[i] = (float)(i + 1);
    }

    storePfmCol(pfm, col, 5);
    storePfmCol(pfm, col, 10);

    removePfmRow(pfm, 0);
    removePfmRow(pfm, 0);
    removePfmRow(pfm, 5);
    /* 5, 10 cols after row removal:
       [1], [2], 3, 4, 5, 6, 7, [8], 9, 10
     */

    mu_assert("Wrong rows number after removal", pfm->rows == 7);

    getPfmCol(pfm, col, 5);
    mu_assert("Wrong col1 value at pos #1", floatEqual(col[0], 3));
    mu_assert("Wrong col1 value at pos #6", floatEqual(col[6], 10));

    getPfmCol(pfm, col, 10);
    mu_assert("Wrong col2 value at pos #1", floatEqual(col[0], 3));
    mu_assert("Wrong col2 value at pos #6", floatEqual(col[6], 10));

    for (i = 0; i < 10; i++) {
	col[i] = (float)(i + 10);
    }

    storePfmCol(pfm, col, 1);
    getPfmCol(pfm, col, 1);

    mu_assert("Wrong retrieved col value at pos #1", floatEqual(col[0], 10));
    mu_assert("Wrong retrieved col value at pos #6", floatEqual(col[6], 16));

    system("rm -f test_storage3.storage*");
    deletePfm(pfm);
    return 0;
}
