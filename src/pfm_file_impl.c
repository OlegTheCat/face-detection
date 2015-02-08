#include "pfm_file_impl.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"
#include "array_list.h"
#include "persistent_float_matrix.h"
#include "range.h"

#define CACHE_HIT 0
#define CACHE_MISS 1

struct PfmiFileImplData {
    FILE *file;
    float *col_buf;

    size_t col_size;

    float *cols_cache;
    int cache_cols_num;
    Range cols_in_cache;
    int last_read_col_idx;

    int cols;

    int initial_rows_num;
    // This should be replaced by hash set in future
    ArrayList removed_rows;
};

typedef struct PfmiFileImplData PfmiFileImplData;

PfmiFileImplData *createImplData(const char *storage_path,
				 int rows,
				 int cols,
				 int cache_cols_num) {
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
    pfmi_data->cols = cols;
    pfmi_data->col_size = sizeof(float) * rows;

    pfmi_data->col_buf = malloc(pfmi_data->col_size);
    pfmi_data->cache_cols_num = cache_cols_num;
    pfmi_data->cols_cache = SAFE_MALLOC(pfmi_data->col_size * cache_cols_num);
    /* printf("Allocated cache from %ld to %ld\n\n", */
    /* 	   (long)pfmi_data->cols_cache, */
    /* 	   ((long)pfmi_data->cols_cache + (long)(pfmi_data->col_size * cache_cols_num))); */
    pfmi_data->last_read_col_idx = -1;
    memset(pfmi_data->cols_cache, 0, (pfmi_data->col_size * cache_cols_num));
    pfmi_data->cols_in_cache = createRange(0, cache_cols_num);
    return pfmi_data;
}

void seekToCol(PfmiFileImplData *pfmi_data, int col_idx) {
    fseek(pfmi_data->file,
	  pfmi_data->col_size * col_idx,
	  SEEK_SET);
}

void readPfmiBuffer(PfmiFileImplData *pfmi_data) {
    fread(pfmi_data->col_buf, sizeof(float), pfmi_data->initial_rows_num, pfmi_data->file);
}

void writePfmiBuffer(PfmiFileImplData *pfmi_data) {
    fwrite(pfmi_data->col_buf, sizeof(float), pfmi_data->initial_rows_num, pfmi_data->file);
}

void populateCacheWithRange(PfmiFileImplData *pfmi_data) {

    /* printf("populating cache with range: %d , %d\n\n", pfmi_data->cols_in_cache.from, */
    /* 	   pfmi_data->cols_in_cache.to); */
    seekToCol(pfmi_data, pfmi_data->cols_in_cache.from);
    fread(pfmi_data->cols_cache, sizeof(float),
	  pfmi_data->initial_rows_num * rangeSize(&pfmi_data->cols_in_cache),
	  pfmi_data->file);
}

void populateCache(PfmiFileImplData *pfmi_data, int start_col_idx) {
    int finish_col;

    finish_col =
	(start_col_idx + pfmi_data->cache_cols_num > pfmi_data->cols) ?
	pfmi_data->cols :
	start_col_idx + pfmi_data->cache_cols_num;

    pfmi_data->cols_in_cache = createRange(start_col_idx, finish_col);

    populateCacheWithRange(pfmi_data);
}

void cacheNextColsHunk(PfmiFileImplData *pfmi_data) {
    populateCache(pfmi_data,
		  (pfmi_data->cols_in_cache.to == pfmi_data->cols)
		  ? 0
		  : pfmi_data->cols_in_cache.to);
}

void writeBufToCache(PfmiFileImplData *pfmi_data, int col_idx) {
    if (inRange(&pfmi_data->cols_in_cache, col_idx)) {
	/* printf("col_idx %d is in range [%d;%d]\n\n", col_idx, pfmi_data->cols_in_cache.from, */
	/*        pfmi_data->cols_in_cache.to); */
	/* printf("Trying to write data in range %ld : %ld \n\n", */
	/*        (long)pfmi_data->cols_cache + (long)((col_idx - pfmi_data->cols_in_cache.from) * pfmi_data->col_size), */
	/*        (long)pfmi_data->cols_cache + (long)((col_idx - pfmi_data->cols_in_cache.from) * pfmi_data->col_size + pfmi_data->col_size)); */
	/* fflush(stdout); */
	memcpy(pfmi_data->cols_cache +
	       (col_idx - pfmi_data->cols_in_cache.from) * pfmi_data->initial_rows_num,
	       pfmi_data->col_buf,
	       pfmi_data->col_size);
    }
}

int readBufFromCache(PfmiFileImplData *pfmi_data, int col_idx) {
    if (pfmi_data->cache_cols_num == 0) {
	return CACHE_MISS;
    } else if (inRange(&pfmi_data->cols_in_cache, col_idx)) {
	memcpy(pfmi_data->col_buf,
	       pfmi_data->cols_cache +
	       (col_idx - pfmi_data->cols_in_cache.from) * pfmi_data->initial_rows_num,
	       pfmi_data->col_size);
	return CACHE_HIT;
    } else if (col_idx == pfmi_data->cols_in_cache.to &&
	       col_idx == pfmi_data->last_read_col_idx + 1) {
	cacheNextColsHunk(pfmi_data);
	return readBufFromCache(pfmi_data, col_idx);
    }

    return CACHE_MISS;
}

void cachedRead(PfmiFileImplData *pfmi_data, int col_idx) {

    if (readBufFromCache(pfmi_data, col_idx) == CACHE_HIT) return;

    seekToCol(pfmi_data, col_idx);
    readPfmiBuffer(pfmi_data);

    pfmi_data->last_read_col_idx = col_idx;
}

void cachedWrite(PfmiFileImplData *pfmi_data, int col_idx) {
    writeBufToCache(pfmi_data, col_idx);
    seekToCol(pfmi_data, col_idx);
    writePfmiBuffer(pfmi_data);
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

int getPfmiFileCol(Pfmi *pfmi, float *buf, int col_idx) {
    FILE *f;
    PfmiFileImplData *pfmi_data;

    pfmi_data = (PfmiFileImplData *)pfmi->impl_data;
    f = pfmi_data->file;
    if (ferror(f)) return 1;

    cachedRead(pfmi_data, col_idx);

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

int storePfmiFileCol(Pfmi *pfmi, const float *col, int col_idx) {
    FILE *f;
    PfmiFileImplData *pfmi_data;

    pfmi_data = (PfmiFileImplData *)pfmi->impl_data;

    f = pfmi_data->file;
    extendPfmCol(col,
		 pfmi_data->col_buf,
		 &pfmi_data->removed_rows,
		 pfmi_data->initial_rows_num);
    cachedWrite(pfmi_data, col_idx);
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

int removePfmiFileRow(Pfmi *pfmi, int row_idx) {
    int adjusted_row_idx;
    PfmiFileImplData *pfmi_data;

    pfmi_data = (PfmiFileImplData *)pfmi->impl_data;
    adjusted_row_idx = adjustRowIdx(row_idx, &pfmi_data->removed_rows);
    addToArrayList(&pfmi_data->removed_rows, &adjusted_row_idx);
    return 0;
}

void deletePfmiFile(Pfmi *pfmi) {
    PfmiFileImplData *pfmi_data;
    pfmi_data = (PfmiFileImplData *)pfmi->impl_data;

    if (pfmi != NULL) {
	if (pfmi_data != NULL) {
	    fclose(pfmi_data->file);
	    deleteArrayList(&pfmi_data->removed_rows);
	    free(pfmi_data->cols_cache);
	    free(pfmi_data->col_buf);
	    free(pfmi_data);
	}
	free(pfmi);
    }
}

Pfmi *createPfmFileImpl(const char *storage_path,
			int rows,
			int cols) {
    return createBufferedPfmFileImpl(storage_path,
				     rows,
				     cols,
				     cols / 4);
}

Pfmi *createBufferedPfmFileImpl(const char *storage_path,
				int rows,
				int cols,
				int preload_cols_num) {
    Pfmi *pfmi;
    void *data;

    data = createImplData(storage_path, rows, cols, preload_cols_num);
    if (data == NULL) return NULL;

    pfmi = malloc(sizeof(Pfmi));
    pfmi->impl_data = data;
    pfmi->get_col_func = getPfmiFileCol;
    pfmi->store_col_func = storePfmiFileCol;
    pfmi->remove_row_func = removePfmiFileRow;
    pfmi->impl_delete_func = deletePfmiFile;

    return pfmi;
}
