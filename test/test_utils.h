#pragma once

/* #define UNIQUE_FILE(prefix, suffix) \ */
/*     (prefix) ## __COUNTER__ ## "." ## (suffix) */
/* #define TEST_FILE UNIQUE_FILE("test", "data") */

#define MAX_TEST_FILE_NAME_LENGTH 350

#define FILE_NAME_HANDLE __file_name__

#define WITH_TEST_FILE_NAME(stmt)				\
    do {							\
	char FILE_NAME_HANDLE[MAX_TEST_FILE_NAME_LENGTH];	\
	getTestFileName(FILE_NAME_HANDLE);			\
	(stmt);							\
    } while (0)

char *getTestFileName(char *result);
char *getUniqueFileName(const char *prefix, const char *suffix, char *result);

struct PersistentFloatMatrix;
enum Label;

void fillPfmWithRandomData(struct PersistentFloatMatrix *pfm);
struct DataSet *getRandomDataSet(int num_examples, int num_features);
struct DataSet *getDistributedRandomDataSet(int num_examples, int num_features);
enum Label *getRandomLabels(int num_examples,
			    int *num_pos_examples,
			    int *num_neg_examples);

float randomFloat();
int misclassifiedExamplesNum(const enum Label *res_labels,
			     const enum Label *labels,
			     int num_examples);
