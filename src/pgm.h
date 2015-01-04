#pragma once

#define DEFAULT_MAX_VAL 255

struct PgmImage {
    int maxVal;
    int width;
    int height;
    int *data;
};

typedef struct PgmImage PgmImage;

int imgVal(const PgmImage *image, int row, int col);
void setImgVal(PgmImage *image, int row, int col, int val);

PgmImage *readPgmImage(const char *filename);
PgmImage *createPgmImage(int width, int height, int maxVal);
PgmImage *subImage(const PgmImage *source, int x, int y, int w, int h);
int pgmImagesEquals(const PgmImage *img1, const PgmImage *img2);
void savePgmImage(const char *filename, const PgmImage *img);
void deletePgmImage(PgmImage *img);
