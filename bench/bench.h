#pragma once

#define START_BENCHMARK(num)			\
    double __start_time ## num = startBenchmark()

#define END_BENCHMARK(num)			\
    double __end_time ## num = endBenchmark()

#define GET_TIME(num) \
    getTime(__start_time ## num, __end_time ## num)


typedef void (*BenchmarkFunc)();

double startBenchmark();
double endBenchmark();
double getTime(double start, double end);

double performBenchmark(BenchmarkFunc fn);
double performBenchmarkTimes(BenchmarkFunc fn, int times);

void reportElapsedTime(const char *bench_name, double time);
void procReportElapsedTime(const char *bench_name, double time);
void rootReportElapsedTime(const char *bench_name, double time);
