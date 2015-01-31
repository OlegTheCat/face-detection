#pragma once

struct DataSet;
struct TrainEvaluator;
struct AdaBoost;
enum Label;

void initWeights(float *weights, struct DataSet *ds);
void normalizeWeigths(float *weights, int num_examples);
void updateWeigths(const enum Label *res_labels,
		   const enum Label *labels,
		   float beta,
		   float *weights,
		   int num_examples);
float calcWeightedError(const enum Label *res_labels,
			const enum Label *labels,
			const float *weights,
			int num_examples);
void getBestStump(Rds *stumps,
		  int stumps_num,
		  DataSet *ds,
		  float *weights,
		  Label *res_labels,
		  Rds *best_stump,
		  float *min_wg_error);
void trainWeak(struct AdaBoost *ab, struct DataSet *ds, float *weights);

void trainAdaBoostSingleThreaded(void *data,
				 void *classifier,
				 struct DataSet *ds);

struct TrainEvaluator *createAbStTrainEvaluator();

