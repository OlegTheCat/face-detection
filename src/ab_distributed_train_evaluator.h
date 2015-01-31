#pragma once

struct DataSet;
struct TrainEvaluator;

void trainAdaBoostDistributed(void *data,
			      void *classifier,
			      struct DataSet *ds);

struct TrainEvaluator *createAbDistributedTrainEvaluator();
