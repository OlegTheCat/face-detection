#include "ab_bench.h"

#include "bench.h"
#include "ada_boost.h"
#include "evaluators.h"
#include "ab_distributed_train_evaluator.h"
#include "data_set.h"
#include "test_utils.h"
#include "mpi_utils.h"

void benchAdaBoost(const char *bench_name,
		   int num_examples,
		   int num_features,
		   int num_weaks) {
    DataSet *ds;
    AdaBoost *ab;

    ds = getRandomDataSet(num_examples, num_features);
    ab = createAdaBoost(num_weaks);

    START_BENCHMARK(0);
    trainAdaBoost(ab, ds);
    END_BENCHMARK(0);
    reportElapsedTime(bench_name, GET_TIME(0));

    deleteAdaBoost(ab);
    deleteDataSet(ds);
}

void benchDistributedAdaBoost(const char *bench_name,
			      int num_examples,
			      int num_features,
			      int num_weaks) {
    DataSet *ds;
    AdaBoost *ab;
    TrainEvaluator *te;

    ds = getDistributedRandomDataSet(num_examples, num_features);
    te = createAbDistributedTrainEvaluator();
    ab = createAdaBoostWithEvaluators(num_weaks, te, NULL);

    START_BENCHMARK(0);
    trainAdaBoost(ab, ds);
    END_BENCHMARK(0);
    rootReportElapsedTime(bench_name, GET_TIME(0));

    deleteAdaBoost(ab);
    deleteDataSet(ds);
    free(te);
}


void performAdaBoostBenchs() {
    benchAdaBoost("AB 100x100 train", 100, 100, 50);
    benchAdaBoost("AB 200x200 train", 200, 200, 50);
    benchAdaBoost("AB 300x300 train", 300, 300, 50);
    benchAdaBoost("AB 500x500 train", 500, 500, 50);
}

void performDistributedAdaBoostBenchs() {
    benchDistributedAdaBoost("Distributed AB 100x100 train", 100, 100, 50);
    benchDistributedAdaBoost("Distributed AB 200x200 train", 200, 200, 50);
    benchDistributedAdaBoost("Distributed AB 300x300 train", 300, 300, 50);
    benchDistributedAdaBoost("Distributed AB 500x500 train", 500, 500, 50);
}
