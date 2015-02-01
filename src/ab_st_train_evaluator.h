#pragma once

struct DataSet;
struct TrainEvaluator;
struct AdaBoost;
enum Label;
struct RestrictedDecisionStump;

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
void getBestStump(struct RestrictedDecisionStump *stumps,
		  int stumps_num,
		  struct DataSet *ds,
		  float *weights,
		  enum Label *res_labels,
		  struct RestrictedDecisionStump *best_stump,
		  float *min_wg_error);
void trainWeak(struct AdaBoost *ab, struct DataSet *ds, float *weights);

void trainAdaBoostSingleThreaded(void *data,
				 void *classifier,
				 struct DataSet *ds);

struct TrainEvaluator *createAbStTrainEvaluator();

