#pragma once

struct PersistentFloatMatrixImpl;
struct PersistentFloatMatrix;

typedef int (*GetColFunc)(struct PersistentFloatMatrixImpl *pfm, float *buf, int col_idx);
typedef int (*StoreColFunc)(struct PersistentFloatMatrixImpl *pfm, const float *col, int col_idx);
typedef int (*RemoveRowFunc)(struct PersistentFloatMatrixImpl *pfm, int row_idx);
typedef void (*ImplDeleteFunc)(struct PersistentFloatMatrixImpl *pfmi);

struct PersistentFloatMatrixImpl {
    void *impl_data;
    GetColFunc get_col_func;
    StoreColFunc store_col_func;
    RemoveRowFunc remove_row_func;
    ImplDeleteFunc impl_delete_func;
};

typedef struct PersistentFloatMatrixImpl Pfmi;

struct PersistentFloatMatrix {
    int rows;
    int cols;
    Pfmi *impl;
};

typedef struct PersistentFloatMatrix PersistentFloatMatrix;
typedef PersistentFloatMatrix Pfm;

Pfm *createPfm(const char *storage_path,
	       int rows,
	       int cols);
Pfm *createPfmWithImpl(const char *storage_path,
		       int rows,
		       int cols,
		       Pfmi *impl);
int getPfmCol(Pfm *pfm, float *buf, int col_idx);
int storePfmCol(Pfm *pfm, const float *col, int col_idx);
int removePfmRow(Pfm *pfm, int row_idx);
void deletePfm(Pfm *pfm);
