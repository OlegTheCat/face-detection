#include "ada_boost.h"

#include <math.h>
#include <float.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "data_set.h"
#include "persistent_float_matrix.h"

AdaBoost *createAdaBoost(int max_features) {
    AdaBoost *ab;

    ab = malloc(sizeof(AdaBoost));

    ab->weighted_stumps = createArrayList(sizeof(Wrds));
    ab->threshold = 0;
    ab->max_stumps = max_features;

    return ab;
}

void initWeights(float *weights, DataSet *ds) {
    int i;

    for (i = 0; i < getExamplesNum(ds); i++) {
	if (ds->labels[i] == positive_label) {
	    weights[i] = (float)1 / getPosExamplesNum(ds);
	} else if (ds->labels[i] == negative_label) {
	    weights[i] = (float)1 / getNegExamplesNum(ds);
	}
    }
}

void normalizeWeigths(float *weights, int num_examples) {
    int i;
    float wg_sum;

    wg_sum = 0;
    for (i = 0; i < num_examples; i++) {
	wg_sum += weights[i];
    }

    for (i = 0; i < num_examples; i++) {
	weights[i] /= wg_sum;
    }
}

void updateWeigths(const Label *res_labels,
		   const Label *labels,
		   float beta,
		   float *weights,
		   int num_examples) {
    int i;

    for (i = 0; i < num_examples; i++) {
	if (res_labels[i] == labels[i]) {
	    weights[i] *= beta;
	}
    }
}

float calcWeightedError(const Label *res_labels,
			const Label *labels,
			const float *weights,
			int num_examples) {
    float wg_error;
    int i;

    wg_error = 0;
    for (i = 0; i < num_examples; i++) {
	if (res_labels[i] != labels[i]) {
	    wg_error += weights[i];
	}
    }

    return wg_error;
}



void trainWeak(AdaBoost *ab, DataSet *ds, float *weights) {
    Wrds wrds;
    Rds *stumps, best_stump;
    float wg_error, min_wg_error, beta, alpha;
    Label *res_labels;
    int i;

    stumps = malloc(sizeof(Rds) * getFeaturesNum(ds));
    for (i = 0; i < getFeaturesNum(ds); i++) {
	stumps[i] = createRds(i);
	trainRds(&stumps[i], ds);
    }

    res_labels = malloc(sizeof(float) * getExamplesNum(ds));
    min_wg_error = FLT_MAX;
    for (i = 0; i < getFeaturesNum(ds); i++) {
	classifyDataWithRds(&stumps[i], ds->data, res_labels);
	wg_error = calcWeightedError(res_labels,
				     ds->labels,
				     weights,
				     getExamplesNum(ds));
	assert(!isnan(wg_error));
	if (wg_error < min_wg_error) {
	    min_wg_error = wg_error;
	    best_stump = stumps[i];
	}
    }

    beta = min_wg_error / (1 - min_wg_error);
    alpha = logf(1 / beta);

    /* printf("min_wg_error == %f\n", min_wg_error); */
    /* printf("beta == %f\n", beta); */
    /* printf("alpha == %f\n", alpha); */

    classifyDataWithRds(&best_stump, ds->data, res_labels);

    updateWeigths(res_labels,
		  ds->labels,
		  beta,
		  weights,
		  getExamplesNum(ds));

    wrds.wg = alpha;
    wrds.stump = best_stump;
    /* printf("Collecting stump with feature_idx = %d\n", best_stump.feature_idx); */
    addToArrayList(&ab->weighted_stumps, &wrds);

    ab->threshold += 0.5 * alpha;
    free(res_labels);
    free(stumps);
}

void trainAdaBoost(AdaBoost *ab, DataSet *ds) {
    float *weights;
    int t;

    weights = malloc(sizeof(float) * getExamplesNum(ds));
    initWeights(weights, ds);

    for (t = 0; t < ab->max_stumps; t++) {
	normalizeWeigths(weights, getExamplesNum(ds));
	trainWeak(ab, ds, weights);
    }

    free(weights);
}

void classifyDataWithAdaBoost(const AdaBoost *ab,
			      Pfm *data,
			      Label *labels) {
    Label *rds_labels;
    float *wrds_output;
    int i, j;
    Wrds *wrds;

    wrds_output = malloc(sizeof(float) * data->rows);
    memset(wrds_output, 0, sizeof(float) * data->rows);
    rds_labels = malloc(sizeof(Label) * data->rows);

    for (i = 0; i < ab->weighted_stumps.size; i++) {
	wrds = getFromArrayList(&ab->weighted_stumps, i);
	classifyDataWithRds(&wrds->stump, data, rds_labels);

	for (j = 0; j < data->rows; j++) {
	    if (rds_labels[j] == positive_label) {
		wrds_output[j] += wrds->wg;
	    }
	}
    }

    for (i = 0; i < data->rows; i++) {
	if (wrds_output[i] >= ab->threshold) {
	    labels[i] = positive_label;
	} else {
	    labels[i] = negative_label;
	}
    }

    free(wrds_output);
    free(rds_labels);
}

void deleteAdaBoost(AdaBoost *ab) {
    if (ab != NULL) {
	deleteArrayList(&ab->weighted_stumps);
	free(ab);
    }
}
