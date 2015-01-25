#include "partial_pfm_test.h"

#include <stdlib.h>

#include "minunit.h"
#include "persistent_float_matrix.h"
#include "pfm_partial_file_impl.h"
#include "utils.h"

#define PARTIAL_PFMI_STORAGE_FILE "partial_storage.storage"

Pfmi *getPartiaFilePfmi(int start_col, int end_col, int rows) {
    Pfmi *partial_file_pfmi;

    partial_file_pfmi = createPfmPartialFileImpl(PARTIAL_PFMI_STORAGE_FILE,
						 start_col,
						 end_col,
						 rows);
    return partial_file_pfmi;
}

const char *testCreatePfmPartialFileImpl() {
    PersistentFloatMatrix *pfm;
    int rows, cols;

    rows = 100;
    cols = 200;

    system("rm -f " PARTIAL_PFMI_STORAGE_FILE);
    pfm = createPfmWithImpl(PARTIAL_PFMI_STORAGE_FILE,
			    rows,
			    cols,
			    getPartiaFilePfmi(30, 70, rows));

    mu_assert("Error while creating pfm", pfm != NULL);
    mu_assert("Wrong rows num", pfm->rows == rows);
    mu_assert("Wrong cols num", pfm->cols == cols);

    deletePfm(pfm);
    system("rm -f " PARTIAL_PFMI_STORAGE_FILE);
    return 0;
}

const char *testPartialPfmGetStoreCol() {
    PersistentFloatMatrix *pfm;
    float col[10];
    int i;
    int err;

    system("rm -f " PARTIAL_PFMI_STORAGE_FILE);

    pfm = createPfmWithImpl(PARTIAL_PFMI_STORAGE_FILE,
			    10, 20,
			    getPartiaFilePfmi(5, 10, 10));

    for (i = 0; i < 10; i++) {
	col[i] = (float)i;
    }

    err = storePfmCol(pfm, col, 5);
    mu_assert("Wrong store #1 error code", err == 0);
    err = storePfmCol(pfm, col, 10);
    mu_assert("Wrong store #2 error code", err == PFMI_OUT_OF_COL_RANGE);

    err = getPfmCol(pfm, col, 5);
    mu_assert("Wrong get #1 error code", err == 0);
    for (i = 0; i < 10; i++) {
	mu_assert("Wrong value in col #5", floatEqual(col[i], i));
    }

    err = getPfmCol(pfm, col, 10);
    mu_assert("Wrong get #2 error code", err == PFMI_OUT_OF_COL_RANGE);

    system("rm -f " PARTIAL_PFMI_STORAGE_FILE);
    deletePfm(pfm);
    return 0;
}

const char *testPartialPfmRemoveRow() {
    PersistentFloatMatrix *pfm;
    float col[10];
    int i;

    system("rm -f " PARTIAL_PFMI_STORAGE_FILE);

    pfm = createPfmWithImpl(PARTIAL_PFMI_STORAGE_FILE,
			    10, 20,
			    getPartiaFilePfmi(5, 15, 10));

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

    system("rm -f " PARTIAL_PFMI_STORAGE_FILE);
    deletePfm(pfm);
    return 0;

}
