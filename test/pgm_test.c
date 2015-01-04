#include "pgm_test.h"

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "pgm.h"
#include "minunit.h"
#include "utils.h"

char *testReadImage() {
    PgmImage *image;

    image = readPgmImage(TEST_IMG);

    mu_assert("Error while reading image", image != NULL);
    mu_assert("Wrong width", image->width == 640);
    mu_assert("Wrong heigth", image->height == 426);

    deletePgmImage(image);
    return 0;
}

char *testCreateImage() {
    PgmImage *image;

    image = createPgmImage(10, 20, 30);

    mu_assert("Error while creating image", image != NULL);
    mu_assert("Wrong created width", image->width == 10);
    mu_assert("Wrong created heigth", image->height == 20);

    deletePgmImage(image);
    return 0;
}

char *testSaveImage() {
    PgmImage *image, *restored;
    const char *save_path = "test_save.pgm";
    int i, j;

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

    system("rm test_save.pgm");
    deletePgmImage(image);
    deletePgmImage(restored);
    return 0;
}

char *testSubImage() {
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