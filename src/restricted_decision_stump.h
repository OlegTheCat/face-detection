#pragma once

#include "data_set.h"

struct RestrictedDecisionStump {
    int feature_idx;
    Label left_val;
    Label right_val;
    float threshold;
};

typedef struct RestrictedDecisionStump RestrictedDecisionStump;

RestrictedDecisionStump createRds(int feature_idx);
void trainRds(RestrictedDecisionStump *rds, struct DataSet *ds);
