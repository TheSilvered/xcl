#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "xc.h"

#define ARRAY_SIZE 50

void printIntArray(XCArray *array) {
    printf("[ ");
    for (int *value = xcArrayNext(array, NULL); value; value = xcArrayNext(array, value))
        printf("%d ", *value);
    printf("]\n");
}

int main(void) {
    srand((uint)clock());
    XCArray *array = xcArrayNew(sizeof(int), ARRAY_SIZE);
    for (int i = ARRAY_SIZE; i > 0; i--) {
        int val = rand() % 99;
        xcArrayAppend(array, &val);
    }
    printIntArray(array);
    xcArraySort(array, (XCComparator)xcCompare_int);
    printIntArray(array);

    return 0;
}
