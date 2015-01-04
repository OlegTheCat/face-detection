#include "pointer_array.h"

#include <stdlib.h>
#include <stdio.h>

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

void *getFromPointerArray(PointerArray *pa, int idx) {
    return pa->elements[idx];
}

void deletePointerArray(PointerArray *pa) {
    if (pa->elements != NULL) free(pa->elements);
}
