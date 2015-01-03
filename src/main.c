#include <stdio.h>
#include "constants.h"
#include "pgm.h"

int main() {
    PgmImage *image;
    image = readPgmImage(TEST_IMG);

    savePgmImage("out.pgm", image);
    return 0;
}
