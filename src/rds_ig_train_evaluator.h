#pragma once

struct DataSet;
struct TrainEvaluator;

void trainRdsWithInformationGain(void *data,
				 void *classifier,
				 struct DataSet *ds);

struct TrainEvaluator *createRdsIgTrainEvaluator();

