#include "test_utils.h"

#include <unistd.h>
#include <stdio.h>

int getUniqueFileIndex() {
    static int i = 0;
    return (int)getpid() + i++;
}

char *getTestFileName(char *result) {
    return getUniqueFileName("test", "data", result);
}

char *getUniqueFileName(const char *prefix, const char *suffix, char *result) {
    sprintf(result, "%s%d.%s", prefix, getUniqueFileIndex(), suffix);

    return result;
}
