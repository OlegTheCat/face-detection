#pragma once

#include "data_set.h"

struct PersistentFloatMatrix;

struct RestrictedDecisionStump {
    int feature_idx;
    Label left_val;
    Label right_val;
    float threshold;
};

typedef struct RestrictedDecisionStump RestrictedDecisionStump;
typedef RestrictedDecisionStump Rds;

RestrictedDecisionStump createRds(int feature_idx);
void trainRds(RestrictedDecisionStump *rds, struct DataSet *ds);
void classifyDataWithRds(const RestrictedDecisionStump *rds,
			 struct PersistentFloatMatrix *pfm,
			 Label *labels);
