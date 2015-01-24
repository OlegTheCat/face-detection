#pragma once

struct DataSet;
struct TrainEvaluator;

void trainAdaBoostSingleThreaded(void *data,
				 void *classifier,
				 struct DataSet *ds);

struct TrainEvaluator *createAbStTrainEvaluator();

