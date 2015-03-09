#include "ab_distributed_train_test.h"

#include <stdlib.h>
#include <stdio.h>

#include "minunit.h"
#include "test_utils.h"
#include "ada_boost.h"
#include "ab_distributed_train_evaluator.h"
#include "data_set.h"
#include "persistent_float_matrix.h"
#include "pfm_distributed_file_impl.h"
#include "evaluators.h"

const char *testCreateDistributedAb() {
    AdaBoost *ab;
    TrainEvaluator *te;

    te = createAbDistributedTrainEvaluator();
    ab = createAdaBoostWithEvaluators(10, te, NULL);

    mu_assert("TrainEvaluator == NULL", te != NULL);
    mu_assert("AdaBoost == NULL", ab != NULL);

    free(te);
    deleteAdaBoost(ab);
    return 0;
}

const char *testTrainDistributedAb() {
    TrainEvaluator *te;
    AdaBoost *ab;
    DataSet *ds;
    int num_examples, num_features;

    num_examples = 50;
    num_features = 100;

    ds = getDistributedRandomDataSet(num_examples, num_features);
    te = createAbDistributedTrainEvaluator();
    ab = createAdaBoostWithEvaluators(10, te, NULL);

    trainAdaBoost(ab, ds);

    deleteDataSet(ds);
    deleteAdaBoost(ab);
    free(te);
    return 0;
}
