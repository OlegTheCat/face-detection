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
    Label *labels;
    ArrayList disqualified_rows;
};

typedef struct DataSet DataSet;

DataSet *createDataSet(const char *pos_list,
		       const char *neg_list,
		       int img_width,
		       int img_height);
inline int getExamplesNum(DataSet *ds);
inline int getFeaturesNum(DataSet *ds);
void deleteDataSet(DataSet *ds);

void readImageList(const char *filename,
		   PgmImage ***images,
		   int *images_count);

void subSampleImage(const PgmImage *image, PgmImage ***samples,
		    int sample_width, int sample_heigth,
		    int sample_step_x, int sample_step_y,
		    int *samples_count);
