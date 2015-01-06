#include "persistent_float_matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "utils.h"

struct PersistentFloatMatrixImpl {
    FILE *file;
};

typedef struct PersistentFloatMatrixImpl Pfmi;
typedef struct PersistentFloatMatrix Pfm;

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

int getPfmCol(Pfm *pfm, float *buf, int col_idx) {
    FILE *f;

    f = pfm->impl->file;
    fseek(f, pfm->rows * col_idx * sizeof(float), SEEK_SET);
    fread(buf, sizeof(float), pfm->rows, f);
    if (ferror(f)) return 1;
    rewind(f);

    return 0;
}

int storePfmCol(Pfm *pfm, const float *col, int col_idx) {
    FILE *f;

    f = pfm->impl->file;
    fseek(f, pfm->rows * col_idx * sizeof(float), SEEK_SET);
    fwrite(col, sizeof(float), pfm->rows, f);
    if (ferror(f)) return 1;
    rewind(f);

    return 0;
}

void deletePfm(PersistentFloatMatrix *pfm) {
    if (pfm != NULL) {
	if (pfm->impl != NULL) {
	    fclose(pfm->impl->file);
	    free(pfm->impl);
	}
	free(pfm);
    }
}
