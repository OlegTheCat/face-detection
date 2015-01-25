#include "persistent_float_matrix.h"

#include <stdlib.h>

#include "pfm_file_impl.h"

Pfm *createPfm(const char *storage_path, int rows, int cols) {
    return createPfmWithImpl(storage_path,
			     rows,
			     cols,
			     createPfmFileImpl(storage_path, rows, cols));
}

Pfm *createPfmWithImpl(const char *storage_path,
		       int rows,
		       int cols,
		       Pfmi *impl) {
    (void) storage_path;
    Pfm *pfm;

    if (impl == NULL) return NULL;

    pfm = malloc(sizeof(Pfm));
    pfm->impl = impl;
    pfm->rows = rows;
    pfm->cols = cols;

    return pfm;
}

int getPfmCol(PersistentFloatMatrix *pfm, float *buf, int col_idx) {
    return pfm->impl->get_col_func(pfm->impl, buf, col_idx);
}

int storePfmCol(PersistentFloatMatrix *pfm, const float *col, int col_idx) {
    return pfm->impl->store_col_func(pfm->impl, col, col_idx);
}

int removePfmRow(PersistentFloatMatrix *pfm, int row_idx) {
    pfm->rows--;
    return pfm->impl->remove_row_func(pfm->impl, row_idx);
}


void deletePfm(PersistentFloatMatrix *pfm) {
    if (pfm != NULL) {
	if (pfm->impl != NULL) {
	    pfm->impl->impl_delete_func(pfm->impl);
	}
	free(pfm);
    }
}
