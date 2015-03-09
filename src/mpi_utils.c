#include "mpi_utils.h"

Range getIdxRangeForProcess(int arr_size,
			    int comm_rank,
			    int comm_size) {
    float part;
    int start_idx, end_idx, size_per_process;

    part = (float)1 / comm_size;
    size_per_process = part * arr_size;

    start_idx = comm_rank * size_per_process;
    if (comm_rank != comm_size - 1) {
	end_idx = start_idx + size_per_process;
    } else {
	end_idx = arr_size;
    }

    return createRange(start_idx, end_idx);
}
