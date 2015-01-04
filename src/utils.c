#include "utils.h"

#include <stdio.h>

int fileExists(const char *path) {
    FILE *f;
    int exists;

    f = fopen(path, "r");
    exists = f != NULL;
    if (exists) fclose(f);

    return exists;
}

