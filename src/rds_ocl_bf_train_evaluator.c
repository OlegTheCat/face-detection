#include "rds_ocl_bf_train_evaluator.h"

#include <CL/cl.h>

#include "restricted_decision_stump.h"
#include "evaluators.h"

void trainRdsWithBruteForceOpencl(void *data, void *classifier, struct DataSet *ds) {
    (void)data;
    (void)classifier;
    (void)ds;
}

struct TrainEvaluator *createRdsOclBfTrainEvaluator() {
    TrainEvaluator *train_evaluator;

    train_evaluator = malloc(sizeof(TrainEvaluator));
    train_evaluator->train_func = trainRdsWithBruteForceOpencl;
    train_evaluator->evaluator_data = NULL;

    return train_evaluator;
}

