#include "bench.h"

#include <sys/time.h>

#include <time.h>
#include <float.h>
#include <stdio.h>
#include <mpi.h>

#include "mpi_utils.h"

/* double getCurrentTime() { */
/*     return (double)clock() / CLOCKS_PER_SEC; */
/* } */

double getCurrentTime() {
    struct timeval tim;
    gettimeofday(&tim, NULL);

    return tim.tv_sec + (tim.tv_usec / 1000000.0);
}

double startBenchmark() {
    return getCurrentTime();
}

double endBenchmark() {
    return getCurrentTime();
}

double getTime(double start, double end) {
    return end - start;
}

double performBenchmark(BenchmarkFunc fn) {
    double start, end;

    start = startBenchmark();
    fn();
    end = endBenchmark();

    return getTime(end, start);
}

double performBenchmarkTimes(BenchmarkFunc fn, int times) {
    int i;
    double min_time, time;

    min_time = FLT_MAX;
    for (i = 0; i < times; i++) {
	time = performBenchmark(fn);

	if (time < min_time) {
	    min_time = time;
	}
    }

    return min_time;
}

void reportElapsedTime(const char *bench_name, double time) {
    printf("Benchmark '%s' lasted %f seconds\n\n", bench_name, time);
}

void procReportElapsedTime(const char *bench_name, double time) {
    proc_printf("Benchmark '%s' lasted %f seconds\n\n", bench_name, time);
}

void rootReportElapsedTime(const char *bench_name, double time) {
    root_printf("Benchmark '%s' lasted %f seconds\n\n", bench_name, time);
}
