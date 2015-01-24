#include "pfm_file_impl.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"
#include "array_list.h"
#include "persistent_float_matrix.h"

struct PfmiFileImplData {
    FILE *file;
    float *col_buf;
    int initial_rows_num;
    // This should be replaced by hash set in future
    ArrayList removed_rows;
};

typedef struct PfmiFileImplData PfmiFileImplData;

PfmiFileImplData *createImplData(const char *storage_path, int rows, int cols) {
    PfmiFileImplData *pfmi_data;
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

    pfmi_data = malloc(sizeof(PfmiFileImplData));
    pfmi_data->file = f;
    pfmi_data->removed_rows = createArrayList(sizeof(int));
    pfmi_data->initial_rows_num = rows;
    pfmi_data->col_buf = malloc(sizeof(float) * rows);

    return pfmi_data;
}

void seekToCol(PfmiFileImplData *pfmi_data, int col_idx) {
    fseek(pfmi_data->file,
	  pfmi_data->initial_rows_num * col_idx * sizeof(float),
	  SEEK_SET);
}

void readPfmiBuffer(PfmiFileImplData *pfmi_data) {
    fread(pfmi_data->col_buf, sizeof(float), pfmi_data->initial_rows_num, pfmi_data->file);
}

void writePfmiBuffer(PfmiFileImplData *pfmi_data) {
    fwrite(pfmi_data->col_buf, sizeof(float), pfmi_data->initial_rows_num, pfmi_data->file);
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

int getPfmiFileCol(Pfm *pfm, float *buf, int col_idx) {
    FILE *f;
    PfmiFileImplData *pfmi_data;

    pfmi_data = (PfmiFileImplData *)pfm->impl->impl_data;
    f = pfmi_data->file;
    seekToCol(pfmi_data, col_idx);
    readPfmiBuffer(pfmi_data);
    if (ferror(f)) return 1;

    filterPfmCol(pfmi_data->col_buf,
		 buf,
		 &pfmi_data->removed_rows,
		 pfmi_data->initial_rows_num);
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

int storePfmiFileCol(Pfm *pfm, const float *col, int col_idx) {
    FILE *f;
    PfmiFileImplData *pfmi_data;

    pfmi_data = (PfmiFileImplData *)pfm->impl->impl_data;
    f = pfmi_data->file;
    extendPfmCol(col,
		 pfmi_data->col_buf,
		 &pfmi_data->removed_rows,
		 pfmi_data->initial_rows_num);
    seekToCol(pfmi_data, col_idx);
    writePfmiBuffer(pfmi_data);
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

int removePfmiFileRow(PersistentFloatMatrix *pfm, int row_idx) {
    int adjusted_row_idx;
    PfmiFileImplData *pfmi_data;

    pfmi_data = (PfmiFileImplData *)pfm->impl->impl_data;
    adjusted_row_idx = adjustRowIdx(row_idx, &pfmi_data->removed_rows);
    addToArrayList(&pfmi_data->removed_rows, &adjusted_row_idx);
    pfm->rows--;
    return 0;
}

void deletePfmiFile(Pfmi *pfmi) {
    PfmiFileImplData *pfmi_data;
    pfmi_data = (PfmiFileImplData *)pfmi->impl_data;

    if (pfmi != NULL) {
	if (pfmi_data != NULL) {
	    fclose(pfmi_data->file);
	    deleteArrayList(&pfmi_data->removed_rows);
	    free(pfmi_data->col_buf);
	    free(pfmi_data);
	}
	free(pfmi);
    }
}

Pfmi *createPfmFileImpl(const char *storage_path, int rows, int cols) {
    Pfmi *pfmi;
    void *data;

    data = createImplData(storage_path, rows, cols);
    if (data == NULL) return NULL;

    pfmi = malloc(sizeof(Pfmi));
    pfmi->impl_data = data;
    pfmi->get_col_func = getPfmiFileCol;
    pfmi->store_col_func = storePfmiFileCol;
    pfmi->remove_row_func = removePfmiFileRow;
    pfmi->impl_delete_func = deletePfmiFile;

    return pfmi;
}
