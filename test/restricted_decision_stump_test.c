#include "restricted_decision_stump_test.h"

#include "minunit.h"
#include "restricted_decision_stump.h"
#include "persistent_float_matrix.h"
#include "utils.h"

#define STORAGE_FILE "rds_ds.storage"

static PersistentFloatMatrix *getDataForDataSet() {
    PersistentFloatMatrix *data;
    float buf[10];
    int i;

    for (i = 0; i < 10; i++) {
	buf[i] = (float)(10 - i);
    }

    data = createPfm(STORAGE_FILE, 10, 20);

    for (i = 0; i < 20; i++) {
	storePfmCol(data, buf, i);
    }

    return data;

}

static Label *getLabels1ForDataSet(int *num_pos_examples, int *num_neg_examples) {
    Label *labels;

    labels = malloc(sizeof(Label) * 10);

    labels[0] = positive_label;
    labels[1] = positive_label;
    labels[2] = positive_label;
    labels[3] = positive_label;
    labels[4] = positive_label;
    labels[5] = positive_label;
    labels[6] = negative_label;
    labels[7] = negative_label;
    labels[8] = negative_label;
    labels[9] = negative_label;

    *num_pos_examples = 6;
    *num_neg_examples = 4;

    return labels;
}

static DataSet *getDataSet1() {
    DataSet *ds;
    int pos_examples_num, neg_examples_num;

    ds = malloc(sizeof(DataSet));

    ds->data = getDataForDataSet();
    ds->labels = getLabels1ForDataSet(&pos_examples_num, &neg_examples_num);
    ds->pos_examples_num = pos_examples_num;
    ds->neg_examples_num = neg_examples_num;

    return ds;
}

static Label *getLabels2ForDataSet(int *num_pos_examples, int *num_neg_examples) {
    Label *labels;

    labels = malloc(sizeof(Label) * 10);

    labels[0] = positive_label;
    labels[1] = positive_label;
    labels[2] = negative_label;
    labels[3] = negative_label;
    labels[4] = negative_label;
    labels[5] = positive_label;
    labels[6] = positive_label;
    labels[7] = positive_label;
    labels[8] = positive_label;
    labels[9] = positive_label;

    *num_pos_examples = 6;
    *num_neg_examples = 4;

    return labels;
}

static DataSet *getDataSet2() {
    DataSet *ds;
    int pos_examples_num, neg_examples_num;

    ds = malloc(sizeof(DataSet));

    ds->data = getDataForDataSet();
    ds->labels = getLabels2ForDataSet(&pos_examples_num, &neg_examples_num);
    ds->pos_examples_num = pos_examples_num;
    ds->neg_examples_num = neg_examples_num;

    return ds;
}

static Label *getLabels3ForDataSet(int *num_pos_examples, int *num_neg_examples) {
    Label *labels;

    labels = malloc(sizeof(Label) * 10);

    labels[0] = positive_label;
    labels[1] = positive_label;
    labels[2] = positive_label;
    labels[3] = positive_label;
    labels[4] = positive_label;
    labels[5] = positive_label;
    labels[6] = positive_label;
    labels[7] = positive_label;
    labels[8] = positive_label;
    labels[9] = positive_label;

    *num_pos_examples = 6;
    *num_neg_examples = 4;

    return labels;
}

static DataSet *getDataSet3() {
    DataSet *ds;
    int pos_examples_num, neg_examples_num;

    ds = malloc(sizeof(DataSet));

    ds->data = getDataForDataSet();
    ds->labels = getLabels3ForDataSet(&pos_examples_num, &neg_examples_num);
    ds->pos_examples_num = pos_examples_num;
    ds->neg_examples_num = neg_examples_num;

    return ds;
}

static Label *getLabels4ForDataSet(int *num_pos_examples, int *num_neg_examples) {
    Label *labels;

    labels = malloc(sizeof(Label) * 10);

    // Multiple splits
    labels[0] = positive_label;
    labels[1] = positive_label;
    labels[2] = negative_label;
    labels[3] = positive_label;
    labels[4] = positive_label;
    labels[5] = negative_label;
    labels[6] = negative_label;
    labels[7] = positive_label;
    labels[8] = positive_label;
    labels[9] = positive_label;

    *num_pos_examples = 6;
    *num_neg_examples = 4;

    return labels;
}

static DataSet *getDataSet4() {
    DataSet *ds;
    int pos_examples_num, neg_examples_num;

    ds = malloc(sizeof(DataSet));

    ds->data = getDataForDataSet();
    ds->labels = getLabels4ForDataSet(&pos_examples_num, &neg_examples_num);
    ds->pos_examples_num = pos_examples_num;
    ds->neg_examples_num = neg_examples_num;

    return ds;
}


const char *testTrainRds1() {
    RestrictedDecisionStump rds;
    DataSet *ds;

    system("rm -f " STORAGE_FILE);

    rds = createRds(5);

    ds = getDataSet1();
    trainRds(&rds, ds);

    mu_assert("Wrong threshold1", floatEqual(rds.threshold, 4.5));
    mu_assert("Wrong left_val1", rds.left_val == negative_label);
    mu_assert("Wrong right_val1", rds.right_val == positive_label);

    deleteDataSet(ds);
    system("rm -f " STORAGE_FILE);

    return 0;
}

const char *testTrainRds2() {
    RestrictedDecisionStump rds;
    DataSet *ds;

    system("rm -f " STORAGE_FILE);

    rds = createRds(5);
    ds = getDataSet2();
    trainRds(&rds, ds);

    mu_assert("Wrong threshold2", floatEqual(rds.threshold, 5.5));
    mu_assert("Wrong left_val2", rds.left_val == positive_label);
    mu_assert("Wrong right_val2", rds.right_val == negative_label);

    deleteDataSet(ds);
    system("rm -f " STORAGE_FILE);

    return 0;
}

const char *testTrainRds3() {
    RestrictedDecisionStump rds;
    DataSet *ds;

    system("rm -f " STORAGE_FILE);

    rds = createRds(5);
    ds = getDataSet3();
    trainRds(&rds, ds);

    mu_assert("Wrong left_val3", rds.left_val == positive_label);
    mu_assert("Wrong right_val3", rds.right_val == positive_label);

    deleteDataSet(ds);
    system("rm -f " STORAGE_FILE);

    return 0;
}

const char *testTrainRds4() {
    RestrictedDecisionStump rds;
    DataSet *ds;

    system("rm -f " STORAGE_FILE);

    rds = createRds(5);
    ds = getDataSet4();
    // Just checking for memory errors
    trainRds(&rds, ds);

    /* mu_assert("Wrong threshold4", floatEqual(rds.threshold, 5.5)); */
    /* mu_assert("Wrong left_val4", rds.left_val == positive_label); */
    /* mu_assert("Wrong right_val4", rds.right_val == positive_label); */

    deleteDataSet(ds);
    system("rm -f " STORAGE_FILE);

    return 0;
}

const char *testClassifyDataWithRds() {
    RestrictedDecisionStump rds;
    DataSet *ds;
    Label res_labels[10];
    int i;

    system("rm -f " STORAGE_FILE);

    rds = createRds(5);

    ds = getDataSet1();
    trainRds(&rds, ds);

    classifyDataWithRds(&rds, ds->data, res_labels);

    for (i = 0; i < 10; i++) {
	mu_assert("Wrong res label", res_labels[i] == ds->labels[i]);
    }

    deleteDataSet(ds);
    system("rm -f " STORAGE_FILE);


    ds = getDataSet3();
    trainRds(&rds, ds);

    classifyDataWithRds(&rds, ds->data, res_labels);

    for (i = 0; i < 10; i++) {
	mu_assert("Wrong res label", res_labels[i] == ds->labels[i]);
    }

    deleteDataSet(ds);
    system("rm -f " STORAGE_FILE);

    return 0;
}
