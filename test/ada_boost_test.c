#include "ada_boost_test.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "minunit.h"
#include "test_utils.h"
#include "ada_boost.h"
#include "data_set.h"
#include "persistent_float_matrix.h"

static Pfm *getRandomData(int rows, int cols) {
    Pfm *data;

    WITH_TEST_FILE_NAME(data = createPfm(FILE_NAME_HANDLE, rows, cols));

    fillPfmWithRandomData(data);
    return data;
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

    ds = getRandomDataSet(50, 50);
    ab = createAdaBoost(3);

    trainAdaBoost(ab, ds);

    deleteDataSet(ds);
    deleteAdaBoost(ab);
    return 0;
}

const char *testTrainAdaBoost2() {
    AdaBoost *ab;
    DataSet *ds;

    ds = getRandomDataSet(2, 2);
    ab = createAdaBoost(10);

    trainAdaBoost(ab, ds);

    deleteDataSet(ds);
    deleteAdaBoost(ab);
    return 0;
}

// Checking that 2-feature AdaBoost performs
// better than any single RDS
// UPD: Seems that can be not true in some cases
//      (especially when data set is randomly generated)
//      e.g. srand(1421013563)
const char *testClassifyDataWithAdaBoost() {
    AdaBoost *ab;
    DataSet *ds;
    RestrictedDecisionStump rds;
    Label ab_labels[150];
    Label rds_labels[150];
    int i, ab_error, rds_error;

    ds = getRandomDataSet(150, 100);
    ab = createAdaBoost(30);

    trainAdaBoost(ab, ds);
    classifyDataWithAdaBoost(ab, ds->data, ab_labels);
    ab_error = misclassifiedExamplesNum(ab_labels,
					ds->labels,
					getExamplesNum(ds));
    /* printf("ab error = %d\n", ab_error); */

    for (i = 0; i < 100; i++) {
	/* printf("checking rds #%d\n", i); */
	rds = createRds(i);
	trainRds(&rds, ds);
	classifyDataWithRds(&rds, ds->data, rds_labels);
	rds_error = misclassifiedExamplesNum(rds_labels,
					     ds->labels,
					     getExamplesNum(ds));
	(void)rds_error;
	/* printf("rds error = %d\n", rds_error); */

	mu_assert("RDS performs better than AB", rds_error >= ab_error);
    }

    deleteDataSet(ds);
    deleteAdaBoost(ab);

    return 0;
}

// Overtraining case
const char *testClassifyDataWithAdaBoost2() {
    AdaBoost *ab;
    DataSet *ds;
    Label res_labels[2];
    int i;

    ds = getRandomDataSet(2, 2);
    ds->labels[0] = positive_label;
    ds->labels[1] = negative_label;
    ds->pos_examples_num = 1;
    ds->neg_examples_num = 1;

    ab = createAdaBoost(100);

    trainAdaBoost(ab, ds);
    classifyDataWithAdaBoost(ab, ds->data, res_labels);

    for (i = 0; i < 2; i++) {
	mu_assert("Wrong res_labels value", res_labels[i] == ds->labels[i]);
    }

    deleteDataSet(ds);
    deleteAdaBoost(ab);

    return 0;
}
