#pragma once

#include "data_set.h"

struct PersistentFloatMatrix;
struct TrainEvaluator;
struct ClassifyEvaluator;

struct RestrictedDecisionStump {
    int feature_idx;
    Label left_val;
    Label right_val;
    float threshold;
    struct TrainEvaluator *train_evaluator;
    struct ClassifyEvaluator *classify_evaluator;
};

typedef struct RestrictedDecisionStump RestrictedDecisionStump;
typedef RestrictedDecisionStump Rds;

RestrictedDecisionStump createRds(int feature_idx);
RestrictedDecisionStump createRdsWithEvaluators(int feature_idx,
						struct TrainEvaluator *te,
						struct ClassifyEvaluator *ce);
void trainRds(RestrictedDecisionStump *rds, struct DataSet *ds);
void classifyDataWithRds(const RestrictedDecisionStump *rds,
			 struct PersistentFloatMatrix *pfm,
			 Label *labels);
