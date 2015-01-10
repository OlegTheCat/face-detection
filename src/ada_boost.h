#pragma once

#include "restricted_decision_stump.h"

struct DataSet;

struct WeightedRestrictedDecisionStump {
    RestrictedDecisionStump rds;
    float wg;
};

typedef struct WeightedRestrictedDecisionStump WeightedRestrictedDecisionStump;

typedef WeightedRestrictedDecisionStump Wrds;

struct AdaBoost {
    ArrayList weighted_stumps;
    float threshold;
};

typedef struct AdaBoost AdaBoost;

AdaBoost *createAdaBoost();
void trainAdaBoost(AdaBoost *ab, struct DataSet *ds);
void classifyDataWithAdaBoost(const AdaBoost *ab,
			      struct PersistentFloatMatrix *data,
			      Label *labels);
void deleteAdaBoost(AdaBoost *ab);
