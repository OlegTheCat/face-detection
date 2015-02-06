#include "ab_bench.h"

#include "bench.h"
#include "ada_boost.h"
#include "data_set.h"
#include "test_utils.h"

void performAdaBoostBenchs() {
    DataSet *ds100x100, *ds200x200, *ds200x300;
    AdaBoost *ab1, *ab2, *ab3;

    ds100x100 = getRandomDataSet(100, 100);
    ds200x200 = getRandomDataSet(200, 200);
    ds200x300 = getRandomDataSet(200, 300);

    ab1 = createAdaBoost(50);
    ab2 = createAdaBoost(50);
    ab3 = createAdaBoost(50);

    START_BENCHMARK(0);
    trainAdaBoost(ab1, ds100x100);
    END_BENCHMARK(0);

    reportElapsedTime("AB 100x100 train", GET_TIME(0));

    START_BENCHMARK(1);
    trainAdaBoost(ab2, ds200x200);
    END_BENCHMARK(1);

    reportElapsedTime("AB 100x100 train", GET_TIME(1));

    START_BENCHMARK(2);
    trainAdaBoost(ab3, ds200x300);
    END_BENCHMARK(2);

    reportElapsedTime("AB 100x100 train", GET_TIME(2));
}

