#include "array_list_test.h"

#include <stdlib.h>

#include "minunit.h"
#include "array_list.h"

char *testCreateArrayList() {
    ArrayList al;

    al = createArrayList(sizeof(int));
    mu_assert("Initial size != 0", al.size == 0);
    deleteArrayList(&al);

    return 0;
}

char *testAddToArrayList() {
    int *a, *b, *c;
    ArrayList al;

    a = malloc(sizeof(int));
    b = malloc(sizeof(int));
    c = malloc(sizeof(int));

    *a = 10;
    *b = 20;
    *c = 30;

    al = createArrayList(sizeof(int *));

    addToArrayList(&al, &a);
    addToArrayList(&al, &a);
    addToArrayList(&al, &a);
    addToArrayList(&al, &a);
    addToArrayList(&al, &b);
    addToArrayList(&al, &b);
    addToArrayList(&al, &b);
    addToArrayList(&al, &c);
    addToArrayList(&al, &c);
    addToArrayList(&al, &c);
    addToArrayList(&al, &c);
    addToArrayList(&al, &c);

    mu_assert("Wrong size after adding", al.size == 12);
    mu_assert("Wrong element #3 value",
	      **((int **)getFromArrayList(&al, 3)) == 10);
    mu_assert("Wrong element #5 value",
	      **((int **)getFromArrayList(&al, 5)) == 20);
    mu_assert("Wrong element #10 value",
	      **((int **)getFromArrayList(&al, 10)) == 30);

    free(a);
    free(b);
    free(c);
    deleteArrayList(&al);

    return 0;
}

char *testAddToArrayList2() {
    struct TestStruct {
	int a;
	float b;
    };

    ArrayList al;
    struct TestStruct ts1, ts2;

    ts1.a = 1;
    ts1.b = 2;
    ts2.a = 3;
    ts2.b = 4;

    al = createArrayList(sizeof(struct TestStruct));

    addToArrayList(&al, &ts1);
    addToArrayList(&al, &ts2);

    mu_assert("Wrong value in ts1.a",
	      ((struct TestStruct *)getFromArrayList(&al, 0))->a == ts1.a);
    mu_assert("Wrong value in ts2.a",
	      ((struct TestStruct *)getFromArrayList(&al, 1))->a == ts2.a);

    deleteArrayList(&al);
    return 0;
}

char *testRawArray() {
    ArrayList al;
    int a, b;
    int *arr;

    al = createArrayList(sizeof(int));
    a = 10;
    b = 20;

    addToArrayList(&al, &a);
    addToArrayList(&al, &a);
    addToArrayList(&al, &a);
    addToArrayList(&al, &b);
    addToArrayList(&al, &b);
    addToArrayList(&al, &b);

    arr = (int *)rawArrayFromArrayList(&al);

    mu_assert("Wrong raw array value at pos 1", arr[1] == a);
    mu_assert("Wrong raw array value at pos 5", arr[5] == b);

    deleteArrayList(&al);
    free(arr);

    return 0;
}
