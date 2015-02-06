#include "test_utils.h"

#include <unistd.h>
#include <stdio.h>

#include "persistent_float_matrix.h"
#include "data_set.h"
#include "pfm_distributed_file_impl.h"

int getUniqueFileIndex() {
    static int i = 0;
    return (int)getpid() + i++;
}

char *getTestFileName(char *result) {
    return getUniqueFileName("test", "data", result);
}

char *getUniqueFileName(const char *prefix, const char *suffix, char *result) {
    sprintf(result, "%s%d.%s", prefix, getUniqueFileIndex(), suffix);

    return result;
}

void fillPfmWithRandomData(Pfm *pfm) {
    float *buf;
    int i, j;

    buf = malloc(sizeof(float) * pfm->rows);

    for (i = 0; i < pfm->cols; i++) {
	for (j = 0; j < pfm->rows; j++) {
	    buf[j] = randomFloat();
	}
	storePfmCol(pfm, buf, i);
    }

    free(buf);
}

Label *getRandomLabels(int num_examples,
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

float randomFloat() {
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

static Pfm *getRandomData(int rows, int cols) {
    Pfm *data;

    WITH_TEST_FILE_NAME(data = createPfm(FILE_NAME_HANDLE, rows, cols));

    fillPfmWithRandomData(data);
    return data;
}

DataSet *getRandomDataSet(int num_examples, int num_features) {
    DataSet *ds;

    ds = malloc(sizeof(DataSet));
    ds->data = getRandomData(num_examples, num_features);
    ds->labels = getRandomLabels(num_examples,
				 &(ds->pos_examples_num),
				 &(ds->neg_examples_num));

    return ds;
}

DataSet *getDistributedRandomDataSet(int num_examples, int num_features) {
    DataSet *ds;

    ds = malloc(sizeof(DataSet));
    WITH_TEST_FILE_NAME
	(ds->data = createPfmWithImpl(FILE_NAME_HANDLE,
				      num_examples, num_features,
				      createPfmDistributedFileImpl(FILE_NAME_HANDLE,
								   num_examples, num_features)));

    fillPfmWithRandomData(ds->data);
    ds->labels = getRandomLabels(num_examples,
				 &(ds->pos_examples_num),
				 &(ds->neg_examples_num));


    return ds;
}
