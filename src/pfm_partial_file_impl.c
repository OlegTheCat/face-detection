#include "pfm_partial_file_impl.h"

#include <stdlib.h>
#include <stdio.h>

#include "persistent_float_matrix.h"
#include "pfm_file_impl.h"

struct PfmiPartialFileImplData {
    Pfmi *file_impl;
    int start_col;
    int end_col;
};

typedef struct PfmiPartialFileImplData PfmiPartialFileImplData;

PfmiPartialFileImplData *createPfmiData(const char *storage_path,
					int start_col,
					int end_col,
					int rows) {
    PfmiPartialFileImplData *data;
    Pfmi *file_impl;

    file_impl = createPfmFileImpl(storage_path, rows, end_col - start_col);

    if (file_impl == NULL) return NULL;

    data = malloc(sizeof(PfmiPartialFileImplData));
    data->file_impl = file_impl;
    data->start_col = start_col;
    data->end_col = end_col;

    return data;
}

int inRange(int from, int to, int target_val) {
    if (target_val >= from && target_val < to)
	return 1;

    return 0;
}

int getPfmiPartialFileCol(Pfmi *pfmi, float *buf, int col_idx) {
    PfmiPartialFileImplData *pfmi_data;

    pfmi_data = (PfmiPartialFileImplData *)pfmi->impl_data;

    if (inRange(pfmi_data->start_col, pfmi_data->end_col, col_idx)) {
	return pfmi_data->file_impl->get_col_func(pfmi_data->file_impl, buf,
						  col_idx - pfmi_data->start_col);
    } else {
	return PFMI_OUT_OF_COL_RANGE;
    }
}

int storePfmiPartialFileCol(Pfmi *pfmi, const float *col, int col_idx) {
    PfmiPartialFileImplData *pfmi_data;

    pfmi_data = (PfmiPartialFileImplData *)pfmi->impl_data;
    if (inRange(pfmi_data->start_col, pfmi_data->end_col, col_idx)) {
	return pfmi_data->file_impl->store_col_func(pfmi_data->file_impl, col,
						    col_idx - pfmi_data->start_col);
    } else {
	return PFMI_OUT_OF_COL_RANGE;
    }

}

int removePfmiPartialFileRow(Pfmi *pfmi, int row_idx) {
    return ((PfmiPartialFileImplData *)pfmi->impl_data)->
	file_impl->remove_row_func(((PfmiPartialFileImplData *)pfmi->impl_data)->
				   file_impl, row_idx);
}

void deletePfmiPartialFile(Pfmi *pfmi) {
    PfmiPartialFileImplData *pfmi_data;

    if (pfmi != NULL) {
	pfmi_data = pfmi->impl_data;
	if (pfmi_data != NULL) {
	    if (pfmi_data->file_impl != NULL) {
		pfmi_data->file_impl->
		    impl_delete_func(pfmi_data->file_impl);
	    }
	    free(pfmi_data);
	}
	free(pfmi);
    }
}

Pfmi *createPfmPartialFileImpl(const char *storage_path,
			       int start_col,
			       int end_col,
			       int rows) {
    Pfmi *pfmi;
    void *data;

    data = createPfmiData(storage_path, start_col, end_col, rows);

    if (data == NULL) return NULL;

    pfmi = malloc(sizeof(Pfmi));
    pfmi->impl_data = data;
    pfmi->get_col_func = getPfmiPartialFileCol;
    pfmi->store_col_func = storePfmiPartialFileCol;
    pfmi->remove_row_func = removePfmiPartialFileRow;
    pfmi->impl_delete_func = deletePfmiPartialFile;

    return pfmi;
}
