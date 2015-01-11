#include "ada_boost_test.h"

#include <stdlib.h>

#include "minunit.h"
#include "ada_boost.h"
#include "data_set.h"
#include "persistent_float_matrix.h"

#define AB_STORAGE_FILE "ab_ds.storage"

static float randomFloat() {
    return ((float)rand() / RAND_MAX) * rand();
}

int misclassifiedExamplesNum(const Label *res_labels,
			     const Label *labels,
			     int num_examples) {
    int i, misclassified_num;

    misclassified_num = 0;
    for (i = 0; i < num_examples; i++) {
	if (res_labels[i] != labels[i]) {
	    misclassified_num++;
	}
    }

    return misclassified_num;
}

static PersistentFloatMatrix *getRandomData(int rows, int cols) {
    PersistentFloatMatrix *data;
    float *buf;
    int i, j;

    data = createPfm(AB_STORAGE_FILE, rows, cols);

    buf = malloc(sizeof(float) * rows);


    for (i = 0; i < cols; i++) {
	for (j = 0; j < rows; j++) {
	    buf[j] = randomFloat();
	}
	storePfmCol(data, buf, i);
    }

    free(buf);

    return data;
}

static Label *getRandomLabels(int num_examples,
			      int *num_pos_examples,
			      int *num_neg_examples) {
    Label *labels;
    int i, random_num;

    labels = malloc(sizeof(float) * num_examples);
    *num_pos_examples = 0;
    *num_neg_examples = 0;
    for (i = 0; i < num_examples; i++) {
	random_num = rand() % 2;
	if (random_num == 0) {
	    labels[i] = positive_label;
	    (*num_pos_examples)++;
	} else {
	    labels[i] = negative_label;
	    (*num_neg_examples)++;
	}
    }

    return labels;
}

static DataSet *getRandomDataSet(int num_examples, int num_features) {
    DataSet *ds;

    ds = malloc(sizeof(DataSet));
    ds->data = getRandomData(num_examples, num_features);
    ds->labels = getRandomLabels(num_examples,
				 &(ds->pos_examples_num),
				 &(ds->neg_examples_num));

    return ds;
}

const char *testCreateAdaBoost() {
    AdaBoost *ab;

    ab = createAdaBoost(10);

    mu_assert("AdaBoost == NULL", ab != NULL);

    deleteAdaBoost(ab);
    return 0;
}

const char *testTrainAdaBoost1() {
    AdaBoost *ab;
    DataSet *ds;

    system("rm -f " AB_STORAGE_FILE);
    ds = getRandomDataSet(50, 50);
    ab = createAdaBoost(3);

    trainAdaBoost(ab, ds);

    system("rm -f " AB_STORAGE_FILE);
    deleteDataSet(ds);
    deleteAdaBoost(ab);
    return 0;
}

const char *testTrainAdaBoost2() {
    AdaBoost *ab;
    DataSet *ds;

    system("rm -f " AB_STORAGE_FILE);
    ds = getRandomDataSet(2, 2);
    ab = createAdaBoost(10);

    trainAdaBoost(ab, ds);

    system("rm -f " AB_STORAGE_FILE);
    deleteDataSet(ds);
    deleteAdaBoost(ab);
    return 0;
}

// Checking that 2-feature AdaBoost performs
// better than any single RDS
const char *testClassifyDataWithAdaBoost() {
    AdaBoost *ab;
    DataSet *ds;
    RestrictedDecisionStump rds;
    Label ab_labels[100], rds_labels[100];
    int i, ab_error, rds_error;

    system("rm -f " AB_STORAGE_FILE);
    ds = getRandomDataSet(100, 100);
    ab = createAdaBoost(2);

    trainAdaBoost(ab, ds);
    classifyDataWithAdaBoost(ab, ds->data, ab_labels);
    ab_error = misclassifiedExamplesNum(ab_labels,
					ds->labels,
					getExamplesNum(ds));

    for (i = 0; i < 100; i++) {
	rds = createRds(i);
	trainRds(&rds, ds);
	classifyDataWithRds(&rds, ds->data, rds_labels);
	rds_error = misclassifiedExamplesNum(rds_labels,
					     ds->labels,
					     getExamplesNum(ds));
	mu_assert("RDS performs better than AB", rds_error >= ab_error);
    }
    system("rm -f " AB_STORAGE_FILE);
    deleteDataSet(ds);
    deleteAdaBoost(ab);

    return 0;
}
