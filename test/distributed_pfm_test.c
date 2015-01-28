#include "distributed_pfm_test.h"

#include <stdlib.h>

#include "minunit.h"
#include "test_utils.h"
#include "persistent_float_matrix.h"
#include "pfm_distributed_file_impl.h"
#include "utils.h"

Pfmi *getDistributedFilePfmi(const char *file_name, int rows, int cols) {
    Pfmi *pfmi;

    pfmi = createPfmDistributedFileImpl(file_name,
					cols,
					rows);

    return pfmi;
}

const char *testCreatePfmDistributedFileImpl() {
    PersistentFloatMatrix *pfm;
    int rows, cols;

    rows = 100;
    cols = 200;

    WITH_TEST_FILE_NAME
	(pfm = createPfmWithImpl(FILE_NAME_HANDLE,
				 rows,
				 cols,
				 getDistributedFilePfmi(FILE_NAME_HANDLE, rows, cols)));

    mu_assert("Error while creating pfm", pfm != NULL);
    mu_assert("Wrong rows num", pfm->rows == rows);
    mu_assert("Wrong cols num", pfm->cols == cols);

    deletePfm(pfm);
    return 0;
}

const char *testDistributedPfmGetStoreCol() {
    return 0;
}

const char *testDistributedPfmRemoveRow() {
    return 0;
}
