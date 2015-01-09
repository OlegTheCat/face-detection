#include "restricted_decision_stump.h"

#include <string.h>
#include <math.h>

typedef RestrictedDecisionStump Rds;

Rds createRds(int feature_idx) {
    Rds rds;

    rds.feature_idx = feature_idx;
    rds.left_val = negative_label;
    rds.right_val = negative_label;
    rds.threshold = 0;

    return rds;
}

struct ValLabel {
    float val;
    Label label;
    int idx;
};

typedef struct ValLabel ValLabel;

int compareValLabel(const void *p1, const void *p2) {
    const ValLabel *vl1, *vl2;

    vl1 = (const ValLabel *)p1;
    vl2 = (const ValLabel *)p2;

    if (vl1->val < vl2->val) return -1;
    else if (vl1->val > vl2->val) return 1;
    else return 0;
}

typedef struct ValLabel ValLabel;

void sortValsWithLabels(float *feature_vals, Label *labels, ValLabel **val_labels, int num_examples) {
    ValLabel val_label;
    int i;

    *val_labels = malloc(sizeof(ValLabel) * num_examples);

    for (i = 0; i < num_examples; i++) {
	val_label.val = feature_vals[i];
	val_label.label = labels[i];
	val_label.idx = i;

	(*val_labels)[i] = val_label;
    }

    qsort(*val_labels, num_examples, sizeof(ValLabel), compareValLabel);
}

float entropy(int num_pos_examples, int num_neg_examples) {
    float entropy, pos_percent, neg_percent;

    pos_percent = num_pos_examples / (num_pos_examples + num_neg_examples);
    neg_percent = num_neg_examples / (num_pos_examples + num_neg_examples);

    entropy = (-pos_percent * log2f(pos_percent)) + (-neg_percent * log2f(neg_percent));

    return entropy;
}

void countPosNegExamplesNum(ValLabel *val_labels, int num_examples,
			    int *num_pos_examples,
			    int *num_neg_examples) {
    int i;

    *num_pos_examples = 0;
    *num_neg_examples = 0;
    for (i = 0; i < num_examples; i++) {
	if (val_labels[i].label == positive_label) {
	    (*num_pos_examples)++;
	} else if (val_labels[i].label == negative_label) {
	    (*num_neg_examples)++;
	}
    }

}

float entropyForValLabels(ValLabel *val_labels, int num_examples) {
    int num_pos_examples, num_neg_examples;

    countPosNegExamplesNum(val_labels, num_examples,
			   &num_pos_examples, &num_neg_examples);

    return entropy(num_pos_examples, num_neg_examples);
}

float informationGainForValLabels(ValLabel *val_labels,
				  int split_after_idx,
				  float full_entropy,
				  int num_examples) {
    float information_gain, entropy1, entropy2;

    entropy1 = entropyForValLabels(val_labels, split_after_idx);
    entropy2 = entropyForValLabels(val_labels + split_after_idx, num_examples);

    information_gain = full_entropy -
	entropy1 * (split_after_idx / num_examples) -
	entropy2 * ((num_examples - split_after_idx) / num_examples);

    return information_gain;
}

int getBestSplitIdx(ValLabel *val_labels, float full_entropy, int num_examples) {
    int i, j, best_split_idx;
    float information_gain_max, information_gain;

    information_gain_max = 0;
    j = 0;
    for (i = 1; i < num_examples; i++) {
	if (val_labels[j].label != val_labels[i].label) {
	    information_gain = informationGainForValLabels(val_labels,
							   j,
							   full_entropy,
							   num_examples);
	    if (information_gain > information_gain_max) {
		information_gain_max = information_gain;
		best_split_idx = j;
	    }
	}

	j = i;
    }

    return best_split_idx;
}

Label mostCommonLabel(ValLabel *val_labels, int num_examples) {
    int num_pos_examples, num_neg_examples;

    countPosNegExamplesNum(val_labels, num_examples,
			   &num_pos_examples, &num_neg_examples);

    return (num_pos_examples > num_neg_examples) ?
	positive_label :
	negative_label;
}

void trainRds(Rds *rds, DataSet *ds) {
    float *feature_vals, full_entropy, threshold;
    Label *labels;
    ValLabel *val_labels;
    int best_split_idx;

    feature_vals = malloc(sizeof(float) * getExamplesNum(ds));
    getFeatureVals(ds, feature_vals, rds->feature_idx);

    labels = malloc(sizeof(Label) * getExamplesNum(ds));
    memcpy(labels, ds->labels, sizeof(Label) * getExamplesNum(ds));

    sortValsWithLabels(feature_vals, labels,
		       &val_labels, getExamplesNum(ds));

    full_entropy = entropy(getPosExamplesNum(ds),
			   getNegExamplesNum(ds));

    best_split_idx = getBestSplitIdx(val_labels, full_entropy, getExamplesNum(ds));

    threshold = (val_labels[best_split_idx].val +
		 val_labels[best_split_idx].val) / 2;



    rds->threshold = threshold;
    rds->left_val = mostCommonLabel(val_labels, best_split_idx);
    rds->right_val = mostCommonLabel(val_labels + best_split_idx, getExamplesNum(ds));

    free(feature_vals);
    free(labels);
    free(val_labels);
}
