#include "restricted_decision_stump.h"

#include "persistent_float_matrix.h"
#include "impl_config.h"
#include "evaluators.h"

Rds createRds(int feature_idx) {
    return createRdsWithEvaluators(feature_idx, NULL, NULL);
}

RestrictedDecisionStump createRdsWithEvaluators(int feature_idx,
						struct TrainEvaluator *te,
						struct ClassifyEvaluator *ce) {
    Rds rds;

    rds.feature_idx = feature_idx;
    rds.left_val = negative_label;
    rds.right_val = negative_label;
    rds.threshold = 0;

    if (te == NULL) {
	rds.train_evaluator =
	    getCurrentImplConfig()->rds_train_evaluator;
    } else {
	rds.train_evaluator = te;
    }

    if (ce == NULL) {
	rds.classify_evaluator =
	    getCurrentImplConfig()->rds_classify_evaluator;
    } else {
	rds.classify_evaluator = ce;
    }

    return rds;
}


void trainRds(RestrictedDecisionStump *rds, struct DataSet *ds) {
    rds->
	train_evaluator->
	train_func(rds->
		   train_evaluator->
		   evaluator_data, rds, ds);
}

void classifyDataWithRds(const RestrictedDecisionStump *rds,
			 struct PersistentFloatMatrix *pfm,
			 Label *labels) {
    rds->
	classify_evaluator->
	classify_func(rds->
		      classify_evaluator->
		      evaluator_data, rds, pfm, labels);
}

