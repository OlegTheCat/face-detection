#pragma once

#define MAX_HAAR_RECTS 3

struct HaarRect {
    int x1;
    int y1;
    int x2;
    int y2;
    float wg;
};

typedef struct HaarRect HaarRect;

struct HaarFeature {
    HaarRect rects[MAX_HAAR_RECTS];
    int rects_count;
};

typedef struct HaarFeature HaarFeature;

void generateFeatures(int window_width, int window_height,
		      HaarFeature **features, int *num_features);
