#include "pointer_array.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

PointerArray createPointerArray() {
    PointerArray pa;
    pa.capacity = 10;
    pa.size = 0;
    pa.elements = malloc(sizeof(void *) * pa.capacity);

    return pa;
}

void enlargePointerArray(PointerArray *pa) {
    pa->capacity *= 2;
    pa->elements = realloc(pa->elements, pa->capacity * sizeof(void *));
}

void addToPointerArray(PointerArray *pa, void *p) {
    if (pa->size + 1 > pa->capacity) {
	enlargePointerArray(pa);
    }

    pa->elements[pa->size++] = p;
}

void *getFromPointerArray(const PointerArray *pa, int idx) {
    return pa->elements[idx];
}


void **rawArrayFromPointerArray(const PointerArray *pa) {
    void **arr;

    arr = malloc(sizeof(void *) * pa->size);
    memcpy(arr, pa->elements, sizeof(void *) * pa->size);

    return arr;
}

void deletePointerArray(PointerArray *pa) {
    if (pa->elements != NULL) free(pa->elements);
}
