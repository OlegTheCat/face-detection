#pragma once

#define START_BENCHMARK(num)			\
    float __start_time ## num = startBenchmark()

#define END_BENCHMARK(num)			\
    float __end_time ## num = endBenchmark()

#define GET_TIME(num) \
    getTime(__start_time ## num, __end_time ## num)

typedef void (*BenchmarkFunc)();

float startBenchmark();
float endBenchmark();
float getTime(float start, float end);

float performBenchmark(BenchmarkFunc fn);
float performBenchmarkTimes(BenchmarkFunc fn, int times);

void reportElapsedTime(const char *bench_name, float time);
