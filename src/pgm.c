#include "pgm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    /* printf("\nReading image file: %s\n", filename); */

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

    /* printf("\n width  = %d",(*img).width); */
    /* printf("\n height = %d",(*img).height); */
    /* printf("\n maxVal = %d",(*img).maxVal); */
    /* printf("\n type = %d", type); */
    /* printf("\n"); */

    img->data = malloc(img->width * img->height * sizeof(int));

    if (type == 5) {
	while(getc(in_file) != '\n');

	for (row = (*img).height - 1; row >= 0; row--)
	    for (col = 0; col < (*img).width; col++) {
		(*img).data[row * img->width + col] = (int)getc(in_file);
	    }
    }

    fclose(in_file);
    /* printf("\nDone reading file.\n"); */

    return img;
}


void savePgmImage(const char * filename, const PgmImage *img)
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
    if (img != NULL) {
	if (img->data != NULL) free(img->data);
	free(img);
    }
}

PgmImage *createPgmImage(int width, int height, int maxVal) {
    PgmImage *image;

    image = malloc(sizeof(PgmImage));
    image->data = malloc(width * height * sizeof(int));
    memset(image->data, 0, width * height * sizeof(int));

    image->width = width;
    image->height = height;
    image->maxVal = maxVal;

    return image;
}

PgmImage *subImage(const PgmImage *source, int x, int y, int w, int h) {
    PgmImage *image;
    int i0, i, j0, j;

    if ((x + w > source->width) || (y + h > source->height)) return NULL;

    image = createPgmImage(w, h, DEFAULT_MAX_VAL);

    for (i = y, i0 = 0; i < y + h; i++, i0++) {
	for (j = x, j0 = 0; j < x + w; j++, j0++) {
	    setImgVal(image, i0, j0, imgVal(source, i, j));
	}
    }

    return image;
}

FloatMatrix *floatMatrixFromImage(PgmImage *img) {
    FloatMatrix *fm;
    int i;

    fm = createFloatMatrix(img->height, img->width);
    for (i = 0; i < img->height * img->width; i++) {
	fm->data[i] = (float)img->data[i];
    }

    return fm;
}


int imgVal(const PgmImage *image, int row, int col) {
    return image->data[row * image->width + col];
}

void setImgVal(PgmImage *image, int row, int col, int val) {
    image->data[row * image->width + col] = val;
}

int pgmImagesEquals(const PgmImage *img1, const PgmImage *img2) {
    int i, j;

    if (img1->width != img2->width ||
	img1->height != img2->height) return 0;

    for (i = 0; i < img1->height; i++) {
	for (j = 0; j < img1->width; j++) {
	    if (imgVal(img1, i, j) != imgVal(img2, i, j)) {
		return 0;
	    }
	}
    }

    return 1;
}
