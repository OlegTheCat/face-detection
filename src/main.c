#include <stdio.h>
#include "constants.h"
#include "pgm.h"
#include "data_set.h"

int main() {
    PgmImage *image;
    image = readPgmImage(TEST_IMG);

    savePgmImage("out.pgm", image);


    PgmImage **images;

    readImageList(NEG_LIST, &images);

    savePgmImage("out2.pgm", images[3]);
    return 0;
}
