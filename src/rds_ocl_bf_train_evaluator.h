#pragma once

struct TrainEvaluator;
struct DataSet;

void trainRdsWithBruteForceOpencl(void *data,
				  void *classifier,
				  struct DataSet *ds);

struct TrainEvaluator *createRdsOclBfTrainEvaluator();
