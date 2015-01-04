#pragma once

#define DEFAULT_MAX_VAL 255

struct PgmImage {
    int maxVal;
    int width;
    int height;
    int *data;
};

typedef struct PgmImage PgmImage;

int imgVal(PgmImage *image, int row, int col);
int setImgVal(PgmImage *image, int row, int col, int val);

PgmImage *readPgmImage(const char *filename);
PgmImage *createPgmImage(int width, int height, int maxVal);
PgmImage *subImage(PgmImage *source, int x, int y, int w, int h);
void savePgmImage(const char *filename, PgmImage *img);
void deletePgmImage(PgmImage *img);
