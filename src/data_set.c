#include "data_set.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "img_proc.h"
#include "haar_feature.h"
#include "persistent_float_matrix.h"

DataSet *createDataSet(const char *pos_list,
		       const char *neg_list,
		       int img_width,
		       int img_height,
		       const char *storage_file) {
    DataSet *ds;
    Label *labels;
    PgmImage **pos_images, **neg_images;
    FloatMatrix *mat, **iis;
    float *feature_vals;
    PersistentFloatMatrix *pfm;
    HaarFeature *features;
    int i, j, num_pos_images, num_neg_images,
	num_total_images, num_features;

    readImageList(pos_list, &pos_images, &num_pos_images);
    readImageList(neg_list, &neg_images, &num_neg_images);
    num_total_images = num_pos_images + num_neg_images;
    labels = malloc(sizeof(Label) * num_total_images);
    iis = malloc(sizeof(FloatMatrix *) * num_total_images);

    for (i = 0; i < num_pos_images; i++) {
	mat = floatMatrixFromImage(pos_images[i]);
	convertToNormalized(mat);
	iis[i] = computeII(mat, 0);
	labels[i] = positive_label;
	deleteFloatMatrix(mat);
    }

    for (i = 0; i < num_neg_images; i++) {
	mat = floatMatrixFromImage(neg_images[i]);
	convertToNormalized(mat);
	iis[i + num_pos_images] = computeII(mat, 0);
	labels[i + num_pos_images] = negative_label;
	deleteFloatMatrix(mat);
    }

    // Increasing width and height by 1 due to dealing
    // with IIs, not input images
    generateHaarFeatures(img_width + 1, img_height + 1,
			 &features, &num_features);

    pfm = createPfm(storage_file,
		    num_total_images,
		    num_features);

    feature_vals = malloc(sizeof(float) * num_total_images);
    for (i = 0; i < num_features; i++) {
	for (j = 0; j < num_total_images; j++) {
	    feature_vals[j] = evaluateFeature(&features[i],
					      iis[j]);
	}
	storePfmCol(pfm, feature_vals, i);
    }

    ds = malloc(sizeof(DataSet));

    ds->labels = labels;
    ds->data = pfm;
    ds->pos_examples_num = num_pos_images;
    ds->neg_examples_num = num_neg_images;

    for (i = 0; i < num_pos_images; i++) {
	deletePgmImage(pos_images[i]);
    }
    free(pos_images);
    for (i = 0; i < num_neg_images; i++) {
	deletePgmImage(neg_images[i]);
    }
    free(neg_images);
    for (i = 0; i < num_total_images; i++) {
	deleteFloatMatrix(iis[i]);
    }
    free(iis);
    free(features);
    free(feature_vals);

    return ds;
}

void getFeatureVals(DataSet *ds, float *vals, int feature_idx) {
    getPfmCol(ds->data, vals, feature_idx);
}

int getExamplesNum(DataSet *ds) {
    return ds->data->rows;
}

int getPosExamplesNum(DataSet *ds) {
    return ds->pos_examples_num;
}

int getNegExamplesNum(DataSet *ds) {
    return ds->neg_examples_num;
}

int getFeaturesNum(DataSet *ds) {
    return ds->data->cols;
}

void deleteDataSet(DataSet *ds) {
    if (ds != NULL) {
	if (ds->labels != NULL) free(ds->labels);
	if (ds->data != NULL) deletePfm(ds->data);
	free(ds);
    }
}


void readImageList(const char *filename,
		   PgmImage ***images,
		   int *images_count) {
    FILE *file;
    char line[256];
    int images_num;
    int counter;

    /* printf("Reading images from list %s\n", filename); */

    file = fopen(filename, "rt");
    if (file == NULL) {
	printf("Cannot find file list %s\n", filename);
	if (images != NULL) *images = NULL;
	if (images != NULL) *images_count = -1;
	return;
    }

    fscanf(file, "%d", &images_num);
    /* printf("Number of images = %d\n", images_num); */

    if (images_count != NULL) *images_count = images_num;
    if (images != NULL) {
	*images = malloc(sizeof(PgmImage *) * images_num);

	while(getc(file) != '\n');

	counter = 0;

	while (fgets(line, sizeof(line), file)) {
	    line[strlen(line) - 1] = '\0';
	    (*images)[counter++] = readPgmImage(line);
	}
    }

    fclose(file);
}

void subSampleImage(const PgmImage *image, PgmImage ***samples,
		    int sample_width, int sample_height,
		    int sample_step_x, int sample_step_y,
		    int *samples_count) {

    ArrayList samples_array;
    PgmImage *sample;
    int x, y;

    if (image->width <= sample_width || image->height <= sample_height) {
	if (samples != NULL) *samples = NULL;
	if (samples_count != NULL) *samples_count = 0;
	return;
    }

    samples_array = createArrayList(sizeof(PgmImage *));

    for (y = 0; y + sample_height < image->height; y += sample_step_y) {
	for (x = 0; x + sample_width < image->width; x += sample_step_x) {
	    sample = subImage(image,
			      x,
			      y,
			      sample_width,
			      sample_height);

	    addToArrayList(&samples_array, &sample);
	}
    }

    if (samples != NULL) *samples = (PgmImage **)rawArrayFromArrayList(&samples_array);
    if (samples_count != NULL) *samples_count = samples_array.size;
    deleteArrayList(&samples_array);
}
