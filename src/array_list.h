#pragma once

#include <stdlib.h>

struct ArrayList {
    int size;
    int capacity;
    size_t element_size;
    void *data;
};

typedef struct ArrayList ArrayList;

ArrayList createArrayList(size_t element_size);
void addToArrayList(ArrayList *pa, void *p);
void *getFromArrayList(const ArrayList *pa, int idx);
void deleteArrayList(ArrayList *pa);
void *rawArrayFromArrayList(const ArrayList *pa);
