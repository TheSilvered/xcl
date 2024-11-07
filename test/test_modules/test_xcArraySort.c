#include <time.h>
#include "test.h"
#include "xc.h"

static bool test_xcArraySort_checkSorted(XCArray *intArray) {
    if (intArray->len == 0)
        return true;
    int val = *(int *)xcArrayGet(intArray, 0);
    for (usize i = 1; i < intArray->len; i++) {
        int newVal = *(int *)xcArrayGet(intArray, i);
        if (val > newVal)
            return false;
        val = newVal;
    }
    return true;
}

TestResult test_xcArraySort_empty(void) {
    XCArray array;
    xcArrayInitFromData(&array, sizeof(int), 0, NULL);
    xcArraySort(&array, xcCompare_int);
    return TR_success;
}

TestResult test_xcArraySort_sorted(void) {
    const int arrSize = 100;

    XCArray array;
    if (!xcArrayInit(&array, sizeof(int), arrSize))
        return TR_allocFailed;
    for (int i = 0; i < arrSize; i++)
        xcArrayAppend(&array, &i);
    xcArraySort(&array, xcCompare_int);
    if (!test_xcArraySort_checkSorted(&array))
        return TR_failure;
    xcArrayDestroy(&array, NULL);
    return TR_success;
}

TestResult test_xcArraySort_reversed(void) {
    const int arrSize = 100;

    XCArray array;
    if (!xcArrayInit(&array, sizeof(int), arrSize))
        return TR_allocFailed;
    for (int i = 0; i < arrSize; i++)
        xcArrayAppend(&array, xcIRef(int, 99 - i));
    xcArraySort(&array, xcCompare_int);
    if (!test_xcArraySort_checkSorted(&array))
        return TR_failure;
    xcArrayDestroy(&array, NULL);
    return TR_success;
}

TestResult test_xcArraySort_random(void) {
    const int arrSize = 100;
    srand((uint)clock());
    XCArray array;
    if (!xcArrayInit(&array, sizeof(int), arrSize))
        return TR_allocFailed;
    for (int i = 0; i < arrSize; i++)
        xcArrayAppend(&array, xcIRef(int, rand()));
    xcArraySort(&array, xcCompare_int);
    if (!test_xcArraySort_checkSorted(&array))
        return TR_failure;
    xcArrayDestroy(&array, NULL);
    return TR_success;
}

void test_xcArraySort__addTests(void) {
    testSetModule("test_xcArraySort");
    testAdd(TEST(test_xcArraySort_empty));
    testAdd(TEST(test_xcArraySort_sorted));
    testAdd(TEST(test_xcArraySort_reversed));
    testAdd(TEST(test_xcArraySort_random));
}
