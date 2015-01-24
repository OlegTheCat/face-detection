#include "ab_st_classify_evaluator.h"

#include <string.h>

#include "ada_boost.h"
#include "persistent_float_matrix.h"
#include "evaluators.h"

void classifyDataWithAbSingleThreaded(void *data,
				      const void *classifier,
				      struct PersistentFloatMatrix *pfm,
				      enum Label *labels) {
    (void)data;
    Label *rds_labels;
    float *wrds_output;
    int i, j;
    Wrds *wrds;
    AdaBoost *ab;

    ab = (AdaBoost *)classifier;

    wrds_output = malloc(sizeof(float) * pfm->rows);
    memset(wrds_output, 0, sizeof(float) * pfm->rows);
    rds_labels = malloc(sizeof(Label) * pfm->rows);

    for (i = 0; i < ab->weighted_stumps.size; i++) {
	wrds = getFromArrayList(&ab->weighted_stumps, i);
	classifyDataWithRds(&wrds->stump, pfm, rds_labels);

	for (j = 0; j < pfm->rows; j++) {
	    if (rds_labels[j] == positive_label) {
		wrds_output[j] += wrds->wg;
	    }
	}
    }

    for (i = 0; i < pfm->rows; i++) {
	if (wrds_output[i] >= ab->threshold) {
	    labels[i] = positive_label;
	} else {
	    labels[i] = negative_label;
	}
    }

    free(wrds_output);
    free(rds_labels);

}

ClassifyEvaluator *createAbStClassifyEvaluator() {
    ClassifyEvaluator *evaluator;

    evaluator = malloc(sizeof(ClassifyEvaluator));
    evaluator->evaluator_data = NULL;
    evaluator->classify_func = classifyDataWithAbSingleThreaded;

    return evaluator;
}
