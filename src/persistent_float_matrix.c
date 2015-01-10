#include "persistent_float_matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "array_list.h"

struct PersistentFloatMatrixImpl {
    FILE *file;
    float *col_buf;
    int initial_rows_num;
    // This should be replaced by hash set in future
    ArrayList removed_rows;
};

typedef struct PersistentFloatMatrixImpl Pfmi;

Pfmi *createImpl(const char *storage_path, int rows, int cols) {
    Pfmi *pfmi;
    FILE *f;
    float *zero_col;
    int i, needs_fill;

    if (fileExists(storage_path)) {
	f = fopen(storage_path, "r+b");
	needs_fill = 0;
    } else {
	f = fopen(storage_path, "w+b");
	needs_fill = 1;
    }

    if (f == NULL) return NULL;

    if (needs_fill) {
	zero_col = malloc(rows * sizeof(float));
	memset(zero_col, 0, rows * sizeof(float));

	for (i = 0; i < cols; i++) {
	    /* if (i % 100 == 0) printf("Filled pfm col #%d with zeros\n", i); */
	    fwrite(zero_col, sizeof(float), rows, f);
	    if (ferror(f) != 0) {
		printf("Error while filling pfm\n");
		free(zero_col);
		fclose(f);
		return NULL;
	    }
	}

	free(zero_col);
	rewind(f);
    }

    pfmi = malloc(sizeof(Pfmi));
    pfmi->file = f;
    pfmi->removed_rows = createArrayList(sizeof(int));
    pfmi->initial_rows_num = rows;
    pfmi->col_buf = malloc(sizeof(float) * rows);

    return pfmi;
}

Pfm *createPfm(const char *storage_path, int rows, int cols) {
    Pfm *pfm;
    Pfmi *pfmi;

    pfmi = createImpl(storage_path, rows, cols);
    if (pfmi == NULL) return NULL;

    pfm = malloc(sizeof(Pfm));
    pfm->impl = pfmi;
    pfm->rows = rows;
    pfm->cols = cols;

    return pfm;
}

void seekToCol(Pfmi *pfmi, int col_idx) {
    fseek(pfmi->file,
	  pfmi->initial_rows_num * col_idx * sizeof(float),
	  SEEK_SET);
}

void readPfmiBuffer(Pfmi *pfmi) {
    fread(pfmi->col_buf, sizeof(float), pfmi->initial_rows_num, pfmi->file);
}

void writePfmiBuffer(Pfmi *pfmi) {
    fwrite(pfmi->col_buf, sizeof(float), pfmi->initial_rows_num, pfmi->file);
}

int getIntFromArrayList(const ArrayList *al, int idx) {
    return *((int *)getFromArrayList(al, idx));
}

int isRemoved(int idx, const ArrayList *removed_rows) {
    int i;

    for (i = 0; i < removed_rows->size; i++) {
	if (getIntFromArrayList(removed_rows, i) == idx) {
	    return 1;
	}
    }

    return 0;
}

void filterPfmCol(const float *retrieved_col,
		  float *res_col,
		  ArrayList *removed_rows,
		  int num_rows) {
    int i, j;

    j = 0;
    for (i = 0; i < num_rows; i++) {
	if (isRemoved(i, removed_rows)) {
	    continue;
	}

	res_col[j] = retrieved_col[i];
	j++;
    }
}

int getPfmCol(Pfm *pfm, float *buf, int col_idx) {
    FILE *f;

    f = pfm->impl->file;
    seekToCol(pfm->impl, col_idx);
    readPfmiBuffer(pfm->impl);
    if (ferror(f)) return 1;

    filterPfmCol(pfm->impl->col_buf,
		 buf,
		 &pfm->impl->removed_rows,
		 pfm->impl->initial_rows_num);
    rewind(f);

    return 0;
}

void extendPfmCol(const float *in_col,
		  float *to_write_col,
		  ArrayList *removed_rows,
		  int num_rows) {
    int i, j;

    j = 0;
    for (i = 0; i < num_rows; i++) {
	if (isRemoved(i, removed_rows)) {
	    to_write_col[i] = 0.0f;
	    continue;
	}

	to_write_col[i] = in_col[j];
	j++;
    }
}

int storePfmCol(Pfm *pfm, const float *col, int col_idx) {
    FILE *f;

    f = pfm->impl->file;
    extendPfmCol(col,
		 pfm->impl->col_buf,
		 &pfm->impl->removed_rows,
		 pfm->impl->initial_rows_num);
    seekToCol(pfm->impl, col_idx);
    writePfmiBuffer(pfm->impl);
    if (ferror(f)) return 1;
    rewind(f);

    return 0;
}

int adjustRowIdx(int row_idx, const ArrayList *removed_rows) {
    int i, row_offset;

    row_offset = 0;
    for (i = 0; i < removed_rows->size; i++) {
	if (getIntFromArrayList(removed_rows, i) <= row_idx) {
	    row_offset++;
	}
    }

    return row_idx + row_offset;
}

int removePfmRow(PersistentFloatMatrix *pfm, int row_idx) {
    int adjusted_row_idx;

    adjusted_row_idx = adjustRowIdx(row_idx, &pfm->impl->removed_rows);
    addToArrayList(&pfm->impl->removed_rows, &adjusted_row_idx);
    pfm->rows--;
    return 0;
}

void deletePfm(PersistentFloatMatrix *pfm) {
    if (pfm != NULL) {
	if (pfm->impl != NULL) {
	    fclose(pfm->impl->file);
	    deleteArrayList(&pfm->impl->removed_rows);
	    free(pfm->impl->col_buf);
	    free(pfm->impl);
	}
	free(pfm);
    }
}
