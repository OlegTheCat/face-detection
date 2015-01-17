#include "rds_single_threaded_classify_evaluator.h"

#include "restricted_decision_stump.h"
#include "persistent_float_matrix.h"
#include "evaluators.h"

void classifyDataWithRdsSingleThreaded(void *data,
				       const void *classifier,
				       Pfm *pfm,
				       Label *labels) {
    (void)data;

    int i;
    float *feature_vals;
    Rds *rds;

    rds = (Rds *)classifier;
    feature_vals = malloc(sizeof(float) * pfm->rows);
    getPfmCol(pfm, feature_vals, rds->feature_idx);

    for (i = 0; i < pfm->rows; i++) {
	if (feature_vals[i] < rds->threshold) {
	    labels[i] = rds->left_val;
	} else {
	    labels[i] = rds->right_val;
	}
    }

    free(feature_vals);
}

ClassifyEvaluator *createRdsSingleThreadedClassifyEvaluator() {
    ClassifyEvaluator *evaluator;

    evaluator = malloc(sizeof(ClassifyEvaluator));
    evaluator->evaluator_data = NULL;
    evaluator->classify_func = classifyDataWithRdsSingleThreaded;

    return evaluator;
}

