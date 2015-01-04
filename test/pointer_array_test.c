#include "pointer_array_test.h"

#include <stdlib.h>

#include "minunit.h"
#include "pointer_array.h"

char *testCreatePointerArray() {
    PointerArray pa;

    pa = createPointerArray();
    mu_assert("Initial size != 0", pa.size == 0);
    deletePointerArray(&pa);

    return 0;
}

char *testAddToPointerArray() {
    int *a, *b, *c;
    PointerArray pa;

    a = malloc(sizeof(int));
    b = malloc(sizeof(int));
    c = malloc(sizeof(int));

    *a = 10;
    *b = 20;
    *c = 30;

    pa = createPointerArray();

    addToPointerArray(&pa, a);
    addToPointerArray(&pa, a);
    addToPointerArray(&pa, a);
    addToPointerArray(&pa, a);
    addToPointerArray(&pa, b);
    addToPointerArray(&pa, b);
    addToPointerArray(&pa, b);
    addToPointerArray(&pa, c);
    addToPointerArray(&pa, c);
    addToPointerArray(&pa, c);
    addToPointerArray(&pa, c);
    addToPointerArray(&pa, c);

    mu_assert("Wrong size after adding", pa.size == 12);
    mu_assert("Wrong element #3 value",
	      *((int *)getFromPointerArray(&pa, 3)) == 10);
    mu_assert("Wrong element #5 value",
	      *((int *)getFromPointerArray(&pa, 5)) == 20);
    mu_assert("Wrong element #10 value",
	      *((int *)getFromPointerArray(&pa, 10)) == 30);

    free(a);
    free(b);
    free(c);
    deletePointerArray(&pa);

    return 0;
}
