#include "haar_feature_test.h"

#include <stdlib.h>
#include <stdio.h>

#include "minunit.h"
#include "haar_feature.h"
#include "img_proc.h"
#include "utils.h"

static FloatMatrix *getFloatMatrix() {
    FloatMatrix *fm;

    fm = createFloatMatrix(4, 4);

    setMatVal(fm, 0, 0, 1);
    setMatVal(fm, 0, 1, 2);
    setMatVal(fm, 0, 2, 3);
    setMatVal(fm, 0, 3, 4);
    setMatVal(fm, 1, 0, 5);
    setMatVal(fm, 1, 1, 6);
    setMatVal(fm, 1, 2, 7);
    setMatVal(fm, 1, 3, 8);
    setMatVal(fm, 2, 0, 9);
    setMatVal(fm, 2, 1, 10);
    setMatVal(fm, 2, 2, 11);
    setMatVal(fm, 2, 3, 12);
    setMatVal(fm, 3, 0, 13);
    setMatVal(fm, 3, 1, 14);
    setMatVal(fm, 3, 2, 15);
    setMatVal(fm, 3, 3, 16);

    return fm;
}

const char *testCreateFeature() {
    HaarRect hr;

    hr = createHaarRect(5, 1, 1, 2, 2, 1);
    createHaarFeature2(hr, hr);
    createHaarFeature3(hr, hr, hr);
    createHaarFeatureFromCoords2(1,
				 1, 1, 2, 2, 1,
				 1, 1, 2, 2, 1);
    createHaarFeatureFromCoords3(1,
				 1, 1, 2, 2, 1,
				 1, 1, 2, 2, 1,
				 1, 1, 2, 2, 1);
    return 0;
}

void printFloatMatrix(FloatMatrix *fm) {
    int i, j;
    for (i = 0; i < fm->rows; i++) {
	for (j = 0; j < fm->cols; j++) {
	    printf("%9.2f", matVal(fm, i, j));
	}
	printf("\n");
    }
}

const char *testEvaluateFeature() {
    FloatMatrix *fm, *ii;
    HaarFeature hf1, hf2;

    fm = getFloatMatrix();
    ii = computeII(fm, 0);

    hf1 = createHaarFeatureFromCoords2(ii->cols,
				       0, 0, 4, 0, 1,
				       0, 0, 0, 4, 1);

    hf2 = createHaarFeatureFromCoords2(ii->cols,
				       0, 0, 4, 4, 1,
				       1, 1, 2, 1, -1);
    /* printFloatMatrix(fm); */
    /* printf("\n"); */
    /* printFloatMatrix(ii); */

    /* printf("hf1 value = %f\n", evaluateFeature(&hf1, ii)); */
    /* printf("hf2 value = %f\n", evaluateFeature(&hf2, ii)); */
    mu_assert("Wrong value of hf1",
	      floatEqual(evaluateFeature(&hf1, ii), 0));
    mu_assert("Wrong value of hf2",
	      floatEqual(evaluateFeature(&hf2, ii), 136 - 13));

    deleteFloatMatrix(fm);
    deleteFloatMatrix(ii);

    return 0;
}

const char *testGenerateHaarFeatures() {
    HaarFeature *features;
    int num_features;
    int w, h;

    w = 3;
    h = 3;

    generateHaarFeatures(w, h, &features, &num_features);
    /* printf("3x3 num_features = %d\n", num_features); */
    mu_assert("num_features == 0 for 3x3 window", num_features != 0);
    free(features);

    w = 24;
    h = 24;

    generateHaarFeatures(w, h, &features, &num_features);
    /* printf("24x24 num_features = %d\n", num_features); */
    mu_assert("num_features == 0 for 24x24 window", num_features != 0);

    free(features);
    return 0;
}
