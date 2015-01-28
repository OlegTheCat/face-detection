#include "distributed_pfm_test.h"

#include <stdlib.h>

#include "minunit.h"
#include "persistent_float_matrix.h"
#include "pfm_distributed_file_impl.h"
#include "utils.h"

#define DISTRIBUTED_PFMI_STORAGE_FILE "distributed_storage.storage"

Pfmi *getDistributedFilePfmi(int rows, int cols) {
    Pfmi *pfmi;

    pfmi = createPfmDistributedFileImpl(DISTRIBUTED_PFMI_STORAGE_FILE,
					cols,
					rows);

    return pfmi;
}

const char *testCreatePfmDistributedFileImpl() {
    PersistentFloatMatrix *pfm;
    int rows, cols;

    rows = 100;
    cols = 200;

    system("rm -f " DISTRIBUTED_PFMI_STORAGE_FILE);
    pfm = createPfmWithImpl(DISTRIBUTED_PFMI_STORAGE_FILE,
			    rows,
			    cols,
			    getDistributedFilePfmi(rows, cols));

    mu_assert("Error while creating pfm", pfm != NULL);
    mu_assert("Wrong rows num", pfm->rows == rows);
    mu_assert("Wrong cols num", pfm->cols == cols);

    deletePfm(pfm);
    system("rm -f " DISTRIBUTED_PFMI_STORAGE_FILE);
    return 0;
}

const char *testDistributedPfmGetStoreCol() {
    return 0;
}

const char *testDistributedPfmRemoveRow() {
    return 0;
}
