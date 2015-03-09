#include <stdio.h>
#include <mpi.h>

#include "impl_config.h"

#include "ab_bench.h"
#include "pfm_bench.h"
#include "mpi_utils.h"

void runBenchs() {
    int comm_size, comm_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    if (PROCESS_IS_ROOT(comm_rank)) {
	performAdaBoostBenchs();
	performPfmBenchs();
    }

    MPI_Barrier(MPI_COMM_WORLD);

    performDistributedAdaBoostBenchs();
}

int main() {
    MPI_Init(NULL, NULL);
    setImplConfig(getDefaultImplConfig());

    runBenchs();

    deleteImplConfig(getCurrentImplConfig());
    MPI_Finalize();

    return 0;
}
