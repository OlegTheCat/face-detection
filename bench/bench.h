#pragma once

typedef void (*BenchmarkFunc)();

float startBenchmark();
float endBenchmark();
float getTime(float start, float end);

float performBenchmark(BenchmarkFunc fn);
float performBenchmarkTimes(BenchmarkFunc fn, int times);
