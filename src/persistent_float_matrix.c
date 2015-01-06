#include "persistent_float_matrix.h"

#include <stdlib.h>
#include <stdio.h>
#include <sqlite3.h>

struct PersistentFloatMatrixImpl {
    sqlite3 *db;
};

typedef struct PersistentFloatMatrixImpl Pfmi;
typedef struct PersistentFloatMatrix Pfm;

int initDb(sqlite3 *db, int rows, int cols) {
    const char *create_stmt;
    char insert_stmt[300];
    int i, j, err;

    create_stmt =
	"CREATE TABLE PFM_VALS("
	"ROW INT NOT NULL, "
        "COL INT NOT NULL, "
        "VAL REAL);";

    err = sqlite3_exec(db, create_stmt, NULL, NULL, NULL);

    if (err != SQLITE_OK) {
	return err;
    }

    for (i = 0; i < rows; i++) {
	for (j = 0; j < cols; j++) {
	    sprintf(insert_stmt,
		    "INSERT INTO PFM_VALS (ROW, COL) "
                    "VALUES (%d, %d);", i, j);
	    /* printf("executing: %s\n", insert_stmt); */
	    err = sqlite3_exec(db, insert_stmt, NULL, NULL, NULL);
	    if (err != SQLITE_OK) {
		return err;
	    }
	}
    }

    return SQLITE_OK;
}

Pfmi *createImpl(const char *storage_path, int rows, int cols) {
    Pfmi *pfmi;
    sqlite3 *db;
    int err;

    err = sqlite3_open(storage_path, &db);
    if (err != SQLITE_OK) {
	goto create_impl_err;
    }

    err = initDb(db, rows, cols);
    if (err != SQLITE_OK) {
	goto create_impl_err;
    }

    pfmi = malloc(sizeof(Pfmi));
    pfmi->db = db;

    return pfmi;

    create_impl_err:
    if (db != NULL) sqlite3_close(db);
    return NULL;
}

Pfm *createPfm(const char *storage_path, int rows, int cols) {
    Pfm *pfm;
    Pfmi *pfmi;

    pfmi = createImpl(storage_path, rows, cols);
    if (pfmi == NULL) return NULL;

    pfm = malloc(sizeof(Pfm));
    pfm->impl = pfmi;

    return pfm;
}

float *getPfmCol(int idx) {
    (void)idx;
    return 0;
}

void deletePfm(PersistentFloatMatrix *pfm) {
    if (pfm != NULL) {
	if (pfm->impl != NULL) {
	    sqlite3_close(pfm->impl->db);
	    free(pfm->impl);
	}
	free(pfm);
    }
}
