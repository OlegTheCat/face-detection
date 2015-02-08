#include "utils.h"

#include <stdio.h>
#include <math.h>

float sqrf(float val) {
    return val * val;
}

int floatEqual(float f1, float f2) {
    return floatEqualEps(f1, f2, 0.0000001f);
}

int floatEqualEps(float f1, float f2, float eps) {
    return fabsf(f1 - f2) < eps;
}


int fileExists(const char *path) {
    FILE *f;
    int exists;

    f = fopen(path, "r");
    exists = f != NULL;
    if (exists) fclose(f);

    return exists;
}

void freeNonNull(void *ptr) {
    if (ptr != NULL) {
	free(ptr);
    }
}

void *safeMalloc(size_t size, const char *file, int line) {
    void *p;

    p = malloc(size);

    if (p == NULL) {
	fprintf(stderr, "%s:%d: Out of memory error\n",
		file, line);
	exit(EXIT_FAILURE);
    }

    return p;
}
