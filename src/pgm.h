#pragma once

struct PgmImage {
    int maxVal;
    int width;
    int height;
    int *data;
};

typedef struct PgmImage PgmImage;

PgmImage *readPgmImage(const char *filename);
void savePgmImage(const char *filename, PgmImage *img);
void deletePgmImage(PgmImage *img);
