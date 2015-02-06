#include "bench.h"

#include <time.h>
#include <float.h>
#include <stdio.h>

float startBenchmark() {
    return (float)clock() / CLOCKS_PER_SEC;
}

float endBenchmark() {
    return (float)clock() / CLOCKS_PER_SEC;
}

float getTime(float start, float end) {
    return end - start;
}

float performBenchmark(BenchmarkFunc fn) {
    float start, end;

    start = startBenchmark();
    fn();
    end = endBenchmark();

    return getTime(end, start);
}

float performBenchmarkTimes(BenchmarkFunc fn, int times) {
    int i;
    float min_time, time;

    min_time = FLT_MAX;
    for (i = 0; i < times; i++) {
	time = performBenchmark(fn);

	if (time < min_time) {
	    min_time = time;
	}
    }

    return min_time;
}

void reportElapsedTime(const char *bench_name, float time) {
    printf("Benchmark %s lasted %f seconds\n\n", bench_name, time);
}

