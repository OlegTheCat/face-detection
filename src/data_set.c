#include "data_set.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void readImageList(const char *filename, PgmImage ***images) {
    FILE *file;
    char line[256];
    int images_num;
    int counter;

    printf("Reading images from list %s\n", filename);

    file = fopen(filename, "rt");
    fscanf(file, "%d", &images_num);
    printf("Number of images = %d\n", images_num);

    *images = malloc(sizeof(PgmImage *) * images_num);

    while(getc(file) != '\n');

    counter = 0;

    while (fgets(line, sizeof(line), file)) {
	str[strlen(str) - 1] = '\0';
	(*images)[counter++] = readPgmImage(line);
    }
}
