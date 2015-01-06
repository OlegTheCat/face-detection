#pragma once

#define MAX_HAAR_RECTS 3

#include "float_matrix.h"

struct HaarRect {
    int p1;
    int p2;
    int p3;
    int p4;
    float wg;
};

typedef struct HaarRect HaarRect;

struct HaarFeature {
    HaarRect rects[MAX_HAAR_RECTS];
    int rects_count;
};

typedef struct HaarFeature HaarFeature;

HaarFeature createHaarFeature2(HaarRect hr1, HaarRect hr2);
HaarFeature createHaarFeature3(HaarRect hr1, HaarRect hr2, HaarRect hr3);
HaarFeature createHaarFeatureFromCoords2(int offset,
					 int x1, int y1, int w1, int h1, float wg1,
					 int x2, int y2, int w2, int h2, float wg2);
HaarFeature createHaarFeatureFromCoords3(int offset,
					 int x1, int y1, int w1, int h1, float wg1,
					 int x2, int y2, int w2, int h2, float wg2,
					 int x3, int y3, int w3, int h3, float wg3);
HaarRect createHaarRect(int offset, int x, int y, int w, int h, float wg);

float evaluateFeature(const HaarFeature *hf, const FloatMatrix *fm);

void generateHaarFeatures(int window_width, int window_height,
			  HaarFeature **features, int *num_features);


