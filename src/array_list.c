#include "array_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ArrayList createArrayList(size_t element_size) {
    ArrayList al;
    al.capacity = 10;
    al.size = 0;
    al.element_size = element_size;
    al.data = malloc(al.element_size * al.capacity);

    return al;
}

void enlargeArrayList(ArrayList *al) {
    al->capacity *= 2;
    al->data = realloc(al->data,
		       al->capacity * al->element_size);
}

void addToArrayList(ArrayList *al, void *p) {
    if (al->size + 1 > al->capacity) {
	enlargeArrayList(al);
    }

    memcpy(al->data + al->element_size * al->size,
	   p, al->element_size);

    al->size++;
}

void *getFromArrayList(const ArrayList *al, int idx) {
    return al->data + idx * al->element_size;
}


void *rawArrayFromArrayList(const ArrayList *al) {
    void *arr;

    arr = malloc(al->element_size * al->size);
    memcpy(arr, al->data, al->element_size * al->size);

    return arr;
}

void deleteArrayList(ArrayList *al) {
    if (al != NULL && al->data != NULL) {
	free(al->data);
    }
}
