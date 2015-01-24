#pragma once

#include "restricted_decision_stump.h"

struct DataSet;
struct TrainEvaluator;
struct ClassifyEvaluator;

struct WeightedRestrictedDecisionStump {
    RestrictedDecisionStump stump;
    float wg;
};

typedef struct WeightedRestrictedDecisionStump WeightedRestrictedDecisionStump;

typedef WeightedRestrictedDecisionStump Wrds;

struct AdaBoost {
    ArrayList weighted_stumps;
    int max_stumps;
    float threshold;
    struct TrainEvaluator *train_evaluator;
    struct ClassifyEvaluator *classify_evaluator;
};

typedef struct AdaBoost AdaBoost;

AdaBoost *createAdaBoost(int max_features);
AdaBoost *createAdaBoostWithEvaluators(int max_features,
				       struct TrainEvaluator *train_evaluator,
				       struct ClassifyEvaluator *classify_evaluator);
void trainAdaBoost(AdaBoost *ab, struct DataSet *ds);
void classifyDataWithAdaBoost(const AdaBoost *ab,
			      struct PersistentFloatMatrix *data,
			      Label *labels);
void deleteAdaBoost(AdaBoost *ab);
