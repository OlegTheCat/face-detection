#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#include "impl_config.h"
#include "ocl_env.h"
#include "mpi_utils.h"

#include "minunit.h"
#include "pgm_test.h"
#include "array_list_test.h"
#include "data_set_test.h"
#include "float_matrix_test.h"
#include "img_proc_test.h"
#include "haar_feature_test.h"
#include "persistent_float_matrix_test.h"
#include "restricted_decision_stump_test.h"
#include "ada_boost_test.h"
#include "partial_pfm_test.h"
#include "distributed_pfm_test.h"
#include "ab_distributed_train_test.h"

int tests_run = 0;

static const char *dummy() {
    mu_assert("This should never come up", 1 == 1);
    return 0;
}

static const char *runAllTests() {
    int comm_size, comm_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);

    if (PROCESS_IS_ROOT(comm_rank)) {
	mu_run_test(dummy);
	mu_run_test(testReadImage);
	mu_run_test(testCreateImage);
	mu_run_test(testSaveImage);
	mu_run_test(testSubImage);
	mu_run_test(testToFloatMatrix);
	mu_run_test(testCreateArrayList);
	mu_run_test(testAddToArrayList);
	mu_run_test(testAddToArrayList2);
	mu_run_test(testRawArray);
	mu_run_test(testReadImageList);
	mu_run_test(testSubSampleImage);
	mu_run_test(testCreateDataSet);
	mu_run_test(testCreateFloatMatrix);
	mu_run_test(testSetGetValues);
	mu_run_test(testMatSum);
	mu_run_test(testComputeII);
	mu_run_test(testStdDev);
	mu_run_test(testMean);
	mu_run_test(testConvertToNormalized);
	mu_run_test(testCreateFeature);
	mu_run_test(testEvaluateFeature);
	mu_run_test(testGenerateHaarFeatures);
	mu_run_test(testCreatePfm);
	mu_run_test(testGetStorePfmCol);
	mu_run_test(testRemovePfmRow);
	mu_run_test(testTrainRds1);
	mu_run_test(testTrainRds2);
	mu_run_test(testTrainRds3);
	mu_run_test(testTrainRds4);
	mu_run_test(testClassifyDataWithRds);
	mu_run_test(testClassifyDataWithRds2);
	mu_run_test(testCreateAdaBoost);
	mu_run_test(testTrainAdaBoost1);
	mu_run_test(testTrainAdaBoost2);
	/* mu_run_test(testClassifyDataWithAdaBoost); */
	mu_run_test(testClassifyDataWithAdaBoost2);
	mu_run_test(testCreatePfmPartialFileImpl);
	mu_run_test(testPartialPfmGetStoreCol);
	mu_run_test(testPartialPfmRemoveRow);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    mu_run_mpi_test(testCreatePfmDistributedFileImpl);
    mu_run_mpi_test(testDistributedPfmGetStoreCol);
    mu_run_mpi_test(testDistributedPfmRemoveRow);
    mu_run_mpi_test(testStoreAllCols);
    mu_run_mpi_test(testRandomFill);

    mu_run_mpi_test(testCreateDistributedAb);
    mu_run_mpi_test(testTrainDistributedAb);

    return 0;
}

int main() {
    const char *result;

    MPI_Init(NULL, NULL);
    srand(time(NULL));
    initSingleGpuEnvironment();
    setImplConfig(getDefaultImplConfig());

    result = runAllTests();

    if (result != 0) {
	root_eprintf("%s\n", result);
    } else {
	root_printf("ALL TESTS PASSED\n");
    }

    root_printf("Tests run: %d\n", tests_run);

    MPI_Finalize();
    deleteImplConfig(getCurrentImplConfig());
    deleteOclEnvironment();
    return result != 0;
}
