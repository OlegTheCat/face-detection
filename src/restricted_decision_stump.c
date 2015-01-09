#include "restricted_decision_stump.h"

#include <string.h>
#include <math.h>
#include <stdio.h>
#include <assert.h>

#include "persistent_float_matrix.h"

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
    float pos_entropy, neg_entropy;
    pos_percent = (float)num_pos_examples / (num_pos_examples + num_neg_examples);
    neg_percent = (float)num_neg_examples / (num_pos_examples + num_neg_examples);
    /* printf("num_pos_examples = %d\n", num_pos_examples); */
    /* printf("num_neg_examples = %d\n", num_neg_examples); */
    /* printf("pos_percent = %f\n", pos_percent); */
    /* printf("neg_percent = %f\n", neg_percent); */

    pos_entropy = (pos_percent == 0) ? 0 : (-pos_percent * log2f(pos_percent));
    neg_entropy = (neg_percent == 0) ? 0 : (-neg_percent * log2f(neg_percent));
    entropy =  pos_entropy + neg_entropy;

    assert(entropy >= 0);

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
    int split1_examples_num, split2_examples_num;

    split1_examples_num = split_after_idx + 1;
    split2_examples_num = num_examples - split1_examples_num;

    entropy1 = entropyForValLabels(val_labels,
				   split1_examples_num);
    entropy2 = entropyForValLabels(val_labels + split1_examples_num,
				   split2_examples_num);

    information_gain = full_entropy -
	entropy1 * (split1_examples_num / num_examples) -
	entropy2 * (split2_examples_num / num_examples);

    assert(information_gain >= 0);

    return information_gain;
}

int getBestSplitIdx(ValLabel *val_labels, float full_entropy, int num_examples) {
    int i, j, best_split_idx;
    float information_gain_max, information_gain;

    best_split_idx = -1;
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
    int best_split_idx, split1_examples_num, split2_examples_num;

    feature_vals = malloc(sizeof(float) * getExamplesNum(ds));
    getFeatureVals(ds, feature_vals, rds->feature_idx);

    labels = malloc(sizeof(Label) * getExamplesNum(ds));
    memcpy(labels, ds->labels, sizeof(Label) * getExamplesNum(ds));

    sortValsWithLabels(feature_vals, labels,
		       &val_labels, getExamplesNum(ds));

    full_entropy = entropy(getPosExamplesNum(ds),
			   getNegExamplesNum(ds));
    /* printf("Full entropy = %f\n", full_entropy); */

    best_split_idx = getBestSplitIdx(val_labels, full_entropy, getExamplesNum(ds));

    if (best_split_idx != -1) {
	split1_examples_num = best_split_idx + 1;
	split2_examples_num = getExamplesNum(ds) - split1_examples_num;

	threshold = (val_labels
		     [best_split_idx].
		     val +
		     val_labels[best_split_idx + 1].val) / 2;

	rds->threshold = threshold;
	rds->left_val = mostCommonLabel(val_labels, split1_examples_num);
	rds->right_val = mostCommonLabel(val_labels + split1_examples_num,
					 split2_examples_num);
    } else {
	rds->threshold = 0;
	rds->right_val = rds->left_val =
	    mostCommonLabel(val_labels, getExamplesNum(ds));
    }

    free(feature_vals);
    free(labels);
    free(val_labels);
}

void classifyData(const Rds *rds,
		  PersistentFloatMatrix *pfm,
		  Label *labels) {
    int i;
    float *feature_vals;

    feature_vals = malloc(sizeof(float) * pfm->cols);
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
