#include "pgm_test.h"

#include <stdio.h>
#include <stdlib.h>

#include "minunit.h"
#include "test_utils.h"
#include "constants.h"
#include "pgm.h"
#include "utils.h"

const char *testReadImage() {
    PgmImage *image;

    image = readPgmImage(TEST_IMG);

    mu_assert("Error while reading image", image != NULL);
    mu_assert("Wrong width", image->width == 640);
    mu_assert("Wrong heigth", image->height == 426);

    deletePgmImage(image);
    return 0;
}

const char *testCreateImage() {
    PgmImage *image;

    image = createPgmImage(10, 20, 30);

    mu_assert("Error while creating image", image != NULL);
    mu_assert("Wrong created width", image->width == 10);
    mu_assert("Wrong created heigth", image->height == 20);

    deletePgmImage(image);
    return 0;
}

const char *testSaveImage() {
    PgmImage *image, *restored;
    char save_path[MAX_TEST_FILE_NAME_LENGTH];
    int i, j;

    getUniqueFileName("test_img", "pgm", save_path);
    image = readPgmImage(TEST_IMG);
    savePgmImage(save_path, image);

    mu_assert("Image was not saved", fileExists(save_path));

    restored = readPgmImage(save_path);

    mu_assert("Error while reading restored image", restored != NULL);
    mu_assert("Wrong restored width", image->width == restored->width);
    mu_assert("Wrong restored heigth", image->height == restored->height);

    for (i = 0; i < image->height; i++) {
	for (j = 0; j < image->width; j++) {
	    mu_assert("Wrong value of pixel", imgVal(image, i, j) == imgVal(restored, i, j));
	}
    }

    deletePgmImage(image);
    deletePgmImage(restored);
    return 0;
}

const char *testSubImage() {
    PgmImage *image, *sub_image;
    int x, y, w, h, i, i0, j, j0;

    x = 10;
    y = 20;
    w = 30;
    h = 40;

    image = readPgmImage(TEST_IMG);
    sub_image = subImage(image, x, y, w, h);

    mu_assert("Error while creating subimage", sub_image != NULL);
    mu_assert("Wrong subimage width", sub_image->width == w);
    mu_assert("Wrong subimage heigth", sub_image->height == h);

    for (i = y, i0 = 0; i < y + h; i++, i0++) {
	for (j = x, j0 = 0; j < x + w; j++, j0++) {
	    mu_assert("Wrong subimage pixel value",
		      imgVal(image, i, j) == imgVal(sub_image, i0, j0));
	}
    }
    deletePgmImage(image);
    deletePgmImage(sub_image);
    return 0;
}

const char *testToFloatMatrix() {
    PgmImage *image;
    FloatMatrix *fm;

    image = readPgmImage(TEST_IMG);
    fm = floatMatrixFromImage(image);

    mu_assert("Wrong mat rows", fm->rows == image->height);
    mu_assert("Wrong mat cols", fm->cols == image->width);
    mu_assert("Wrong mat data[3, 5]",
	      floatEqual(imgVal(image, 3, 5), matVal(fm, 3, 5)));
    mu_assert("Wrong mat cols[10, 20]",
	      floatEqual(imgVal(image, 10, 20), matVal(fm, 10, 20)));

    deletePgmImage(image);
    deleteFloatMatrix(fm);

    return 0;
}

