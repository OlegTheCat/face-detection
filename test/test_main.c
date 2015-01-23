#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "impl_config.h"

#include "rds_ig_train_evaluator.h"
#include "rds_single_threaded_classify_evaluator.h"
#include "rds_ocl_bf_train_evaluator.h"

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

int tests_run = 0;

static void createConfigSet(ImplConfig ***configs, int *num_configs) {
    ImplConfig *config;

    *num_configs = 1;
    *configs = malloc(sizeof(ImplConfig *) * (*num_configs));

    config = malloc(sizeof(ImplConfig));
    config->rds_train_evaluator = createRdsIgTrainEvaluator();
    config->rds_classify_evaluator = createRdsSingleThreadedClassifyEvaluator();
    (*configs)[0] = config;

    config = malloc(sizeof(ImplConfig));
    config->rds_train_evaluator = createRdsOclBfTrainEvaluator();
    config->rds_classify_evaluator = createRdsSingleThreadedClassifyEvaluator();
    (*configs)[1] = config;
}

static const char *dummy() {
    mu_assert("This should never come up", 1 == 1);
    return 0;
}

static const char *runAllTests() {
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

    return 0;
}

static const char *runAllTestsWithConfig(ImplConfig *config) {

    setImplConfig(config);

    return runAllTests();
}



int main() {
    const char *result;
    ImplConfig **configs;
    int i, num_configs;

    srand(time(NULL));
    createConfigSet(&configs, &num_configs);

    for (i = 0; i < num_configs; i++) {
	printf("==== Running tests for config #%d ====\n", i);
	result = runAllTestsWithConfig(configs[i]);

	if (result != 0) {
	    fprintf(stderr, "%s\n", result);
	    break;
	}
    }

    if (result == 0) {
	printf("ALL TESTS PASSED\n");
    }

    printf("Tests run: %d\n", tests_run);

    for (i = 0; i < num_configs; i++) {
	deleteImplConfig(configs[i]);
    }
    free(configs);

    return result != 0;
}
