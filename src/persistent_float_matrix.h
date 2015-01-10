#pragma once

struct PersistentFloatMatrixImpl;

struct PersistentFloatMatrix {
    int rows;
    int cols;
    struct PersistentFloatMatrixImpl *impl;
};

typedef struct PersistentFloatMatrix PersistentFloatMatrix;
typedef PersistentFloatMatrix Pfm;

PersistentFloatMatrix *createPfm(const char *storage_path,
				 int rows,
				 int cols);
int getPfmCol(PersistentFloatMatrix *pfm, float *buf, int col_idx);
int storePfmCol(PersistentFloatMatrix *pfm, const float *col, int col_idx);
int removePfmRow(PersistentFloatMatrix *pfm, int row_idx);
void deletePfm(PersistentFloatMatrix *pfm);
