#include "ab_bench.h"

#include "bench.h"
#include "ada_boost.h"
#include "evaluators.h"
#include "ab_distributed_train_evaluator.h"
#include "data_set.h"
#include "test_utils.h"
#include "mpi_utils.h"

void performAdaBoostBenchs() {
    DataSet *ds100x100, *ds200x200, *ds200x300, *ds500x500;
    AdaBoost *ab1, *ab2, *ab3, *ab4;

    ds100x100 = getRandomDataSet(100, 100);
    ds200x200 = getRandomDataSet(200, 200);
    ds200x300 = getRandomDataSet(200, 300);
    ds500x500 = getRandomDataSet(500, 500);

    ab1 = createAdaBoost(50);
    ab2 = createAdaBoost(50);
    ab3 = createAdaBoost(50);
    ab4 = createAdaBoost(50);

    START_BENCHMARK(0);
    trainAdaBoost(ab1, ds100x100);
    END_BENCHMARK(0);
    reportElapsedTime("AB 100x100 train", GET_TIME(0));

    START_BENCHMARK(1);
    trainAdaBoost(ab2, ds200x200);
    END_BENCHMARK(1);
    reportElapsedTime("AB ds200x200 train", GET_TIME(1));

    START_BENCHMARK(2);
    trainAdaBoost(ab3, ds200x300);
    END_BENCHMARK(2);
    reportElapsedTime("AB 200x300 train", GET_TIME(2));

    START_BENCHMARK(3);
    trainAdaBoost(ab4, ds500x500);
    END_BENCHMARK(3);
    reportElapsedTime("AB 500x500 train", GET_TIME(3));
}

void performDistributedAdaBoostBenchs() {
    DataSet *ds100x100, *ds200x200, *ds200x300, *ds500x500;
    AdaBoost *ab1, *ab2, *ab3, *ab4;
    TrainEvaluator *te1, *te2, *te3, *te4;

    ds100x100 = getDistributedRandomDataSet(100, 100);
    ds200x200 = getDistributedRandomDataSet(200, 200);
    ds200x300 = getDistributedRandomDataSet(200, 300);
    ds500x500 = getDistributedRandomDataSet(500, 500);

    te1  = createAbDistributedTrainEvaluator();
    te2  = createAbDistributedTrainEvaluator();
    te3  = createAbDistributedTrainEvaluator();
    te4  = createAbDistributedTrainEvaluator();
    ab1  = createAdaBoostWithEvaluators(50, te1 , NULL);
    ab2  = createAdaBoostWithEvaluators(50, te2 , NULL);
    ab3  = createAdaBoostWithEvaluators(50, te3 , NULL);
    ab4  = createAdaBoostWithEvaluators(50, te4 , NULL);

    START_BENCHMARK(0);
    trainAdaBoost(ab1, ds100x100);
    END_BENCHMARK(0);
    rootReportElapsedTime("Distributed AB 100x100 train", GET_TIME(0));

    START_BENCHMARK(1);
    trainAdaBoost(ab2, ds200x200);
    END_BENCHMARK(1);
    rootReportElapsedTime("Distributed AB 200x200 train", GET_TIME(1));

    START_BENCHMARK(2);
    trainAdaBoost(ab3, ds200x300);
    END_BENCHMARK(2);
    rootReportElapsedTime("Distributed AB 200x300 train", GET_TIME(2));

    START_BENCHMARK(3);
    trainAdaBoost(ab4, ds500x500);
    END_BENCHMARK(3);
    rootReportElapsedTime("Distributed AB 500x500 train", GET_TIME(3));
}
