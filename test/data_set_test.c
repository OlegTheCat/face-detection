#include "data_set_test.h"

#include <stdlib.h>

#include "minunit.h"
#include "data_set.h"
#include "constants.h"

char *testReadImageList() {
    PgmImage **images, *image1, *image2;
    int i, images_count;

    readImageList(TEST_NEG_LIST, &images, &images_count);
    image1 = readPgmImage("face-detection-data/neg/neg-0002.pgm");
    image2 = readPgmImage("face-detection-data/neg/neg-0012.pgm");
    mu_assert("Wrong images count", images_count == 13);
    mu_assert("Wrong image at position 0",
	      pgmImagesEquals(images[0], image1));
    mu_assert("Wrong image at position 9",
	      pgmImagesEquals(images[9], image2));

    deletePgmImage(image1);
    deletePgmImage(image2);
    for (i = 0; i < images_count; i++) {
	deletePgmImage(images[i]);
    }
    free(images);
    return 0;
}

char *testSubSampleImage() {
    PgmImage *image, **samples;
    int i, samples_count;

    image = readPgmImage(TEST_IMG);
    subSampleImage(image, &samples, 200, 200, 100, 100, &samples_count);

    mu_assert("Wrong samples count", samples_count == 15);

    deletePgmImage(image);
    for (i = 0; i < samples_count; i++) {
	deletePgmImage(samples[i]);
    }
    free(samples);
    return 0;
}
