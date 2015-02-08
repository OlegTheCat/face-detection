#pragma once

#include <stdlib.h>

#define SAFE_MALLOC(expr)			\
    safeMalloc((expr), __FILE__, __LINE__)

float sqrf(float val);
int floatEqual(float f1, float f2);
int floatEqualEps(float f1, float f2, float eps);
int fileExists(const char *path);
void freeNonNull(void *ptr);
void *safeMalloc(size_t size, const char *file, int line);
