#include "haar_feature.h"

#include "array_list.h"

HaarFeature createHaarFeature2(HaarRect hr1, HaarRect hr2) {
    HaarFeature hf;

    hf.rects[0] = hr1;
    hf.rects[1] = hr2;
    hf.rects_count = 2;

    return hf;
}

HaarFeature createHaarFeature3(HaarRect hr1, HaarRect hr2, HaarRect hr3) {
    HaarFeature hf;

    hf.rects[0] = hr1;
    hf.rects[1] = hr2;
    hf.rects[2] = hr3;
    hf.rects_count = 3;

    return hf;
}

HaarFeature createHaarFeatureFromCoords2(int offset,
					 int x1, int y1, int w1, int h1, float wg1,
					 int x2, int y2, int w2, int h2, float wg2) {
    return createHaarFeature2(createHaarRect(offset, x1, y1, w1, h1, wg1),
			      createHaarRect(offset, x2, y2, w2, h2, wg2));
}

HaarFeature createHaarFeatureFromCoords3(int offset,
					 int x1, int y1, int w1, int h1, float wg1,
					 int x2, int y2, int w2, int h2, float wg2,
					 int x3, int y3, int w3, int h3, float wg3) {
    return createHaarFeature3(createHaarRect(offset, x1, y1, w1, h1, wg1),
			      createHaarRect(offset, x2, y2, w2, h2, wg2),
			      createHaarRect(offset, x3, y3, w3, h3, wg3));
}

HaarRect createHaarRect(int offset, int x, int y, int w, int h, float wg) {
    HaarRect hr;

    hr.p1 = x + y * offset;
    hr.p2 = x + w + y * offset;
    hr.p3 = x + (y + h) * offset;
    hr.p4 = x + w + (y + h) * offset;
    hr.wg = wg;

    return hr;
}

float evaluateFeature(const HaarFeature *hf, const FloatMatrix *fm) {
    int i;
    float value;
    const float *data;

    value = 0;
    data = fm->data;
    for (i = 0; i < hf->rects_count; i++) {
	value += (data[hf->rects[i].p1] +
		  data[hf->rects[i].p4] -
		  data[hf->rects[i].p3] -
		  data[hf->rects[i].p2]) * hf->rects[i].wg;
    }

    return value;
}

void generateHaarFeatures(int window_width, int window_height,
		      HaarFeature **features, int *num_features) {
    ArrayList features_array;
    int x, y, dx, dy;
    HaarFeature feature;

    features_array = createArrayList(sizeof(HaarFeature));

    for (x = 0; x < window_width; x++) {
        for (y = 0; y < window_height; y++) {
            for (dx = 1; dx < window_width; dx++) {
                for (dy = 1; dy < window_height; dy++) {
                    // haar_x2
                    if ((x + dx * 2 < window_width) && (y + dy < window_height)) {
                        feature = createHaarFeatureFromCoords2(window_width,
							       x, y, dx * 2, dy, -1,
							       x + dx, y, dx, dy, +2);

			addToArrayList(&features_array, &feature);
                    }
                    // haar_y2
                    if ((x + dx < window_width) && (y + dy * 2 < window_height)) {
                        feature = createHaarFeatureFromCoords2(window_width,
							       x, y, dx, dy * 2, -1,
							       x, y + dy, dx, dy, +2);
			addToArrayList(&features_array, &feature);
                    }
                    // haar_x3
                    if ((x + dx * 3 < window_width) && (y + dy < window_height)) {
                        feature = createHaarFeatureFromCoords2(window_width,
							       x, y, dx * 3, dy, -1,
							       x + dx, y, dx, dy, +3);
			addToArrayList(&features_array, &feature);
                    }
                    // haar_y3
                    if ((x + dx < window_width) && (y + dy * 3 < window_height)) {
                        feature = createHaarFeatureFromCoords2(window_width,
							       x, y, dx, dy * 3, -1,
							       x, y + dy, dx, dy, +3);
			addToArrayList(&features_array, &feature);
                    }

                    // x2_y2
                    if ((x + dx * 2 < window_width) && (y + dy * 2 < window_height)) {
                        feature = createHaarFeatureFromCoords3(window_width,
							       x, y, dx * 2, dy * 2, -1,
							       x, y, dx, dy, +2,
							       x + dx, y + dy, dx, dy, +2);
			addToArrayList(&features_array, &feature);
                    }
                }
            }
        }
    }

    *features = rawArrayFromArrayList(&features_array);
    *num_features = features_array.size;

    deleteArrayList(&features_array);
}
