#include "pfm_distributed_file_impl.h"

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#include "persistent_float_matrix.h"
#include "pfm_partial_file_impl.h"

struct PfmiDistributedFileImplData {
    Pfmi *partial_file_impl;
};

typedef struct PfmiDistributedFileImplData PfmiDistributedFileImplData;

char *manglePathForProcess(const char *path, int process_rank, char *result_path) {
    sprintf(result_path, "%s.process_%d", path, process_rank);
    return result_path;
}

int getPfmiDistributedFileCol(Pfmi *pfmi, float *buf, int col_idx) {
    PfmiDistributedFileImplData *pfmi_data;

    pfmi_data = (PfmiDistributedFileImplData *)pfmi->impl_data;

    return pfmi_data->partial_file_impl->get_col_func(pfmi_data->partial_file_impl,
						      buf,
						      col_idx);
}

int storePfmiDistributedFileCol(Pfmi *pfmi, const float *col, int col_idx) {
    PfmiDistributedFileImplData *pfmi_data;

    pfmi_data = (PfmiDistributedFileImplData *)pfmi->impl_data;

    return pfmi_data->partial_file_impl->store_col_func(pfmi_data->partial_file_impl,
							col,
							col_idx);
}

int removePfmiDistributedFileRow(Pfmi *pfmi, int row_idx) {
    return ((PfmiDistributedFileImplData *)pfmi->impl_data)->
	partial_file_impl->remove_row_func(((PfmiDistributedFileImplData *)pfmi->impl_data)->
					   partial_file_impl, row_idx);
}

void deletePfmiDistributedFile(Pfmi *pfmi) {
    PfmiDistributedFileImplData *pfmi_data;

    if (pfmi != NULL) {
	pfmi_data = pfmi->impl_data;
	if (pfmi_data != NULL) {
	    if (pfmi_data->partial_file_impl != NULL) {
		pfmi_data->partial_file_impl->
		    impl_delete_func(pfmi_data->partial_file_impl);
	    }
	    free(pfmi_data);
	}
	free(pfmi);
    }
}

PfmiDistributedFileImplData *createDistributedPfmiData(const char *storage_path,
						       int comm_rank,
						       int comm_size,
						       int cols,
						       int rows) {
    PfmiDistributedFileImplData *data;
    Pfmi *partial_file_impl;
    int start_col, end_col, cols_per_process;
    float cols_part;

    cols_part = (float)1 / comm_size;
    cols_per_process = cols_part * cols;

    start_col = comm_rank * cols_per_process;
    if (comm_rank != comm_size - 1) {
	end_col = start_col + cols_per_process;
    } else {
	end_col = cols;
    }

    partial_file_impl = createPfmPartialFileImpl(storage_path,
						 start_col,
						 end_col,
						 rows);

    data = malloc(sizeof(PfmiDistributedFileImplData));
    data->partial_file_impl = partial_file_impl;

    return data;
}

Pfmi *createPfmDistributedFileImpl(const char *storage_path,
				   int cols,
				   int rows) {
    int comm_size, comm_rank;
    char proc_storage_path[350];
    Pfmi *pfmi;
    void *data;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    manglePathForProcess(storage_path, comm_rank, proc_storage_path);

    data = createDistributedPfmiData(proc_storage_path, comm_rank, comm_size, cols, rows);

    pfmi = malloc(sizeof(Pfmi));
    pfmi->impl_data = data;
    pfmi->get_col_func = getPfmiDistributedFileCol;
    pfmi->store_col_func = storePfmiDistributedFileCol;
    pfmi->remove_row_func = removePfmiDistributedFileRow;
    pfmi->impl_delete_func = deletePfmiDistributedFile;

    return pfmi;
}
