#pragma once

#include "pgm.h"
#include "array_list.h"

struct PersistentFloatMatrix;

typedef enum {
    positive_label,
    negative_label
} Label;

struct DataSet {
    struct PersistentFloatMatrix *data;
    int pos_examples_num;
    int neg_examples_num;
    Label *labels;
};

typedef struct DataSet DataSet;

DataSet *createDataSet(const char *pos_list,
		       const char *neg_list,
		       int img_width,
		       int img_height,
		       const char *storage_file);

int getExamplesNum(DataSet *ds);
int getPosExamplesNum(DataSet *ds);
int getNegExamplesNum(DataSet *ds);
int getFeaturesNum(DataSet *ds);
void getFeatureVals(DataSet *ds, float *vals, int feature_idx);

void deleteDataSet(DataSet *ds);

void readImageList(const char *filename,
		   PgmImage ***images,
		   int *images_count);

void subSampleImage(const PgmImage *image, PgmImage ***samples,
		    int sample_width, int sample_heigth,
		    int sample_step_x, int sample_step_y,
		    int *samples_count);
