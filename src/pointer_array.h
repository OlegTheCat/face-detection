#pragma once

struct PointerArray {
    int size;
    int capacity;
    void **elements;
};

typedef struct PointerArray PointerArray;

PointerArray createPointerArray();
void addToPointerArray(PointerArray *pa, void *p);
void deletePointerArray(PointerArray *pa);
