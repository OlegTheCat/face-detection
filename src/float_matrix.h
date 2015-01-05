#pragma once

struct FloatMatrix {
    int width;
    int height;
    float *data;
};

typedef struct FloatMatrix FloatMatrix;

FloatMatrix *createFloatMatrix(int width, int height);
float matVal(const FloatMatrix *fm, int row, int col);
void setMatVal(FloatMatrix *fm, int row, int col, float val);
float matSum(FloatMatrix *fm);
void deleteFloatMatrix(FloatMatrix *fm);

