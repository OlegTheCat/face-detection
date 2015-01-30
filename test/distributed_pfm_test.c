#include "distributed_pfm_test.h"

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#include "minunit.h"
#include "test_utils.h"
#include "persistent_float_matrix.h"
#include "pfm_distributed_file_impl.h"
#include "utils.h"
#include "mpi_utils.h"

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
				 getDistributedFilePfmi(FILE_NAME_HANDLE,
							rows, cols)));

    mu_assert("Error while creating pfm", pfm != NULL);
    mu_assert("Wrong rows num", pfm->rows == rows);
    mu_assert("Wrong cols num", pfm->cols == cols);

    deletePfm(pfm);
    return 0;
}

const char *testDistributedPfmGetStoreCol() {
    PersistentFloatMatrix *pfm;
    const int rows = 10, cols = 20;
    float col[rows];
    int i, comm_size, comm_rank;
    int err;
    Range col_range;

    WITH_TEST_FILE_NAME
	(pfm = createPfmWithImpl(FILE_NAME_HANDLE,
				 rows, cols,
				 getDistributedFilePfmi(FILE_NAME_HANDLE,
							rows, cols)));

    for (i = 0; i < rows; i++) {
	col[i] = (float)i;
    }

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    col_range = getIdxRangeForProcess(cols, comm_rank, comm_size);

    err = storePfmCol(pfm, col, col_range.from);
    mu_assert("Wrong store #1 error code", err == 0);
    err = storePfmCol(pfm, col, col_range.to);
    mu_assert("Wrong store #2 error code", err == PFMI_OUT_OF_COL_RANGE);

    err = getPfmCol(pfm, col, col_range.from);
    mu_assert("Wrong get #1 error code", err == 0);
    for (i = 0; i < 10; i++) {
	mu_assert("Wrong value in col #5", floatEqual(col[i], i));
    }

    err = getPfmCol(pfm, col, col_range.to);
    mu_assert("Wrong get #2 error code", err == PFMI_OUT_OF_COL_RANGE);

    deletePfm(pfm);

    return 0;
}

const char *testDistributedPfmRemoveRow() {
    PersistentFloatMatrix *pfm;
    const int rows = 10, cols = 20;
    float col[rows];
    int i, comm_size, comm_rank;
    Range col_range;

    WITH_TEST_FILE_NAME
	(pfm = createPfmWithImpl(FILE_NAME_HANDLE,
				 rows, cols,
				 getDistributedFilePfmi(FILE_NAME_HANDLE,
							rows, cols)));

    for (i = 0; i < rows; i++) {
	col[i] = (float)(i + 1);
    }

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    col_range = getIdxRangeForProcess(cols, comm_rank, comm_size);

    storePfmCol(pfm, col, col_range.from);
    removePfmRow(pfm, 0);
    removePfmRow(pfm, 0);
    removePfmRow(pfm, 5);

    getPfmCol(pfm, col, col_range.from);
    mu_assert("Wrong col1 value at pos #1", floatEqual(col[0], 3));
    mu_assert("Wrong col1 value at pos #6", floatEqual(col[6], 10));

    deletePfm(pfm);
    return 0;
}
