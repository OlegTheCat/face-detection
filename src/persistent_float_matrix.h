#pragma once

struct PersistentFloatMatrixImpl;

struct PersistentFloatMatrix {
    int rows;
    int cols;
    struct PersistentFloatMatrixImpl *impl;
};

typedef struct PersistentFloatMatrix PersistentFloatMatrix;

PersistentFloatMatrix *createPfm(const char *storage_path,
				 int rows,
				 int cols);
float *getPfmCol(int idx);
void deletePfm(PersistentFloatMatrix *pfm);
