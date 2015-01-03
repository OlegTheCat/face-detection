#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"

PgmImage *readPgmImage (const char *filename) {

    FILE *in_file;
    char ch;
    int row, col, type;
    /* int ch_int; */
    PgmImage *img;

    img = malloc(sizeof(PgmImage));

    in_file = fopen(filename, "r");
    if (in_file == NULL) {
	fprintf(stderr, "Error: Unable to open file %s\n\n", filename);
	exit(8);
    }

    printf("\nReading image file: %s\n", filename);

    ch = getc(in_file);
    if(ch != 'P') {
	printf("ERROR(1): Not valid pgm/ppm file type\n");
	exit(1);
    }

    ch = getc(in_file);
    type = ch - 48;

    if(type != 5) {
	printf("ERROR(2): Not valid pgm/ppm file type\n");
	exit(1);
    }

    while(getc(in_file) != '\n');

    while (getc(in_file) == '#') {
	while (getc(in_file) != '\n');
    }

    /*there seems to be a difference between color and b/w.  This line is needed
      by b/w but doesn't effect color reading...*/
    fseek(in_file, -1, SEEK_CUR);             /* backup one character*/

    fscanf(in_file,"%d", &((*img).width));
    fscanf(in_file,"%d", &((*img).height));
    fscanf(in_file,"%d", &((*img).maxVal));

    printf("\n width  = %d",(*img).width);
    printf("\n height = %d",(*img).height);
    printf("\n maxVal = %d",(*img).maxVal);
    printf("\n type = %d", type);
    printf("\n");

    img->data = malloc(img->width * img->height * sizeof(int));

    if (type == 5) {
	while(getc(in_file) != '\n');

	for (row = (*img).height - 1; row >= 0; row--)
	    for (col = 0; col < (*img).width; col++) {
		(*img).data[row * img->width + col] = (int)getc(in_file);
	    }
    }

    fclose(in_file);
    printf("\nDone reading file.\n");

    return img;
}


void savePgmImage(const char * filename, PgmImage *img)
{
    int i, j, nr, nc;
    FILE *iop;

    nr = img->height;
    nc = img->width;

    iop = fopen(filename, "w");
    fprintf(iop, "P5\n");
    fprintf(iop, "%d %d\n", nc, nr);
    fprintf(iop, "%d\n", img->maxVal);

    for(i = nr - 1; i >= 0; i--) {
	for(j = 0; j < nc; j++) {
	    putc(img->data[i * nc + j], iop);
	}
    }
    fprintf(iop, "\n");
    fclose(iop);
}

void deletePgmImage(PgmImage *img) {
    if (img->data != NULL) free(img->data);
    if (img->data != NULL) free(img);
}
