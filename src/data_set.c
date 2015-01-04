#include "data_set.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "rect.h"
#include "utils.h"

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
	line[strlen(line) - 1] = '\0';
	(*images)[counter++] = readPgmImage(line);
    }
}

void subSampleImage(PgmImage *image, PgmImage ***samples,
		    int sample_width, int sample_height,
		    int sample_step_x, int sample_step_y) {

    PointerArray samples_array;
    int x, y;

    if (image->width <= sample_width || image->height <= sample_height) {
	*samples = NULL;
	return;
    }

    samples_array = createPointerArray();

    for (y = 0; y + sample_height < image->height; y += sample_step_y) {
	for (x = 0; x + sample_width < image->width; x += sample_step_x) {
	    PgmImage *sample = subImage(image,
					x,
					y,
					sample_width,
					sample_height);

	    addToPointerArray(&samples_array, sample);
	}
    }


    *samples = (PgmImage **)samples_array.elements;
}
